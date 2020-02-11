#include <iostream>
#include <string>
#include "compiler.hpp"

void Conditions::eq(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    } 
    addCommand("SUB "+longToString(mem2));
    addCommand("JZERO "+longToString(cmdIndex+2));
    addCommand("JUMP $j");
    jumps.push(commands.size()-1);
}

void Conditions::notEq(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    } 
    addCommand("SUB "+longToString(mem2));
    addCommand("JZERO $j");
    jumps.push(commands.size()-1);
}

void Conditions::less(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    }
    addCommand("SUB "+longToString(mem2));
    addCommand("JNEG "+longToString(cmdIndex+2));
    addCommand("JUMP $j");
    jumps.push(commands.size()-1);
}

void Conditions::greater(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    }
    addCommand("SUB "+longToString(mem2));
    addCommand("JPOS "+longToString(cmdIndex+2));
    addCommand("JUMP $j");
    jumps.push(commands.size()-1);
}

void Conditions::lessEq(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    }
    addCommand("SUB "+longToString(mem2));
    addCommand("JPOS $j");
    jumps.push(commands.size()-1);
}

void Conditions::greaterEq(char* a,char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    whileLoop.push(commands.size());
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    } 
    addCommand("SUB "+longToString(mem2));
    addCommand("JNEG $j");
    jumps.push(commands.size()-1);
}
