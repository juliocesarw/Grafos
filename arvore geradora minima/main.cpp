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

struct NoVizinho {
    Vertice *PonteiroParavizinho;
    NoVizinho *proximoVizinho;
    int peso;
};

struct Vertice {
    int id;
    NoVizinho *listaDevizinhos;
    NoVizinho * ultimoVizinho;
};

struct Arestas
{
    Vertice* a;
    Vertice* b;
    int peso;
    Arestas * prox;
    Arestas * ante;
};

struct Fila{
    Arestas * inicio;
    Arestas * fim;
};

Vertice *grafo;
Vertice *grafoMinimo;
Fila fila;

void inicializa(int tamanho){
    grafo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafo[i].id=i;
        grafo[i].listaDevizinhos = NULL;
        grafo[i].ultimoVizinho = NULL;
    }
}
void inicializaGrafoMinimo(int tamanho){
    grafoMinimo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafoMinimo[i].id=i;
        grafoMinimo[i].listaDevizinhos = NULL;
        grafoMinimo[i].ultimoVizinho = NULL;
    }
}
void inicializaFila(){
    fila.inicio = NULL;
    fila.fim = NULL;
};

//assinaturas
void inicializa(int tamanho);
int escolhaInicial();
int gerarNumAleatorio(int limite);
void controle();
void registrarVizinhancaGND(Vertice& x, Vertice& y);
bool saoVizinhos(Vertice x, Vertice y);
bool classificacaoDoGradoND(int tamanho);
bool percorreGrafo(bool * vetor, int indice);
bool percorreGrafoMinimo(bool * vetor, int indice);
void fazerLista(int tamanho);
void inserirNaLista(Arestas * novo);
void imprimirLista();
void gerarDotGND(int tamanho, string caminho);
void gerarDotGrafoMinimo(int tamanho, string caminho);
void criarArvoreMinima(int tamanho);
bool procurarCiclo(int verticeA, int verticeB, int tamanho);
void imprimir(int tamanho);
void imprimirGrafoMinimo(int tamanho);

int escolhaInicial(){
    system("cls");
    cout << "\n\n\t[1] - Criar um grafo nao direcionado" << endl;
    cout << "\t[2] - Importar um arquivo .dot" << endl;
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

bool saoVizinhos(Vertice x, Vertice y){

    if(x.listaDevizinhos == NULL){
        return false;
    }
    else{
        NoVizinho * percorreVizinhanca = x.listaDevizinhos;
    
        while(percorreVizinhanca != NULL){
            if(percorreVizinhanca->PonteiroParavizinho->id == y.id){
                return true;
            }
            else{
                percorreVizinhanca = percorreVizinhanca->proximoVizinho;
            }
        }
    }
    
    return false;
};

void registrarVizinhancaGND(Vertice& x, Vertice& y){

    int pesoAleatorio = gerarNumAleatorio(10) + 1;

    NoVizinho * novox = new NoVizinho;
    novox->PonteiroParavizinho = &y;
    novox->proximoVizinho = NULL;
    novox->peso = pesoAleatorio;
    
    NoVizinho * novoy = new NoVizinho;
    novoy->PonteiroParavizinho = &x;
    novoy->proximoVizinho = NULL;
    novoy->peso = pesoAleatorio;

    // para o x
    if(x.listaDevizinhos == NULL){
        x.listaDevizinhos = novox;
        x.ultimoVizinho = novox;
    }
    else{
        x.ultimoVizinho->proximoVizinho = novox;
        x.ultimoVizinho = novox;
    }

    // para o y
    if(y.listaDevizinhos == NULL){
        y.listaDevizinhos = novoy;
        y.ultimoVizinho = novoy;
    }
    else{
        y.ultimoVizinho->proximoVizinho = novoy;
        y.ultimoVizinho = novoy;

    }

}

void criarGrafoNaoDirecional(int tamanho, int porcentagem){

    int n1, n2;

    for (int i = 0; i < porcentagem; i++)
    {
        n1 = gerarNumAleatorio(tamanho);
        n2 = gerarNumAleatorio(tamanho);
        
        if(n1 == n2){
            i--;
        }
        else if(saoVizinhos(grafo[n1],grafo[n2])){
            i--;
        }
        else{
            registrarVizinhancaGND(grafo[n1],grafo[n2]);
        }
    }
    
}

void gerarDotGND(int tamanho, string caminho){
    ofstream arq(caminho);
    if(!arq.is_open()){
        cout << "nao aberto" << endl;
    }
    else{
        arq << "Graph {" << endl;
        for (int i = 0; i < tamanho; i++)
        {
            arq << i << ";" << endl;
        }
        for (int j = 0; j < tamanho; j++)
        {
            NoVizinho * percorre = grafo[j].listaDevizinhos;
            while (percorre != NULL)
            {
                if(percorre->PonteiroParavizinho->id < j){
                    percorre = percorre->proximoVizinho;
                }
                else{
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label=" << percorre->peso << ",weight=" << percorre->peso << "];" << endl;
                    percorre = percorre->proximoVizinho;
                }
            }
            
        }
        arq << "}";
    }

    arq.close();
}

