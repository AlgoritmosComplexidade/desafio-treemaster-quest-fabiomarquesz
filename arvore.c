#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar os dados do documento
typedef struct Documento {
    int id;
    char titulo[100];
    char autor[50];
    int ano_publicacao;
} Documento;

// Estrutura para o nó da Árvore Binária de Busca
typedef struct No {
    Documento documento;
    struct No* esquerda;
    struct No* direita;
} No;

// Cria um novo nó da árvore
No* criar_no(int id, char* titulo, char* autor, int ano) {
    No* novo_no = (No*)malloc(sizeof(No));

    if (novo_no == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo_no->documento.id = id;
    strcpy(novo_no->documento.titulo, titulo);
    strcpy(novo_no->documento.autor, autor);
    novo_no->documento.ano_publicacao = ano;

    novo_no->esquerda = NULL;
    novo_no->direita = NULL;

    return novo_no;
}

// Insere recursivamente um nó na Árvore Binária de Busca
No* inserir_no(No* raiz, int id, char* titulo, char* autor, int ano) {
    if (raiz == NULL) {
        return criar_no(id, titulo, autor, ano);
    }

    if (id < raiz->documento.id) {
        raiz->esquerda = inserir_no(
            raiz->esquerda,
            id,
            titulo,
            autor,
            ano
        );
    } 
    else if (id > raiz->documento.id) {
        raiz->direita = inserir_no(
            raiz->direita,
            id,
            titulo,
            autor,
            ano
        );
    }

    return raiz;
}

// Percorre a árvore em ordem: Esquerda -> Raiz -> Direita
void exibir_em_ordem(No* raiz, int* contador) {
    if (raiz != NULL) {
        exibir_em_ordem(raiz->esquerda, contador);

        printf("Documento %d:\n", *contador);
        printf("ID: %d\n", raiz->documento.id);
        printf("Titulo: %s\n", raiz->documento.titulo);
        printf("Autor: %s\n", raiz->documento.autor);
        printf("Ano de publicacao: %d\n\n",
               raiz->documento.ano_publicacao);

        (*contador)++;

        exibir_em_ordem(raiz->direita, contador);
    }
}

// Calcula a altura da árvore (número de níveis)
int calcular_altura(No* raiz) {
    if (raiz == NULL) {
        return 0; // Árvore vazia possui altura 0
    }

    int altura_esquerda = calcular_altura(raiz->esquerda);
    int altura_direita = calcular_altura(raiz->direita);

    // Retorna a maior altura entre esquerda e direita,
    // adicionando 1 para representar o nível atual
    if (altura_esquerda > altura_direita) {
        return altura_esquerda + 1;
    } 
    else {
        return altura_direita + 1;
    }
}

// Função auxiliar do algoritmo DSW:
// realiza rotações para a esquerda para comprimir a "videira"
void dsw_comprimir(No* auxiliar, int quantidade) {
    No* percorrimento = auxiliar;

    for (int i = 0; i < quantidade; i++) {
        No* filho = percorrimento->direita;
        No* filho_direito = filho->direita;

        // Executa uma rotação para a esquerda
        filho->direita = filho_direito->esquerda;
        filho_direito->esquerda = filho;
        percorrimento->direita = filho_direito;

        percorrimento = filho_direito;
    }
}

// Algoritmo de Balanceamento Day-Stout-Warren (DSW)
void dsw_balancear(No** raiz) {
    if (*raiz == NULL) {
        return;
    }

    // Nó auxiliar apontando para a raiz real.
    // Ele facilita a realização das rotações.
    No* auxiliar = criar_no(0, "", "", 0);
    auxiliar->direita = *raiz;

    // ============================================================
    // FASE 1: Transformar a árvore em uma "videira"
    // Utiliza rotações para a direita
    // ============================================================

    No* cauda = auxiliar;
    No* restante = cauda->direita;
    int quantidade_nos = 0;

    while (restante != NULL) {

        if (restante->esquerda == NULL) {
            cauda = restante;
            restante = restante->direita;
            quantidade_nos++;
        } 
        else {

            // Executa uma rotação para a direita
            No* temporario = restante->esquerda;

            restante->esquerda = temporario->direita;
            temporario->direita = restante;
            restante = temporario;

            cauda->direita = temporario;
        }
    }

    // ============================================================
    // FASE 2: Transformar a "videira" em uma árvore balanceada
    // Utiliza rotações para a esquerda
    // ============================================================

    int potencia_de_2 = 1;

    // Encontra a maior potência de 2 menor ou igual a
    // (quantidade_nos + 1)
    while (potencia_de_2 <= quantidade_nos + 1) {
        potencia_de_2 *= 2;
    }

    potencia_de_2 /= 2;

    int folhas = quantidade_nos + 1 - potencia_de_2;

    dsw_comprimir(auxiliar, folhas);

    quantidade_nos = quantidade_nos - folhas;

    while (quantidade_nos > 1) {
        dsw_comprimir(auxiliar, quantidade_nos / 2);
        quantidade_nos /= 2;
    }

    // Define a nova raiz e libera o nó auxiliar
    *raiz = auxiliar->direita;

    free(auxiliar);
}

int main() {

    No* raiz = NULL;

    int id, ano;
    char titulo[100];
    char autor[50];

    printf("=== Sistema de Indexacao CodeForest (Aventureiro) ===\n\n");

    // ============================================================
    // CADASTRO LINEAR
    // Sem utilização de laços, conforme a regra do desafio
    // ============================================================

    // Documento 1
    printf("--- Cadastro do Primeiro Documento ---\n");

    printf("Digite o ID: ");
    scanf("%d", &id);

    printf("Digite o Titulo: ");
    scanf(" %99[^\n]", titulo);

    printf("Digite o Autor: ");
    scanf(" %49[^\n]", autor);

    printf("Digite o Ano de publicacao: ");
    scanf("%d", &ano);

    raiz = inserir_no(raiz, id, titulo, autor, ano);

    printf("Documento cadastrado com sucesso!\n\n");


    // Documento 2
    printf("--- Cadastro do Segundo Documento ---\n");

    printf("Digite o ID: ");
    scanf("%d", &id);

    printf("Digite o Titulo: ");
    scanf(" %99[^\n]", titulo);

    printf("Digite o Autor: ");
    scanf(" %49[^\n]", autor);

    printf("Digite o Ano de publicacao: ");
    scanf("%d", &ano);

    raiz = inserir_no(raiz, id, titulo, autor, ano);

    printf("Documento cadastrado com sucesso!\n\n");


    // ============================================================
    // EXIBIÇÃO EM ORDEM E MÉTRICAS
    // ============================================================

    printf("=== Documentos cadastrados ===\n");

    int contador_exibicao = 1;

    exibir_em_ordem(raiz, &contador_exibicao);


    // Calcula a altura antes do balanceamento DSW
    int altura_antes = calcular_altura(raiz);


    // Aplica o algoritmo de balanceamento DSW
    printf("Aplicando algoritmo de balanceamento DSW...\n");

    dsw_balancear(&raiz);


    // Calcula a altura após o balanceamento DSW
    int altura_depois = calcular_altura(raiz);


    // Exibe as métricas de desempenho
    printf("\n=== Metricas de desempenho ===\n");

    printf(
        "Altura da arvore antes do balanceamento: %d\n",
        altura_antes
    );

    printf(
        "Altura da arvore apos o balanceamento DSW: %d\n",
        altura_depois
    );

    return 0;
}
