#include <cstdio>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctype.h>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

struct Vertice; 

struct Vizinho {
    Vertice *vizinho;
    Vizinho *proximoVizinho;
};

struct Vertice {
    int id;
    Vizinho *vizinhos;
    Vizinho * ultimoVizinho;
};

Vertice *grafo;
void inicializa(int tamanho){
    grafo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafo[i].id=i;
        grafo[i].vizinhos = NULL;
        grafo[i].ultimoVizinho = NULL;
    }
}

//assinaturas
void inicializa(int tamanho);
int escolhaInicial();
int gerarNumAleatorio(int limite);
void criarGrafoNaoDirecional(int tamanho, int porcentagem);
void criarGrafoDirecional(int tamanho, int **matriz, int porcentagem);
void inserir(int indice, int indiceVizinho);
bool buscaSeJaEvizinho(int indice, int indicePesquisa);
void controle();
void imprimeVizinhos(int tamanho);

int escolhaInicial(){
    system("cls");
    cout << "\n\n\t[1] - Criar um grafo nao direcionado" << endl;
    cout << "\t[2] - Criar um grafo direcionado" << endl;
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

void criarGrafoNaoDirecional(int tamanho, int porcentagem){

    int no1 = gerarNumAleatorio(tamanho); // 8
    cout << no1 << " - ";
    int no2 = gerarNumAleatorio(tamanho); // 3
    cout << no2 << " somos um par" << endl;
    
    for (int i = 0; i < porcentagem; i++)
    {
        if(no1 == no2 || buscaSeJaEvizinho(no1, no2) == true){
            cout << "par repetido ou invalido" << endl;
            i--;
        }
        else{
            inserir(no1, no2);
        }
        no1 = gerarNumAleatorio(tamanho);
        cout << no1 << " - ";
        no2 = gerarNumAleatorio(tamanho);
        cout << no2 << " somos um par" << endl;
    } 
}

void criarGrafoDirecional(int tamanho, int ** matriz, int porcentagem){

    int l = gerarNumAleatorio(tamanho);
    int c = gerarNumAleatorio(tamanho);
    for (int i = 0; i < porcentagem; i++)
    {
        if(matriz[l][c] == 1 || l == c){
            i--;
        }
        else{
            matriz[l][c] = 1;
        }
        l = gerarNumAleatorio(tamanho);
        c = gerarNumAleatorio(tamanho);
    }
}

void inserir(int indice, int indiceVizinho){

    Vizinho * novo = new Vizinho;
    novo->vizinho = &grafo[indiceVizinho];
    novo->proximoVizinho = NULL;
    if(grafo[indice].vizinhos == NULL){
        grafo[indice].vizinhos = novo;
        grafo[indice].ultimoVizinho = novo;
    }
    else{
        grafo[indice].ultimoVizinho->proximoVizinho = novo;
        grafo[indice].ultimoVizinho = novo;
    }
}

bool buscaSeJaEvizinho(int indice, int indicePesquisa){
    Vizinho * percorre = grafo[indice].vizinhos;
    if(percorre == NULL){
        return false;
    }
    while(percorre != NULL){
        if(percorre->vizinho->id == grafo[indicePesquisa].id){
            cout << percorre->vizinho << "  & " << &grafo[indicePesquisa] << endl;
            return true;
        }else{
            percorre = percorre->proximoVizinho;
        } 
    }
    return false;
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
        case 1:{ // função para grafos nao direcionados
            porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
            criarGrafoNaoDirecional(tamanho, porcentagem);
            // imprimeVizinhos(tamanho);
        }
        default:
        cout << "\topcao invalida!!" << endl;
        break;
    }
}

void imprimeVizinhos(int tamanho){
    Vizinho *v;
    for (int i = 0; i < tamanho; i++)
    {
        cout << i << " : ";
        v = grafo[i].vizinhos;
        while (v != NULL){
            if(v->proximoVizinho != NULL){
                cout << v->vizinho->id << " - ";
                v = v->proximoVizinho;
            }
            else{
                cout << v->vizinho->id;
            }
        } 
        cout << endl;        
    }
    
}

main(){
    srand(time(NULL));
    inicializa(5);
    controle();
    // cout << "teste";
    // system("dot -Tpng ../grafos/grafo.dot -o ../grafos/temqueserdesconexo.png");
    return 0;
}