// https://github.com/raketanamarse/super_pc/

#include <iostream>
#include <cmath>
#include "L.hpp"
#include "F.hpp"
#include "vars.hpp"
#include <ctime> // заголовочный файл с прототипом функции clock()
using namespace std;

// формула навигации 

int main(int, char**) {

    unsigned int start_time =  clock(); // начальное время
    cout << "start prog\n";

    const int kol_el = (NX+1) * (NY+1) * (NZ+1);
    int i;
    double *cube = new double[kol_el];

    for(i=0; i<kol_el; i++){
        kol_el[i]=i;
    }
    

    

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    cout << "runtime = " << search_time << " ms";
}
