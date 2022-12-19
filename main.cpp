// https://github.com/raketanamarse/super_pc/

//сборка и запуск mpi выполняется через команды 
//mpic++ -o2 main.cpp -o main
//mpiexec -n 1 main
//вместо 1 можно указать любое количество процессоров/потоков

#include <iostream>
#include <cmath>
#include <fstream> 
#include <cstring>
#include "l.hpp"
#include "f.hpp"
#include "vars.hpp"
#include "coords.hpp"
#include "mpi.h"
#include <ctime> // заголовочный файл с прототипом функции clock()
using namespace std;

#define file_name "../out.txt"

const int TAG = 0;


// void write_to_file(double *massiv){
//     try{
//         ofstream MyFile(file_name);
//         int *xyz = new int[3];
//         // Write to the file
//         MyFile << NX << ' ' << NY << ' ' << NZ <<"\n"; // NX NY NZ
            
//         for(int n = 0; n < N; n++){
            
//             Count_XYZ(n, xyz);
//             MyFile << massiv[n] << ' ' << xyz[0] << ' ' << xyz[1] << ' ' << xyz[2] <<"\n"; // U x y z
//         }
        
//         // Close the file
//         MyFile.close();
//     }
//     catch(int cod_error){cout << "error write to " << file_name << "with error code" << cod_error;}
// }



int main(int argc, char** argv) {
    
    
    int size, id;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

    const int s = (int)sqrt(size);

    const int nx = (NX - 2) / s + 2;
    const int ny = (NY - 2) / s + 2;

    const int N = nx * ny * NZ; 
    const int nx_ny = nx * ny;

    double *cube = new double[N];//текущий куб
    double *next_cube = new double[N]; //следующий куб

    //инициализация нулями cube БЫСТРОЕ
    memset(cube, '0', sizeof(double)*N); 
    memset(next_cube, '0', sizeof(double)*N);

    for (int t = 1; t < NT; ++t){

        double z = HZ;
        for (int k = NY_NX; k < N - nx_ny; k += nx_ny){

            double y = HY * ((ny - 2) * (id / s) + 1); 
            for (int j = nx; j < nx_ny - nx; j += nx){

               double x = HX * ((nx - 2) * (id % s) + 1);
               for (int i = 1; i < nx - 1; i += 1){

                    int n = i + j + k;
                    next_cube[n] = cube[n] + HT * (F(x, y, z) + DX * LX(cube[n - 1], cube[n], cube[n + 1]) + DY * LY(cube[n - nx], cube[n], cube[n + nx]) + DZ * LZ(cube[n - nx_ny], cube[n], cube[n + nx_ny]));
                    x += HX;
                }
                y += HY;
            }
            z += HZ;
        }
        auto p = cube;
        cube = next_cube;
        next_cube = p;

        MPI_Request request;
        //MPI_Status status;

        //север
        if (0 <= (id + s) && (id + s) < size){

            //отправка сообщения
            double* buffer_north = new double[(nx - 2) * (NZ - 2)];

            int index = 0;

            for (int k = NY_NX; k < N - nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx_ny - 2 * nx) + k; //j = nx*ny - 2nx
                    buffer_north[index] = cube[n];
                    ++index;
                }
            }

            MPI_Isend(buffer_north, (nx - 2) * (NZ - 2), MPI_DOUBLE, id + s, TAG, MPI_COMM_WORLD, &request);


            //принятие сообщения
            MPI_Irecv(buffer_north, (nx - 2) * (NZ - 2), MPI_DOUBLE, id + s, TAG, MPI_COMM_WORLD, &request);

            index = 0;

            for (int k = NY_NX; k < N - nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx_ny - nx) + k; //j = nx*ny - 2nx
                    cube[n] = buffer_north[index];
                    ++index;
                }
            }

                
        }
        //юг
        if (0 <= (id - s) && (id - s) < size){

            //отправка сообщения
            double* buffer_south = new double[(nx - 2) * (NZ - 2)];

            int index = 0;

            for (int k = NY_NX; k < N - nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (2 * nx) + k; //j = 2nx
                    buffer_south[index] = cube[n];
                    ++index;
                }
            }

            MPI_Isend(buffer_south, (nx - 2) * (NZ - 2), MPI_DOUBLE, id - s, TAG, MPI_COMM_WORLD, &request);


            //принятие сообщения
            MPI_Irecv(buffer_south, (nx - 2) * (NZ - 2), MPI_DOUBLE, id - s, TAG, MPI_COMM_WORLD, &request);

            index = 0;

            for (int k = NY_NX; k < N - nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx) + k; //j = nx*ny - 2nx
                    cube[n] = buffer_south[index];
                    ++index;
                }
            }
        }
        //запад
        //восток


    }

   

    // write_to_file(cube);

    // int int end_time = clock(); // конечное время
    // int int search_time = end_time - start_time; // искомое время
    // cout << "runtime = " << search_time << " mks" << endl;
    // cout << "runtime = " << search_time/1000 << " ms" << endl;
    // cout << "runtime = " << search_time/1000000 << " s" << endl;
    // cout << "runtime = " << (double)search_time/60000000 << " min" << endl;
    // //cout << HT <<endl;
    MPI_Finalize();
}


