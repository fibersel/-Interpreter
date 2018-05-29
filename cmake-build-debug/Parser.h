//
// Created by fibersell on 24.04.18.
//
#include <map>
#include <stack>
#include "Poliz.h"

#ifndef LEXANALYSER_PARSER_H
#define LEXANALYSER_PARSER_H

#endif //LEXANALYSER_PARSER_H

using namespace std;


class CustomStruct{

    map<string,int> fields_map;
    int id;

public:
    CustomStruct(map<string,type_of_lex> ids_map,string name,int struct_id ){
        int i;
        id = struct_id;
        for(map<string,type_of_lex >::iterator itr = ids_map.begin();itr != ids_map.end();itr++){
            i = TID.put(string(name + '.' + itr->first).c_str());
            TID[i].put_declare();
            TID[i].put_type(itr->second);
            fields_map[itr->first] = i;
        }
    }


    Ident get_ident_by_name(string name){
        return TID[fields_map[name]];
    }


    int get_id(string field_name){
        return fields_map[field_name];
    }


    int get_id(){
        return id;
    }


    void assign_struct_to_struct(CustomStruct & c1){
        int id1,id2;
        for(auto &item : fields_map){
            id1 = fields_map[item.first];
            id2 = c1.fields_map[item.first];
            if(!TID[id2].get_assign())
                throw "indefinite identifier";
            TID[id1].put_value(TID[id2].get_value());
            TID[id1].put_assign();
        }

    }

};

//название переменной структуры----объект структуры
map<string,CustomStruct> map_id_to_struct_instance;

class Parser{
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    type_of_lex def_type;
    bool marked_op;
    bool assignable = true;
    int in_cycle;
    int c_val;
    string struct_name;
    string id_type;
    Scanner scan;

    stack<int> break_labels;
    stack<int> number_of_labels;

    stack < int > st_int;
    stack < string > st_string;
    stack < type_of_lex > st_lex;

    //стэк для пар<индекс в ПОЛИЗ----название метки>
    stack <pair<int,string>> gotos_stack;

    //тип структуры---набор переменных
    map<string,map<string,type_of_lex>> structs_map;
    //переменная----тип
    map<string,type_of_lex> ids_map;
    //название переменной структуры----тип структуры
    map<string,string> map_id_to_stuct_type;
    //мапа для операций над строками
    map<type_of_lex,type_of_lex> ops_map;
    //мапа для goto-меток
    map<string,int> gotos_map;

    CustomStruct get_instance_by_name(string name){
        return map_id_to_struct_instance.find(name)->second;
    }

