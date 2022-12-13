#pragma once

//Глобальные константы

const unsigned NX = 21, NY = 21, NZ = 21; //Значение количества точек по осям X, Y, Z

const double HX = 1 / NX, HY = 1 / NY, HZ = 1 / NZ; //Значение шага по осям X, Y, Z

const double DX = 0.25, DY = 0.15, DZ = 0.1; //Значение dx, dy, dz в тензоре D

const double PI = 3.14159265359; //число Пи

const double HT = 0.1; //Значение шага по t

const unsigned N = NX * NY * NZ; // общее количество элеметов в массиве (кубе)