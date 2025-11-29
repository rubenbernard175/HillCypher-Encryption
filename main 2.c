/*Assignment: pa01 - Encrypting a plaintext file using the Hill Cypher
Author: Ruben Bernard
Language: C
To compile: gcc -o pa01 pa01.c
To execute: -> ./pa01 kX.txt pX.txt
Class: CIS3360 - SECURITY IN COMPUTING - FALL 2024
Instructor: McAlpin
Due date: 9-30-2024
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// read matrix text from the key file
// opens files, reads file and matrix size
int** readFile(char *filename, int *m){
    FILE *key = fopen(filename, "r"); 
    if (key == NULL) exit(1);

    fscanf(key, "%d", m); 
    int **matrix = malloc(sizeof(int*) * *m);

    for (int i = 0; i < *m; i++){
        matrix[i] = malloc(*m * sizeof(int));
    }

    for (int i = 0; i < *m; i++){
        for (int j = 0; j < *m; j++){
            fscanf(key, "%d", &matrix[i][j]);
        }
    }

    fclose(key);
    return matrix;
}

char* rPlain(char *filename, int n, int *tLength) {
    FILE *plainTextFile = fopen(filename, "r");
    if (plainTextFile == NULL) exit(1);

    char *plainText = malloc(10000 * sizeof(char));
    if (plainText == NULL){
        printf("Memory allocation failed.\n");
        fclose(plainTextFile);
        exit(1);
    }

    int index = 0;
    char ch;

    while ((ch = fgetc(plainTextFile)) != EOF){
        if (isalpha(ch)){
            plainText[index++] = tolower(ch);
        }
    }

    while (index % n != 0){
        plainText[index++] = 'x';
    }

    *tLength = index;
    fclose(plainTextFile);
    return plainText;
}

void matrixVector(int **matrix, int *vector, int *result, int n){
    for (int i = 0; i < n; i++){
        result[i] = 0;
        for (int j = 0; j < n; j++){
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= 26;
    }
}

char* textencryption(int **key, char *plaintext, int n, int length){
    char *ciphertext = malloc(length * sizeof(char));
    int vector[n];
    int result[n];

    for (int i = 0; i < length; i += n){
        for (int j = 0; j < n; j++){
            vector[j] = plaintext[i + j] - 'a';
        }
        matrixVector(key, vector, result, n);

        for (int j = 0; j < n; j++){
            ciphertext[i + j] = result[j] + 'a';
        }
    }
    return ciphertext;
}

void pCipher(char *ciphertext, int length){
    for (int i = 0; i < length; i++){
        printf("%c", ciphertext[i]);
        if ((i + 1) % 80 == 0) printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage: %s keyfile plaintextfile\n", argv[0]);
        return 1;
    }

    int m;
    int **key = readFile(argv[1], &m);
    int length;
    char *plaintext = rPlain(argv[2], m, &length);

    printf("\nKey matrix:\n");
    for (int i = 0; i < m; i++){
        for (int j = 0; j < m; j++){
            printf("%4d", key[i][j]);
        }
        printf("\n");
    }

    printf("\nPlaintext:\n");
    for (int i = 0; i < length; i++){
        printf("%c", plaintext[i]);
        if ((i + 1) % 80 == 0) printf("\n");
    }
    printf("\n");

    char *cipher = textencryption(key, plaintext, m, length);

    printf("\nCiphertext:\n");
    pCipher(cipher, length);

    for (int i = 0; i < m; i++){
        free(key[i]);
    }
    free(key);
    free(plaintext);
    free(cipher);

    return 0;
}

/*=============================================================================
| I RUBEN BERNARD (ru692104) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/