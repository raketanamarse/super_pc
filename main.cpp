// https://github.com/raketanamarse/super_pc/

#include <iostream>
#include <cmath>
#include <fstream> 
#include <cstring>
#include "l.hpp"
#include "f.hpp"
#include "vars.hpp"
#include "coords.hpp"
#include <ctime> // заголовочный файл с прототипом функции clock()
using namespace std;

#define file_name "../out.txt"



void write_to_file(double *massiv){
    try{
        ofstream MyFile(file_name);
        int *xyz = new int[3];
        // Write to the file
        MyFile << NX << ' ' << NY << ' ' << NZ <<"\n"; // NX NY NZ
            
        for(int n = 0; n < N; n++){
            
            Count_XYZ(n, xyz);
            MyFile << massiv[n] << ' ' << xyz[0] << ' ' << xyz[1] << ' ' << xyz[2] <<"\n"; // U x y z
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
    double *cube = new double[N];//текущий куб
    double *next_cube = new double[N]; //следующий куб

    //инициализация нулями cube БЫСТРОЕ
    memset(cube, '0', sizeof(double)*N); 
    memset(next_cube, '0', sizeof(double)*N);

    for (unsigned t = 1; t < NT; ++t){

        double z = HZ;
        for (unsigned k = NY_NX; k < N - NY_NX; k += NY_NX){

            double y = HY; 
            for (unsigned j = NX; j < NY_NX - NX; j += NX){

               double x = HX;
               for (unsigned i = 1; i < NX - 1; i += 1){

                    unsigned n = i + j + k;
                    next_cube[n] = cube[n] + HT * (F(x, y, z) + DX * LX(cube[n - 1], cube[n], cube[n + 1]) + DY * LY(cube[n - NX], cube[n], cube[n + NX]) + DZ * LZ(cube[n - NY_NX], cube[n], cube[n + NY_NX]));
                    x += HX;
                }
                y += HY;
            }
            z += HZ;
        }
        auto p = cube;
        cube = next_cube;
        next_cube = p;
    }

   

    //write_to_file(cube);

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    cout << "runtime = " << search_time << " mks" << endl;
    cout << "runtime = " << search_time/1000 << " ms" << endl;
    cout << "runtime = " << search_time/1000000 << " s" << endl;
    cout << "runtime = " << (double)search_time/60000000 << " min" << endl;
    //cout << HT <<endl;
}


