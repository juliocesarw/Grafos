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


Vertice *grafo;
Vertice *grafoMinimo;

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

//assinaturas
int escolhaInicial();
int gerarNumAleatorio(int limite);
void controle();
void registrarVizinhancaGND(Vertice& x, Vertice& y);
void registrarVizinhancaGrafoMinimo(Vertice& x, Vertice& y, int peso);
bool saoVizinhos(Vertice x, Vertice y);
bool classificacaoDoGradoND(int tamanho);
bool percorreGrafo(bool * vetor, int indice);
bool percorreGrafoMinimo(bool * vetor, int indice);
void gerarDotGND(int tamanho, string caminho);
void gerarDotGrafoMinimo(int tamanho, string caminho);
void criarArvoreMinima(int tamanho);
bool procurarCiclo(int verticeA, int verticeB, int tamanho);
void imprimir(int tamanho);
void imprimirGrafoMinimo(int tamanho);
void leitura_de_arquivo_dot_direcional();

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

void registrarVizinhancaGrafoMinimo(Vertice& x, Vertice& y, int peso){

    NoVizinho * novox = new NoVizinho;
    novox->PonteiroParavizinho = &y;
    novox->proximoVizinho = NULL;
    novox->peso = peso;
    
    NoVizinho * novoy = new NoVizinho;
    novoy->PonteiroParavizinho = &x;
    novoy->proximoVizinho = NULL;
    novoy->peso = peso;

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

int vizinhoMaisProximo(Vertice no, int& peso){

    int idMenor;
    int menorPeso = 11;
    NoVizinho * percorre = no.listaDevizinhos;

    while (percorre != NULL){
        if(percorre->peso < menorPeso && !saoVizinhos(grafoMinimo[no.id], grafoMinimo[percorre->PonteiroParavizinho->id])){
            idMenor = percorre->PonteiroParavizinho->id;
            menorPeso = percorre->peso;
        }
        percorre = percorre->proximoVizinho;
    }
    
    peso = menorPeso;
    return idMenor;    

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

void criarArvoreMinima(int tamanho){

    int peso;
    int indice;
    int indiceVizinho;

    //inicia a arvore minima com o indice aleatorio e o menor vertice dele
    int noInicial = gerarNumAleatorio(tamanho); // 1
    int novoVizinho = vizinhoMaisProximo(grafo[noInicial], peso); // vizinho mais proximo do grafo[valor aleatorio] no grafo principal
    registrarVizinhancaGrafoMinimo(grafoMinimo[noInicial], grafoMinimo[novoVizinho], peso);// este novo vizinho ja e o novo vizinho para a arvore geradora 

    //comeca a percorrer a arvore procurando a menor aresta
    // vou receber o retorno do 
    for (int i = 1; i < tamanho; i++) // vou dar um for da quantidade de arestas na arvore minima
    {
        indice = buscaMenorAresta(peso, indiceVizinho, tamanho);
        registrarVizinhancaGND(grafoMinimo[indice], grafoMinimo[indiceVizinho]);
    }
    

}

int buscaMenorAresta(int& peso, int& indiceVizinho, int tamanho){

    int indice, var;
    for ( int i = 0; i < tamanho; i++)
    {
        if(grafoMinimo[i].listaDevizinhos != NULL){
            // preciso saber o indice da menor aresta
            var = vizinhoMaisProximo(grafo[i], peso);
            if(var < indiceVizinho){
                indiceVizinho = var;
                indice = i;
            }
        }   
    }
    return indice;
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
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label= " << percorre->peso << ", weight= " << percorre->peso << "];" << endl;
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
                    arq << j << " -- " << percorre->PonteiroParavizinho->id << " " << "[label= " << percorre->peso << ", weight= " << percorre->peso << "];" << endl;
                    percorre = percorre->proximoVizinho;
                }
            }
            
        }
        arq << "}";
    }

    arq.close();
}

void controle(){

    int tamanho, porcentagem;
    int op = escolhaInicial();
    if(op == 1){
        tamanho = 0; //quantidade de vertices (nos)
        porcentagem = 0; //quantidade de arestas (ligaçoes)
        system("cls");
    
        cout << "\tInforme o tamanho do grafo: ";
        cin >> tamanho;
    
        inicializa(tamanho);
    
        cout << "\tInforme a porcentagem do grafo: ";
        cin >> porcentagem;
    }
    switch (op)
    {
    case 1:
        porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
        criarGrafoNaoDirecional(tamanho, porcentagem);
        // imprimir(tamanho);
        gerarDotGND(tamanho, "../../grafos/grafoNaoDirecionado.dot");
        system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");
        
        inicializaGrafoMinimo(tamanho);
        criarArvoreMinima(tamanho);
        break;
    case 2:
        leitura_de_arquivo_dot_direcional();
        break;
    default:
        cout << "opcao invalida" << endl;
        break;
    }

}

bool procurarCiclo(int verticeA, int verticeB, int tamanho){

    bool *vetor = new bool[tamanho]();
    percorreGrafoMinimo(vetor, verticeA);
    bool ciclo = vetor[verticeB] == 1 ? true: false;
    delete[] vetor; 
    return ciclo;

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
        percorreGrafoMinimo(vetor, percorre->PonteiroParavizinho->id);
        percorre = percorre->proximoVizinho;
    }
    
    return true;

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

void leitura_de_arquivo_dot_direcional(){

    ifstream arq("../../grafos/grafo.dot");
    if(!arq.is_open()){
        cout << "erro ao abrir arquivo!" << endl;
        return;
    } 
    string linha;
    string tpArq;
    string var1;
    string var2;
    string var3;
    string ignore;
    string peso;
    
    
    getline(arq, linha);
    
    istringstream iss(linha);
    
    iss >> tpArq;
    
    int maior = 0;


    do
    {
        iss.clear();
        getline(arq, linha);
        iss.str(linha);
        iss >> var1;
        iss >> var2;//esta aqui apenas para dar erro
        if(stoi(var1) > maior) maior = stoi(var1);
    } while (iss.fail());
    
    int tamanho = maior + 1;

    inicializa(tamanho);
    iss.str(linha); //resetando a linha
    
    if(tpArq == "Graph"){
        do
        {
            iss >> var1;
            iss >> var2;
            iss >> var3;
            iss >> ignore;
            iss >> peso;
            if(!iss.fail()){
                registrarVizinhancaGrafoMinimo(grafo[stoi(var1)], grafo[stoi(var3)], stoi(peso));
                getline(arq, linha);
                iss.clear();
                iss.str(linha);
            }
        } while (!iss.fail());
        
        if(classificacaoDoGradoND(tamanho)){
            cout << "\tgrafo nao direcional e conexo" << endl;
        }else{
            cout << "\tgrafo nao direcional e desconexo" << endl;
        }
    } 
    else{
        cout << "nao foi possivel ler o arquivo" << endl;
    }  
    
    arq.close(); 

    system("dot -Tpng ../../grafos/grafo.dot -o ../../grafos/grafo.png");
    
    inicializaGrafoMinimo(tamanho);
    criarArvoreMinima(tamanho);

}

main(){
    srand(time(NULL));
    
    controle();

    // cout << "rodou" << endl;

    // system("dot -Tpng ../../grafos/grafoNaoDirecionado.dot -o ../../grafos/grafoNaoDirecionado.png");

    return 0;
}