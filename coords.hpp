#pragma once
#include "vars.hpp"

//функции для рассчета координат x y z при заданном n

const double NY_NX = NY * NX; //


void Count_XYZ(int i, int* arr){
    
    arr[2] = i / NY_NX;
    arr[1] = (i - arr[2] * NY_NX) / NX;
    arr[0] = i - arr[2] * NY_NX - arr[1] * NX; 
    return;
}