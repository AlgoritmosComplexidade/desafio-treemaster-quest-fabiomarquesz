
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * ============================================================
 * ESTRUTURA DE DADOS DO DOCUMENTO
 * ============================================================
 *
 * O ID utiliza unsigned long int para permitir números maiores.
 */
typedef struct Documento {
    unsigned long int id;
    char titulo[100];
    char autor[50];
    int ano_publicacao;
} Documento;


/*
 * ============================================================
 * ESTRUTURA DO NÓ DA ÁRVORE
 * ============================================================
 *
 * A estrutura possui:
 * - Documento armazenado
 * - Ponteiro para filho esquerdo
 * - Ponteiro para filho direito
 * - Altura do nó, utilizada pelo algoritmo AVL
 */
typedef struct No {
    Documento documento;

    struct No* esquerda;
    struct No* direita;

    int altura;
} No;


/*
 * ============================================================
 * ESTRUTURA PARA ARMAZENAR AS MÉTRICAS
 * ============================================================
 *
 * Todas as métricas principais são armazenadas como float,
 * conforme solicitado no desafio.
 */
typedef struct Metricas {
    float altura_antes;
    float altura_depois;
    float numero_rotacoes;
    float tempo_execucao;
} Metricas;


/*
 * ============================================================
 * FUNÇÃO PARA OBTER A ALTURA DE UM NÓ
 * ============================================================
 */
int obter_altura(No* no) {
    if (no == NULL) {
        return 0;
    }

    return no->altura;
}


/*
 * ============================================================
 * FUNÇÃO PARA CALCULAR O MAIOR VALOR ENTRE DOIS NÚMEROS
 * ============================================================
 */
int maior(int primeiro, int segundo) {
    if (primeiro > segundo) {
        return primeiro;
    }

    return segundo;
}


/*
 * ============================================================
 * CRIA UM NOVO NÓ
 * ============================================================
 */
