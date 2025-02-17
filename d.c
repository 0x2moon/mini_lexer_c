#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char next_delimiter = '\0'; // Armazena delimitadores para o próximo token

int is_delimiter(char c) {
    // Verifica se o caractere é um delimitador ou operador de um único caractere
    return strchr(",(){}[];+-*/%&|^~!=<>", c) != NULL;
}

void get_next_token(FILE *file, char **character_sequence) {
    int size = 0;
    char buffer;

    // Se houver um delimitador salvo, retorne-o como token
    if (next_delimiter != '\0') {
        *character_sequence = (char *)calloc(2, sizeof(char));
        (*character_sequence)[0] = next_delimiter;
        (*character_sequence)[1] = '\0';
        next_delimiter = '\0';
        return;
    }

    // Lê o próximo caractere se não houver delimitador pendente
    if (fread(&buffer, sizeof(char), 1, file) != 1) {
        *character_sequence = NULL; // Fim do arquivo
        return;
    }

    // Se o caractere inicial for um delimitador
    if (is_delimiter(buffer)) {
        *character_sequence = (char *)calloc(2, sizeof(char));
        (*character_sequence)[0] = buffer;
        (*character_sequence)[1] = '\0';
        return;
    }

    // Inicializa o lexema com o primeiro caractere
    *character_sequence = (char *)calloc(1, sizeof(char));
    size = 1;
    (*character_sequence)[0] = buffer;

    // Continua lendo caracteres
    while (1) {
        if (fread(&buffer, sizeof(char), 1, file) != 1) {
            break; // Fim do arquivo ou erro
        }

        if (is_delimiter(buffer)) {
            next_delimiter = buffer; // Salva o delimitador para o próximo token
            break;
        } else if (isspace(buffer)) {
            break; // Espaço em branco termina o lexema
        } else {
            // Adiciona o caractere ao lexema
            size++;
            *character_sequence = realloc(*character_sequence, size + 1);
            (*character_sequence)[size - 1] = buffer;
        }
    }

    // Adiciona terminador nulo
    (*character_sequence)[size] = '\0';
}

// Exemplo de uso:
int main() {
    FILE *file = fopen("exemplo.c", "r");
    char *token = NULL;

    while (1) {
        get_next_token(file, &token);
        if (token == NULL) break;

        printf("Token: %s\n", token);
        free(token);
    }

    fclose(file);
    return 0;
}