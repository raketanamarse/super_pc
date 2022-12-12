#include <iostream>
#include <cmath>


const int NX = 20, NY = 20, NZ = 20; //Значение количества интервалов по осям X, Y, Z

const double HX = 1 / NX, HY = 1 / NY, HZ = 1 / NZ; //Значение шага по осям X, Y, Z



const double DX = 0.25, DY = 0.15, DZ = 0.1; //Значение dx, dy, dz в тензоре D

const double PI = 4 * atan(1); //число Пи

const double DT = 0.1; //Значение шага по t



//Функции рассчета вторых производных 

const double HX_2 = HX * HX, HY_2 = HY * HY, HZ_2 = HZ * HZ; //Значение квадрата шага по осям X, Y, Z

double LX(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HX_2; // ??? можно реализовать умножение на 2 через битовую операци, будет быстре
}

double LY(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HY_2; // ??? можно реализовать умножение на 2 через битовую операци, будет быстре
}

double LZ(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HZ_2; // ??? можно реализовать умножение на 2 через битовую операци, будет быстре
}



//Функция f

const double DSum = DX + DY + DZ; //dx + dy + dz

const double DSum_PI_PI = DSum * PI * PI;

double F(double x, double y, double z){

    return DSum_PI_PI * sin(PI * x) * sin(PI * y) * sin(PI * z);
}



int main(int, char**) {
    std::cout << "Hello, world!\n";
}
