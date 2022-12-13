#pragma once
#include "vars.hpp"

//Функции рассчета вторых производных 

const double HX_2 = HX * HX, HY_2 = HY * HY, HZ_2 = HZ * HZ; //Значение квадрата шага по осям X, Y, Z

double LX(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HX_2;
}   

double LY(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HY_2;
}

double LZ(double left_value, double value, double right_value){

    return (left_value - 2 * value + right_value) / HZ_2;
}