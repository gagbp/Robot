//g++ matplot.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
int main() {
    plt::plot({1,3,2,4});
    plt::show();
}