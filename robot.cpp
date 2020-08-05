/*
  Compilar: g++ ants.cpp -fopenmp -std=c++11
  Executar: ./a.out N Ox Oy Dx Dy 
            N  = arquivo com a Matriz
            Ox = x de Origem
            Oy = y de Origem
            Dx = x de Destino
            Dy = y de Destino
*/
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <utility>      /* pair */
#include <unistd.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <math.h>
#include<bits/stdc++.h>
using namespace std;

/*
  Variáveis do sistema
    Tamanho da matriz
    Coordenadas de origem
    Coordenadas de destino
  Step-by-Step
    Criar ambiente 
    Dispor origem  
    Dispor destino 

  Regras
    Regra de movimentação
        4 direções
  
  Ambiente de tarefa 
    Agente -> Robo
    Medida de desempenho -> Quantidade de nos ateh o destino,custo
    Ambiente -> Campo com diferentes terrenos

    Completamente Parcialemente Por causa dos algoritmos de busca
    Deterministico -> algo de busca
    Sequencial -> O proximo no vizitado depende do anterior
    Estatico ,  a nao ser que o rio do campo transborde ou o fogo aumente
    Discreto , me da a entender sobre o conjunto enumeravel

*/

struct Matriz{
  int N;  //Ordem da matriz
  int** Tab; // Valores da Matriz
};

Matriz make_Matriz(int N){
  Matriz m;
  m.N = N;
  m.Tab = (int **)malloc(N * sizeof(int*));
  for(int i = 0; i < N; i++)
    m.Tab[i] = (int *)malloc(N * sizeof(int));
  return m;
}

void destroy_Matriz(Matriz m){
  for (int i = 0; i < m.N; i++)
    free(m.Tab[i]);
  free(m.Tab);
}

Matriz Criar_ambiente(std::string entrada){
  int valor;
  std::ifstream data(entrada);
  data >> valor;
  Matriz m = make_Matriz(valor);

  for(int i = 0 ; i < m.N ; i++ ){
    for(int j = 0 ; j < m.N ; j++ ){
      data >> m.Tab[i][j];
      if(m.Tab[i][j]==0){
        m.Tab[i][j]++;
      }else{
        m.Tab[i][j] = m.Tab[i][j]*5;
      }
    }
  }
  return m;
}

void Print_Matriz(Matriz m){
		printf("    |");
    for (int a = 0; a < m.N; a++)
      printf("%3d |",a);
    printf("\n");
    for (int a = 0; a <= m.N; a++)
      printf("-----");
    printf("\n");
    for (int i = 0; i < m.N; i++)
    {
      printf("%3d |",i);
      for (int j = 0; j < m.N; j++)
      {
        if(m.Tab[i][j] != 0)
          printf(" %3d |", m.Tab[i][j]);
        else
          printf("    |");
      }
      printf("\n");
    }
    printf("\n");
}

struct Aresta{
  int valor;
  std::pair<int,int> destino;
};

Aresta make_Aresta(int v, int x, int y){
  Aresta a;
  a.valor = v;
  a.destino = std::make_pair(x,y);
  return a;
}

struct No{
  int A;
  Aresta* Arestas; //Vetor de arestas <distancia, No destino>
};

No make_No(int N){
  No n;
  n.A = N;
  n.Arestas = (Aresta *) malloc (N * sizeof(Aresta));
  return n;
}

struct Grafo{
  int N; // NxN = Quantidade de Nós
  No** Nos; // Matriz de Nós do grafo
};

Grafo make_Grafo(int N){
  Grafo g;
  g.N = N;
  g.Nos = (No **)malloc(N * sizeof(No *));
  for(int i = 0; i < N; i++){
    g.Nos[i] = (No *)malloc(N * sizeof(No));
  }
  return g;
}

void destroy_Grafo(Grafo g){
  for(int i = 0 ; i < g.N; i++){
    free(g.Nos[i]->Arestas);
    free(g.Nos[i]);
  }
  free(g.Nos);
}

