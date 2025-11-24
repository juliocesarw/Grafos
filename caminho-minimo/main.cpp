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

struct No {// no para fazer o caminho minimo
    int distancia;
    int predecessor;
    bool visitado;
};

Vertice *grafo;
No * vetor;

void inicializa(int tamanho){
    grafo = new Vertice[tamanho];
    for(int i = 0; i < tamanho; i++){
        grafo[i].id=i;
        grafo[i].listaDevizinhos = NULL;
        grafo[i].ultimoVizinho = NULL;
    }
}

void inicializaVetor(int tamanho){

    vetor = new No[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        vetor[i].predecessor = -2;
        vetor[i].distancia = 200000;
        vetor[i].visitado = false;
    }

}

//assinaturas
void inicializa(int tamanho);
int escolhaInicial();
int gerarNumAleatorio(int limite);
void controle();
void registrarVizinhancaGND(Vertice& x, Vertice& y);
bool saoVizinhos(Vertice x, Vertice y);
bool classificacaoDoGradoND(int tamanho);
bool percorreGrafo(bool * vetor, int indice);
void gerarDotGND(int tamanho, string caminho);
void imprimir(int tamanho);
void controleCaminhoMinimo(int tamanho, int inicio, int chegada);

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
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label=" << percorre->peso << ",weight=" << percorre->peso << ", fontcolor=blue];" << endl;
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

    gerarDotGND(tamanho, "../../grafos/grafoNaoDirecionado.dot");
    // system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");
    system("sfdp -Tpng -Gdpi=300 ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");
    // system("dot -Tpng -Gsize=6.8,3.8\! -Gdpi=300 ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");

    cout << "\tGrafo gerado!" << endl;

    system("cls");

    int inicio, fim = 0;

    while (inicio >= 0)
    {
        cout << "\tInforme o ponto de inicio: ";
        cin >> inicio;
        cout << "\tInforme o ponto de fim: ";
        cin >> fim;

        controleCaminhoMinimo(tamanho, inicio, fim);
    }

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

bool atualizaDistancia(int indice, int distanciaAtual, int pontoDeChegada){
    if(indice == pontoDeChegada) return false;
    vetor[indice].visitado = true;
    int menor = 900;

    NoVizinho * p = grafo[indice].listaDevizinhos;
    while (p != NULL)
    {
        if(p->peso + distanciaAtual < vetor[p->PonteiroParavizinho->id].distancia){

            vetor[p->PonteiroParavizinho->id].distancia = p->peso + distanciaAtual;
            vetor[p->PonteiroParavizinho->id].predecessor = indice;
            if(vetor[p->PonteiroParavizinho->id].distancia < menor){
                menor = p->PonteiroParavizinho->id;
            }
        }
        p = p->proximoVizinho;
    }

    return true;
}

void imprimeVetor(int tamanho){
    for (int i = 0; i < tamanho; i++)
    {
        cout << "\tid: "<< i << " - distancia: " << vetor[i].distancia << "\t\t predecessor: " << vetor[i].predecessor << endl;
    }
}

int retornaMenor(int tamanho){
    int indiceMenor = 200;
    int menorDistancia = 200;
    
    for (int i = 0; i < tamanho; i++)
    {
        if(vetor[i].distancia < menorDistancia && vetor[i].visitado == false){
            indiceMenor = i;
            menorDistancia = vetor[i].distancia;
        }
    }
    cout << "o menor e: " << indiceMenor << endl;   
    return indiceMenor;
}

void imprimeCaminho(int saida, int chegada){

    int predecessor = chegada;
    while (predecessor >= 0)
    {
        if(predecessor != saida){
            cout << predecessor << " <- ";
        }else{
            cout << predecessor << endl;
            
        }
        predecessor = vetor[predecessor].predecessor;
    }
    cout << "DISTANCIA: " << vetor[chegada].distancia << endl;
}

void gerarDotDoCaminho(int inicio, int chegada, int tamanho){

ofstream arq("../../grafos/caminhoMinimo.dot");
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
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label=" << percorre->peso << ",weight=" << percorre->peso << ", fontcolor=blue];" << endl;
                    percorre = percorre->proximoVizinho;
                }
            }
            
        }
        arq << "}";
    }

    arq.close();

}

void controleCaminhoMinimo(int tamanho, int inicio, int chegada){
    inicializaVetor(tamanho);
    int inicioIteracao = inicio;
    vetor[inicioIteracao].distancia = 0;
    vetor[inicioIteracao].predecessor = -1; 
    while (atualizaDistancia(inicioIteracao, vetor[inicioIteracao].distancia, chegada))
    {
        // system("cls");
        inicioIteracao = retornaMenor(tamanho);
        // imprimeVetor(tamanho);
        // system("pause");
    }
    
    system("cls");
    imprimeCaminho(inicio, chegada);
    gerarDotDoCaminho(inicio, chegada, tamanho);
    system("pause");
    
}

main(){
    srand(time(NULL));
    
    controle();

    // cout << "rodou" << endl;

    // system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");

    return 0;
}