//
// Created by fibersell on 16.04.18.
//

#include <iostream>
#include <cstring>
#include <regex>
#include <set>

#ifndef LEXANALYSER_LEXGETTER_H
#define LEXANALYSER_LEXGETTER_H
#endif //LEXANALYSER_LEXGETTER_H

using namespace std;

enum type_of_lex
{
    LEX_NULL,
    LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_GOTO, LEX_BREAK, LEX_NOT, LEX_OR, LEX_MAIN, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WHILE, LEX_WRITE, LEX_STRUCT,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_RBC, LEX_LBC,
    LEX_NUM,LEX_CONST_STRING,LEX_UN_PLUS, LEX_UN_MINUS,
    LEX_ID
};


class Lex {  
    type_of_lex t_lex;
    string v_lex;
public:

    Lex ( type_of_lex t = LEX_NULL, string v = "") {
        t_lex = t; v_lex = v;
    }

    type_of_lex get_type () {
        return t_lex;
    }
    string get_value () {
        return v_lex;
    }

    friend ostream& operator << ( ostream &s, Lex l ) {
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }
};


class Ident
{
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident() {
        declare = false;
        assign = false;
    }


    string get_name () {
        return name;
    }


    void put_name (string n) {
        name = n;
    }


    bool get_declare () {
        return declare;
    }


    void put_declare () {
        declare = true;
    }


    type_of_lex get_type () {
        return type;
    }


    void put_type ( type_of_lex t ) {
        type = t;
    }


    bool get_assign () {
        return assign;
    }


    void put_assign () {
        assign = true;
    }


    int get_value () {
        return value;
    }


    void put_value (int v) {
        value = v;
    }
};



class tabl_ident {
    Ident * p;
    int size;
    int top;
public:

    tabl_ident ( int max_size ) {
        p = new Ident[size=max_size];
        top = 1;
    }


    ~tabl_ident () {
        delete []p;
    }


    Ident& operator[] ( int k ) {
        return p[k];
    }


    int put ( const string buf ) {
        for ( int j=1; j<top; ++j )
            if ( !buf.compare(p[j].get_name()) )
                return j;
        p[top].put_name(buf);
        ++top;
    return top-1;
    }

};


class Scanner {
    enum state { H, IDENT, NUMB, COM, STR, DELIM, NEQ, ERR };
    state CS;
    static regex letter;
    static regex digit;
    string code;
    string c;
    string buf;
    int index;


    void clear () {
        buf = "";
    }


    void add () {
        buf += c;
    }


    int look ( const string buf, string *list ) {
        int i = 0;
        while ( list[i].compare("NULL") ) {
            if ( !buf.compare(list[i]) )
                return i;
            ++i;
        }
        return 0;
    }


    void gc () {
        if(has_next())
            c = code.substr(index++,1);
        else
            c = "";
    }


public:

    static string  TW[];
    static type_of_lex words[];
    static string  TD[];
    static type_of_lex dlms[];

    set<type_of_lex> getDelims(){
        set<type_of_lex> tmp;
        for (int i= 1; i < 21; i++)
            tmp.insert(dlms[i]);
        return tmp;
    }


    set<type_of_lex> getWords(){
        set<type_of_lex> tmp;
        for (int i = 0; i < 19;i++)
            tmp.insert(words[i]);
        return tmp;
    }



    bool has_next(){
        return index < code.length();
    }

    string getCode(){
        return code;
    }


    Lex get_lex ();


    Scanner ( const string  program ) {
        index = 0;
        code = program;
        CS = H;
        clear();
        gc();
    }


};


regex Scanner::letter = regex("[a-zA-Z]");

regex Scanner::digit = regex("[0-9]");

string  Scanner::TW[] = {
        "" ,
        "and",
        "bool",
        "do",
        "else",
        "if",
        "false",
        "int",
        "not",
        "or",
        "main",
        "read",
        "break",
        "goto",
        "true",
        "string",
        "while",
        "write",
        "struct",
        "NULL"
};


type_of_lex Scanner::words[] = {
                LEX_NULL,
                LEX_AND,
                LEX_BOOL,
                LEX_DO,
                LEX_ELSE,
                LEX_IF,
                LEX_FALSE,
                LEX_INT,
                LEX_NOT,
                LEX_OR,
                LEX_MAIN,
                LEX_READ,
                LEX_BREAK,
                LEX_GOTO,
                LEX_TRUE,
                LEX_STRING,
                LEX_WHILE,
                LEX_WRITE,
                LEX_NULL
};


string  Scanner:: TD[] = {
        "" ,
        ";",
        "==",
        ",",
        ":",
        "=",
        "(",
        ")",
        "<",
        ">",
        "+",
        "-",
        "*",
        "/",
        "<=",
        "!=",
        ">=",
        "{",
        "}",
        "++",
        "--",
        "NULL"
};


type_of_lex Scanner::dlms[] = {
                LEX_NULL,
                LEX_SEMICOLON,
                LEX_EQ,
                LEX_COMMA,
                LEX_COLON,
                LEX_ASSIGN,
                LEX_LPAREN,
                LEX_RPAREN,
                LEX_LSS,
                LEX_GTR,
                LEX_PLUS,
                LEX_MINUS,
                LEX_TIMES,
                LEX_SLASH,
                LEX_LEQ,
                LEX_NEQ,
                LEX_GEQ,
                LEX_RBC,
                LEX_LBC,
                LEX_UN_PLUS,
                LEX_UN_MINUS,
                LEX_NULL
};


tabl_ident TID(100);


Lex Scanner::get_lex() {

    CS = H;


    int j;

    clear();

    do {
        switch (CS) {
            case H:
                if(!c.compare("\n") || !c.compare(" ") || !c.compare("\r") || !c.compare("\t")) {
                    gc();
                    break;
                }
                else if(!c.compare("/")){
                    gc();
                    if(!c.compare("*"))
                        CS = COM;
                    else throw "illegal statement!";
                } else if(!c.compare("\"")) {
                    CS = STR;
                } else if(regex_match(c,letter)) {
                    CS = IDENT;
                    add();
                } else if(regex_match(c,digit)) {
                    CS = NUMB;
                    add();
                } else {
                    CS = DELIM;
                    add();
                }
                gc();
                break;


            case COM:
                if(c.compare("*"))
                    gc();
                else {
                    gc();
                    if (!c.compare("/")) {
                        gc();
                        CS = H;
                    }
                }
                break;


            case STR:
                if (c.compare("\"")){
                    add();
                    gc();
                } else {
                    gc();
                    return Lex(LEX_CONST_STRING,buf);
                }
                break;


            case NUMB:
                if(regex_match(c,digit)){
                    add();
                    gc();
                } else
                    return Lex(LEX_NUM, buf);
                break;


            case IDENT:
                if(regex_match(c,letter) || regex_match(c,digit)){
                    add();
                    gc();
                } else if ( j = look(buf,TW)) {
                    return Lex(words[j],to_string(j));
                } else {
                    j = TID.put(buf);
                    return Lex(LEX_ID,to_string(j));
                }
                break;


            case DELIM:
                add();
                if(j = look(buf,TD)) {
                    if (has_next())
                        gc();
                    return Lex(dlms[j],to_string(j));
                } else if (j = look(buf.substr(0,1),TD)){
                    return Lex(dlms[j],to_string(j));
                } else
                    throw 1;
        }

    } while (true);


}