Grafo Matriz_to_Grafo(Matriz M){
  Grafo g = make_Grafo(M.N);
  
  for(int i=0; i < g.N; i++){
    for(int j=0; j < g.N; j++){
      No n;
      if(i==0){
        if(j==0){
            //<0,0>
            n = make_No(2); // 2 por terem duas arestas 
            n.Arestas[0] = make_Aresta(M.Tab[0][1], 0, 1);
            n.Arestas[1] = make_Aresta(M.Tab[1][0], 1, 0);
        }else{
          if(j==g.N-1){
            //<0,N-1>
            n = make_No(2);// 2 por ter 2 arestas 
            n.Arestas[0] = make_Aresta(M.Tab[0][j-1], 0, j-1);
            n.Arestas[1] = make_Aresta(M.Tab[1][j], 1, j);
          }else{
            //<0,j> topo
            n = make_No(3); // max 3 arestas 
            n.Arestas[0] = make_Aresta(M.Tab[0][j-1], 0, j-1);
            n.Arestas[1] = make_Aresta(M.Tab[0][j+1], 0, j+1);
            n.Arestas[2] = make_Aresta(M.Tab[1][j], 1, j);
          }
        }
      }else{
        if(i==g.N-1){
          if(j==0){
            //<N-1,0>
            n = make_No(2);// 2 por ter 2 arestas 
            n.Arestas[0] = make_Aresta(M.Tab[i-1][0], i-1, 0);
            n.Arestas[1] = make_Aresta(M.Tab[i][1], i, 1);
          }else{
            if(j==g.N-1){
            //<N-1,N-1>
            n = make_No(2);// 2 por ter 2 arestas 
            n.Arestas[0] = make_Aresta(M.Tab[i-1][j], i-1, j);
            n.Arestas[1] = make_Aresta(M.Tab[i][j-1], i, j-1);
            }else{
              //<N-1,j>
              n = make_No(3); // max 3 arestas 
              n.Arestas[0] = make_Aresta(M.Tab[i][j-1], i, j-1);
              n.Arestas[1] = make_Aresta(M.Tab[i][j+1], i, j+1);
              n.Arestas[2] = make_Aresta(M.Tab[i-1][j], i-1, j);
            }
          }
        }else{
          if(j==0){
            //<i,0> esquerda
            n = make_No(3); // max 3 arestas 
            n.Arestas[0] = make_Aresta(M.Tab[i-1][0], i-1, 0);
            n.Arestas[1] = make_Aresta(M.Tab[i+1][0], i+1, 0);
            n.Arestas[2] = make_Aresta(M.Tab[i][1], i, 1);
          }else{
            if(j==g.N-1){
              //<i,N-1> direita
              n = make_No(3); // max 3 arestas 
              n.Arestas[0] = make_Aresta(M.Tab[i-1][j], i-1, j);
              n.Arestas[1] = make_Aresta(M.Tab[i][j-1], i, j-1);
              n.Arestas[2] = make_Aresta(M.Tab[i+1][j], i+1, j);
            }else{
              //<i,j>
              n = make_No(4); // 4 arestas
              n.Arestas[0] = make_Aresta(M.Tab[i][j-1], i, j-1);
              n.Arestas[1] = make_Aresta(M.Tab[i][j+1], i, j+1);
              n.Arestas[2] = make_Aresta(M.Tab[i-1][j], i-1, j);
              n.Arestas[3] = make_Aresta(M.Tab[i+1][j], i+1, j);
            }
          }
        }
      }
      g.Nos[i][j] = n;
    }
  }

  return g;
}

void Print_Grafo(Grafo G){
  printf("Grafo com %d nós\n", G.N*G.N);
  for(int i=0; i < G.N; i++){
    for(int j=0; j < G.N; j++){
      printf("\tNó <%2d,%2d>:\n",i,j);
      for(int a=0; a < G.Nos[i][j].A ; a++){
        printf("\t\t%2d: %2d <%2d,%2d>\n", a, G.Nos[i][j].Arestas[a].valor, G.Nos[i][j].Arestas[a].destino.first, G.Nos[i][j].Arestas[a].destino.second);
      }
    }
  }
}

struct Robo{
  std::pair <int,int> Posicao; // <x,y> - posição atual na matriz
  std::list <std::pair <int,int> > Caminho; // <x,y> - lista das posições anteriores do robo
};

Robo make_Robo(int x, int y, std::list <std::pair <int,int> > caminho){ // Cria um agente com posição <x,y>
  Robo r;
  r.Posicao = std::make_pair(x,y);
  r.Caminho = caminho;
  return r;
}

int main(int argc, char **argv){
  /*  
    N  = arquivo com a Matriz
    Ox = x de Origem
    Oy = y de Origem
    Dx = x de Destino
    Dy = y de Destino
  */
  std::string arquivo = argv[1];
  std::pair <int,int> O, D;
  O = make_pair(atoi(argv[2]), atoi(argv[3]));
  D = make_pair(atoi(argv[4]), atoi(argv[5]));
  std::list <std::pair <int,int> > c;

  Matriz Ambiente = Criar_ambiente(arquivo);
  //Print_Matriz(Ambiente);
  
  Grafo G = Matriz_to_Grafo(Ambiente);
  //Print_Grafo(G);

  Robo Bender = make_Robo(O.first, O.second, c);

  destroy_Grafo(G);
  destroy_Matriz(Ambiente);

  return 0;
}