//
// Created by fibersell on 06.05.18.
//

#include "Stack.h"

#ifndef LEXANALYSER_EXECUTER_H
#define LEXANALYSER_EXECUTER_H

#endif //LEXANALYSER_EXECUTER_H


class Executer {

    Stack<Lex> args;
    Lex pc_el;

public:

    void execute ( Poliz& prog );

    int get_int_val_by_lex(Lex lex){
        if(lex.get_type() == LEX_NUM)
            return lex.get_value();
        else if(lex.get_type() == LEX_ID){
            if (!TID[lex.get_value()].get_assign())
                throw "indefinite identifier";
            return TID[lex.get_value()].get_value();
        }
    }

    int get_bool_val_by_lex(Lex lex){
        if(lex.get_type() == LEX_TRUE || lex.get_type() == LEX_FALSE)
            return lex.get_value();
        else if(lex.get_type() == LEX_ID)
            if (!TID[lex.get_value()].get_assign())
                throw "indefinite identifier";
            return TID[lex.get_value()].get_value();
    }


    int get_string_val_by_lex(Lex lex){
        if(lex.get_type() == LEX_CONST_STRING)
            return lex.get_value();
        else if(lex.get_type() == LEX_ID)
            if (!TID[lex.get_value()].get_assign())
                throw "indefinite identifier";
            return TID[lex.get_value()].get_value();
    }

    void put_bool_lex_by_val(int v){
        if(v > 0)
            args.push(Lex(LEX_TRUE,v));
        else
            args.push(Lex(LEX_FALSE,v));
    }

    void print_id(Lex lex){
        switch (TID[lex.get_value()].get_type()){
            case LEX_INT:
                cout << TID[lex.get_value()].get_value() << endl;
                break;
            case LEX_STRING:
                cout << TCS[TID[lex.get_value()].get_value()] << endl;
                break;
            case LEX_BOOL:
                if(TID[lex.get_value()].get_value() > 0)
                    cout << "true" << endl;
                else
                    cout << "false" << endl;
                break;
        }
    }

    void Lex_out(Lex lex){
        switch (lex.get_type()){
            case LEX_CONST_STRING:
                cout << TCS[lex.get_value()] << endl;
                break;
            case LEX_NUM:
                cout << lex.get_value() << endl;
                break;
            case LEX_TRUE:
                cout << "true" << endl;
                break;
            case LEX_FALSE:
                cout << "false" << endl;
                break;
            case LEX_ID:
                print_id(lex);
                break;
        }
    }


};


