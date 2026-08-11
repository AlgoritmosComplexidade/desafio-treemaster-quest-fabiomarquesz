#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura base do documento, conforme especificado no desafio
typedef struct Documento {
    int id;
    char titulo[100];
    char autor[50];
    int ano_publicacao;
} Documento;

// Estrutura para um nó da árvore binária de busca
typedef struct No {
    Documento doc;
    struct No* esquerdo; // Ponteiro para documentos com ID menor
    struct No* direito;  // Ponteiro para documentos com ID maior
} No;

// Criando um apelido para facilitar a leitura do código
typedef No* Arvore;

// Função responsável por alocar memória e criar um novo nó folha
No* criar_no(int id, char* titulo, char* autor, int ano) {
    No* novo = (No*)malloc(sizeof(No));
    
    // Verificação simples de segurança para alocação de memória
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    
    // Copiando os dados recebidos para dentro do nó
    novo->doc.id = id;
    strcpy(novo->doc.titulo, titulo);
    strcpy(novo->doc.autor, autor);
    novo->doc.ano_publicacao = ano;
    
    // Como é um nó novo (folha), não tem filhos ainda
    novo->esquerdo = NULL;
    novo->direito = NULL;
    
    return novo;
}

// Função recursiva para inserir um documento na Árvore Binária de Busca
Arvore inserir(Arvore raiz, int id, char* titulo, char* autor, int ano) {
    // Se a posição estiver vazia, cria o nó e o insere aqui
    if (raiz == NULL) {
        return criar_no(id, titulo, autor, ano);
    }
    
    // Regra da árvore de busca: IDs menores vão para a esquerda
    if (id < raiz->doc.id) {
        raiz->esquerdo = inserir(raiz->esquerdo, id, titulo, autor, ano);
    } 
    // IDs maiores vão para a direita
    else if (id > raiz->doc.id) {
        raiz->direito = inserir(raiz->direito, id, titulo, autor, ano);
    }
    
    // Retorna a raiz atual para manter a árvore conectada
    return raiz;
}

// Função recursiva para percorrer a árvore "em-ordem" (Esquerda -> Raiz -> Direita)
// O ponteiro 'contador' é usado para numerar a ordem de exibição ("Documento 1", "Documento 2")
void exibir_em_ordem(Arvore raiz, int* contador) {
    if (raiz != NULL) {
        // 1. Desce tudo para a esquerda (menores IDs primeiro)
        exibir_em_ordem(raiz->esquerdo, contador);
        
        // 2. Processa o nó atual (imprime os dados)
        printf("Documento %d:\n", *contador);
        printf("ID: %d\n", raiz->doc.id);
        printf("Título: %s\n", raiz->doc.titulo);
        printf("Autor: %s\n", raiz->doc.autor);
        printf("Ano de publicação: %d\n\n", raiz->doc.ano_publicacao);
        
        // Incrementa o contador para o próximo documento
        (*contador)++;
        
        // 3. Desce tudo para a direita (maiores IDs)
        exibir_em_ordem(raiz->direito, contador);
    }
}

int main() {
    Arvore raiz = NULL; // Inicializa a base da biblioteca como vazia
    
    // Variáveis temporárias para ler as entradas do usuário
    int id, ano;
    char titulo[100], autor[50];
    
    printf("=== Bem-vindo ao Sistema CodeForest Solutions ===\n\n");
    
    // --- CADASTRO DO PRIMEIRO DOCUMENTO ---
    printf("--- Cadastro do Primeiro Documento ---\n");
    
    printf("Digite o ID: ");
    scanf("%d", &id);
    
    printf("Digite o Título: ");
    // O espaço antes do % consome a quebra de linha ('\n') deixada pelo scanf anterior
    // O [^\n] diz para o C ler toda a frase até o usuário apertar Enter
    scanf(" %99[^\n]", titulo); 
    
    printf("Digite o Autor: ");
    scanf(" %49[^\n]", autor);
    
    printf("Digite o Ano de publicação: ");
    scanf("%d", &ano);
    
    // Insere o primeiro documento na árvore
    raiz = inserir(raiz, id, titulo, autor, ano);
    printf("Documento cadastrado com sucesso!\n\n");
    
    
    // --- CADASTRO DO SEGUNDO DOCUMENTO ---
    printf("--- Cadastro do Segundo Documento ---\n");
    
    printf("Digite o ID: ");
    scanf("%d", &id);
    
    printf("Digite o Título: ");
    scanf(" %99[^\n]", titulo); 
    
    printf("Digite o Autor: ");
    scanf(" %49[^\n]", autor);
    
    printf("Digite o Ano de publicação: ");
    scanf("%d", &ano);
    
    // Insere o segundo documento na árvore
    raiz = inserir(raiz, id, titulo, autor, ano);
    printf("Documento cadastrado com sucesso!\n\n");
    
    
    // --- EXIBIÇÃO DOS DADOS ---
    printf("=== Lista de Documentos Cadastrados ===\n\n");
    
    // Iniciamos o contador em 1 para a formatação pedida (Documento 1, Documento 2...)
    int contador_exibicao = 1; 
    
    // Chamamos a função passando a raiz e o endereço do contador
    exibir_em_ordem(raiz, &contador_exibicao);
    
    return 0;
}
