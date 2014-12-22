%syntax_error {
RAISE(Syntax);
}

%include {
#include <cassert>
#include "ParserDef.h"
#include "Exception/Exception.hpp"
#include <iostream>
#include "TypeDB/expr.hpp"
#include "TypeDB/tblSelector.hpp"
#include "TypeDB/tblExpr.hpp"
#include "Stmt/updateStmt.hpp"
#include "Stmt/selectStmt.hpp"
#include "Stmt/insertStmt.hpp"
extern Lex curLex;
}

%code {
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Lexer.h"
namespace Parser {
    Stmt::Stmt* CreateAST(std::string st) {
        std::vector<std::string*> strs;
        void *parser = ParseAlloc(malloc);
        std::stringstream stream(st);
        try {
            yyFlexLexer lexer(&stream);
            while (lexer.yylex()) {
                if (curLex.raw)
                    strs.push_back(curLex.raw);
                Parse(parser, curLex.type, curLex.raw, 0);
            }
            Stmt::Stmt* ret;
            Parse(parser, 0, 0, &ret);
            return ret;
        } catch(Exception::Exception *e) {
            ParseFree(parser, free);
            for (auto p : strs)
                delete p;
            throw e;
        }
        for (auto p : strs)
            delete p;
    }
}
}

%left OR .
%left AND .
%left EQ NE .
%left LE GE LT GT .
%left PLUS MINUS .
%nonassoc NOT .
%nonassoc RLC .

%extra_argument {Stmt::Stmt** ret}

%start_symbol stmt

%token_type {std::string*}
%token_destructor {delete $$;}

%type stmt {Stmt::Stmt*}
stmt(A) ::= selectStmt(B). {*ret = A = B;}
stmt(A) ::= insertStmt(B). {*ret = A = B;}
//stmt(A) ::= updateStmt(B). {*ret = A = B;}

%type selectStmt {Stmt::SelectStmt*}
%destructor selectStmt {delete $$;}
selectStmt(A) ::= selectClause(B) fromClause(C) . {A = B; A->from.swap(*C); delete C;}
selectStmt(A) ::= selectStmt(B) whereClause(C) . {A = B; A->where = C;}
selectStmt(A) ::= selectStmt(B) groupbyClause(C) . {A = B; A->groupby = C;}

%type insertStmt {Stmt::InsertStmt*}
%destructor insertStmt {delete $$;}
insertStmt(A) ::= INSERT INTO IDENTIFIER(B) VALUES rows(C) . {A = new Stmt::InsertStmt; A->tbl.rows.swap(*C); A->tbl_name = *B; delete C;}

%type selectClause {Stmt::SelectStmt*}
%destructor selectClause {delete $$;}
selectClause(A) ::= SELECT tblExprList(B) . {A = new Stmt::SelectStmt; A->select.swap(*B); delete B;}
selectClause(A) ::= SELECT STAR . {A = new Stmt::SelectStmt; A->selectAll = true;}

%type tblExprList {std::vector<TypeDB::TblExpr*>*}
%destructor tblExprList {delete $$;}
tblExprList(A) ::= tblExprList(B) COMMA tblExpr(C) . {A = B; A->push_back(C);}
tblExprList(A) ::= tblExpr(C) . {A = new std::vector<TypeDB::TblExpr*>; A->push_back(C);}

%type tblExpr {TypeDB::TblExpr*}
%destructor tblExpr{delete $$;}
tblExpr(A) ::= IDENTIFIER(B) DOT IDENTIFIER(C) . {A = new TypeDB::ReadTblExpr(*B, *C);}
tblExpr(A) ::= IDENTIFIER(B) . {A = new TypeDB::ReadTblExpr(*B);}
tblExpr(A) ::= SUM LLC tblExpr(B) RLC . {A = new TypeDB::UnaryTblExpr(B, TypeDB::UnaryTblExpr::Sum);}
tblExpr(A) ::= AVG LLC tblExpr(B) RLC . {A = new TypeDB::UnaryTblExpr(B, TypeDB::UnaryTblExpr::Avg);}
tblExpr(A) ::= MIN LLC tblExpr(B) RLC . {A = new TypeDB::UnaryTblExpr(B, TypeDB::UnaryTblExpr::Min);}
tblExpr(A) ::= MAX LLC tblExpr(B) RLC . {A = new TypeDB::UnaryTblExpr(B, TypeDB::UnaryTblExpr::Max);}

