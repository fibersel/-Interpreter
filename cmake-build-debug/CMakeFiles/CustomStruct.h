//
// Created by fibersell on 29.04.18.
//

#include "../LexGetter.h"

#ifndef LEXANALYSER_CUSTOMSTRUCT_H
#define LEXANALYSER_CUSTOMSTRUCT_H

#endif //LEXANALYSER_CUSTOMSTRUCT_H





class CustomStruct{

    map<string,bool> boolVars;
    map<string,int> intVars;
    map<string,string> stringVars;
    string name;

public:
    CustomStruct(map<string,type_of_lex > StructMap,string name){
        this->name = name;
        for(map<string,type_of_lex >::iterator itr = StructMap.begin();itr != StructMap.end();itr++){
            switch (itr->second){
                case LEX_STRING:
                    stringVars[itr->first] = NULL;
                    break;
                case LEX_INT:
                    intVars[itr->first] = NULL;
                    break;
                case LEX_BOOL:
                    boolVars[itr->first] = NULL;
                    break;
                default:
                    throw StructMap;
            }

        }
    }
};