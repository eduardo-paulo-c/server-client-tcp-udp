#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define FILENAME "file.bin"

void send_file(FILE *file, int client_socket) {
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) == -1) {
            perror("Erro ao enviar dados");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao fazer bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) == -1) {
        perror("Erro ao escutar");
        exit(EXIT_FAILURE);
    }

    printf("Aguardando conexão...\n");
    client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket == -1) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    send_file(file, client_socket);
    printf("Arquivo enviado com sucesso.\n");

    fclose(file);
    close(client_socket);
    close(server_fd);

    return 0;
}
