
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"

class Visitor {
public: 
    Visitor() {}

    virtual void ProgPreDecl(const Prog &prog) { }
    virtual void ProgPostDecl(const Prog &prog) { }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &visitor;

public:
    TreeTraveler(Visitor &visitor) : visitor(visitor) {}

    template <typename T>
    void operator()(const T &t) { }
};

template <>
void TreeTraveler::operator()(const Prog &prog);

#endif
