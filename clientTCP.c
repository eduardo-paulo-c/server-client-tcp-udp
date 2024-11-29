#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define FILENAME "received_file.bin"

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    FILE *file;
    ssize_t bytes_received;
    clock_t start, end;
    double time_spent;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Erro ao conectar ao servidor");
        exit(EXIT_FAILURE);
    }

    file = fopen(FILENAME, "wb");
    if (!file)
    {
        perror("Erro ao criar arquivo");
        exit(EXIT_FAILURE);
    }

    start = clock();
    while ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, 1, bytes_received, file);
    }
    end = clock();

    if (bytes_received == -1)
    {
        perror("Erro ao receber dados");
        exit(EXIT_FAILURE);
    }

    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Arquivo recebido com sucesso.\n");
    printf("Taxa de download: %.2f bytes/segundo\n", 10 * 1024 * 1024 / time_spent);

    fclose(file);
    close(client_fd);

    return 0;
}