    void gl(){
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

    void M();
    void S();
    void E();
    void E1();
    void D();
    void D_STRUCT();
    void T();
    void F();
    void I();


    void check_id();
    void check_id(Ident ident);
    void check_op();
    void check_not();
    void push_break_labels(int pos);
    void eq_bool();
    bool is_numerable(type_of_lex lex);
    bool is_stringable(type_of_lex lex);
    void fill_gotos_table();

public:
    Poliz prog;
    Parser(const char * program) : scan(program){
        in_cycle = 0;
        ops_map[LEX_EQ] = LEX_EQ_STRING;
        ops_map[LEX_NEQ] = LEX_NEQ_STRING;
        ops_map[LEX_LSS] = LEX_LSS_STRING;
        ops_map[LEX_LEQ] = LEX_LEQ_STRING;
        ops_map[LEX_GEQ] = LEX_GEQ_STRING;
        ops_map[LEX_GTR] = LEX_GTR_STRING;
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
        if(c_type != LEX_END)
            throw curr_lex;
    }
    else
        throw curr_lex;
    fill_gotos_table();
}





void Parser::S() {

    int pl0, pl1, pl2, pl3;

    if(c_type == LEX_LBC){
        gl();
        if(c_type == LEX_RBC){
            gl();
            return;
        }
        S();
        while (true){
            if(c_type == LEX_RBC){
                gl();
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
        //
        pl2 = prog.get_free ();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));
        //
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();
        //
        pl3 = prog.get_free();
        prog.blank();
        prog.put_lex (Lex(POLIZ_GO));
        prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()),pl2);
        //
        if (c_type == LEX_ELSE){
            gl();
            S();
        }
        //
        prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl3);
        //
    }



    else if(c_type == LEX_WHILE){
        gl();
        in_cycle++;
        number_of_labels.push(0);
        if(c_type != LEX_LPAREN)
            throw curr_lex;

        //
        pl2 = prog.get_free ();
        //


        gl();
        E();
        eq_bool();

        //
        pl3 = prog.get_free ();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));
        //

        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        S();

        prog.put_lex(Lex(POLIZ_LABEL,pl2));
        prog.put_lex(Lex(POLIZ_GO));
        int place = prog.get_free();
        prog.put_lex(Lex(POLIZ_LABEL,place),pl3);
        push_break_labels(place);
        in_cycle--;
    }



    else if(c_type == LEX_FOR){
        gl();
        in_cycle++;
        number_of_labels.push(0);
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        E();
        prog.put_lex(Lex(POLIZ_STACK_CLEAR));
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;

        pl0 = prog.get_free();

        gl();
        E();
        eq_bool();

        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));

        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));

        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        pl3 = prog.get_free();
        gl();
        E();

        prog.put_lex(Lex(POLIZ_STACK_CLEAR));
        prog.put_lex(Lex(POLIZ_LABEL,pl0));
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl2);

        if(c_type != LEX_RPAREN)
            throw curr_lex;

        gl();
        S();

        prog.put_lex(Lex(POLIZ_LABEL,pl3));
        prog.put_lex(Lex(POLIZ_GO));
        int place = prog.get_free();
        prog.put_lex(Lex(POLIZ_LABEL,place),pl1);
        push_break_labels(place);
        in_cycle--;
    }




    else if(c_type == LEX_BREAK){
        if (!in_cycle)
            throw curr_lex;
        number_of_labels.top()++;
        int place = prog.get_free();
        break_labels.push(place);
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }




    else if (c_type == LEX_READ){
        gl();
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        gl();
        I();
        if(!assignable)
            throw curr_lex;
        prog.put_lex(Lex(LEX_READ));
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }



    else if(c_type == LEX_WRITE){
        gl();
        if(c_type != LEX_LPAREN)
            throw curr_lex;
        int ctr = 1;
        gl();
        E();
        while (c_type == LEX_COMMA){
            gl();
            E();
            ++ctr;
        }
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
        prog.put_lex(Lex(LEX_NUM,ctr));
        prog.put_lex(Lex(LEX_WRITE));
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }



    else if(c_type == LEX_ID && !TID[c_val].get_declare()){
        TID[c_val].put_declare();
        int val = c_val;
        gotos_map[TID[c_val].get_name()] = prog.get_free();
        gl();
        if(c_type != LEX_COLON)
            throw "bad syntax";
        TID[val].put_goto_label();
        gl();
        S();
    }

    else if(c_type == LEX_ID && TID[c_val].get_goto_label()){
        throw "double goto label declare";
    }


    else if(c_type == LEX_GOTO){
        gl();
        if(c_type != LEX_ID)
            throw "bad syntax";
        pl0 = prog.get_free();
        gotos_stack.push(pair<int,string>(pl0,TID[c_val].get_name()));
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        gl();
        if(c_type != LEX_SEMICOLON)
            throw "bad syntax";
        gl();
    }


    else if (c_type == LEX_STRUCT){
        gl();
        if (c_type != LEX_ID || structs_map.count(TID[c_val].get_name()) || TID[c_val].get_declare() || ids_map.count(TID[c_val].get_name()))
            throw curr_lex;
        else
            TID[c_val].put_declare();
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
        gl();
    }




    else if (c_type == LEX_STRING || c_type == LEX_INT || c_type == LEX_BOOL ){
        def_type = c_type;
        do{
            gl();
            D();
        } while (c_type == LEX_COMMA);
        if (c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }



    else if(c_type == LEX_ID && structs_map.count(TID[c_val].get_name())){
        struct_name = TID[c_val].get_name();
        def_type = LEX_STRUCT;
        do{
            gl();
            if(c_type != LEX_ID || TID[c_val].get_declare())
                throw curr_lex;
            map_id_to_stuct_type.insert(pair<string,string>(TID[c_val].get_name(),struct_name));
            map_id_to_struct_instance.insert(pair<string,CustomStruct>(TID[c_val].get_name(),CustomStruct(structs_map[struct_name],TID[c_val].get_name(),c_val)));
            D();
        } while(c_type == LEX_COMMA);
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }



    else{
        E();
        prog.put_lex(Lex(POLIZ_STACK_CLEAR));
        gl();
    }

}


