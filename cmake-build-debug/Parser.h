//
// Created by fibersell on 24.04.18.
//


#ifndef LEXANALYSER_PARSER_H
#define LEXANALYSER_PARSER_H

#endif //LEXANALYSER_PARSER_H



class Parser{
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    type_of_lex def_type;
    int in_cycle;
    int c_val;
    string struct_name;
    Scanner scan;
    stack < int > st_int;
    stack < type_of_lex > st_lex;
    map<string,map<string,type_of_lex>> structs_map;
    map<string,type_of_lex> ids_map;

    void gl(){
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

    void M();
    void C();
    void O();
    void S();
    void E();
    void E1();
    void D();
    void D_STRUCT();
    void D1();
    void T();
    void F();
    void I();


    void dec(type_of_lex type);
    void dec_struct(string name);
    void check_id();
    void check_id_in_read();
    void check_op();
    void check_not();
    void eq_bool();
    bool is_numerable(type_of_lex lex);
    bool is_stringable(type_of_lex lex);

public:
    Parser(const char * program) : scan(program){
        in_cycle = 0;
    }

    void analyse();

};


void Parser::analyse() {
    gl();
    M();


}


void Parser::M() {
    if(c_type == LEX_MAIN){
        gl();
        S();
        gl();
        if(c_type != LEX_END)
            throw curr_lex;
    }
    else
        throw curr_lex;
}





void Parser::S() {
    if(c_type == LEX_LBC){
        gl();
        if(c_type == LEX_RBC)
            return;
        S();
        while (true){
            gl();
            if(c_type == LEX_RBC){
                return;
            }
            S();
        }
    }



    else if(c_type == LEX_IF){
        gl();
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        gl();
        if (c_type == LEX_ELSE){
            gl();
            S();
        }
    }



    else if(c_type == LEX_FOR){
        gl();
        in_cycle++;
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
        E();
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        in_cycle--;
    }





    else if(c_type == LEX_WHILE){
        gl();
        in_cycle++;
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        in_cycle--;
    }



    else if(c_type == LEX_BREAK){
        if (!in_cycle)
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
    }



    else if (c_type == LEX_GOTO){
        gl();
        if(c_type != LEX_ID || structs_map.count(TID[c_val].get_name()))
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
    }



    else if (c_type == LEX_READ){
        gl();
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        if(c_type != LEX_ID || structs_map.count(TID[c_val].get_name()))
            throw curr_lex;
        check_id_in_read();
        gl();
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
    }



    else if(c_type == LEX_WRITE){
        gl();
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        while (c_type == LEX_COMMA){
            gl();
            E();
        }
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
    }


    /*else if(c_type == LEX_ID){
        gl();
        if(c_type != LEX_COLON)
            throw curr_lex;
        gl();
        S();
    }*/
    else if (c_type == LEX_STRUCT){
        gl();
        if (c_type != LEX_ID || structs_map.count(TID[c_val].get_name()))
            throw curr_lex;
        string struct_name = TID[c_val].get_name();
        gl();
        if (c_type != LEX_LBC)
            throw curr_lex;
        ids_map.clear();
        while (true){
            gl();
            if(c_type == LEX_RBC)
                break;
            D_STRUCT();
        }
        structs_map[struct_name] = ids_map;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
    }



    else if (c_type == LEX_STRING || c_type == LEX_INT || c_type == LEX_BOOL || (c_type == LEX_ID && structs_map.count(TID[c_val].get_name()))){
        def_type = c_type;
        string name = NULL;
        if(c_type == LEX_ID)
            name = TID[c_val].get_name();
        gl();
        D();
        while (c_type == LEX_COMMA){
            gl();
            D();
        }
        if(name != NULL)
            dec_struct(name);
        else
            dec(def_type);
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
    }



    else if(c_type == LEX_ID && structs_map.count(TID[c_val].get_name())){
        gl();
    }



    else E();

}


void Parser::D_STRUCT() {
    if(c_type != LEX_STRING && c_type != LEX_INT && c_type != LEX_BOOL)
        throw curr_lex;
    def_type = c_type;
    gl();
    string name = TID[c_val].get_name();
    D1();
    ids_map[name] = def_type;
    while (c_type == LEX_COMMA){
        gl();
        name = TID[c_val].get_name();
        D1();
        ids_map[name] = def_type;
    }


}



void Parser::D1(){
    if (c_type != LEX_ID || ids_map.count(TID[c_val].get_name()))
        throw curr_lex;
    gl();
    if(c_type == LEX_ASSIGN){
        gl();
        switch (c_type){
            case LEX_CONST_STRING:
                if(def_type == LEX_STRING)
                    gl();
                else
                    throw curr_lex;
                break;
            case LEX_NUM:
                if(def_type == LEX_INT)
                    gl();
                else throw curr_lex;
                break;
            case LEX_TRUE:
            case LEX_FALSE:
                if (def_type == LEX_BOOL)
                    gl();
                else
                    throw curr_lex;
                break;
            default:
                throw curr_lex;
        }
    }
}




void Parser::D(){
    if (c_type != LEX_ID || structs_map.count(TID[c_val].get_name()))
        throw curr_lex;
    if (c_type != LEX_ID)
        st_int.push(c_val);
    gl();
    if(c_type == LEX_ASSIGN){
        gl();
        switch (c_type){
            case LEX_CONST_STRING:
                if(def_type == LEX_STRING)
                    gl();
                else
                    throw curr_lex;
                break;
            case LEX_NUM:
                if(def_type == LEX_INT)
                    gl();
                else throw curr_lex;
                break;
            case LEX_TRUE:
            case LEX_FALSE:
                if (def_type == LEX_BOOL)
                    gl();
                else
                    throw curr_lex;
                break;
            default:
                throw curr_lex;
        }
    }
}


void Parser::E() {
    E1();
    if (c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_GEQ || c_type == LEX_EQ || c_type == LEX_LEQ){
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}


void Parser::E1() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR){
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }

}


