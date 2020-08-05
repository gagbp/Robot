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
#include <iostream>
#include <fstream>
#include <omp.h>
#include <list>         /* list*/

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
    

*/

struct Matriz{
	int N; // Tamanho da matriz quadrada
  int** tab; // Ponteiro para a matriz NxN
};

struct Robo{
    std::pair <int,int> Posicao; // <x,y> - posição atual na matriz
    std::list <std::pair <int,int> > PosicaoAnt; // <x,y> - lista das posições anteriores da formiga
};

int main(){
    return 0;
}