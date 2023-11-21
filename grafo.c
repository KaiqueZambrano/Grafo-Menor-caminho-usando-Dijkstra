#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define vertice int

typedef struct grafo {
    int vertices;
    int arestas;
    int **adjacencia;
} *Grafo;

int **criarMatriz(int linhas, int colunas, int peso) {
    // alocar matriz (linhas)
    int **matriz = malloc(linhas * sizeof(int*));

    // alocar matriz (colunas)
    for(int i = 0; i < colunas; i++)
        matriz[i] = malloc(colunas * sizeof(int*));

    // preencher pesos
    for(int i = 0; i < linhas; i++)
        for(int j = 0; j < colunas; j++)
            matriz[i][j] = peso;

    return matriz;
}

Grafo criarGrafo(int vertices) {
    Grafo grafo = malloc(sizeof(*grafo));
    
    grafo->vertices = vertices;
    grafo->arestas = 0;

    // matriz (peso 0 = sem adjacencia)
    grafo->adjacencia = criarMatriz(vertices, vertices, 0);

    return grafo;
}

bool insereArco(Grafo g, vertice v, vertice w, int peso) {
    if(g->adjacencia[v][w] != 0 
            || v >= g->vertices || v < 0
            || w >= g->vertices || w < 0 
            || v == w)
        return false;

    g->adjacencia[v][w] = peso;
    g->arestas++;
    
    return true;
}

bool removeArco(Grafo g, vertice v, vertice w) {
    if(g->adjacencia[v][w] == 0 
            || v >= g->vertices || v < 0
            || w >= g->vertices || w < 0 
            || v == w)
        return false;
    
    g->adjacencia[v][w] = 0;
    g->arestas--;

    return true;   
}

void imprimeGrafo(Grafo g) {
    for(vertice v = 0; v < g->vertices; v++) {
        // imprime o vertice v
        printf("%2d:", v);
        for(vertice w = 0; w < g->vertices; w++)
            if(g->adjacencia[v][w] != 0)
                // bem como suas arestas e respectivos pesos
                printf(" %2d(%2d)", w, g->adjacencia[v][w]);
        printf("\n");
    }
}

void caminhoMaisCurto(Grafo g, int origem, int destino, int minimo) {
    // o minimo (para comparar os comprimentos) e a nova distancia (analisa e compara com o min)
    int min = minimo, novaDist;
    
    int *M = (int*)malloc(g->vertices * sizeof(int));   // vetor pra saber os vertices visitados
    int *A = (int*)malloc(g->vertices * sizeof(int));   // vetor pra saber o caminho mais curto
    int *L = (int*)malloc(g->vertices * sizeof(int));   // vetor pra saber os comprimentos partindo da origem
    
    int *caminho = (int*)malloc(g->vertices * 3 * sizeof(int));

    // inicializa vetores
    for(int i = 0; i < g->vertices; i++) {
        M[i] = 0;                                       // nao visitado
        A[i] = -1;                                      // nao tem caminho
        L[i] = minimo;                                  // minimo
    }

    // inicio (v = origem)
    int v = origem;
    L[v] = 0;

    // enquanto v nao chegar no destino e houver um caminho
    while(v != destino && v != -1) {
        // percorrer os possiveis adjacentes de v
        for(int w = 0; w < g->vertices; w++)
            // se adjacente e nao visitado 
            if(g->adjacencia[v][w] != 0 && M[w] == 0) {
                novaDist = L[v] + g->adjacencia[v][w];  // calcula nova dist
                // compara pra saber se foi o menor encontrado
                if(novaDist < L[w]) {
                    L[w] = novaDist;                    // atualiza o comprimento
                    A[w] = v;                           // atualiza o caminho
                }
            }
        
        M[v] = 1;                                       // v visitado
        min = minimo;                                   // reseta o minimo
        v = -1;                                         // se nao encontrou novo caminho

        // encontrar novo caminho (analisar outro v)
        for(int i = 0; i < g->vertices; i++)
            // nao foi visitado e tem arco com menor peso
            if(M[i] == 0 && L[i] < min) {
                min = L[i];                             // atualiza o min
                v = i;                                  // atualiza o v
            }
    }

    // se chegou ao destino
    if(v == destino) {
        printf("Caminho mais curto entre %d e %d tem comprimento %d, caminho: \n", 
            origem, destino, L[destino]);

        caminho[0] = destino;

        int k = 1;
        while(v != origem) {
            caminho[k] = A[v];
            v = A[v];
            k++;
        }

        for(int i = k-1; i >= 0; i--)
            printf("%4d", caminho[i]);

        printf("\n\n");
    }
    else
        printf("nao existe caminho entre %d e %d \n", origem, destino);
}

int main(void) {
    Grafo g = criarGrafo(6);
    
    insereArco(g, 0, 1, 1);
    insereArco(g, 0, 2, 4);

    insereArco(g, 1, 0, 1);
    insereArco(g, 1, 2, 4);
    insereArco(g, 1, 3, 2);
    insereArco(g, 1, 4, 7);

    insereArco(g, 2, 0, 4);
    insereArco(g, 2, 1, 4);
    insereArco(g, 2, 3, 3);
    insereArco(g, 2, 4, 5);
    
    insereArco(g, 3, 1, 2);
    insereArco(g, 3, 2, 3);
    insereArco(g, 3, 4, 4);
    insereArco(g, 3, 5, 6);

    insereArco(g, 4, 1, 7);
    insereArco(g, 4, 2, 5);
    insereArco(g, 4, 3, 4);
    insereArco(g, 4, 5, 7);

    insereArco(g, 5, 3, 6);
    insereArco(g, 5, 4, 7);
    
    caminhoMaisCurto(g, 0, 5, 300000);
    
    return 0;
}