void gerarDotGrafoMinimo(int tamanho, string caminho){
    ofstream arq(caminho);
    if(!arq.is_open()){
        cout << "nao aberto" << endl;
    }
    else{
        arq << "Graph {" << endl;
        for (int i = 0; i < tamanho; i++)
        {
            arq << i << ";" << endl;
        }
        for (int j = 0; j < tamanho; j++)
        {
            NoVizinho * percorre = grafoMinimo[j].listaDevizinhos;
            while (percorre != NULL)
            {
                if(percorre->PonteiroParavizinho->id < j){
                    percorre = percorre->proximoVizinho;
                }
                else{
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label=" << percorre->peso << ",weight=" << percorre->peso << "];" << endl;
                    percorre = percorre->proximoVizinho;
                }
            }
            
        }
        arq << "}";
    }

    arq.close();
}

void controle(){
    int tamanho = 0; //quantidade de vertices (nos)
    int porcentagem = 0; //quantidade de arestas (ligaçoes)
    system("cls");

    cout << "\tInforme o tamanho do grafo: ";
    cin >> tamanho;

    inicializa(tamanho);

    cout << "\tInforme a porcentagem do grafo: ";
    cin >> porcentagem;

    porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
    criarGrafoNaoDirecional(tamanho, porcentagem);
    imprimir(tamanho);
    gerarDotGND(tamanho, "../../grafos/grafoNaoDirecionado.dot");
    system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");
    
    inicializaFila();
    fazerLista(tamanho);
    imprimirLista();

    // inicializaGrafoMinimo(tamanho);
    // criarArvoreMinima(tamanho);

    // imprimirGrafoMinimo(tamanho);

}

void criarArvoreMinima(int tamanho){

    int a, b;
    Arestas * percorre = fila.inicio;
    while (percorre != NULL)
    {
        a = percorre->a->id;
        b = percorre->b->id;
        if(!procurarCiclo(a,b, tamanho)){
            registrarVizinhancaGND(grafoMinimo[a], grafoMinimo[b]);
        }
        percorre = percorre->prox;
    }
    gerarDotGrafoMinimo(tamanho, "../../grafos/ArvoreMinima.dot");
    system("dot -Tpng ../../grafos/ArvoreMinima.dot -o ../../grafos/ArvoreMinima.png");

}

bool procurarCiclo(int verticeA, int verticeB, int tamanho){

    bool *vetor = new bool[tamanho]();
    percorreGrafoMinimo(vetor, verticeA);
    bool ciclo = vetor[verticeB] == 1 ? true: false;
    delete[] vetor; 
    return ciclo;

}

