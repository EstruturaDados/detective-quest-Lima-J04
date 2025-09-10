#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101
#define MAX_PISTA 100
#define MAX_SUSPEITO 50

// =======================
// Structs
// =======================

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct NodoPista {
    char pista[MAX_PISTA];
    struct NodoPista* esquerda;
    struct NodoPista* direita;
} NodoPista;

typedef struct HashNode {
    char pista[MAX_PISTA];
    char suspeito[MAX_SUSPEITO];
    struct HashNode* proximo;
} HashNode;

// =======================
// Funções auxiliares
// =======================

int hash(char* str) {
    int total = 0;
    while (*str) total += *str++;
    return total % HASH_SIZE;
}

// =======================
// Funções principais
// =======================

Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (!novaSala) {
        perror("Erro ao alocar sala");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

const char* obterPistaPorSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Luvas ensanguentadas";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca desaparecida";
    if (strcmp(nomeSala, "Quarto") == 0) return "Perfume suspeito";
    if (strcmp(nomeSala, "Sala de Estar") == 0) return "Pegadas sujas";
    if (strcmp(nomeSala, "Porão") == 0) return "Documento rasgado";
    return NULL;
}

NodoPista* inserirPista(NodoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        NodoPista* novo = (NodoPista*)malloc(sizeof(NodoPista));
        if (!novo) {
            perror("Erro ao alocar pista");
            exit(1);
        }
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

void inserirNaHash(HashNode* hashTable[], const char* pista, const char* suspeito) {
    int idx = hash((char*)pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    if (!novo) {
        perror("Erro ao alocar hash node");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = hashTable[idx];
    hashTable[idx] = novo;
}

char* encontrarSuspeito(HashNode* hashTable[], const char* pista) {
    int idx = hash((char*)pista);
    HashNode* atual = hashTable[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

void listarPistas(NodoPista* raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    listarPistas(raiz->direita);
}

int contarPistasSuspeito(NodoPista* raiz, HashNode* hashTable[], const char* suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    char* s = encontrarSuspeito(hashTable, raiz->pista);
    if (s && strcmp(s, suspeito) == 0) count++;
    count += contarPistasSuspeito(raiz->esquerda, hashTable, suspeito);
    count += contarPistasSuspeito(raiz->direita, hashTable, suspeito);
    return count;
}

void explorarSalas(Sala* atual, NodoPista** arvorePistas, HashNode* hashTable[]) {
    if (atual == NULL) return;

    printf("\nVocê entrou na sala: %s\n", atual->nome);
    const char* pista = obterPistaPorSala(atual->nome);
    if (pista) {
        printf("Você encontrou uma pista: %s\n", pista);
        *arvorePistas = inserirPista(*arvorePistas, pista);  // 🔧 Sem cast desnecessário
    } else {
        printf("Nenhuma pista nesta sala.\n");
    }

    char escolha;
    printf("Deseja ir para esquerda (e), direita (d) ou sair (s)? ");
    scanf(" %c", &escolha);

    if (escolha == 'e')
        explorarSalas(atual->esquerda, arvorePistas, hashTable);
    else if (escolha == 'd')
        explorarSalas(atual->direita, arvorePistas, hashTable);
    else
        printf("\nSaindo da exploração...\n");
}

void verificarSuspeitoFinal(NodoPista* arvorePistas, HashNode* hashTable[]) {
    char acusado[MAX_SUSPEITO];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);  // 🔧 Leitura pode ser insegura, considere fgets se quiser segurança

    int evidencias = contarPistasSuspeito(arvorePistas, hashTable, acusado);

    printf("\nVocê acusou: %s\n", acusado);
    if (evidencias >= 2)
        printf("Acusação válida! %d evidências encontradas contra %s.\n", evidencias, acusado);
    else
        printf("Acusação inválida. Apenas %d evidência(s) contra %s.\n", evidencias, acusado);
}

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {
    Sala* raiz = criarSala("Sala de Estar");
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Cozinha");
    raiz->esquerda->esquerda = criarSala("Quarto");
    raiz->direita->direita = criarSala("Porão");

    HashNode* hashTable[HASH_SIZE] = {0};

    inserirNaHash(hashTable, "Luvas ensanguentadas", "Sr. Black");
    inserirNaHash(hashTable, "Faca desaparecida", "Sra. White");
    inserirNaHash(hashTable, "Perfume suspeito", "Sra. White");
    inserirNaHash(hashTable, "Pegadas sujas", "Sr. Green");
    inserirNaHash(hashTable, "Documento rasgado", "Sr. Black");

    NodoPista* arvorePistas = NULL;

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete pistas para descobrir o culpado.\n");

    explorarSalas(raiz, &arvorePistas, hashTable);

    printf("\nPistas coletadas:\n");
    listarPistas(arvorePistas);

    verificarSuspeitoFinal(arvorePistas, hashTable);

    return 0;
}

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

