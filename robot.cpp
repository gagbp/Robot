/*
  Compilar: g++ robot.cpp -std=c++11
  Executar: ./a.out N Ox Oy Dx Dy M
            N  = arquivo com a Matriz
            Ox = x de Origem
            Oy = y de Origem
            Dx = x de Destino
            Dy = y de Destino
            M  = método(1- BFS, 2-Dijkstra, 3-A*)
*/
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <utility>      /* pair */
#include <unistd.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include<bits/stdc++.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

using namespace std;



int id_cont = 1;
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


  dijkkstra psdo code
  1:	function Dijkstra(Graph, source):
  2:	for each vertex v in Graph:	// Initialization
  3:	dist[v] := infinity	// initial distance from source to vertex v is set to infinite
  4:	previous[v] := undefined	// Previous node in optimal path from source
  5:	dist[source] := 0	// Distance from source to source
  6:	Q := the set of all nodes in Graph	// all nodes in the graph are unoptimized - thus are in Q
  7:	while Q is not empty:	// main loop
  8:	u := node in Q with smallest dist[ ]
  9:	remove u from Q
  10:	for each neighbor v of u:	// where v has not yet been removed from Q.
  11:	alt := dist[u] + dist_between(u, v)
  12:	if alt < dist[v]	// Relax (u,v)
  13:	dist[v] := alt
  14:	previous[v] := u
  15:	return previous[ ]
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
		printf("\033[s");
    printf("   |");
    for (int a = 0; a < m.N ; a++)
      printf("%3d|",a);
    printf("\n");
    for (int a = 0; a <= m.N; a++)
      printf("----");
    printf("\n");
    for (int i = 0; i < m.N; i++)
    {
      printf("%3d|",i);
      for (int j = 0; j < m.N; j++)
      {
        if(m.Tab[i][j] != 0){
          if(m.Tab[i][j] == 99 || m.Tab[i][j] == 9 || m.Tab[i][j] == 49 || m.Tab[i][j] == 149 ){
            printf(MAG " 0 |" RESET);
          }else if(m.Tab[i][j] == 98 || m.Tab[i][j] == 8 || m.Tab[i][j] == 48 || m.Tab[i][j] == 148){
            printf(YEL " 0 |" RESET);
          }else if(m.Tab[i][j] == 1){
            printf(GRN " 0 |" RESET);
          }else if(m.Tab[i][j] == 5){
            printf(WHT " 0 |" RESET);
          }else if(m.Tab[i][j] == 10){
            printf(CYN " 0 |" RESET);
          }else if(m.Tab[i][j] == 15){
            printf(RED " 0 |" RESET);
          }else{
            printf(" 0 |");
          }
        }else{
          //printf("|");
        }
      }
      printf("\n");
    }
    printf("\n");
  printf("\033[u");
}

void ImprimirMatrizVisual(Matriz M,std::string saida){
  ofstream img (saida);
  img << "P3" << endl;
  img << M.N*10 << " " << M.N*10 << endl;
  img << "255" << endl;

  int r,g,b;

  for (int i = 0; i < M.N*10;  i += 1) {
    for (int j = 0; j < M.N*10; j += 1) {
      if(M.Tab[i/10][j/10] == 1){
        r = 0;
        g = 255;
        b = 0;
      }else if(M.Tab[i/10][j/10] == 5){
        r = 102;
        g = 51;
        b = 0;
      }else if(M.Tab[i/10][j/10] == 10){
        r = 0;
        g = 204;
        b = 255;
      }else if(M.Tab[i/10][j/10] == 15){
        r = 255;
        g = 102;
        b = 0;
      }else if(M.Tab[i/10][j/10] == 0){
        r = 0;
        g = 0;
        b = 0;
      }else if(M.Tab[i/10][j/10] == 99 || M.Tab[i/10][j/10] == 9 || M.Tab[i/10][j/10] == 49 || M.Tab[i/10][j/10] == 149){
        r = 255;
        g = 255;
        b = 255;
      }else if(M.Tab[i/10][j/10] == 98 || M.Tab[i/10][j/10] == 8 || M.Tab[i/10][j/10] == 48 || M.Tab[i/10][j/10] == 148){
        r = 255;
        g = 255;
        b = 0;
      }else{
        r = 122;
        g = 122;
        b = 122;
      }
      img << r << " " << g << " " << b << endl;
    }
  }
}

