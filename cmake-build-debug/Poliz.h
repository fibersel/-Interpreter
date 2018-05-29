//
// Created by Аби on 03.05.2018.
//

#include "LexGetter.h"
#include <list>

#ifndef LEXANALYSER_POLIZ_H
#define LEXANALYSER_POLIZ_H

#endif //LEXANALYSER_POLIZ_H

class Poliz
{
    vector<Lex> p;
    int free;
public:


    Poliz () {
        p = vector<Lex>(1000);
        free = 0;
    };



    void put_lex(Lex l) {
        p[free]=l;
        ++free;
    };


    void put_lex(Lex l, int place) {
        p[place]=l;
    };


    void blank() {
        ++free;
    };


    int get_free() {
        return free;
    };


    Lex& operator[] ( int index ) {
        if ( index > free )
            throw "POLIZ:indefinite element of array";
        else
            return p[index];
    };


    void print() {
        for ( int i = 0; i < free; ++i )
            cout << p[i] << endl;
    };

};