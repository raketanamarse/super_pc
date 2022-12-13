#include <iostream>
#include <cmath>
#include "L.hpp"
#include "F.hpp"
#include "vars.hpp"

#include <ctime> // заголовочный файл с прототипом функции clock()

using namespace std;


int main(int, char**) {
    cout << "Hello, world!\n";
    unsigned int start_time =  clock(); // начальное время




    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    cout << "work time programma\n";
     cout << search_time;
}