void Parser::D_STRUCT() {
    if(c_type != LEX_STRING && c_type != LEX_INT && c_type != LEX_BOOL)
        throw curr_lex;
    def_type = c_type;
    string name;
    do {
        gl();
        name = TID[c_val].get_name();
        if(ids_map.count(name))
            throw curr_lex;
        ids_map[name] = def_type;
        gl();
    } while  (c_type == LEX_COMMA);
}




void Parser::D(){
    Lex prev = curr_lex;
    if (c_type != LEX_ID)
        throw curr_lex;
    if (TID[c_val].get_declare())
        throw "declared twice!";
    else{
        TID[c_val].put_declare();
        TID[c_val].put_type(def_type);
    }
    gl();
    if(c_type == LEX_ASSIGN){
        st_lex.push(def_type);
        if(def_type == LEX_STRUCT)
            st_string.push(struct_name);
        prog.put_lex(prev);
        st_lex.push(c_type);
        gl();
        E();
        check_op();
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
    if (c_type == LEX_TRUE)
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE,1));
        gl();
    } else if(c_type == LEX_FALSE )
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE,0));
        gl();
    } else if(c_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    } else if(c_type == LEX_CONST_STRING)
    {
        st_lex.push(LEX_STRING);
        prog.put_lex(curr_lex);
        gl();
    } else if(c_type == LEX_MINUS){
        st_lex.push(LEX_INT);
        prog.put_lex(Lex(LEX_NUM,0));
        st_lex.push(LEX_MINUS);
        gl();
        E();
        check_op();
    }
    else if(c_type == LEX_NOT)
    {
        gl();
        F();
        check_not();
    } else if (c_type == LEX_LPAREN)
    {
        gl();
        E();
        if(c_type != LEX_RPAREN)
            throw curr_lex;
        gl();
    } else if(c_type == LEX_ID || LEX_UN_MINUS || LEX_UN_PLUS) {
        I();
        if(c_type == LEX_ASSIGN){
            if(!assignable)
                throw curr_lex;
            st_lex.push(c_type);
            gl();
            E();
            check_op();
        }
    } else {
        I();
    }
}


void Parser::I() {
    string name,field_name;
    assignable = true;
    if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS){
        assignable = false;
        type_of_lex un_type = c_type;
        gl();
        if(c_type != LEX_ID || structs_map.count(TID[c_val].get_name()) || !TID[c_val].get_declare())
            throw curr_lex;
        //поле структуры
        if(map_id_to_stuct_type.count(TID[c_val].get_name())){
            name = TID[c_val].get_name();
            gl();
            if(c_type != LEX_POINT)
                throw curr_lex;
            gl();
            if(c_type != LEX_ID)
                throw curr_lex;
            field_name = TID[c_val].get_name();
            if(structs_map[map_id_to_stuct_type[name]][field_name] != LEX_INT)
                throw curr_lex;
            CustomStruct t = map_id_to_struct_instance.find(name)->second;
            check_id(t.get_ident_by_name(field_name));


            //
            prog.put_lex(Lex(LEX_ID,map_id_to_struct_instance.find(name + '.' + field_name)->second.get_id()));
            //


        //просто идентификатор
        } else{
            if(TID[c_val].get_type() != LEX_INT || !TID[c_val].get_declare() )
                throw curr_lex;
            check_id();


            //
            prog.put_lex(Lex(LEX_ID,c_val));
            //


        }


        //
        if(un_type == LEX_UN_PLUS)
            prog.put_lex(Lex(LEX_PRE_UN_PLUS));
        else if(un_type == LEX_UN_MINUS)
            prog.put_lex(Lex(LEX_PRE_UN_MINUS));
        //


        gl();
        if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS)
            throw curr_lex;




    } else if(c_type == LEX_ID && !structs_map.count(TID[c_val].get_name()) && TID[c_val].get_declare()){
        name = TID[c_val].get_name();
        int prev_v = c_val;
        type_of_lex prev_t = c_type;
        gl();
        //поле структуры
        if(c_type == LEX_POINT){
            gl();
            if(c_type != LEX_ID)
                throw curr_lex;
            field_name = TID[c_val].get_name();
            if(!structs_map[map_id_to_stuct_type[name]].count(field_name))
                throw curr_lex;
            CustomStruct t = map_id_to_struct_instance.find(name)->second;
            check_id(t.get_ident_by_name(field_name));


            //
            prog.put_lex(Lex(LEX_ID,map_id_to_struct_instance.find(name)->second.get_id(field_name)));
            //


            gl();
            if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS){
                if(structs_map[map_id_to_stuct_type[name]][field_name] != LEX_INT)
                    throw curr_lex;


                //
                if(c_type == LEX_UN_PLUS)
                    prog.put_lex(Lex(LEX_POST_UN_PLUS));
                else
                    prog.put_lex(Lex(LEX_POST_UN_MINUS));
                //


                gl();
                assignable = false;
            }
        } else{
            //инстанс структуры
            if(map_id_to_struct_instance.count(name)){
                st_lex.push(LEX_STRUCT);
                st_string.push(map_id_to_stuct_type[name]);


                //
                prog.put_lex(Lex(LEX_ID,map_id_to_struct_instance.find(name)->second.get_id()));
                //


            }
            //просто идентификатор
            else {

                check_id(TID[prev_v]);


                //
                prog.put_lex (Lex(LEX_ID, prev_v));
                //


                if(c_type == LEX_UN_PLUS || c_type == LEX_UN_MINUS){
                    if(TID[prev_v].get_type() != LEX_INT || !TID[prev_v].get_declare())
                        throw curr_lex;


                    //
                    if(c_type == LEX_UN_PLUS)
                        prog.put_lex(Lex(LEX_POST_UN_PLUS));
                    else
                        prog.put_lex(Lex(LEX_POST_UN_MINUS));
                    //


                    gl();
                    assignable = false;
                }
            }

        }
    }
}



