#include <cstdio>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctype.h>
#include <cstring>
#include <ctime>

//asinatura das funçoes
int escolhaInicial();
int gerarNumAleatorio(int limite);  
void criarGrafoNaoDirecional( int tamanho, int ** matriz, int porcentagem);
void controle();
void imprimirMatriz( int tamanho, int ** matriz);
void inicializarMatriz(int tamanho, int **matriz);
void criarGrafoDirecional(int tamanho, int ** matriz, int porcentagem);

void inicializarMatriz(int tamanho, int **matriz){
    for(int i = 0; i < tamanho; i++){
        matriz[i] = (int *)malloc(tamanho * sizeof(int));
        for (int j = 0; j < tamanho; j++)
        {
            matriz[i][j] = 0;
        }
        
    }
}

int escolhaInicial(){
    system("cls");
    cout << "\n\n\t[1] - Criar um grafo direcionado" << endl;
    cout << "\t[2] - Criar um grafo nao direcionado" << endl;
    cout << "\t[3] - Importar um arquivo .dot" << endl;
    cout << "\tEscolha: " ;
    int op;
    cin >> op;
    return op;
}

int gerarNumAleatorio(int limite){
    int n = 0;
    for (int i = 0; i < limite; i++)
    {
        n = rand() % limite;
    }
    return n;
    
}

void criarGrafoNaoDirecional(int tamanho, int ** matriz, int porcentagem){

    int l = gerarNumAleatorio(tamanho);
    int c = gerarNumAleatorio(tamanho);
    for (int i = 0; i < porcentagem; i++)
    {
        if(matriz[l][c] == 1){
            i--;
        }
        else{
            matriz[l][c] = 1;
            matriz[c][l] = 1;
        }
        l = gerarNumAleatorio(tamanho);
        c = gerarNumAleatorio(tamanho);
    }
    
    
}

void criarGrafoDirecional(int tamanho, int ** matriz, int porcentagem){

    int l = gerarNumAleatorio(tamanho);
    int c = gerarNumAleatorio(tamanho);
    for (int i = 0; i < porcentagem; i++)
    {
        if(matriz[l][c] == 1){
            i--;
        }
        else{
            matriz[l][c] = 1;
        }
        l = gerarNumAleatorio(tamanho);
        c = gerarNumAleatorio(tamanho);
    }
    
    
}

void controle(){
    int tamanho = 0; //quantidade de vertices (nos)
    int porcentagem = 0; //quantidade de arestas (ligaçoes)
    int op = escolhaInicial();
    if(op == 1 || op == 2){
        system("cls");
        cout << "\tInforme o tamanho do grafo: ";
        cin >> tamanho;
        cout << "\tInforme a porcentagem do grafo: ";
        cin >> porcentagem;
    }
    switch (op)
    {
        case 1:{ // função para grafos direcionados
            int **matriz;
            matriz = (int **)malloc(tamanho * sizeof(int *));
            inicializarMatriz(tamanho, matriz);
            porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
            criarGrafoNaoDirecional(tamanho, matriz, porcentagem);
            imprimirMatriz(tamanho, matriz);
        }
        break;
        case 2:
            porcentagem = tamanho * tamanho - 1;
        // função para grafos nao direcionados
        break;
        case 3:
            // função para importar .dot
        break;
        default:
        cout << "\topcao invalida!!" << endl;
        break;
    }
}

void imprimirMatriz(int tamanho, int **matriz){
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            cout << matriz[i][j] << "  ";
        }
        cout << endl;
    }
}

int main(){
    srand(time(NULL));
    controle();
    return 0;
}