//
// Created by fibersell on 07.05.18.
//

#ifndef LEXANALYSER_STACK_H
#define LEXANALYSER_STACK_H

#endif //LEXANALYSER_STACK_H

template <class T>
class Stack{

    deque<T> stck;

public:
    Stack(){

    }


    void push(T t){
        stck.push_back(t);
    }

    T pop(){
        T t;
        t = stck.back();
        stck.pop_back();
        return t;
    }


    bool empty(){
        return stck.empty();
    }

    void clear(){
        stck.clear();
    }
};