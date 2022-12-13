// https://github.com/raketanamarse/super_pc/


#include <iostream>
#include <cmath>
#include <fstream> 
#include "L.hpp"
#include "F.hpp"
#include "vars.hpp"
#include <ctime> // заголовочный файл с прототипом функции clock()
using namespace std;

#define file_name "out.txt"



void write_to_file(double *massiv){
    try{
        ofstream MyFile(file_name);

        // Write to the file
        for(int i=0; i<N; i++){
            MyFile << massiv[i] << ' ' << count_X(i) << ' ' << count_Y(i) << ' ' << count_Z(i) <<"\n"; // U x y z
        }
        
        // Close the file
        MyFile.close();
    }
    catch(int cod_error){cout << "error write to " << file_name << "with error code" << cod_error;}
}



int main(int, char**) {

    unsigned int start_time =  clock(); // начальное время
    cout << "start prog\n";

    int i;
    double *cube = new double[N];

    for(i=0; i<N; i++){
        cube[i]=i;
    }
    

    write_to_file(cube);

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    cout << "runtime = " << search_time << " ms";
}


