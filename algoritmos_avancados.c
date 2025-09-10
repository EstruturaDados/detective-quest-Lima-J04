#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101
#define MAX_PISTA 100
#define MAX_SUSPEITO 50

// =======================
// Structs
// =======================

// Árvore binária de salas
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST de pistas
typedef struct NodoPista {
    char pista[MAX_PISTA];
    struct NodoPista* esquerda;
    struct NodoPista* direita;
} NodoPista;

// Par pista-suspeito para a tabela hash
typedef struct HashNode {
    char pista[MAX_PISTA];
    char suspeito[MAX_SUSPEITO];
    struct HashNode* proximo;
} HashNode;

// =======================
// Funções auxiliares
// =======================

// Função de hash simples (soma dos caracteres módulo tamanho)
int hash(char* str) {
    int total = 0;
    while (*str) total += *str++;
    return total % HASH_SIZE;
}

// =======================
// Funções principais
// =======================

/**
 * Cria dinamicamente um cômodo (sala) com o nome especificado.
 */
Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

/**
 * Associa pistas fixas a salas usando lógica codificada.
 */
const char* obterPistaPorSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Luvas ensanguentadas";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca desaparecida";
    if (strcmp(nomeSala, "Quarto") == 0) return "Perfume suspeito";
    if (strcmp(nomeSala, "Sala de Estar") == 0) return "Pegadas sujas";
    if (strcmp(nomeSala, "Porão") == 0) return "Documento rasgado";
    return NULL;
}

/**
 * Insere uma nova pista na BST de pistas.
 */
NodoPista* inserirPista(NodoPista* raiz, char* pista) {
    if (raiz == NULL) {
        NodoPista* novo = (NodoPista*)malloc(sizeof(NodoPista));
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

/**
 * Insere uma pista e seu suspeito associado na tabela hash.
 */
void inserirNaHash(HashNode* hashTable[], char* pista, char* suspeito) {
    int idx = hash(pista);
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = hashTable[idx];
    hashTable[idx] = novo;
}

/**
 * Retorna o suspeito associado a uma pista.
 */
char* encontrarSuspeito(HashNode* hashTable[], char* pista) {
    int idx = hash(pista);
    HashNode* atual = hashTable[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * Percorre a BST e exibe as pistas.
 */
void listarPistas(NodoPista* raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    listarPistas(raiz->direita);
}

/**
 * Conta quantas vezes um suspeito aparece nas pistas coletadas.
 */
int contarPistasSuspeito(NodoPista* raiz, HashNode* hashTable[], char* suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    char* s = encontrarSuspeito(hashTable, raiz->pista);
    if (s && strcmp(s, suspeito) == 0) count++;
    count += contarPistasSuspeito(raiz->esquerda, hashTable, suspeito);
    count += contarPistasSuspeito(raiz->direita, hashTable, suspeito);
    return count;
}

/**
 * Função recursiva que permite ao jogador explorar as salas da mansão.
 */
void explorarSalas(Sala* atual, NodoPista** arvorePistas, HashNode* hashTable[]) {
    if (atual == NULL) return;

    printf("\nVocê entrou na sala: %s\n", atual->nome);
    const char* pista = obterPistaPorSala(atual->nome);
    if (pista) {
        printf("Você encontrou uma pista: %s\n", pista);
        *arvorePistas = inserirPista(*arvorePistas, (char*)pista);
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

/**
 * Realiza o julgamento final com base nas pistas coletadas.
 */
void verificarSuspeitoFinal(NodoPista* arvorePistas, HashNode* hashTable[]) {
    char acusado[MAX_SUSPEITO];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int evidencias = contarPistasSuspeito(arvorePistas, hashTable, acusado);

    printf("\nVocê acusou: %s\n", acusado);
    if (evidencias >= 2)
        printf("Acusação válida! %d evidências encontradas contra %s.\n", evidencias, acusado);
    else
        printf("Acusação inválida. Apenas %d evidência(s) contra %s.\n", evidencias, acusado);

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {
    // Criação do mapa fixo da mansão
    Sala* raiz = criarSala("Sala de Estar");
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Cozinha");
    raiz->esquerda->esquerda = criarSala("Quarto");
    raiz->direita->direita = criarSala("Porão");

    // Tabela hash para pistas e suspeitos
    HashNode* hashTable[HASH_SIZE] = {0};

    // Inserindo pares pista-suspeito
    inserirNaHash(hashTable, "Luvas ensanguentadas", "Sr. Black");
    inserirNaHash(hashTable, "Faca desaparecida", "Sra. White");
    inserirNaHash(hashTable, "Perfume suspeito", "Sra. White");
    inserirNaHash(hashTable, "Pegadas sujas", "Sr. Green");
    inserirNaHash(hashTable, "Documento rasgado", "Sr. Black");

    // Árvore de pistas
    NodoPista* arvorePistas = NULL;

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete pistas para descobrir o culpado.\n");

    // Início da exploração
    explorarSalas(raiz, &arvorePistas, hashTable);

    // Listando pistas
    printf("\nPistas coletadas:\n");
    listarPistas(arvorePistas);

    // Acusação final
    verificarSuspeitoFinal(arvorePistas, hashTable);

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

    return 0;
}

