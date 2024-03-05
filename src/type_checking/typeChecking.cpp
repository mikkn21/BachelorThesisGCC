#include "typeChecking.hpp"
#include "../visitor.hpp"
#include <stack>
#include <string>

namespace grammar {

using namespace std;

class typeChecker : Visitor {


    stack<string> typeStack = stack<string>(); 

    void preVisit(VarDecl &vardecl) override {
       clear(typeStack); 
    }


    void postVisit(VarDecl &vardecl) override {
                 
        auto t1 = pop(typeStack);
        auto t2 = pop(typeStack);

        if ( t2 != t1) {
            throw new TypeCheckError("Type does not match expression");
        }


    } 

    void preVisit(PrimitiveType &primtype) override {
        // Change to use the symbolTable instead
        typeStack.push(primtype.type);
    }


    void postVisit(int &value) override {
        typeStack.push("int");
    }

    void postVisit(bool &value) override {
        typeStack.push("bool");
    }

    void postVisit(BinopExp &binop) override  {
        auto t1 = pop(typeStack);
        auto t2 = pop(typeStack);
        
        if ( t2 != t1) {
            throw new TypeCheckError("Type of lefthand side does not match Type of righthand side");
        }
    
        typeStack.push(t1);
    }

private:

    template<typename T>
    void clear(stack<T> myStack) {
        stack<string>().swap(myStack);
    }
   
    template<typename T>
    T pop(stack<T> myStack) {
        auto t1 = myStack.top(); 
        myStack.pop();
        return t1;

    } 


}; 





} // namespace grammar