void Executer::execute ( Poliz& prog )
{
    string s1,s2;
    int i, j, index = 0;
    bool k,l;
    Lex t1,t2;
    int size = prog.get_free();
    while (index < size){
        pc_el = prog[index];
        switch (pc_el.get_type()){
            case POLIZ_LABEL:
                args.push(Lex(LEX_NUM,pc_el.get_value()));
                break;
            case POLIZ_GO:
                index = args.pop().get_value() - 1;
                break;
            case POLIZ_FGO:
                i = args.pop().get_value();
                j = get_bool_val_by_lex(args.pop());
                if(!j)
                    index = i - 1;
                break;
            case POLIZ_STACK_CLEAR:
                args.clear();
                break;
            case LEX_READ:
                t1 = args.pop();
                if(TID[t1.get_value()].get_type() == LEX_INT || TID[t1.get_value()].get_type() == LEX_BOOL){
                    cin >> i;
                    TID[t1.get_value()].put_value(i);
                } else if(TID[t1.get_value()].get_type() == LEX_STRING){
                    cin >> s1;
                    TCS.push_back(s1);
                    TID[t1.get_value()].put_value(TCS.size() - 1);
                }
                break;
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUM:
            case LEX_CONST_STRING:
            case LEX_ID:
            case POLIZ_ADDRESS:
                args.push ( pc_el );
                break;
            case LEX_NOT:
                i = get_bool_val_by_lex(args.pop());
                put_bool_lex_by_val(!i);
                break;
            case LEX_OR:
                i = get_bool_val_by_lex(args.pop());
                j = get_bool_val_by_lex(args.pop());
                put_bool_lex_by_val(i || j);
                break;
            case LEX_AND:
                i = get_bool_val_by_lex(args.pop());
                j = get_bool_val_by_lex(args.pop());
                put_bool_lex_by_val(i && j);
                break;
            case LEX_PLUS:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                args.push(Lex(LEX_NUM,i + j));
                break;
            case LEX_MINUS:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                args.push(Lex(LEX_NUM,j - i));
                break;
            case LEX_TIMES:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                args.push(Lex(LEX_NUM,i * j));
                break;
            case LEX_SLASH:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                if(j == 0)
                    throw "divided by zero";
                args.push(Lex(LEX_NUM,j / i));
                break;
            case LEX_CONCAT:
                i = get_string_val_by_lex(args.pop());
                j = get_string_val_by_lex(args.pop());
                TCS.push_back(TCS[j] + TCS[i]);
                args.push(Lex(LEX_CONST_STRING,TCS.size() - 1));
                break;
            case LEX_ASSIGN:
                t1 = args.pop();
                t2 = args.pop();
                if(TID[t2.get_value()].get_type() == LEX_INT){
                    i = get_int_val_by_lex(t1);
                    TID[t2.get_value()].put_value(i);
                    TID[t2.get_value()].put_assign();
                    args.push(Lex(LEX_NUM,i));
                } else if(TID[t2.get_value()].get_type() == LEX_BOOL){
                    i = get_bool_val_by_lex(t1);
                    TID[t2.get_value()].put_value(i);
                    TID[t2.get_value()].put_assign();
                    put_bool_lex_by_val(i);
                } else if(TID[t2.get_value()].get_type() == LEX_STRING){
                    i = get_string_val_by_lex(t1);
                    TID[t2.get_value()].put_value(i);
                    TID[t2.get_value()].put_assign();
                    args.push(Lex(LEX_CONST_STRING,i));
                } else if(TID[t2.get_value()].get_type() == LEX_STRUCT){
                    s1 = TID[t1.get_value()].get_name();
                    s2 = TID[t2.get_value()].get_name();
                    map_id_to_struct_instance.find(s2)->second
                            .assign_struct_to_struct(map_id_to_struct_instance.find(s1)->second);
                    args.push(Lex(LEX_STRUCT,t2.get_value()));
                }
                break;
            case LEX_POST_UN_MINUS:
                t1 = args.pop();
                if(!TID[t1.get_value()].get_assign())
                    throw "indefinite identifier";
                i = TID[t1.get_value()].get_value();
                args.push(Lex(LEX_NUM,i));
                TID[t1.get_value()].put_value(i - 1);
                break;
            case LEX_POST_UN_PLUS:
                t1 = args.pop();
                if(!TID[t1.get_value()].get_assign())
                    throw "indefinite identifier";
                i = TID[t1.get_value()].get_value();
                args.push(Lex(LEX_NUM,i));
                TID[t1.get_value()].put_value(i + 1);
                break;
            case LEX_PRE_UN_MINUS:
                t1 = args.pop();
                if(!TID[t1.get_value()].get_assign())
                    throw "indefinite identifier";
                i = TID[t1.get_value()].get_value();
                args.push(Lex(LEX_NUM,i - 1));
                TID[t1.get_value()].put_value(i - 1);
                break;
            case LEX_PRE_UN_PLUS:
                t1 = args.pop();
                if(!TID[t1.get_value()].get_assign())
                    throw "indefinite identifier";
                i = TID[t1.get_value()].get_value();
                args.push(Lex(LEX_NUM,i + 1));
                TID[t1.get_value()].put_value(i + 1);
                break;
            case LEX_LEQ:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j <= i);
                break;
            case LEX_LSS:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j < i);
                break;
            case LEX_GTR:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j > i);
                break;
            case LEX_GEQ:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j <= i);
                break;
            case LEX_EQ:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j == i);
                break;
            case LEX_NEQ:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(j != i);
                break;
            case LEX_LEQ_STRING:
                i = get_string_val_by_lex(args.pop());
                j = get_string_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] <= TCS[i]);
                break;
            case LEX_LSS_STRING:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] < TCS[i]);
                break;
            case LEX_GTR_STRING:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] > TCS[i]);
                break;
            case LEX_GEQ_STRING:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] >= TCS[i]);
                break;
            case LEX_EQ_STRING:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] == TCS[i]);
                break;
            case LEX_NEQ_STRING:
                i = get_int_val_by_lex(args.pop());
                j = get_int_val_by_lex(args.pop());
                put_bool_lex_by_val(TCS[j] != TCS[i]);
                break;
            case LEX_WRITE:
                t1 = args.pop();
                Stack<Lex> tmp;
                int ctr = t1.get_value();
                for(int i = 0;i < ctr;i++)
                    tmp.push(args.pop());
                while (!tmp.empty())
                    Lex_out(tmp.pop());
                break;
        }
        ++index;
    }



}



class Interpretator {

    Parser pars;
    Executer E;

public:

    Interpretator ( char* program ): pars (program) {

    };


    void interpretation ();
};

void Interpretator::interpretation () {
    pars.analyse ();
    E.execute ( pars.prog );
}