void Parser::check_id(Ident ident){
    if(ident.get_declare())
        st_lex.push(ident.get_type());
    else
        throw "not declared";
}



void Parser::check_id()
{
    if ( TID[c_val].get_declare() ){
        st_lex.push(TID[c_val].get_type());
        if(TID[c_val].get_type() == LEX_STRUCT)
            st_string.push(map_id_to_stuct_type[TID[c_val].get_name()]);
    }
    else
        throw "not declared";
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
        if(op == LEX_PLUS || op == LEX_ASSIGN){
            r = LEX_STRING;
            if(op == LEX_PLUS)
                op = LEX_CONCAT;
        }
        else if(op == LEX_LEQ || op == LEX_LSS || op == LEX_GEQ || op == LEX_GTR || op == LEX_NEQ || op == LEX_EQ){
            op = ops_map[op];
            r = LEX_BOOL;
        }
        else
            throw "wrong types are in operation";
    } else if(t1 == LEX_STRUCT && t2 == LEX_STRUCT){
        if(op == LEX_ASSIGN){
            string s1 = st_string.top();
            st_string.pop();
            string s2 = st_string.top();
            st_string.pop();
            if(s1 == s2)
                st_string.push(s1);
            else
                throw "trying to assign struct of different types";
            r = LEX_STRUCT;
        } else throw "Unsupported operation";
    }
    else throw "wrong types are in operation";
    st_lex.push(r);
    prog.put_lex (Lex(op));
}


void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    prog.put_lex (Lex (LEX_NOT) );
}


bool Parser::is_numerable(type_of_lex lex){
    return lex == LEX_NUM || lex == LEX_INT;
}

bool Parser::is_stringable(type_of_lex lex){
    return lex == LEX_STRING || lex == LEX_CONST_STRING;
}


void Parser::push_break_labels(int label_pos){
    int ctr = number_of_labels.top(),pos;
    number_of_labels.pop();
    while (ctr > 0){
        pos = break_labels.top();
        prog.put_lex(Lex(POLIZ_LABEL,label_pos),pos);
        break_labels.pop();
        ctr--;
    }
};



void Parser::fill_gotos_table(){
    while (!gotos_stack.empty()){
        if(!gotos_map.count(gotos_stack.top().second))
            throw "unknown label";
        prog.put_lex(Lex(POLIZ_LABEL,gotos_map[gotos_stack.top().second]),gotos_stack.top().first);
        gotos_stack.pop();
    }
};