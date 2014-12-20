%{
#include "Exception/Exception.hpp"
#include <vector>
#include <string>
#include "Parser.h"
#include "ParserDef.h"
std::vector<char> sb;
Lex curLex;

#define BuildOpX(X, str) do {curLex.type = X; curLex.raw = str; return 1;} while(0)
#define BuildLex(X) do {\
BuildOpX(X, new std::string(yytext));\
}while(0)
#define BuildOp(X) do {\
BuildOpX(X, nullptr);\
}while(0)
#define BuildStr(X) do {\
BuildOpX(X, new std::string(sb.begin(), sb.end()));\
}while(0)
%}

DIGIT                ([0-9])
INTEGER              ({DIGIT}+)
IDENTIFIER           ([_A-Za-z][_0-9A-Za-z]*)
WHITESPACE           ([ \t]+)

%option outfile="Lexer.cpp" HEADER-FILE="Lexer.h"
%option c++
%x S

%%
{WHITESPACE}                    {}

"+"                             {BuildOp(PLUS);}
"-"                             {BuildOp(MINUS);}
"*"                             {BuildOp(STAR);}
","                             {BuildOp(COMMA);}
">"                             {BuildOp(GT);}
"<"                             {BuildOp(LT);}
">="                            {BuildOp(GE);}
"<="                            {BuildOp(LE);}
"="                             {BuildOp(EQ);}
"<>"                            {BuildOp(NE);}
"("                             {BuildOp(LLC);}
")"                             {BuildOp(RLC);}
"not"                           {BuildOp(NOT);}
"and"                           {BuildOp(AND);}
"or"                            {BuildOp(OR);}
"."                             {BuildOp(DOT);}
"select"                        {BuildOp(SELECT);}
"from"                          {BuildOp(FROM);}
"where"                         {BuildOp(WHERE);}
"sum"                           {BuildOp(SUM);}
"avg"                           {BuildOp(AVG);}
"min"                           {BuildOp(MIN);}
"max"                           {BuildOp(MAX);}
"group"                         {BuildOp(GROUP);}
"by"                            {BuildOp(BY);}

{INTEGER}                       {BuildLex(INTEGER);}

<INITIAL>\"                     {BEGIN S; sb.clear();}
<S><<EOF>>                      {RAISE(Syntax);}
<S>\"                           {BEGIN INITIAL; BuildStr(STRING);}
<S>"\\n"                        {sb.push_back('\n');}
<S>"\\t"                        {sb.push_back('\t');}
<S>"\\\""                       {sb.push_back('\"');}
<S>"\\\\"                       {sb.push_back('\\');}
<S>.                            {sb.push_back(YYText()[0]);}

{IDENTIFIER}                    {BuildLex(IDENTIFIER);}

<<EOF>>                         {return 0;}
.                               {RAISE(Syntax);}
%%

int yyFlexLexer::yywrap() { return 1; }