void imprimir(int tamanho){

    for (int i = 0; i < tamanho; i++)
    {
        NoVizinho* atual = grafo[i].listaDevizinhos;
        cout << "\n";
        cout << i << ": - ";
        while (atual != NULL) {
        cout << atual->PonteiroParavizinho->id << "(" << atual->peso << ")" << " ";
        atual = atual->proximoVizinho;
        }
    }
cout << "\n" << endl;
    
}

void imprimirGrafoMinimo(int tamanho){

    for (int i = 0; i < tamanho; i++)
    {
        NoVizinho* atual = grafoMinimo[i].listaDevizinhos;
        cout << "\n";
        cout << i << ": - ";
        while (atual != NULL) {
        cout << atual->PonteiroParavizinho->id << "(" << atual->peso << ")" << " ";
        atual = atual->proximoVizinho;
        }
    }
cout << "\n" << endl;
    
}

bool classificacaoDoGradoND(int tamanho){

    bool *vetor = new bool[tamanho]();

    percorreGrafo(vetor, 0);

    for (int i = 0; i < tamanho; i++)
    {
        if(vetor[i] == 0) return false; 
    }
    delete[] vetor; 
    return true;
}

bool percorreGrafo(bool * vetor, int indice){

    if(vetor[indice] == 1){
        return true;
    }

    vetor[indice] = 1;

    NoVizinho * percorre = grafo[indice].listaDevizinhos;

    while (percorre != NULL)
    {
        percorreGrafo(vetor, percorre->PonteiroParavizinho->id);
        percorre = percorre->proximoVizinho;
    }
    
    return true;

}

bool percorreGrafoMinimo(bool * vetor, int indice){

    if(vetor[indice] == 1){
        return true;
    }

    vetor[indice] = 1;

    NoVizinho * percorre = grafoMinimo[indice].listaDevizinhos;

    while (percorre != NULL)
    {
        percorreGrafo(vetor, percorre->PonteiroParavizinho->id);
        percorre = percorre->proximoVizinho;
    }
    
    return true;

}

Arestas * criarAresta(Vertice * no, Vertice * vizinho, int peso){
    Arestas * novo = new Arestas;
    novo->a = no;
    novo->b = vizinho;
    novo->peso = peso;
    novo->ante = NULL;
    novo->prox = NULL;
    return novo;
}

void inserirNaLista(Arestas * novo){
    
    if(fila.inicio == NULL){
        fila.inicio = novo;
        fila.fim = novo;
    }
    else{
        Arestas * percorre = fila.inicio;
        while (percorre != NULL)
        {
            if(novo->peso < percorre->peso){

                if(percorre->ante == NULL){
                    novo->prox = fila.inicio;
                    fila.inicio->ante = novo;
                    fila.inicio = novo;
                    return;
                }else{
                    percorre->ante->prox = novo;
                    novo->ante = percorre->ante;
                    novo->prox = percorre;
                    percorre->ante = novo;
                    return;
                }
            }
            else{
                percorre = percorre->prox;
            }
        }
        fila.fim->prox = novo;
        novo->ante = fila.fim;
        fila.fim = novo;
    }
}

void fazerLista(int tamanho){

    for (int i = 0; i < tamanho; i++)
    {
        NoVizinho * percorre = grafo[i].listaDevizinhos;
        while(percorre != NULL){
            if(i > percorre->PonteiroParavizinho->id){
                percorre = percorre->proximoVizinho;
            }
            else{
                Arestas * novo = criarAresta(&grafo[i], percorre->PonteiroParavizinho, percorre->peso);
                inserirNaLista(novo);
                percorre = percorre->proximoVizinho;
            }
        }
    }
}

void imprimirLista(){
    Arestas * percorre = fila.inicio;
    while (percorre != NULL)
    {
        cout << percorre->a->id << " - " << percorre->b->id << " = " << percorre->peso << endl;
        percorre = percorre->prox;
    }
}

main(){
    srand(time(NULL));
    
    controle();

    // cout << "rodou" << endl;

    // system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");

    return 0;
}