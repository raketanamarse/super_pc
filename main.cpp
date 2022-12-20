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
#include "mpi.h"
#include <ctime> // заголовочный файл с прототипом функции clock()
using namespace std;


const int TAG = 0;

int main(int argc, char** argv) {
    
    int start_time = clock(); // конечное время
    int size, id;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

    const int s = (int)sqrt(size);

    const int nx = (NX - 2) / s + 2;
    const int ny = (NY - 2) / s + 2;

    const int N = nx * ny * NZ; 
    const int nx_ny = nx * ny;

    int id_top = id + s;
    int id_down = id - s;
    int id_ost_s = id % s;
    int N_nx_ny = N - nx_ny; // N - nx_ny

    //инициализация соседей
    const bool top = (0 <= (id_top) && (id_top) < size);
    const bool down = (0 <= (id_down) && (id_down) < size);
    const bool left = (id_ost_s != 0);
    const bool right = (((id + 1) % s) != 0);

    int req_size = 0;
    if (top == true){

        req_size += 2;
    }
    if (down == true){

        req_size += 2;
    }
    if (left == true){

        req_size += 2;
    }
    if (right == true){

        req_size += 2;
    }

    MPI_Request reqs[req_size];
    MPI_Status stats[req_size];


    double *cube = new double[N];//текущий куб
    double *next_cube = new double[N]; //следующий куб

    //инициализация нулями cube БЫСТРОЕ
    memset(cube, '0', sizeof(double)*N); 
    memset(next_cube, '0', sizeof(double)*N);

    for (int t = 1; t < NT; ++t){

        double z = HZ;
        for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

            double y = HY * ((ny - 2) * (id / s) + 1); 
            for (int j = nx; j < nx_ny - nx; j += nx){

               double x = HX * ((nx - 2) * (id_ost_s) + 1);
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

        int req_i = 0;

        double* buffer_north;
        double* buffer_north_r;
        double* buffer_south;
        double* buffer_south_r;
        double* buffer_west;
        double* buffer_west_r;
        double* buffer_east;
        double* buffer_east_r;

        //север-------------------------------------------------------------------------------------------------------------
        if (top == true){

            buffer_north = new double[(nx - 2) * (NZ - 2)];

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx_ny - 2 * nx) + k; //j = nx*ny - 2nx
                    buffer_north[index] = cube[n];
                    ++index;
                }
            }

            MPI_Isend(buffer_north, (nx - 2) * (NZ - 2), MPI_DOUBLE, id_top, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;


            buffer_north_r = new double[(nx - 2) * (NZ - 2)];
            //принятие сообщения
            MPI_Irecv(buffer_north_r, (nx - 2) * (NZ - 2), MPI_DOUBLE, id_top, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;            
        }

        //юг----------------------------------------------------------------------------------------------------------------
        if (down == true){

            //отправка сообщения
            buffer_south = new double[(nx - 2) * (NZ - 2)];

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx) + k; //j = nx
                    buffer_south[index] = cube[n];
                    index++;
                }
            }

            MPI_Isend(buffer_south, (nx - 2) * (NZ - 2), MPI_DOUBLE, id_down, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;

            buffer_south_r = new double[(nx - 2) * (NZ - 2)];

            //принятие сообщения
            MPI_Irecv(buffer_south_r, (nx - 2) * (NZ - 2), MPI_DOUBLE, id_down, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;
        }

        //запад--------------------------------------------------------------------------------------------------------
        if (left == true){

            buffer_west = new double[(ny - 2) * (NZ - 2)];

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int j = nx; j < nx_ny - nx; j += nx){

                    int n = 1 + j + k; //i = 1
                    buffer_west[index] = cube[n];
                    index++;
                }
            }

            MPI_Isend(buffer_west, (ny - 2) * (NZ - 2), MPI_DOUBLE, id - 1, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;

            buffer_west_r = new double[(ny - 2) * (NZ - 2)];

            //принятие сообщения
            MPI_Irecv(buffer_west_r, (ny - 2) * (NZ - 2), MPI_DOUBLE, id - 1, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;
        }

        //восток
        if (right == true){

            buffer_east = new double[(ny - 2) * (NZ - 2)];

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int j = nx; j < nx_ny - nx; j += nx){

                    int n = (nx - 2) + j + k; //i = nx - 2
                    buffer_east[index] = cube[n];
                    index++;
                }
            }

            MPI_Isend(buffer_east, (ny - 2) * (NZ - 2), MPI_DOUBLE, id + 1, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;

            buffer_east_r = new double[(ny - 2) * (NZ - 2)];

            //принятие сообщения
            MPI_Irecv(buffer_east_r, (ny - 2) * (NZ - 2), MPI_DOUBLE, id + 1, TAG, MPI_COMM_WORLD, &reqs[req_i]);
            req_i++;
        }


        
        //█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
        MPI_Waitall(req_size, reqs, stats);

        if (top == true){

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + (nx_ny - nx) + k; //j = nx*ny - nx
                    cube[n] = buffer_north_r[index];
                    index++;
                }
            }
        }

        if (down == true){

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int i = 1; i < nx - 1; i += 1){

                    int n = i + 0 + k; //j = 0
                    cube[n] = buffer_south_r[index];
                    index++;
                }
            }
        }

        if (left == true){

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int j = nx; j < nx_ny - nx; j += nx){

                    int n = j + k; //i = 0
                    cube[n] = buffer_west_r[index];
                    index++;
                }
            }
        }

        if (right == true){

            int index = 0;

            for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

                for (int j = nx; j < nx_ny - nx; j += nx){

                    int n = (nx - 1) + j + k; //i = nx - 1
                    cube[n] = buffer_east_r[index];
                    index++;
                }
            }
        }

    }

    //█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
    int end_time = clock(); // конечное время
    int search_time = end_time - start_time; // искомое время


    if(rank != 0){ // отправка данных по времени

    }
    else{ // принять даные по времени








        try{
            ofstream MyFile("time.txt");
            MyFile << "runtime = " << search_time << " mks" <<"\n";
            MyFile << "runtime = " << search_time/1000 << " ms" <<"\n";
            MyFile << "runtime = " << search_time/1000000 << " s" <<"\n";
            MyFile << "runtime = " << (double)search_time/60000000 << " min" <<"\n";
        
            MyFile.close();
        }
        catch(int cod_error){cout << "error write to " << file_name << "with error code" << cod_error;}
    }

    // cout << "runtime = " << search_time << " mks" << endl;
    // cout << "runtime = " << search_time/1000 << " ms" << endl;
    // cout << "runtime = " << search_time/1000000 << " s" << endl;
    // cout << "runtime = " << (double)search_time/60000000 << " min" << endl;
    // //cout << HT <<endl;



    string file_name =  "./out_result/out_";
    file_name += to_string(id);
    file_name += ".txt";
    cout << file_name;
    try{
        ofstream MyFile(file_name);
        // Write to the file
        MyFile << NX << ' ' << NY << ' ' << NZ <<"\n"; // NX NY NZ

        for (int k = nx_ny; k < N_nx_ny; k += nx_ny){

            for (int j = nx; j < nx_ny - nx; j += nx){

               for (int i = 1; i < nx - 1; i += 1){

                    int n = i + j + k;
                    MyFile << cube[n] << ' ' << (id_ost_s) * (nx - 2) + i << ' ' << (id / s) * (ny - 2) + j / nx << ' ' << k / nx_ny <<"\n"; // U i j k
                    
                }
            }
        }
        // Close the file
        MyFile.close();
    }
    catch(int cod_error){cout << "error write to " << file_name << "with error code" << cod_error;}

    
    MPI_Finalize();
}


