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
    char *prefixo, *flag;
    struct nodo *p[26];
};

// Declarando raiz
struct nodo *root = NULL;

// Protótipos de funções
void inserir(char* palavra, struct nodo **p);
void imprimir(struct nodo** root, int filho);
void abreColchetes();
void fechaColchetes();
int verifica(struct nodo** p);

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    int i, count;
    char caracter, word[MAX];

    // loop to read characters
    while ((caracter) != EOF) {
        i = 0;
        caracter = getchar();
        while ((caracter >= 65) && (caracter <= 122)) { //A-z
            word[i] = caracter;
            caracter = getchar();
            i++;
        }
        // word's insertion
        if (i) {
            count++; //contador de palavras
            word[i] = '\0';
            //printf("word: %s", word);
            inserir(word, &root);
        }
    }

    // Função de impressão
    imprimir(&root, 0);

    printf("\n");

    return (EXIT_SUCCESS);
}

void inserir(char *palavra, struct nodo** p) {
    int i, j;

    if (*p == NULL) {
        (*p) = calloc(1, sizeof (struct nodo));
        (*p)->prefixo = strdup(palavra);
        (*p)->flag = calloc(strlen(palavra) + 1, sizeof (char));
        (*p)->flag[strlen(palavra)] = 1;
    }

    for (i = 0; i < strlen((*p)->prefixo); i++)
        if (palavra[i] != (*p)->prefixo[i])
            break;

    if (i < strlen((*p)->prefixo)) {
        if (palavra[i] == '\0')
            (*p)->flag[i] = 1;
        else {
            struct nodo *pai = calloc(1, sizeof (struct nodo));
            pai->prefixo = strndup((*p)->prefixo, i);
            pai->flag = calloc(i + 1, sizeof (char));

            for (j = 0; j <= i; j++)
                pai->flag[j] = (*p)->flag[j];

            inserir(palavra + i + 1, &pai->p[palavra[i] - 'a']);
            pai->p[(*p)->prefixo[i] - 'a'] = *p;
            j = (strlen((*p)->prefixo) - i) * sizeof (char); // ver se é +1 ou +i

            memmove((*p)->prefixo, (*p)->prefixo + i + 1, j);
            (*p)->prefixo = realloc((*p)->prefixo, j);

            memmove((*p)->flag, (*p)->flag + i + 1, j);
            (*p)->flag = realloc((*p)->flag, j);

            *p = pai; //seta o root como pai

        }
    } else if (palavra[i] == '\0') {
        (*p)->flag[i] = 1;
    } else {
        for (j = 0; j < 26; j++)
            if ((*p)->p[j] != NULL)
                break;
        if (j < 26)
            inserir(palavra + i + 1, &(*p)->p[palavra[i] - 'a']);
        else { //aumenta o prefixo
            (*p)->flag = realloc((*p)->flag, strlen(palavra) + 1);
            for (j = strlen((*p)->prefixo) + 1; j < strlen(palavra); j++)
                (*p)->flag[j] = 0;
            (*p)->flag[j] = 1;
            free((*p)->prefixo);

            (*p)->prefixo = strdup(palavra);
        }
    }
}


/**
 * 
 * @param palavra string
 * @param p ponteiro para estrutura
 */

/*

void inserir(char *palavra, struct nodo **p) {
    int i = 0, j, tamPrefix;

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
 */

void imprimir(struct nodo** p, int filho) {
    int i, j, tamPrefix, verif = 0;

    if (p == NULL) {
        return;
    }

    tamPrefix = strlen((*p)->prefixo);
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
            abreColchetes();

            if (strlen((*p)->p[j]->prefixo) > 0) {
                if ((*p)->p[j]->flag[0] == 1) {
                    printf("%c", j + 65);
                } else
                    printf("%c", j + 97);
                imprimir(&(*p)->p[j], 2);
            }
            // casa, caso
            if (strlen((*p)->p[j]->prefixo) == 0) {
                verif = verifica(&(*p)->p[j]);
                if (verif == 1)
                    printf("%c", j + 97);
                else
                    printf("%c", j + 65);
                imprimir(&(*p)->p[j], 2);
            }



        }
    }

    fechaColchetes();
}

void abreColchetes() {
    printf("[");
}

void fechaColchetes() {
    printf("]");
}

/**
 * @param p
 * @return 1, caso haja um ponteirio válido
 */
int verifica(struct nodo** p) {
    int tamPrefix, j;

    tamPrefix = strlen((*p)->prefixo);

    if (tamPrefix == 0) {
        for (j = 0; j < TAMALFABETO; ++j) {
            if ((*p)->p[j] != NULL)
                return 1;
        }
    }

    return 0;
}