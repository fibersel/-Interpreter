#include <iostream>
#include <fstream>
#include "cmake-build-debug/LexGetter.h"
#include "cmake-build-debug/Parser.h"


using namespace std;



int main() {
    Scanner scan("/home/fibersell/Рабочий стол/LexAnalyser/test.txt");

    Lex tempest;
    set<type_of_lex> dlms = scan.getDelims();
    set<type_of_lex> words = scan.getWords();


    /*
    try {
        do{
            tempest = scan.get_lex();
            if (words.count(tempest.get_type())){
                cout << scan.TW[tempest.get_value()] << endl;
            } else if (dlms.count(tempest.get_type())){
                cout << scan.TD[tempest.get_value()] << endl;
            } else if (tempest.get_type() == LEX_ID){
                cout << TID[tempest.get_value()].get_name() << endl;
            } else if (tempest.get_type() == LEX_CONST_STRING){
                cout << TCS[tempest.get_value()] << endl;
            } else if (tempest.get_type() == LEX_NUM) {
                cout << TCN[tempest.get_value()] << endl;
            }
        } while(tempest.get_type() != LEX_END);
    } catch (const char * c){
        cout << c << endl;
         cout << "terminating...";
        exit(-1);
    }*/


    Parser parser = Parser("/home/fibersell/Рабочий стол/LexAnalyser/test.txt");

    parser.analyse();


    return 0;
}