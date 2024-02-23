
#include "../ast.hpp"
#include "symbol_table.hpp"
#include "../visitor.hpp"

Prog symbol_collection(Prog ast);

class SymbolCollectionVisitor : Visitor {
    void visitProgPreDecl(const Prog &prog) override;
    // void visitProgPostDecl(const Prog &prog) override;
    // void visitVarAssignPostId(const Decl &decl) override;
    // void visitVarAssignPreId(const VarAssign &varAssign) override;
};
