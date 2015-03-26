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
#define TAMALFABETO 26

// Estrutura da Árvore Patricia

struct nodo {
    char * prefixo, *flag;
    struct nodo *p[26];
};

// Declarando raiz
struct nodo *root = NULL;

// Protótipos de funções
void read_word(int argc, char** argv);
void inserir(char* palavra, struct nodo **p);
void imprimir(struct nodo** root, int filho);
void abreColchetes();
void fechaColchetes();

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

    // Função de impressão
    imprimir(&root, 0);

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
            printf(" word[%i]: %s\n", count, word);
            inserir(word, &root);
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
    int i = 0, count = 0, j, tamPrefix;

    printf("\npalavra: %s", palavra);
    printf("\nponteiro: %p\n", p);

   

    if (*p == NULL) {
        *p = calloc(1, sizeof (struct nodo));
        (*p)->prefixo = strdup(palavra);
        (*p)->flag = calloc(strlen(palavra) + 1, sizeof (char)); // +1 para incluir o '\0'
        (*p)->flag[strlen(palavra)] = 1;
    }

    tamPrefix = strlen((*p)->prefixo);

    for (i = 0; i < tamPrefix; ++i) {
        if (palavra[i] != (*p)->prefixo[i])
            break; //ou palavra acabou ou as letras são diferentes
    }

    if (i < tamPrefix) {
        if (palavra[i] == '\0')
            (*p)->flag[i] = 1;
        else {
            struct nodo * pai = calloc(1, sizeof (struct nodo));
            pai->prefixo = strndup((*p)->prefixo, i);
            pai->flag = calloc(i + 1, sizeof (char));
            // Copiando as flags
            for (j = 0; j < tamPrefix; ++j)
                pai->flag[j] = (*p)->flag[j];
            if (palavra + i + 1 != '\0') {
                inserir(palavra + i + 1, &pai->p[palavra[i] - 'a']);
            }

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
                int tamPalavra = strlen(palavra);
                (*p)->flag = realloc((*p)->flag, tamPalavra + 1);
                for (j = tamPrefix + 1; j < tamPalavra; j++)
                    (*p)->flag[j] = 0;
                (*p)->flag[j] = 1;
                free((*p)->prefixo);
                (*p)->prefixo = strdup(palavra);
            }

        }
    }
}

void imprimir(struct nodo** p, int filho) {
    int i, j, tamPrefix, contaColhetes = 0;

    if (p == NULL) {
        printf("\n Árvore vazia");
        return;
    }

    tamPrefix = strlen((*p)->prefixo);
    //printf("\ntamanho do prefixo: %d", tamPrefix);

    //printf("\n[");
    if (filho == 0) {
        abreColchetes();
    }

    // Impressão do prefixo

    for (i = 0; i < tamPrefix; ++i) {
        //printf("\nflag: %d", (*p)->flag[i]);

        if ((*p)->flag[i + 1] == 1)
            printf("%c", (*p)->prefixo[i] - 32); // Impressão de maiúsculas
        else
            printf("%c", (*p)->prefixo[i]);
    }

    // Verificar se há filhos
    for (j = 0; j < TAMALFABETO; ++j) {
        if ((*p)->p[j] != NULL) {
            // printf("\n%d:", j);
            abreColchetes();
            //
            if (strlen((*p)->p[j]->prefixo) == 0) {
                if (filho == 2)
                    printf("%c", j + 65);
                else
                    printf("%c", j + 97);
                imprimir(&(*p)->p[j], 2);
                // break;
            } else {
                printf("%c", j + 97); // Imprime a letra correspondente ao ponteiro
                imprimir(&(*p)->p[j], 1);
            }
        }
    }

    // Fechando chaves dos filhos
    fechaColchetes();
}

void abreColchetes() {
    printf("[");
}

void fechaColchetes() {
    printf("]");
}