void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_AND || c_type == LEX_SLASH){
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}


void Parser::F() {
    if (c_type == LEX_TRUE || c_type == LEX_FALSE || c_type == LEX_NUM || c_type == LEX_CONST_STRING){
        if(c_type == LEX_TRUE || c_type == LEX_FALSE)
            st_lex.push(LEX_BOOL);
        else st_lex.push(c_type);
        gl();
    } else if(c_type == LEX_NOT){
        gl();
        F();
        check_not();
    } else if (c_type == LEX_LPAREN){
        gl();
        E();
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
    } else if(c_type == LEX_ID){
        if (structs_map.count(TID[c_val].get_name()))
            throw curr_lex;
        check_id();
        type_of_lex prev_type = TID[c_val].get_type();
        gl();
        if(c_type == LEX_ASSIGN){
            st_lex.push(c_type);
            gl();
            E();
            check_op();
        } else if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS){
            if((prev_type != LEX_INT) && (prev_type != LEX_NUM))
                throw curr_lex;
            gl();
        }
    } else {
        I();
    }
}


void Parser::I() {
    if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS){
        gl();
        if(c_type != LEX_ID || structs_map.count(TID[c_val].get_name()))
            throw curr_lex;
        if((TID[c_val].get_type() != LEX_INT) && (TID[c_val].get_type() != LEX_NUM))
            throw curr_lex;
        check_id();
        gl();
        if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS)
            throw curr_lex;
    } else throw curr_lex;
}


void Parser::dec(type_of_lex type) {
    int i;
    while (!st_int.empty()){
        i = st_int.top();
        st_int.pop();
        if (TID[i].get_declare())
            throw "declared twice!";
        else{
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}


void Parser::dec_struct(string name){

}



void Parser::check_id()
{
    if ( TID[c_val].get_declare() )
        st_lex.push(TID[c_val].get_type());
    else
        throw "not declared";
}



void Parser::check_id_in_read(){
    if(!TID[c_val].get_declare())
        throw "not declared!";
}



void Parser::eq_bool() {
    if(st_lex.top() != LEX_BOOL)
        throw "the expression is not boolean!";
    st_lex.pop();
}


void Parser::check_op ()
{
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
    t2 = st_lex.top();
    st_lex.pop();
    op = st_lex.top();
    st_lex.pop();
    t1 = st_lex.top();
    st_lex.pop();
    if(is_numerable(t1) && is_numerable(t2)){
        if(op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH
           || op == LEX_ASSIGN)
            r = LEX_INT;
        else if(op == LEX_LEQ || op == LEX_LSS || op == LEX_GEQ || op == LEX_GTR || op == LEX_NEQ || op == LEX_EQ)
            r = LEX_BOOL;
        else
            throw "wrong types are in operation";
    } else if(t1 == LEX_BOOL && t2 == LEX_BOOL){
        if(op == LEX_OR || op == LEX_AND || op == LEX_ASSIGN)
            r = LEX_BOOL;
        else
            throw "wrong types are in operation";
    } else if(is_stringable(t1) && is_stringable(t2)){
        if(op == LEX_PLUS || op == LEX_ASSIGN)
            r = LEX_STRING;
        else if(op == LEX_LSS || op == LEX_GTR || op == LEX_EQ || op == LEX_NEQ)
            r = LEX_BOOL;
        else
            throw "wrong types are in operation";
    } else throw "wrong types are in operation";
    cout << "result:  "<< Scanner::TW[r] << endl;
    st_lex.push(r);
}


void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else
    {
        st_lex.pop();
        st_lex.push (LEX_BOOL);

    }
}


bool Parser::is_numerable(type_of_lex lex){
    return lex == LEX_NUM || lex == LEX_INT;
}

bool Parser::is_stringable(type_of_lex lex){
    return lex == LEX_STRING || lex == LEX_CONST_STRING;
}