
#include "visitor.hpp"
#include "ast.hpp"
#include <iostream>

template <typename T>
void astAccept(T element, Visitor visitor) {

}

template <>
void astAccept(Decl decl, Visitor visitor) {
    visitor.visitVarAssignPostId(decl);
};

template <>
void astAccept(Prog element, Visitor visitor) {
    visitor.visitProgPreDecl(element);
    astAccept(element.decl, visitor);
    visitor.visitProgPostDecl(element);
}