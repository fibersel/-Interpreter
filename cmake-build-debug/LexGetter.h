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
    LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FOR,LEX_FALSE, LEX_INT, LEX_GOTO, LEX_BREAK, LEX_NOT, LEX_OR, LEX_MAIN, LEX_READ, LEX_TRUE, LEX_STRING, LEX_WHILE, LEX_WRITE, LEX_STRUCT,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_RBC, LEX_LBC,
    LEX_NUM,LEX_CONST_STRING,LEX_UN_PLUS, LEX_UN_MINUS,
    LEX_ID,LEX_END
};


class Lex {
    type_of_lex t_lex;
    int v_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0) {
        t_lex = t; v_lex = v;
    }

    type_of_lex get_type () { return t_lex; }
    int get_value () { return v_lex; }
    friend ostream& operator << ( ostream &s, Lex l ) {
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }
};

class Ident
{
    char * name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident () {
        declare = false;
        assign = false;
    }


    char *get_name () {
        return name;
    }


    void put_name (const char *n) {
        name = new char [ strlen(n) + 1 ];
        strcpy ( name, n );
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

    int put ( const char *buf );
};


int tabl_ident::put ( const char *buf ) {
    for ( int j=1; j<top; ++j )
        if ( !strcmp(buf, p[j].get_name()) )
            return j;
    p[top].put_name(buf);
    ++top;
    return top-1;
}


class Scanner {
    enum state { H, IDENT, NUMB, COM, STR, DELIM, NEQ, ERR };
    state CS;
    static regex letter;
    static regex digit;
    FILE *fp;
    char c;
    string buf;


    void clear () {
        buf = "";
    }


    void add () {
        buf += c;
    }


    int look ( const char * buf, char ** list ) {
        int i = 0;
        while ( list[i] ) {
            if ( !strcmp(buf,list[i]))
                return i;
            ++i;
        }
        return 0;
    }


    void gc () {
        c = fgetc(fp);
    }


public:

    static char *  TW[];
    static type_of_lex words[];
    static char *  TD[];
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




    Lex get_lex ();


    Scanner ( const char * program ) {
        fp = fopen(program,"r");
        CS = H;
        clear();
        gc();
    }


};

char *  Scanner::TW[] = {
        "" ,
        "and",
        "bool",
        "do",
        "else",
        "if",
        "for",
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
        NULL
};


type_of_lex Scanner::words[] = {
                LEX_NULL,
                LEX_AND,
                LEX_BOOL,
                LEX_DO,
                LEX_ELSE,
                LEX_IF,
                LEX_FOR,
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
                LEX_STRUCT,
                LEX_NULL
};


char *  Scanner:: TD[] = {
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
        NULL
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
                LEX_LBC,
                LEX_RBC,
                LEX_UN_PLUS,
                LEX_UN_MINUS,
                LEX_NULL
};


tabl_ident TID(100);
vector<string> TCS;
vector<string> TCN;

Lex Scanner::get_lex() {

    CS = H;


    int d, j;

    clear();

    do {
        switch (CS) {
            case H:
                if(c == '\n' || c == ' ' || c == '\r' || c == '\t') {
                    gc();
                    break;
                }
                else if(c == '/'){
                    gc();
                    if(c == '*')
                        CS = COM;
                    else return Lex(LEX_SLASH,13);
                } else if(c == '\"')
                    CS = STR;
                else if(isalpha(c)) {
                    CS = IDENT;
                    add();
                } else if(isdigit(c)) {
                    CS = NUMB;
                    add();
                } else if(c == EOF){
                    return Lex(LEX_END);
                } else {
                    CS = DELIM;
                    add();
                }
                gc();
                break;


            case COM:
                if(c == EOF)
                    return Lex(LEX_END);
                else if (c == '*'){
                    gc();
                    if (c == '/') {
                        gc();
                        CS = H;
                    } else if(c == EOF)
                        return Lex(LEX_END);
                } else if(c == EOF)
                    return Lex(LEX_END);
                else gc();
                break;


            case STR:
                if (c == EOF)
                    throw "incomplete string";
                else if (c == '\"'){
                    gc();
                    TCS.push_back(buf);
                    j = TCS.size() - 1;
                    return Lex(LEX_CONST_STRING,j);
                }
                else {
                    add();
                    gc();
                }
                break;


            case NUMB:
                if(isdigit(c)){
                    add();
                    gc();
                } else {
                    TCN.push_back(buf);
                    j = TCN.size() - 1;
                    return Lex(LEX_NUM, j);
                }
                break;


            case IDENT:
                if(isalpha(c) || isdigit(c)){
                    add();
                    gc();
                } else if ( j = look(buf.c_str(),TW)) {
                    return Lex(words[j],j);
                } else {
                    j = TID.put(buf.c_str());
                    return Lex(LEX_ID,j);
                }
                break;


            case DELIM:
                add();
                if(j = look(buf.c_str(),TD)) {
                    gc();
                    return Lex(dlms[j],j);
                } else if (j = look(buf.substr(0,1).c_str(),TD)){
                    return Lex(dlms[j],j);
                } else
                    throw "sequence could not be parsed";
        }

    } while (true);


}