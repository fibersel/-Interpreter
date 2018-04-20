#include <iostream>
#include <fstream>
#include "cmake-build-debug/LexGetter.h"


using namespace std;



int main() {
    string tmp = "", code = "";
    ifstream file("/home/fibersell/Рабочий стол/LexAnalyser/test.txt");

    while (!file.eof()) {
        getline(file,tmp);
        code += tmp + "\n";
    }
    file.close();

    reverse(code.rbegin(),code.rend());
    regex whitespace("[ \n\t\r]");
    for (int i = 0; regex_match(code.substr(0,1),whitespace) ; ++i) {
        code = code.substr(1,code.length() - 1);
    }


    reverse(code.rbegin(),code.rend());

    Scanner scan(code);

    Lex tempest;
    Ident tmp1;
    set<type_of_lex> dlms = scan.getDelims();
    set<type_of_lex> words = scan.getWords();

    try {
        while (scan.has_next()) {
            tempest = scan.get_lex();
            if (tempest.get_type() == LEX_ID){
                tmp1 = TID[atoi(tempest.get_value().c_str())];
                cout << tmp1.get_name() << endl;
            } else if (dlms.count(tempest.get_type()))
                cout << scan.TD[atoi(tempest.get_value().c_str())] << endl;
            else if (words.count(tempest.get_type()))
                cout << scan.TW[atoi(tempest.get_value().c_str())] << endl;
            else
                cout << tempest.get_value() << endl;
        }
    } catch (string c){
        cout << c;
        exit(-1);
    }






    return 0;
}