No* criar_no(
    unsigned long int id,
    char* titulo,
    char* autor,
    int ano
) {
    No* novo_no = (No*)malloc(sizeof(No));

    if (novo_no == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    novo_no->documento.id = id;

    strcpy(
        novo_no->documento.titulo,
        titulo
    );

    strcpy(
        novo_no->documento.autor,
        autor
    );

    novo_no->documento.ano_publicacao = ano;

    novo_no->esquerda = NULL;
    novo_no->direita = NULL;

    /*
     * Todo novo nó começa com altura 1.
     */
    novo_no->altura = 1;

    return novo_no;
}


/*
 * ============================================================
 * ROTAÇÃO SIMPLES PARA A DIREITA
 * ============================================================
 *
 * Utilizada nos casos:
 *
 *        A
 *       /
 *      B
 *
 * Transformando em:
 *
 *      B
 *       \
 *        A
 */
No* rotacao_direita(No* no) {

    No* novo_no = no->esquerda;
    No* subarvore = novo_no->direita;

    novo_no->direita = no;
    no->esquerda = subarvore;

    /*
     * Atualiza as alturas após a rotação.
     */
    no->altura = 1 + maior(
        obter_altura(no->esquerda),
        obter_altura(no->direita)
    );

    novo_no->altura = 1 + maior(
        obter_altura(novo_no->esquerda),
        obter_altura(novo_no->direita)
    );

    return novo_no;
}


/*
 * ============================================================
 * ROTAÇÃO SIMPLES PARA A ESQUERDA
 * ============================================================
 *
 * Utilizada nos casos:
 *
 *      A
 *       \
 *        B
 *
 * Transformando em:
 *
 *        B
 *       /
 *      A
 */
No* rotacao_esquerda(No* no) {

    No* novo_no = no->direita;
    No* subarvore = novo_no->esquerda;

    novo_no->esquerda = no;
    no->direita = subarvore;

    /*
     * Atualiza as alturas após a rotação.
     */
    no->altura = 1 + maior(
        obter_altura(no->esquerda),
        obter_altura(no->direita)
    );

    novo_no->altura = 1 + maior(
        obter_altura(novo_no->esquerda),
        obter_altura(novo_no->direita)
    );

    return novo_no;
}


/*
 * ============================================================
 * CALCULA O FATOR DE BALANCEAMENTO
 * ============================================================
 *
 * Fator = altura da esquerda - altura da direita
 *
 * Valores permitidos na AVL:
 *
 * -1
 *  0
 * +1
 */
int fator_balanceamento(No* no) {

    if (no == NULL) {
        return 0;
    }

    return obter_altura(no->esquerda)
         - obter_altura(no->direita);
}


/*
 * ============================================================
 * INSERÇÃO NA ÁRVORE AVL
 * ============================================================
 *
 * A inserção é semelhante a uma ABB.
 *
 * Após inserir:
 *
 * 1. Atualiza a altura.
 * 2. Calcula o fator de balanceamento.
 * 3. Executa rotação simples ou dupla quando necessário.
 *
 * A variável ponteiro_rotacoes contabiliza todas as rotações.
 */
No* inserir_avl(
    No* raiz,
    unsigned long int id,
    char* titulo,
    char* autor,
    int ano,
    int* ponteiro_rotacoes
) {

    /*
     * Caso a posição esteja vazia,
     * cria um novo nó.
     */
    if (raiz == NULL) {

        return criar_no(
            id,
            titulo,
            autor,
            ano
        );
    }


    /*
     * Inserção na subárvore esquerda.
     */
    if (id < raiz->documento.id) {

        raiz->esquerda = inserir_avl(
            raiz->esquerda,
            id,
            titulo,
            autor,
            ano,
            ponteiro_rotacoes
        );
    }

    /*
     * Inserção na subárvore direita.
     */
    else if (id > raiz->documento.id) {

        raiz->direita = inserir_avl(
            raiz->direita,
            id,
            titulo,
            autor,
            ano,
            ponteiro_rotacoes
        );
    }

    /*
     * Se o ID já existir, não cria outro nó.
     */
    else {

        return raiz;
    }


    /*
     * Atualiza a altura do nó atual.
     */
    raiz->altura = 1 + maior(
        obter_altura(raiz->esquerda),
        obter_altura(raiz->direita)
    );


    /*
     * Calcula o fator de balanceamento.
     */
    int fator = fator_balanceamento(raiz);


    /*
     * ========================================================
     * CASO ESQUERDA-ESQUERDA
     * ========================================================
     *
     * Exemplo:
     *
     *       30
     *      /
     *     20
     *    /
     *   10
     *
     * Solução: rotação simples para a direita.
     */
    if (fator > 1 &&
        id < raiz->esquerda->documento.id) {

        (*ponteiro_rotacoes)++;

        return rotacao_direita(raiz);
    }


    /*
     * ========================================================
     * CASO DIREITA-DIREITA
     * ========================================================
     *
     * Exemplo:
     *
     *   10
     *     \
     *      20
     *        \
     *         30
     *
     * Solução: rotação simples para a esquerda.
     */
    if (fator < -1 &&
        id > raiz->direita->documento.id) {

        (*ponteiro_rotacoes)++;

        return rotacao_esquerda(raiz);
    }


    /*
     * ========================================================
     * CASO ESQUERDA-DIREITA
     * ========================================================
     *
     * Primeiro:
     * rotação para a esquerda no filho.
     *
     * Depois:
     * rotação para a direita na raiz.
     */
    if (fator > 1 &&
        id > raiz->esquerda->documento.id) {

        (*ponteiro_rotacoes)++;

        raiz->esquerda =
            rotacao_esquerda(raiz->esquerda);

        (*ponteiro_rotacoes)++;

        return rotacao_direita(raiz);
    }


    /*
     * ========================================================
     * CASO DIREITA-ESQUERDA
     * ========================================================
     *
     * Primeiro:
     * rotação para a direita no filho.
     *
     * Depois:
     * rotação para a esquerda na raiz.
     */
    if (fator < -1 &&
        id < raiz->direita->documento.id) {

        (*ponteiro_rotacoes)++;

        raiz->direita =
            rotacao_direita(raiz->direita);

        (*ponteiro_rotacoes)++;

        return rotacao_esquerda(raiz);
    }


    /*
     * Caso a árvore já esteja balanceada,
     * simplesmente retorna a raiz.
     */
    return raiz;
}


/*
 * ============================================================
 * INSERÇÃO NA ÁRVORE UTILIZADA PELO DSW
 * ============================================================
 *
 * Esta árvore funciona como uma ABB comum.
 *
 * O balanceamento não acontece durante a inserção.
 * Posteriormente, o algoritmo DSW será aplicado manualmente.
 */
No* inserir_arvore_dsw(
    No* raiz,
    unsigned long int id,
    char* titulo,
    char* autor,
    int ano
) {

    if (raiz == NULL) {

        return criar_no(
            id,
            titulo,
            autor,
            ano
        );
    }


    if (id < raiz->documento.id) {

        raiz->esquerda =
            inserir_arvore_dsw(
                raiz->esquerda,
                id,
                titulo,
                autor,
                ano
            );
    }

    else if (id > raiz->documento.id) {

        raiz->direita =
            inserir_arvore_dsw(
                raiz->direita,
                id,
                titulo,
                autor,
                ano
            );
    }


    /*
     * Atualiza a altura utilizada apenas
     * para as métricas.
     */
    raiz->altura = 1 + maior(
        obter_altura(raiz->esquerda),
        obter_altura(raiz->direita)
    );

    return raiz;
}


/*
 * ============================================================
 * PERCURSO EM ORDEM
 * ============================================================
 *
 * Esquerda -> Raiz -> Direita
 */
void exibir_em_ordem(
    No* raiz,
    int* contador
) {

    if (raiz != NULL) {

        exibir_em_ordem(
            raiz->esquerda,
            contador
        );

        printf(
            "Documento %d:\n",
            *contador
        );

        printf(
            "ID: %lu\n",
            raiz->documento.id
        );

        printf(
            "Titulo: %s\n",
            raiz->documento.titulo
        );

        printf(
            "Autor: %s\n",
            raiz->documento.autor
        );

        printf(
            "Ano de publicacao: %d\n\n",
            raiz->documento.ano_publicacao
        );

        (*contador)++;

        exibir_em_ordem(
            raiz->direita,
            contador
        );
    }
}


/*
 * ============================================================
 * CALCULA A ALTURA REAL DA ÁRVORE
 * ============================================================
 *
 * Esta função é utilizada para conferir as métricas.
 */
int calcular_altura(No* raiz) {

    if (raiz == NULL) {
        return 0;
    }

    int altura_esquerda =
        calcular_altura(raiz->esquerda);

    int altura_direita =
        calcular_altura(raiz->direita);

    return 1 + maior(
        altura_esquerda,
        altura_direita
    );
}


/*
 * ============================================================
 * ROTAÇÃO PARA A ESQUERDA DO DSW
 * ============================================================
 */
void dsw_comprimir(
    No* auxiliar,
    int quantidade
) {

    No* percorrimento = auxiliar;

    for (
        int i = 0;
        i < quantidade;
        i++
    ) {

        No* filho =
            percorrimento->direita;

        No* filho_direito =
            filho->direita;


        /*
         * Rotação para a esquerda.
         */
        filho->direita =
            filho_direito->esquerda;

        filho_direito->esquerda =
            filho;

        percorrimento->direita =
            filho_direito;

        percorrimento =
            filho_direito;
    }
}


/*
 * ============================================================
 * ALGORITMO DSW
 * ============================================================
 *
 * O algoritmo acontece em duas fases:
 *
 * FASE 1:
 * Árvore -> Videira
 *
 * FASE 2:
 * Videira -> Árvore balanceada
 */
void dsw_balancear(No** raiz) {

    if (*raiz == NULL) {
        return;
    }


    /*
     * Nó auxiliar utilizado para facilitar
     * as rotações.
     */
    No* auxiliar =
        criar_no(0, "", "", 0);

    auxiliar->direita = *raiz;


    /*
     * ========================================================
     * FASE 1
     * Transformação da árvore em uma videira.
     * ========================================================
     */
    No* cauda = auxiliar;

    No* restante =
        cauda->direita;

    int quantidade_nos = 0;


    while (restante != NULL) {

        /*
         * Se não houver filho esquerdo,
         * avançamos pela direita.
         */
        if (restante->esquerda == NULL) {

            cauda = restante;

            restante =
                restante->direita;

            quantidade_nos++;
        }

        /*
         * Caso exista filho esquerdo,
         * fazemos uma rotação para a direita.
         */
        else {

            No* temporario =
                restante->esquerda;

            restante->esquerda =
                temporario->direita;

            temporario->direita =
                restante;

            restante =
                temporario;

            cauda->direita =
                temporario;
        }
    }


    /*
     * ========================================================
     * FASE 2
     * Transformação da videira em uma árvore balanceada.
     * ========================================================
     */

    int potencia_de_2 = 1;


    /*
     * Encontra a maior potência de 2
     * menor ou igual a quantidade de nós + 1.
     */
    while (
        potencia_de_2 <= quantidade_nos + 1
    ) {

        potencia_de_2 *= 2;
    }

    potencia_de_2 /= 2;


    /*
     * Calcula a quantidade inicial de folhas.
     */
    int folhas =
        quantidade_nos + 1
        - potencia_de_2;


    /*
     * Primeira etapa de compressão.
     */
    dsw_comprimir(
        auxiliar,
        folhas
    );


    quantidade_nos -= folhas;


    /*
     * Continua realizando compressões
     * até obter a árvore balanceada.
     */
    while (quantidade_nos > 1) {

        dsw_comprimir(
            auxiliar,
            quantidade_nos / 2
        );

        quantidade_nos /= 2;
    }


    /*
     * Define a nova raiz.
     */
    *raiz = auxiliar->direita;


    /*
     * Libera o nó auxiliar.
     */
    free(auxiliar);
}


/*
 * ============================================================
 * LIBERA A MEMÓRIA DA ÁRVORE
 * ============================================================
 */
void liberar_arvore(No* raiz) {

    if (raiz != NULL) {

        liberar_arvore(
            raiz->esquerda
        );

        liberar_arvore(
            raiz->direita
        );

        free(raiz);
    }
}


/*
 * ============================================================
 * FUNÇÃO PRINCIPAL
 * ============================================================
 */
int main() {

    /*
     * ========================================================
     * DECLARAÇÃO DAS ÁRVORES
     * ========================================================
     *
     * A árvore AVL e a árvore DSW são independentes.
     */
    No* raiz_avl = NULL;
    No* raiz_dsw = NULL;


    /*
     * ========================================================
     * VARIÁVEIS PARA OS DOCUMENTOS
     * ========================================================
     */
    unsigned long int id;
    int ano;

    char titulo[100];
    char autor[50];


    /*
     * ========================================================
     * VARIÁVEIS PARA MÉTRICAS
     * ========================================================
     */
    Metricas metricas_avl;
    Metricas metricas_dsw;


    /*
     * Contador de rotações realizadas pela AVL.
     */
    int rotacoes_avl = 0;


    /*
     * ========================================================
     * CADASTRO DO PRIMEIRO DOCUMENTO
     * ========================================================
     */
    printf(
        "=== Sistema de Indexacao CodeForest ===\n\n"
    );

    printf(
        "--- Cadastro do Primeiro Documento ---\n"
    );

    printf("Digite o ID: ");

    scanf(
        "%lu",
        &id
    );

    printf("Digite o Titulo: ");

    scanf(
        " %99[^\n]",
        titulo
    );

    printf("Digite o Autor: ");

    scanf(
        " %49[^\n]",
        autor
    );

    printf("Digite o Ano de publicacao: ");

    scanf(
        "%d",
        &ano
    );


    /*
     * ========================================================
     * PRIMEIRA INSERÇÃO NA AVL
     * ========================================================
     */
    clock_t inicio_avl =
        clock();

    metricas_avl.altura_antes =
        (float)calcular_altura(raiz_avl);


    raiz_avl =
        inserir_avl(
            raiz_avl,
            id,
            titulo,
            autor,
            ano,
            &rotacoes_avl
        );


    clock_t fim_avl =
        clock();


    metricas_avl.altura_depois =
        (float)calcular_altura(raiz_avl);

    metricas_avl.numero_rotacoes =
        (float)rotacoes_avl;

    metricas_avl.tempo_execucao =
        (float)(
            (double)(fim_avl - inicio_avl)
            / CLOCKS_PER_SEC
        );


    /*
     * ========================================================
     * PRIMEIRA INSERÇÃO NA ÁRVORE DSW
     * ========================================================
     */
    clock_t inicio_dsw =
        clock();

    metricas_dsw.altura_antes =
        (float)calcular_altura(raiz_dsw);


    raiz_dsw =
        inserir_arvore_dsw(
            raiz_dsw,
            id,
            titulo,
            autor,
            ano
        );


    clock_t fim_dsw =
        clock();


    metricas_dsw.altura_depois =
        (float)calcular_altura(raiz_dsw);

    metricas_dsw.numero_rotacoes =
        0.0f;

    metricas_dsw.tempo_execucao =
        (float)(
            (double)(fim_dsw - inicio_dsw)
            / CLOCKS_PER_SEC
        );


    printf(
        "\nDocumento cadastrado com sucesso!\n\n"
    );


    /*
     * ========================================================
     * CADASTRO DO SEGUNDO DOCUMENTO
     * ========================================================
     */
    printf(
        "--- Cadastro do Segundo Documento ---\n"
    );

    printf("Digite o ID: ");

    scanf(
        "%lu",
        &id
    );

    printf("Digite o Titulo: ");

    scanf(
        " %99[^\n]",
        titulo
    );

    printf("Digite o Autor: ");

    scanf(
        " %49[^\n]",
        autor
    );

    printf("Digite o Ano de publicacao: ");

    scanf(
        "%d",
        &ano
    );


    /*
     * ========================================================
     * SEGUNDA INSERÇÃO NA AVL
     * ========================================================
     */
    inicio_avl =
        clock();

    metricas_avl.altura_antes =
        (float)calcular_altura(raiz_avl);


    raiz_avl =
        inserir_avl(
            raiz_avl,
            id,
            titulo,
            autor,
            ano,
            &rotacoes_avl
        );


    fim_avl =
        clock();


    metricas_avl.altura_depois =
        (float)calcular_altura(raiz_avl);

    metricas_avl.numero_rotacoes =
        (float)rotacoes_avl;

    metricas_avl.tempo_execucao =
        (float)(
            (double)(fim_avl - inicio_avl)
            / CLOCKS_PER_SEC
        );


    /*
     * ========================================================
     * SEGUNDA INSERÇÃO NA ÁRVORE DSW
     * ========================================================
     */
    inicio_dsw =
        clock();

    metricas_dsw.altura_antes =
        (float)calcular_altura(raiz_dsw);


    raiz_dsw =
        inserir_arvore_dsw(
            raiz_dsw,
            id,
            titulo,
            autor,
            ano
        );


    fim_dsw =
        clock();


    metricas_dsw.altura_depois =
        (float)calcular_altura(raiz_dsw);

    metricas_dsw.numero_rotacoes =
        0.0f;

    metricas_dsw.tempo_execucao =
        (float)(
            (double)(fim_dsw - inicio_dsw)
            / CLOCKS_PER_SEC
        );


    printf(
        "\nDocumento cadastrado com sucesso!\n\n"
    );


    /*
     * ========================================================
     * EXIBIÇÃO DOS DOCUMENTOS
     * ========================================================
     */
    printf(
        "=== Documentos cadastrados ===\n\n"
    );

    int contador =
        1;

    exibir_em_ordem(
        raiz_avl,
        &contador
    );


    /*
     * ========================================================
     * MÉTRICAS ANTES DO DSW
     * ========================================================
     */
    float altura_dsw_antes =
        (float)calcular_altura(raiz_dsw);


    /*
     * ========================================================
     * APLICAÇÃO MANUAL DO DSW
     * ========================================================
     */
    printf(
        "Aplicando balanceamento DSW manualmente...\n"
    );


    clock_t inicio_balanceamento_dsw =
        clock();


    dsw_balancear(
        &raiz_dsw
    );


    clock_t fim_balanceamento_dsw =
        clock();


    /*
     * Atualiza as métricas após o balanceamento.
     */
    metricas_dsw.altura_antes =
        altura_dsw_antes;

    metricas_dsw.altura_depois =
        (float)calcular_altura(raiz_dsw);


    /*
     * DSW não utiliza rotações AVL.
     *
     * Por isso, para a métrica específica de
     * "rotações AVL", registramos zero.
     */
    metricas_dsw.numero_rotacoes =
        0.0f;


    metricas_dsw.tempo_execucao =
        (float)(
            (double)(
                fim_balanceamento_dsw
                - inicio_balanceamento_dsw
            )
            / CLOCKS_PER_SEC
        );


    /*
     * ========================================================
     * COMPARAÇÃO DE DESEMPENHO
     * ========================================================
     */
    printf(
        "\n=== Comparacao de desempenho ===\n\n"
    );


    /*
     * --------------------------------------------------------
     * COMPARAÇÃO DA ALTURA
     * --------------------------------------------------------
     *
     * Menor altura é melhor.
     *
     * 1 = AVL venceu
     * 0 = DSW venceu
     */
    int venceu_altura =
        metricas_avl.altura_depois
        <= metricas_dsw.altura_depois;


    printf(
        "Altura da arvore: %s (%d)\n",
        venceu_altura
            ? "AVL venceu"
            : "DSW venceu",
        venceu_altura
    );


    /*
     * --------------------------------------------------------
     * COMPARAÇÃO DO NÚMERO DE ROTAÇÕES
     * --------------------------------------------------------
     *
     * Menor número de rotações representa
     * menor quantidade de operações de balanceamento.
     *
     * 1 = AVL venceu
     * 0 = DSW venceu
     *
     * Como o DSW não utiliza as rotações AVL,
     * esta métrica é interpretada como uma métrica
     * específica do balanceamento AVL.
     */
    int venceu_rotacoes =
        metricas_avl.numero_rotacoes
        <= metricas_dsw.numero_rotacoes;


    printf(
        "Numero de rotacoes: %s (%d)\n",
        venceu_rotacoes
            ? "AVL venceu"
            : "DSW venceu",
        venceu_rotacoes
    );


    /*
     * --------------------------------------------------------
     * COMPARAÇÃO DO TEMPO
     * --------------------------------------------------------
     *
     * Menor tempo de execução significa maior eficiência.
     *
     * 1 = AVL venceu
     * 0 = DSW venceu
     */
    int venceu_tempo =
        metricas_avl.tempo_execucao
        <= metricas_dsw.tempo_execucao;


    printf(
        "Tempo de execucao: %s (%d)\n",
        venceu_tempo
            ? "AVL venceu"
            : "DSW venceu",
        venceu_tempo
    );


    /*
     * ========================================================
     * RELATÓRIO DETALHADO DAS MÉTRICAS
     * ========================================================
     */
    printf(
        "\n=== Relatorio detalhado ===\n\n"
    );


    printf(
        "--- AVL ---\n"
    );

    printf(
        "Altura antes: %.2f\n",
        metricas_avl.altura_antes
    );

    printf(
        "Altura depois: %.2f\n",
        metricas_avl.altura_depois
    );

    printf(
        "Numero de rotacoes: %.2f\n",
        metricas_avl.numero_rotacoes
    );

    printf(
        "Tempo de execucao: %.8f segundos\n",
        metricas_avl.tempo_execucao
    );


    printf(
        "\n--- DSW ---\n"
    );

    printf(
        "Altura antes: %.2f\n",
        metricas_dsw.altura_antes
    );

    printf(
        "Altura depois: %.2f\n",
        metricas_dsw.altura_depois
    );

    printf(
        "Numero de rotacoes AVL: %.2f\n",
        metricas_dsw.numero_rotacoes
    );

    printf(
        "Tempo de balanceamento: %.8f segundos\n",
        metricas_dsw.tempo_execucao
    );


    /*
     * ========================================================
     * LIBERAÇÃO DA MEMÓRIA
     * ========================================================
     */
    liberar_arvore(
        raiz_avl
    );

    liberar_arvore(
        raiz_dsw
    );


    return 0;
}
