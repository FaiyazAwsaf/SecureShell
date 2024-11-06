#include <iostream>
#include "exceptions.h"

template <typename T>
class DynamicStack{

    private :
        T* stack_arr;
        int capacity;
        int top_pos;

        void resize(int new_capacity){
            T* new_stack_arr = new T[new_capacity];

            for(int i=0; i<top_pos; i++){
                new_stack_arr[i] = stack_arr[i];
            }

            delete[] stack_arr;
            stack_arr = new_stack_arr;
            capacity = new_capacity;
        }

    public :

        DynamicStack(){
            this->stack_arr = new T[5];
            this->capacity = 5;
            this->top_pos = 0;
        }

        ~DynamicStack(){
            delete[] stack_arr;
        }

        void push(const T& data){
            if(top_pos == capacity-1){
                resize(capacity*2);
            }
            stack_arr[top_pos++] = data;
        }

        void pop(){
            if(!is_empty()){
                //std::cout<<stack_arr[top_pos-1]<<std::endl;
                top_pos--;
            }
            else{
                try{
                    throw StackEmptyException("Empty stack! Cannot pop");
                }
                catch(StackEmptyException e){
                    std::cout<<e.what()<<std::endl;
                }
            }
        }

        T peek(){
            if(!is_empty()){
                return stack_arr[top_pos-1];
            }
            else{
                try{
                    throw StackEmptyException();
                }
                catch(StackEmptyException e){
                    std::cout<<e.what()<<std::endl;
                }

                return T();
            }
        }

        bool is_empty(){
            return top_pos==0;
        }

        int size(){
            return top_pos;
        }

        void show(){
            for(int i=0; i<top_pos; i++){
                std::cout<<stack_arr[i]<<" ";
            }

            std::cout<<std::endl;
        }
};