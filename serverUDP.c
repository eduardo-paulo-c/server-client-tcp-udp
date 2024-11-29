#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define FILENAME "file.bin"

void send_file(FILE *file, int server_socket, struct sockaddr_in *client_addr, socklen_t addr_len)
{
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    int packet_id = 0;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        if (sendto(server_socket, buffer, bytes_read, 0, (struct sockaddr *)client_addr, addr_len) == -1)
        {
            perror("Erro ao enviar pacote");
            exit(EXIT_FAILURE);
        }
        // printf("Enviado pacote %d com %zu bytes.\n", packet_id, bytes_read);
        packet_id++;
    }

    // Envia um pacote final para sinalizar o término
    strcpy(buffer, "END");
    if (sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)client_addr, addr_len) == -1)
    {
        perror("Erro ao enviar pacote final");
        exit(EXIT_FAILURE);
    }
    printf("Pacote final 'END' enviado.\n");
    printf("Arquivo enviado com %d pacotes.\n", packet_id);
}

int main()
{
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Erro ao fazer bind");
        exit(EXIT_FAILURE);
    }

    printf("Aguardando requisição do cliente...\n");

    char buffer[BUFFER_SIZE];
    recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (strcmp(buffer, "REQUEST") == 0)
    {
        printf("Cliente conectado, iniciando envio de arquivo.\n");

        FILE *file = fopen(FILENAME, "rb");
        if (!file)
        {
            perror("Erro ao abrir arquivo");
            exit(EXIT_FAILURE);
        }

        send_file(file, server_socket, &client_addr, addr_len);

        fclose(file);
    }

    close(server_socket);
    return 0;
}
