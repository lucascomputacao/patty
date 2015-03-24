/* 
 * File:   patty.c
 * Author: Lucas Borges <lucascomputacao at gmail.com>
 *
 * Created on 23 de Março de 2015, 16:09
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000000

// Estrutura da Árvore Patricia

struct nodo {
    char * prefixo, *flag;
    struct nodo *p[26];
};

// Declarando raiz
struct nodo *root = NULL;

// Protótipos de funções
void read_word(int argc, char** argv);

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    FILE *arquivo;
    long tamanho;

    printf("\nIniciando programa...\n");

    arquivo = fopen(argv[1], "r");

    if (arquivo != NULL) {
        // to set the position of the end of the file
        fseek(arquivo, 0, SEEK_END);
        // current position in the file
        tamanho = ftell(arquivo);
        //FILE * Tamarq[tamanho*tamanho];
        FILE * Tamarq[tamanho];
    }

    //funçao para ler as palavras do arquivo
    read_word(argc, argv);

    return (EXIT_SUCCESS);
}

/**
 * 
 * @param argc
 * @param argv
 */
void read_word(int argc, char** argv) {
    FILE *arquivo;
    int i, count = 0;
    char caracter, word[MAX];
    //long tamanho;
    printf("\nFunção de leitura de palavras...\n");

    if ((arquivo = fopen(argv[1], "r")) == NULL) {
        printf("Erro ao abrir arquivo!!!\n\n");
        exit(1);
    }

    // loop to read characters
    while ((caracter) != EOF) {
        i = 0;
        caracter = fgetc(arquivo);
        while ((caracter >= 65) && (caracter <= 122)) { //A-z
            word[i] = caracter;
            caracter = fgetc(arquivo);
            i++;
        }
        // word's insertion
        if (i) {
            count++; //contador de palavras
            word[i] = '\0';
            printf(" word[%i]: %s\n",count, word);
        }
    }
    fclose(arquivo);
}

/**
 * 
 * @param palavra string
 * @param p ponteiro para estrutura
 */
void inserir(char *palavra, struct nodo **p) {
    int i, count = 0, j;

    int tam_prefixo = strlen((*p)->prefixo);

    if (*p == NULL) {
        *p = calloc(1, sizeof (struct nodo));
        (*p)->prefixo = strdup(palavra);
        (*p)->flag = calloc(strlen(palavra) + 1, sizeof (char)); // +1 para incluir o '\0'
        (*p)->flag[strlen(palavra)] = 1;
    }
    for (i = 0; i < tam_prefixo; ++i) {
        if (palavra[i] != (*p)->prefixo[i])
            break; //ou palavra acabou ou as letras são diferentes
    }

    if (i < tam_prefixo) {
        if (palavra[i] == '\0')
            (*p)->flag[i] = 1;
        else {
            struct nodo * pai = calloc(1, sizeof (struct nodo));
            pai->prefixo = strndup((*p)->prefixo, i);
            pai->flag = calloc(i + 1, sizeof (char));
            for (j = 0; j < count; ++j)
                pai->flag[j] = (*p)->flag[j];
            inserir(palavra + i + 1, &pai->p[palavra[i] - 'a']);
            pai->p[(*p)->prefixo[i] - 'a'] = *p;
            j = (strlen((*p)->prefixo) - i) * sizeof (char);
            memmove((*p)->prefixo, (*p)->prefixo + i + 1, j);
            (*p)->prefixo = realloc((*p)->prefixo, j);
            memmove((*p)->flag, (*p)->flag + i + 1, j);
            (*p)->flag = realloc((*p)->flag, j);
            *p = pai;
        }
    } else {
        if (palavra[i] == '\0')
            (*p)->flag[i] = 1;
        else {
            for (j = 0; j < 26; ++j) {
                if ((*p)->p[j] != NULL)
                    break;
            }
            if (j < 26)
                inserir(palavra + i + 1, &(*p)->p[palavra[i] - 'a']);
            else {
                (*p)->flag = realloc((*p)->flag, strlen(palavra) + 1);
                for (j = strlen((*p)->prefixo) + 1; j < strlen(palavra); j++)
                    (*p)->flag[j] = 0;
                (*p)->flag[j] = 1;
                free((*p)->prefixo);
                (*p)->prefixo = strdup(palavra);
            }

        }
    }
}
