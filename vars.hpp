#pragma once

//Глобальные константы

const unsigned NX = 10, NY = 10, NZ = 10; //Значение количества точек по осям X, Y, Z

const double HX = 1 / double(NX - 1), HY = 1 / double(NY - 1), HZ = 1 / double(NZ - 1); //Значение шага по осям X, Y, Z

const double DX = 0.25, DY = 0.15, DZ = 0.1; //Значение dx, dy, dz в тензоре D

const double PI = 3.14159265359; //число Пи

const unsigned NT = 10; //Значение количества шагов по T

const double HT = 1 / double(NT - 1); //Значение шага по T

const unsigned N = NX * NY * NZ; // общее количество элеметов в массиве (кубе)