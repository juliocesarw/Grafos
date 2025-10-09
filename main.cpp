#include <cstdio>
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctype.h>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

//asinatura das funçoes
int escolhaInicial();
int gerarNumAleatorio(int limite);  
void criarGrafoNaoDirecional( int tamanho, int ** matriz, int porcentagem);
void controle();
void imprimirMatriz( int tamanho, int ** matriz);
void inicializarMatriz(int tamanho, int **matriz);
void criarGrafoDirecional(int tamanho, int ** matriz, int porcentagem);
void gerarDotNaoDirecional(int **matriz, int tamanho);
void gerarDotDirecional(int **matriz, int tamanho);
void funcaoParaGerarGrafoDoArquivo(int ** matriz, int linha, int coluna);
void leitura_de_arquivo_dot_direcional();
bool classificacao_do_grafo(int ** matriz, int tamanho);
bool percorreGrafo(int ** matriz, int linha, int tamanho, bool * vetor);

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

void criarGrafoNaoDirecional(int tamanho, int ** matriz, int porcentagem){

    int l = gerarNumAleatorio(tamanho);
    int c = gerarNumAleatorio(tamanho);
    for (int i = 0; i < porcentagem; i++)
    {
        if(matriz[l][c] == 1 || l == c){
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
            int **matriz;
            matriz = (int **)malloc(tamanho * sizeof(int *));
            inicializarMatriz(tamanho, matriz);
            porcentagem = ((tamanho * (tamanho - 1) / 2) * porcentagem) / 100 ;
            criarGrafoNaoDirecional(tamanho, matriz, porcentagem);
            imprimirMatriz(tamanho, matriz);
            gerarDotNaoDirecional(matriz, tamanho);
        }
        break;
        case 2:{ // função para grafos direcionados
            int **matriz;
            matriz = (int **)malloc(tamanho * sizeof(int *));
            inicializarMatriz(tamanho, matriz);
            porcentagem = ((tamanho * (tamanho - 1)) * porcentagem) / 100 ;
            criarGrafoDirecional(tamanho, matriz, porcentagem);
            imprimirMatriz(tamanho, matriz);
            gerarDotDirecional(matriz, tamanho);
        }
        break;
        case 3:
            leitura_de_arquivo_dot_direcional();
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

void gerarDotNaoDirecional(int **matriz, int tamanho){

    ofstream arquivo("../grafos/grafo-nao-direcional.dot");
    if(!arquivo.is_open()){
        cout << "arquivo nao aberto" << endl;
    }
    else{
        arquivo << "graph {" << endl;
        for(int n = 0; n < tamanho; n++){
            arquivo << n << ";" << endl;
        }
        for ( int i = 0; i < tamanho - 1; i++)
        {
            for (int j = i + 1; j < tamanho; j++)
            {
                if(matriz[i][j] == 1){
                    arquivo << i << " -- " << j << endl;
                }
            }
            
        }
    }
    
    arquivo << "}";
    arquivo.close();
    
    system("dot -Tpng ../grafos/grafo-nao-direcional.dot -o ../grafos/grafo-nao-direcional.png");
    
}

void gerarDotDirecional(int **matriz, int tamanho){
    
    ofstream arq("../grafos/grafo-direcional.dot");
    if(!arq.is_open()){
        cout << "nao abriu" << endl;
    }
    else{
        arq << "digraph {" << endl;
        for(int n = 0; n < tamanho; n++){
            arq << n << ";" << endl;
        }
        for ( int i = 0; i < tamanho; i++)
        {
            for (int j = 0; j < tamanho; j++)
            {
                if(matriz[i][j] == 1){
                    arq << i << " -> " << j << endl;
                }
            }
            
        }
    }
    
    arq << "}";
    arq.close();

    system("dot -Tpng ../grafos/grafo-direcional.dot -o ../grafos/grafo-direcional.png");

}

void funcaoParaGerarGrafoDoArquivo(int ** matriz, int linha, int coluna){
    
    matriz[linha][coluna] = 1;
    matriz[coluna][linha] = 1;

}

void leitura_de_arquivo_dot_direcional(){

    ifstream arq("../grafos/grafo.dot");
    if(!arq.is_open()) return;
    string linha;
    string tpArq;
    string teste;
    string var1;
    string var2;
    string var3;
    int **matriz;
    
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
    
    matriz = (int **)malloc((maior + 1) * sizeof(int *));
    inicializarMatriz(maior + 1, matriz);
    iss.str(linha); //resetando a linha

    do
    {
        iss >> var1;
        iss >> var2;
        iss >> var3;
        if(!iss.fail()){
            funcaoParaGerarGrafoDoArquivo(matriz, stoi(var1), stoi(var3));
            getline(arq, linha);
            iss.clear();
            iss.str(linha);
        }
    } while (!iss.fail());

    // gerarDotNaoDirecional(matriz, maior + 1);

    if(classificacao_do_grafo(matriz, maior + 1)){
        cout << "\tgrafo conexo" << endl;
    }else{
        cout << "\tgrafo desconexo" << endl;
    }
    
    arq.close(); 

}

bool classificacao_do_grafo(int ** matriz, int tamanho){

    bool * vetor = new bool[tamanho]();

    percorreGrafo(matriz, 0, tamanho, vetor);

    for (int i = 0; i < tamanho; i++)
    {
        cout << vetor[i] << endl;
    }

    for (int i = 0; i < tamanho; i++)
    {
        if(vetor[i] == 0){
            return false;
        }
    }

    return true;

}

bool percorreGrafo(int ** matriz, int linha, int tamanho, bool * vetor){

    if(vetor[linha] == 1){
        return true;
    }

    vetor[linha] = 1;

    for (int coluna = 0; coluna < tamanho; coluna++)
    {
        if(matriz[linha][coluna] == 1 && vetor[coluna] == 0){
            percorreGrafo(matriz, coluna, tamanho, vetor);
        }
    }
    return true;

}

int main(){
    srand(time(NULL));
    controle();
    return 0;
}