struct Aresta{
  int valor;
  std::pair<int,int> destino;
   inline bool operator==(Aresta a){
    if(valor == a.valor && destino == a.destino){
      return true;
    }else{
      return false;
    }
   }
   inline bool operator!=(Aresta a){
    if(valor == a.valor && destino == a.destino){
      return false;
    }else{
      return true;
    }
   }

};

Aresta make_Aresta(int v, int x, int y){
  Aresta a;
  a.valor = v;
  a.destino = std::make_pair(x,y);
  return a;
}

struct No{
  int A; //numeor de arestas
  int id;
  Aresta* Arestas; //Vetor de arestas <distancia, No destino>


  int id_anterior;

  int i,j,valor_bfs;

  float f = INT_MAX;
  float g = INT_MAX;
  float h = INT_MAX;

  inline bool operator==(const No& a){
    if(id == a.id){
      return true;
    }else{
      return false;
    }
  }
    inline bool operator!=(const No& a){
    if(id == a.id){
      return false;
    }else{
      return true;
    }
  }
};

No make_No(int N,int x,int y){
  No n;
  n.i = x;
  n.j = y;
  n.id = id_cont;
  id_cont++;

  n.A = N; // numero de arestas
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
            n = make_No(2,i,j); // 2 por terem duas arestas
            n.Arestas[0] = make_Aresta(M.Tab[0][1], 0, 1);
            n.Arestas[1] = make_Aresta(M.Tab[1][0], 1, 0);
        }else{
          if(j==g.N-1){
            //<0,N-1>
            n = make_No(2,i,j);// 2 por ter 2 arestas
            n.Arestas[0] = make_Aresta(M.Tab[0][j-1], 0, j-1);
            n.Arestas[1] = make_Aresta(M.Tab[1][j], 1, j);
          }else{
            //<0,j> topo
            n = make_No(3,i,j); // max 3 arestas
            n.Arestas[0] = make_Aresta(M.Tab[0][j-1], 0, j-1);
            n.Arestas[1] = make_Aresta(M.Tab[0][j+1], 0, j+1);
            n.Arestas[2] = make_Aresta(M.Tab[1][j], 1, j);
          }
        }
      }else{
        if(i==g.N-1){
          if(j==0){
            //<N-1,0>
            n = make_No(2,i,j);// 2 por ter 2 arestas
            n.Arestas[0] = make_Aresta(M.Tab[i-1][0], i-1, 0);
            n.Arestas[1] = make_Aresta(M.Tab[i][1], i, 1);
          }else{
            if(j==g.N-1){
            //<N-1,N-1>
            n = make_No(2,i,j);// 2 por ter 2 arestas
            n.Arestas[0] = make_Aresta(M.Tab[i-1][j], i-1, j);
            n.Arestas[1] = make_Aresta(M.Tab[i][j-1], i, j-1);
            }else{
              //<N-1,j>
              n = make_No(3,i,j); // max 3 arestas
              n.Arestas[0] = make_Aresta(M.Tab[i][j-1], i, j-1);
              n.Arestas[1] = make_Aresta(M.Tab[i][j+1], i, j+1);
              n.Arestas[2] = make_Aresta(M.Tab[i-1][j], i-1, j);
            }
          }
        }else{
          if(j==0){
            //<i,0> esquerda
            n = make_No(3,i,j); // max 3 arestas
            n.Arestas[0] = make_Aresta(M.Tab[i-1][0], i-1, 0);
            n.Arestas[1] = make_Aresta(M.Tab[i+1][0], i+1, 0);
            n.Arestas[2] = make_Aresta(M.Tab[i][1], i, 1);
          }else{
            if(j==g.N-1){
              //<i,N-1> direita
              n = make_No(3,i,j); // max 3 arestas
              n.Arestas[0] = make_Aresta(M.Tab[i-1][j], i-1, j);
              n.Arestas[1] = make_Aresta(M.Tab[i][j-1], i, j-1);
              n.Arestas[2] = make_Aresta(M.Tab[i+1][j], i+1, j);
            }else{
              //<i,j>
              n = make_No(4,i,j); // 4 arestas
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

struct Saida{
  int NosExp;
  int NosCam;
  int Custo;
};

Saida make_Saida(int a, int b, int c){
  Saida s;
  s.NosExp = a;
  s.NosCam = b;
  s.Custo = c;
  return s;
}

Saida bfs(Grafo g,int x , int y, int d_x , int d_y,Matriz M, Matriz A){
  Saida retorno = make_Saida(0,0,0); // retorno(numero de nos expandidos,numero de nos no caminho, custo)
  std::list<No> lista_No;
  std::list<No> lista_No_Marcado;

  //M.Tab[x][y] = M.Tab[x][y]*10 -1;
  g.Nos[x][y].valor_bfs = 0;
  g.Nos[x][y].id_anterior = g.Nos[x][y].id;
  lista_No_Marcado.push_back(g.Nos[x][y]);
  lista_No.push_back(g.Nos[x][y]);

  //std::cout << "vertice inicial : " << x << " " << y <<"\n";
  while(std::find(lista_No.begin(),lista_No.end(),g.Nos[d_x][d_y]) == lista_No.end()){
    std::list<No>::iterator it = lista_No.begin();
    for(int i = 0 ; i < it->A;i++){
      std::pair<int,int> dest_aux = it->Arestas[i].destino;

      if( std::find(lista_No_Marcado.begin(),lista_No_Marcado.end(),g.Nos[dest_aux.first][dest_aux.second]) != lista_No_Marcado.end() ) {//procura no na lista se encontrou entrou
      //se esta marcado

      }else{
        //se nao esta marcado
        g.Nos[dest_aux.first][dest_aux.second].id_anterior = it->id;
        g.Nos[dest_aux.first][dest_aux.second].valor_bfs = it->Arestas[i].valor;
        lista_No_Marcado.push_back(g.Nos[dest_aux.first][dest_aux.second]);
        lista_No.push_back(g.Nos[dest_aux.first][dest_aux.second]);
        //std::cout << "vertice visitado : " << dest_aux.first << " " << dest_aux.second <<"\n";

        M.Tab[dest_aux.first][dest_aux.second] = M.Tab[dest_aux.first][dest_aux.second]*10 -1;

        Print_Matriz(M);
        retorno.NosExp++;
        //sleep(1);
      }

    }
    lista_No.pop_front();
  }

  //backtrak
  M.Tab[d_x][d_y]--;
  No caminho = g.Nos[d_x][d_y];
  retorno.Custo += g.Nos[d_x][d_y].valor_bfs;
  while(caminho.id != caminho.id_anterior){
    for(int i = 0; i<g.N; i++){
      for(int j = 0 ; j<g.N ; j++){
        if(caminho.id_anterior == g.Nos[i][j].id){
          //std:: cout << "caminho " << i << " " << j << "\n";
          retorno.Custo += g.Nos[i][j].valor_bfs;
          retorno.NosCam++;
          caminho = g.Nos[i][j];
          M.Tab[i][j] --;
          A.Tab[i][j] = M.Tab[i][j];
        }
      }
    }
  }
  Print_Matriz(M);
  return retorno;
}
/// lista.remove n funcionando , fazer com  erase usando o id

Saida Dijkstra(Grafo g, int x, int y,int d_x,int d_y, Matriz M, Matriz A){
  Saida retorno = make_Saida(0,0,0); // retorno first é numero de nos , segundo eh custo
  int distancia[g.N][g.N]; // alocado estatico para teste
  No anterior[g.N][g.N];
  g.Nos[x][y].id_anterior = g.Nos[x][y].id;
  std::list<No> lista_No;

  for (int i = 0 ; i < g.N; i++){
    for (int j = 0 ; j < g.N; j++){
      lista_No.push_back(g.Nos[i][j]);
      if(x == i && y == j){
        distancia[i][j] = 0;
      }else{
        distancia[i][j] = INT_MAX;
      }
    }
  }

  //um dos while eh toda a matriz
  while(std::find(lista_No.begin(),lista_No.end(),g.Nos[d_x][d_y]) != lista_No.end()){
  //while(lista_No.size() != 0){
    //printf("tamanho da lista %ld\n",lista_No.size());
    //procura a menor distancia
    int menor = INT_MAX;
    int x_aux,y_aux;
    for (int i = 0 ; i< g.N ; i++){
      for (int j = 0 ; j<g.N ; j++){
        if(menor > distancia[i][j] && std::find(lista_No.begin(),lista_No.end(),g.Nos[i][j]) != lista_No.end()){
          menor = distancia[i][j];

          if(M.Tab[i][j] == 99 || M.Tab[i][j] == 9 ||
            M.Tab[i][j] == 49 || M.Tab[i][j] == 149 ){
          }else{
            M.Tab[i][j] = M.Tab[i][j]*10 -1;
            retorno.NosExp++;
          }
          Print_Matriz(M);
          x_aux = i;
          y_aux = j;
        }
      }
    }
    //printf("menor %d\n i = %d j = %d\n",menor,x_aux,y_aux);
    //printf("tamanho da lista %ld\n",lista_No.size());
    lista_No.remove(g.Nos[x_aux][y_aux]); // remove o menor da lista
    //printf("tamanho da lista %ld\n",lista_No.size());

    /*
    for(list<No>::iterator it = lista_No.begin();it != lista_No.end();it++){
      std::cout << it->id << "\n";
      if(it->id == g.Nos[x_aux][y_aux].id ){
        printf("oi do %d \n",g.Nos[x_aux][y_aux].id);
        printf("tamanho da lista %ld\n",lista_No.size());
        it =lista_No.erase(it);
        printf("tamanho da lista %ld\n",lista_No.size());
      }
    }
    */

    for(int i = 0; i < g.Nos[x_aux][y_aux].A;i++){
      std::pair<int,int> dest = g.Nos[x_aux][y_aux].Arestas[i].destino ;
      if( std::find(lista_No.begin(),lista_No.end(),g.Nos[dest.first][dest.second]) != lista_No.end() ){
        //procura o no destino da lista se encontrou entrou
        int alt = distancia[x_aux][y_aux] + g.Nos[x_aux][y_aux].Arestas[i].valor;
        if (alt < distancia[dest.first][dest.second]){
          distancia[dest.first][dest.second] = alt;
          anterior[x_aux][y_aux] = g.Nos[dest.first][dest.second];
          g.Nos[dest.first][dest.second].id_anterior = g.Nos[x_aux][y_aux].id;
        }
      }
    }
  }

    //backtrak

  M.Tab[d_x][d_y] --;
  No caminho = g.Nos[d_x][d_y];
  while(caminho.id != caminho.id_anterior){
    for(int i = 0; i<g.N; i++){
      for(int j = 0 ; j<g.N ; j++){
        if(caminho.id_anterior == g.Nos[i][j].id){
          //std:: cout << "caminho " << i << " " << j << "\n";
          caminho = g.Nos[i][j];
          retorno.NosCam++;
          M.Tab[i][j]--;
          A.Tab[i][j] = M.Tab[i][j];
        }
      }
    }
  }

  Print_Matriz(M);
  retorno.Custo = distancia[d_x][d_y];
  return retorno;
  //printf("Distancia dijkstra: %d", distancia[d_x][d_y]);
}

Saida A_estrela(Grafo g,int x,int y , int d_x,int d_y,Matriz M, Matriz A){

  /*
    // A* (star) Pathfinding
    // Initialize both open and closed list
    let the openList equal empty list of nodes
    let the closedList equal empty list of nodes
    // Add the start node
    put the startNode on the openList (leave it's f at zero)
    // Loop until you find the end
    while the openList is not empty
      // Get the current node
      let the currentNode equal the node with the least f value
      remove the currentNode from the openList
      add the currentNode to the closedList
      // Found the goal
      if currentNode is the goal
          Congratz! You've found the end! Backtrack to get path
      // Generate children
      let the children of the currentNode equal the adjacent nodes

      for each child in the children
          // Child is on the closedList
          if child is in the closedList
              continue to beginning of for loop
          // Create the f, g, and h values
          child.g = currentNode.g + distance between child and current
          child.h = distance from child to end
          child.f = child.g + child.h
          // Child is already in openList
          if child.position is in the openList's nodes positions
              if the child.g is higher than the openList node's g
                  continue to beginning of for loop
          // Add the child to the openList
          add the child to the openList
  */
  Saida retorno = make_Saida(0,0,0); // retorno first é numero de nos , segundo eh custo

  g.Nos[x][y].id_anterior = g.Nos[x][y].id;
  M.Tab[x][y] = M.Tab[x][y]*10 -1;
  g.Nos[x][y].valor_bfs = 0;
  std::list<No> open_list;
  std::list<No> close_list;
  g.Nos[x][y].f = 0;
  open_list.push_back(g.Nos[x][y]);
  while(!open_list.empty()){
    float f_aux = open_list.begin()->f;
    auto it_selecionado = open_list.begin();
    for(auto it = open_list.begin();it != open_list.end() ; it++){
      if(f_aux > it->f){
        f_aux = it->f;
        it_selecionado = it;
      }
    }
    open_list.erase(it_selecionado);
    close_list.push_back(it_selecionado.operator*());

    if(it_selecionado->id == g.Nos[d_x][d_y].id){
      //fazer backtracking
      //std:: cout << it_selecionado->id << "\n";
      //std:: cout << it_selecionado->id_anterior << "\n";
      //std:: cout << "backtrack\n";
      //retorno.second = it_selecionado->f;
      auto caminho = it_selecionado;
      retorno.Custo += caminho->valor_bfs;
      //std:: cout << caminho->id << "\n";
      //retorno.first++;
      while(caminho->id != caminho->id_anterior){
        //sleep(1);
        for(auto it = close_list.begin();it != close_list.end() ; it++){
          if(caminho->id_anterior == it->id){
            //std:: cout << "caminho " << caminho->id << "\n";
            M.Tab[caminho->i][caminho->j]--;
            caminho = it;
            retorno.NosCam++;
            retorno.Custo += caminho->valor_bfs;
          //  std:: cout << "TOTAL " << retorno.second << "
          // ATUAL  " << caminho->valor_bfs  << "\n";
          }
        }
      }
      retorno.NosCam++;
      M.Tab[x][y]--;
      A.Tab[x][y] = M.Tab[x][y];
      Print_Matriz(M);
      break;
    }

    for(int i = 0 ; i < it_selecionado->A ; i++){
      std::pair<int, int> pair_aux = it_selecionado->Arestas[i].destino;

      if(std::find(close_list.begin(),close_list.end(),g.Nos[pair_aux.first][pair_aux.second]) != close_list.end()){
        continue;
      }
      No filho = g.Nos[pair_aux.first][pair_aux.second];

      if(std::find(open_list.begin(),open_list.end(),filho) != open_list.end()){
        if(filho.g < it_selecionado->f + it_selecionado->Arestas[i].valor ){
          continue;
        }
      }
      //std:: cout << it_selecionado->id << " -> " << filho.id << "\n";
      filho.valor_bfs = it_selecionado->Arestas[i].valor;
      filho.id_anterior = it_selecionado->id;
      filho.g = it_selecionado->f + it_selecionado->Arestas[i].valor;
      filho.h = sqrt(pow(d_x - pair_aux.first,2 ) + pow(d_y -pair_aux.second ,2));
      filho.f = filho.g + filho.h;
      open_list.push_front(filho);

      if(M.Tab[filho.i][filho.j] == 99 || M.Tab[filho.i][filho.j] == 9 ||
      M.Tab[filho.i][filho.j] == 49 || M.Tab[filho.i][filho.j] == 149 ){
        //nada
      }else{
        M.Tab[filho.i][filho.j] = M.Tab[filho.i][filho.j]*10 -1;
      }
      retorno.NosExp++;
      Print_Matriz(M);
      //sleep(1);
    }

  }
  return retorno;
}


Saida A_estrela2(Grafo g,int x,int y , int d_x,int d_y,Matriz M, Matriz A){


  Saida retorno = make_Saida(0,0,0); // retorno first é numero de nos , segundo eh custo

  g.Nos[x][y].id_anterior = g.Nos[x][y].id;
  M.Tab[x][y] = M.Tab[x][y]*10 -1;
  g.Nos[x][y].valor_bfs = 0;
  std::list<No> open_list;
  std::list<No> close_list;
  g.Nos[x][y].f = 0;
  open_list.push_back(g.Nos[x][y]);
  while(!open_list.empty()){
    float f_aux = open_list.begin()->f;
    auto it_selecionado = open_list.begin();
    for(auto it = open_list.begin();it != open_list.end() ; it++){
      if(f_aux > it->f){
        f_aux = it->f;
        it_selecionado = it;
      }
    }
    open_list.erase(it_selecionado);
    close_list.push_back(it_selecionado.operator*());

    if(it_selecionado->id == g.Nos[d_x][d_y].id){
      //fazer backtracking
      //std:: cout << it_selecionado->id << "\n";
      //std:: cout << it_selecionado->id_anterior << "\n";
      //std:: cout << "backtrack\n";
      //retorno.second = it_selecionado->f;
      auto caminho = it_selecionado;
      retorno.Custo += caminho->valor_bfs;
      //std:: cout << caminho->id << "\n";
      //retorno.first++;
      while(caminho->id != caminho->id_anterior){
        //sleep(1);
        for(auto it = close_list.begin();it != close_list.end() ; it++){
          if(caminho->id_anterior == it->id){
            //std:: cout << "caminho " << caminho->id << "\n";
            M.Tab[caminho->i][caminho->j]--;
            A.Tab[caminho->i][caminho->j] = M.Tab[caminho->i][caminho->j];
            caminho = it;
            retorno.NosCam++;
            retorno.Custo += caminho->valor_bfs;
          //  std:: cout << "TOTAL " << retorno.second << "
          // ATUAL  " << caminho->valor_bfs  << "\n";
          }
        }
      }
      retorno.NosCam++;
      M.Tab[x][y]--;
      A.Tab[x][y] = M.Tab[x][y];
      Print_Matriz(M);
      break;
    }

    for(int i = 0 ; i < it_selecionado->A ; i++){
      std::pair<int, int> pair_aux = it_selecionado->Arestas[i].destino;

      if(std::find(close_list.begin(),close_list.end(),g.Nos[pair_aux.first][pair_aux.second]) != close_list.end()){
        continue;
      }

      g.Nos[pair_aux.first][pair_aux.second].valor_bfs = it_selecionado->Arestas[i].valor;
      g.Nos[pair_aux.first][pair_aux.second].id_anterior = it_selecionado->id;
      g.Nos[pair_aux.first][pair_aux.second].g = it_selecionado->f + it_selecionado->Arestas[i].valor;
      g.Nos[pair_aux.first][pair_aux.second].h = sqrt(pow(d_x - pair_aux.first,2 ) + pow(d_y -pair_aux.second ,2));
      g.Nos[pair_aux.first][pair_aux.second].f = g.Nos[pair_aux.first][pair_aux.second].g + g.Nos[pair_aux.first][pair_aux.second].h;

      No filho = g.Nos[pair_aux.first][pair_aux.second];

      auto filhoIN_openList = std::find(open_list.begin(),open_list.end(),filho);
      if( filhoIN_openList != open_list.end()){
        if(filho.g > filhoIN_openList->g ){
          continue;
        }
      }
      open_list.push_front(filho);
      //std:: cout << it_selecionado->id << " -> " << filho.id << "\n";


      if(M.Tab[filho.i][filho.j] == 99 || M.Tab[filho.i][filho.j] == 9 ||
      M.Tab[filho.i][filho.j] == 49 || M.Tab[filho.i][filho.j] == 149 ){
        //nada
      }else{
        M.Tab[filho.i][filho.j] = M.Tab[filho.i][filho.j]*10 -1;
        retorno.NosExp++;
      }

      Print_Matriz(M);
      //sleep(1);
    }

  }
  return retorno;
}


int main(int argc, char **argv){
  /*
    N  = arquivo com a Matriz
    Ox = x de Origem
    Oy = y de Origem
    Dx = x de Destino
    Dy = y de Destino
    M  = método(1- BFS, 2-Dijkstra, 3-A*)
  */
  ofstream myfile;

  std::string arquivo = argv[1];
  std::pair <int,int> O, D;
  O = std::make_pair(atoi(argv[2]), atoi(argv[3]));
  D = std::make_pair(atoi(argv[4]), atoi(argv[5]));
  std::list <std::pair <int,int> > c;

  Matriz Ambiente = Criar_ambiente(arquivo);

  if(O.first < 0 || O.first >= Ambiente.N || O.second < 0 || O.second >= Ambiente.N){
    printf("\nCoordenadas de entrada inválidas!\n");
    return 0;
  }

  if(D.first < 0 || D.first >= Ambiente.N || D.second < 0 || D.second >= Ambiente.N){
    printf("\nCoordenadas de destino inválidas!\n");
    return 0;
  }

  Grafo G = Matriz_to_Grafo(Ambiente);
  //Print_Grafo(G);


 // Ambiente.Tab[O.first][O.second] = 255;
  //Ambiente.Tab[D.first][D.second] = 0;

  //BFS.Tab[O.first][O.second] = 255;
  //BFS.Tab[D.first][D.second] = 0;

  //Dijk.Tab[O.first][O.second] = 255;
  //Dijk.Tab[D.first][D.second] = 0;

  //Aestrela.Tab[O.first][O.second] = 255;
 // Aestrela.Tab[D.first][D.second] = 0;


  //ImprimirMatrizVisual(Ambiente,"Ambiente.ppm");

  //ImprimirMatrizVisual(BFS,"BFS.ppm");

  Robo Bender = make_Robo(O.first, O.second, c);

  Saida aux = make_Saida(0,0,0);
  char nomearquivo[100];
  sprintf(nomearquivo, "saida%d_(%d,%d)->(%d,%d).txt",atoi(argv[6]),O.first,O.second,D.first,D.second);

  myfile.open (nomearquivo);
  myfile << "Origem: (" << O.first << "," << O.second << ")\nDestino: ("<< D.first << "," << D.second << ")\n";

  //if(atoi(argv[6]) == 1){
    Matriz BFS = Criar_ambiente(arquivo);
    myfile << "BFS\n";
    aux = bfs(G,O.first,O.second,D.first,D.second,BFS,Ambiente);
    myfile << "Nos Expandidos : " << aux.NosExp <<" Nos no Caminho : " << aux.NosCam << " Custo : " << aux.Custo << "\n";

    ImprimirMatrizVisual(BFS,"BFS.ppm");
    ImprimirMatrizVisual(Ambiente,"BFS_caminho.ppm");
    Ambiente = Criar_ambiente(arquivo);
    destroy_Matriz(BFS);
  //}else if(atoi(argv[6]) ==  2){
    Matriz Dijk = Criar_ambiente(arquivo);
    myfile << "Disjktra\n";
    aux = Dijkstra(G,O.first,O.second,D.first,D.second,Dijk,Ambiente);
    myfile << "Nos Expandidos : " << aux.NosExp <<" Nos no Caminho : " << aux.NosCam << " Custo : " << aux.Custo << "\n";
    ImprimirMatrizVisual(Dijk,"Dijk.ppm");
    ImprimirMatrizVisual(Ambiente,"Dijk_caminho.ppm");
    Ambiente = Criar_ambiente(arquivo);
    destroy_Matriz(Dijk);
  //}else if(atoi(argv[6]) == 3){
    Matriz Aestrela = Criar_ambiente(arquivo);
    myfile << "A*\n";
    aux = A_estrela2(G,O.first,O.second,D.first,D.second,Aestrela,Ambiente);
    myfile << "Nos Expandidos : " << aux.NosExp <<" Nos no Caminho : " << aux.NosCam << " Custo : " << aux.Custo << "\n";
    ImprimirMatrizVisual(Aestrela,"Aestrela.ppm");
    ImprimirMatrizVisual(Ambiente,"Aestrela_caminho.ppm");
    destroy_Matriz(Aestrela);
  //}else{
    //printf("Método inválido!!!\n(1- BFS, 2-Dijkstra, 3-A*)");
 // }

  myfile.close();

  destroy_Grafo(G);
  destroy_Matriz(Ambiente);
  return 0;
}