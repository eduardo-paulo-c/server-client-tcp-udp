#Trabalho de Redes de computadores

##Como rodar:
1 - Criar arquivo binario:
#include <stdio.h>
#include <stdlib.h>

#define FILE_SIZE (10L * 1024 * 1024) // 10 MB
#define CHUNK_SIZE 1024              // 1 KB

int main() {
    FILE *file = fopen("arquivo_10mb.bin", "wb");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    char buffer[CHUNK_SIZE] = {0}; // Bloco de 1 KB preenchido com zeros

    for (long i = 0; i < FILE_SIZE / CHUNK_SIZE; i++) {
        if (fwrite(buffer, 1, CHUNK_SIZE, file) != CHUNK_SIZE) {
            perror("Erro ao escrever no arquivo");
            fclose(file);
            return 1;
        }
    }

    printf("Arquivo de 10 MB criado com sucesso para teste de transferência.\n");
    fclose(file);
    return 0;
}

2 - Criar executaveis:
make

3 - Executar primeiro o servidor e depois o cliente
./serverTCP
./clientTCP
./serverUDP
./clientUDP
