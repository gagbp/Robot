//g++ matplot.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7
//clang++ -lpython3.8 -I/usr/local/lib/python3.8/dist-packages/numpy/core/include/ -I/usr/include/python3.8 main.cpp
#include "matplotlibcpp.h"
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

namespace plt = matplotlibcpp;

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

int main() {
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -5; j <= 5; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(::std::sin(0));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    plt::plot_surface(x, y, z);
    plt::show();
}