%type fromClause {std::vector<TypeDB::TableSelector*>*}
%destructor fromClause {delete $$;}
fromClause(A) ::= FROM tables(B) . {A = B;}

%type groupbyClause {TypeDB::Expr*}
%destructor groupbyClause {delete $$;}
groupbyClause(A) ::= GROUP BY expr(B) . {A = B;}

%type tables {std::vector<TypeDB::TableSelector*>*}
%destructor tables {delete $$;}
tables(A) ::= tables(B) COMMA table(C) . {A = B; A->push_back(C);}
tables(A) ::= table(C) . {A = new std::vector<TypeDB::TableSelector*>; A->push_back(C);}

%type table {TypeDB::TableSelector*}
%destructor table {delete $$;}
table(A) ::= IDENTIFIER(B) . {A = new TypeDB::RawTableSelector(*B);}

%type whereClause {TypeDB::Expr*}
%destructor whereClause {delete $$;}
whereClause(A) ::= WHERE expr(B) . {A = B;}

%type updateClause {Stmt::UpdateStmt*}
%destructor updateClause {delete $$;}
updateClause(A) ::= UPDATE IDENTIFIER(B) SET updateRules(C) . {A = new Stmt::UpdateStmt; A->tbl = *B; A->rules.swap(*C); delete C;}

%type updateRules {std::unordered_map<std::string, TypeDB::Expr*>*}
%destructor updateRules {delete $$;}
updateRules(A) ::= updateRules(B) COMMA updateRule(C) . {A = B; (*A)[C->first] = C->second; delete C;}
updateRules(A) ::= updateRule(C) . {A = new std::unordered_map<std::string, TypeDB::Expr*>; (*A)[C->first] = C->second; delete C;}

%type updateRule {std::pair<std::string, TypeDB::Expr*>*}
%destructor updateRule {delete $$;}
updateRule(A) ::= IDENTIFIER(B) EQ expr(C) . {A = new std::pair<std::string, TypeDB::Expr*>(*B, C);}

%type expr {TypeDB::Expr*}
%destructor expr {delete $$;}
//Literal Expr
expr(A) ::= literal(B) . {A = new TypeDB::LiteralExpr(B);}
expr(A) ::= IDENTIFIER(B) . {A = new TypeDB::ReadExpr(*B);}
expr(A) ::= IDENTIFIER(B) DOT IDENTIFIER(C) . {A = new TypeDB::ReadExpr(*B, *C);}
//Simple Expr
expr(A) ::= LLC expr(B) RLC . {A = B;}
expr(A) ::= expr(B) PLUS expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::Plus);}
expr(A) ::= expr(B) MINUS expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::Minus);}
expr(A) ::= expr(B) EQ expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::Equal);}
expr(A) ::= expr(B) NE expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::NotEqual);}
expr(A) ::= expr(B) LT expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::LessThan);}
expr(A) ::= expr(B) GT expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::GreaterThan);}
expr(A) ::= expr(B) LE expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::LessEqual);}
expr(A) ::= expr(B) GE expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::GreaterEqual);}
expr(A) ::= expr(B) AND expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::And);}
expr(A) ::= expr(B) OR expr(C) . {A = new TypeDB::BinaryExpr(B, C, TypeDB::BinaryExpr::Or);}
expr(A) ::= NOT expr(B) . {A = new TypeDB::UnaryExpr(B, TypeDB::UnaryExpr::Not);}

%type rows {std::vector<TypeDB::Row>*}
%destructor row {delete $$;}
rows(A) ::= LLC row(C) RLC . {A = new std::vector<TypeDB::Row>; A->push_back(std::move(*C)); delete C;}
rows(A) ::= rows(B) COMMA LLC row(C) RLC . {A = B; A->push_back(std::move(*C)); delete C;}

%type row {TypeDB::Row*}
row(A) ::= literal(C) . {A = new TypeDB::Row; A->objs.push_back(C);}
row(A) ::= row(B) COMMA literal(C) . {A = B; A->objs.push_back(C);}

%type literal {TypeDB::Object*}
%destructor literal {delete $$;}
literal(A) ::= STRING(B) . {A = TypeDB::StringType::Create(*B);}
literal(A) ::= INTEGER(B) . {A = TypeDB::IntType::Create(std::stoi(*B));}
literal(A) ::= NULL_ . {A = TypeDB::NullType::Create();}
