%{
#include "compiler.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

extern int yylex();
extern int yylineno;
extern FILE *yyin;
int yyerror(string str);
Compiler *com  = new Compiler(); 
Expressions *expr = new Expressions();
Conditions *cond = new Conditions();
%}

%define parse.error verbose
%define parse.lac full

%union {
    char* str;
}

%token <str> _number
%token <str> _declare _begin _end _if _then _else _endif
%token <str> _while _do _endwhile _enddo _for _from _endfor
%token <str> _colon _semicolon _comma _wri _red _identifier _to _downto
%token <str> _rightBracket _leftBracket _assign _eq _l _g _le _ge _ne _add _sub _mul _div _mod

%type <str> value
%type <str> identifier

%%
program:

    _declare declarations _begin commands _end                          { commands.push_back("HALT"); }
    |   _begin commands _end                                            { commands.push_back("HALT"); }
    ;

declarations:

    declarations _comma _identifier                                      		{ com -> declareVar($3,yylineno); }
    | declarations _comma _identifier _leftBracket _number _colon _number _rightBracket	{ com -> declareTab($3,$5,$7,yylineno); }
    | _identifier                                                       		{ com -> declareVar($1,yylineno); }
    | _identifier _leftBracket _number _colon _number _rightBracket                     { com -> declareTab($1,$3,$5,yylineno); }
    ;

commands:

    commands command                                                    { }
    | command                                                           { }
    ;

command:

    identifier _assign expression _semicolon                            { com -> assignVar($1,yylineno);  }
    | _if condition _then commands                                      { com -> elseIf(); whileLoop.pop();} 
        _else commands _endif                                           { com -> assignJump(); }                    
    | _if condition _then commands _endif                               { com -> assignJump(); whileLoop.pop(); } 
    | _while condition _do commands _endwhile                           { com -> endWhile(); com -> assignJump(); }       
    | _do                                                               { whileLoop.push(commands.size()); }
        commands _while condition _enddo                                { com ->endDoWhile(); } 
    | _for _identifier _from value _to value _do                        { com -> startUpFor($2,$4,$6,yylineno); }
        commands _endfor                                                { com -> endUpFor(); } 
    | _for _identifier _from value _downto value _do                    { com -> startDownFor($2,$4,$6,yylineno);}
        commands _endfor                                                { com -> endDownFor(); } 
    | _red identifier _semicolon                                        { com -> read($2,yylineno); }
    | _wri value _semicolon                                             { com -> write($2,yylineno); }
    ;

expression:

    value                                           { expr -> singleVal($1,yylineno); }
    | value _add value                              { expr -> add($1,$3,yylineno); }
    | value _sub value                              { expr -> sub($1,$3,yylineno); }
    | value _mul value                              { expr -> mul($1,$3,yylineno); }
    | value _div value                              { expr -> div($1,$3,yylineno); }
    | value _mod value                              { expr -> mod($1,$3,yylineno); }
    ;

condition:

    value _eq value                                 { cond -> eq($1,$3,yylineno); }
    | value _ne value                               { cond -> notEq($1,$3,yylineno); }
    | value _l value                                { cond -> less($1,$3,yylineno); }
    | value _g value                                { cond -> greater($1,$3,yylineno); }
    | value _le value                               { cond -> lessEq($1,$3,yylineno); }
    | value _ge value                               { cond -> greaterEq($1,$3,yylineno); }
    ;

value:

    _number                                         { genConst($1); }
    | identifier
    ;

identifier:

    _identifier                                     		{ com -> checkIde($1,yylineno); }
    | _identifier _leftBracket _identifier _rightBracket     	{ com -> pushArrayIde($1,$3,yylineno); }
    | _identifier _leftBracket _number _rightBracket         	{ com -> pushArrayNum($1,$3,yylineno); }
    ;

%%

int main(int argv, char* argc[]) {
    if(argv != 3) {
        cout<<"Usage: ./kompilator <input file> <output file>"<<endl;
        exit(1);
    }
    
    yyin = fopen(argc[1], "r");
    if (yyin == NULL) {
        cout<<"File does not exist: "<<argc[1]<<endl;
        exit(1);
    }
    
    yyparse();
    
    ofstream outfile;
    outfile.open(argc[2]);

    vector<string> generatedConsts = generateConstants();
    int command = generatedConsts.size();
    for(int cmd = 0; cmd < command; cmd++) {
        outfile<<generatedConsts[cmd]<<endl;
    }

    for(int cmd = 0; cmd < commands.size(); cmd++) {
        if (commands[cmd].find("JUMP") != string::npos || commands[cmd].find("JNEG") != string::npos || 
            commands[cmd].find("JPOS") != string::npos) {
            string j = commands[cmd].substr(5, commands[cmd].length()-1);
            replace(commands[cmd],j,longToString(stringToLong(j)+command));
        } else if(commands[cmd].find("JZERO") != string::npos) {
            string j = commands[cmd].substr(6, commands[cmd].length()-1);
            replace(commands[cmd],j,longToString(stringToLong(j)+command));
        }
        outfile<<commands[cmd]<<endl;
    }
    cout<<"Compilation finished without errors."<<endl;
	return 0;
}

int yyerror(string err) {
    cout << "ERROR occured! Compilation stopped." <<endl;
    cout << err << " Line: " << yylineno<<endl;
    exit(1);
}
