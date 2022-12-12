#pragma once

//Глобальные константы

const int NX = 20, NY = 20, NZ = 20; //Значение количества интервалов по осям X, Y, Z

const double HX = 1 / NX, HY = 1 / NY, HZ = 1 / NZ; //Значение шага по осям X, Y, Z

const double DX = 0.25, DY = 0.15, DZ = 0.1; //Значение dx, dy, dz в тензоре D

const double PI = 4 * atan(1); //число Пи

const double DT = 0.1; //Значение шага по t