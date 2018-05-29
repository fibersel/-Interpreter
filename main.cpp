#include <iostream>
#include <fstream>
#include "cmake-build-debug/Parser.h"
#include "cmake-build-debug/Executer.h"



using namespace std;



int main() {
    char * test = "/home/fibersell/Рабочий стол/LexAnalyser/test3.txt";
    try {
        Interpretator iptr(test);
        iptr.interpretation();
    } catch (char const * s){
        cout << s;
    }
    return 0;
}