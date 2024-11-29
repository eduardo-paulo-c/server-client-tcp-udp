#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define FILENAME "received_file_udp.bin"

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    FILE *file;
    ssize_t bytes_received;
    socklen_t addr_len = sizeof(server_addr);
    int expected_packets = 0, received_packets = 0;
    clock_t start, end;
    double time_spent;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    strcpy(buffer, "REQUEST");
    sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);

    file = fopen(FILENAME, "wb");
    if (!file)
    {
        perror("Erro ao criar arquivo");
        exit(EXIT_FAILURE);
    }

    printf("Recebendo arquivo...\n");
    start = clock();
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE); // Limpa o buffer antes de cada recebimento
        bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);

        if (bytes_received == -1)
        {
            perror("Erro ao receber pacote");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "END") == 0)
        {
            printf("Pacote final 'END' recebido.\n");
            break;
        }

        fwrite(buffer, 1, bytes_received, file);
        received_packets++;
        // printf("Pacote %d recebido com %zd bytes.\n", received_packets, bytes_received);
    }
    end = clock();

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Calcula pacotes esperados com base no tamanho fixo dos pacotes
    expected_packets = (10 * 1024 * 1024 + BUFFER_SIZE - 1) / BUFFER_SIZE;

    printf("Arquivo recebido com sucesso.\n");
    printf("Taxa de download: %.2f bytes/segundo\n", 10 * 1024 * 1024 / time_spent);
    printf("Pacotes esperados: %d, Pacotes recebidos: %d\n", expected_packets, received_packets);
    printf("Porcentagem de perda de pacotes: %.2f%%\n",
           100.0 * (expected_packets - received_packets) / expected_packets);

    fclose(file);
    close(client_socket);

    return 0;
}
