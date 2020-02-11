#include <iostream>
#include <string>
#include <cstdlib>
#include "compiler.hpp"

using namespace std;

void Expressions::singleVal(char* a, int line) {
    controllVariable(a,line);
    Variable var = variables.at(a);
	long mem = getMemory1Index(var,line);
	if(mem != 0) {
		addCommand("LOAD "+longToString(mem));
	}
}

void Expressions::add(char* a, char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        if(var1.name == "0") {
            addCommand("LOAD "+longToString(mem2));
        } else if(var2.name == "0") {
            addCommand("LOAD "+longToString(mem1));
        } else if(var1.type == "NUM" && abs(stringToLong(var1.name)) < 10) {
            addCommand("LOAD "+longToString(mem2));
            long val = stringToLong(var1.name);
            for(int i = 0; i < abs(val); i++) {
                if(val < 0) {
                    addCommand("DEC");
                } else {
                    addCommand("INC");
                }
            }
        } else if(var2.type == "NUM" && abs(stringToLong(var2.name)) < 10) {
            addCommand("LOAD "+longToString(mem1));
            long val = stringToLong(var2.name);
            for(int i = 0; i < abs(val); i++) {
                if(val < 0) {
                    addCommand("DEC");
                } else {
                    addCommand("INC");
                }
            }
        } else {
            addCommand("LOAD "+longToString(mem1));
            addCommand("ADD "+longToString(mem2));
        }
    } else {
        if(var2.type == "NUM") {
            long val = stringToLong(var2.name);
            if(val != 0 && abs(val) < 10) {
                for(int i = 0; i < abs(val); i++) {
                    if(val < 0) {
                        addCommand("DEC");
                    } else {
                        addCommand("INC");
                    }
                }
            } else if(val != 0) {
                addCommand("ADD "+longToString(mem2));
            }
        } else {
            addCommand("ADD "+longToString(mem2));
        }
    }
}

void Expressions::sub(char* a, char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    } 
    if(var2.type == "NUM") {
        long val = stringToLong(var2.name);
        if(val != 0 && abs(val) < 10) {
            for(int i = 0; i < abs(val); i++) {
                if(val < 0) {
                    addCommand("INC");
                } else {
                    addCommand("DEC");
                }
            }
        } else if(val != 0) {
            addCommand("SUB "+longToString(mem2));
        }
    } else {
        addCommand("SUB "+longToString(mem2));
    }
}

void Expressions::mul(char* a, char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(var1.name == "0") {
        addCommand("SUB 0");
    } else if(var2.name == "0") {
        addCommand("SUB 0");
    } else if(var1.name == "1") {
        addCommand("LOAD "+longToString(mem2));
    } else if(var2.name == "1") {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        }
    } else if(var1.name == "-1") {
        addCommand("SUB 0");
        addCommand("SUB "+longToString(mem2));
    } else if(var2.name == "-1") {
        if(mem1 != 0) {
            addCommand("SUB 0");
            addCommand("SUB "+longToString(mem1));
        } else {
            addCommand("STORE 3");
            addCommand("SUB 0");
            addCommand("SUB 3");
        }
    } else if(var1.type == "NUM" && (abs(stringToLong(var1.name)) & (abs(stringToLong(var1.name)) - 1)) == 0 ) {
        long val = abs(stringToLong(var1.name));
        addCommand("LOAD "+longToString(mem2));
        while(val != 1) {
            addCommand("SHIFT 2");
            val /= 2;
        }
        if(stringToLong(var1.name) < 0) {
            addCommand("STORE 3");
            addCommand("SUB 3");
            addCommand("SUB 3");
        }
    } else if(var2.type == "NUM" && (abs(stringToLong(var2.name)) & (abs(stringToLong(var2.name)) - 1)) == 0 ) {
        long val = abs(stringToLong(var2.name));
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        }
        while(val != 1) {
            addCommand("SHIFT 2");
            val /= 2;
        }
        if(stringToLong(var2.name) < 0) {
            addCommand("STORE 3");
            addCommand("SUB 3");
            addCommand("SUB 3");
        }
    }  else {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        } 
        addCommand("JZERO "+longToString(cmdIndex+84)); 
        addCommand("STORE 3");
        addCommand("JPOS "+longToString(cmdIndex+10));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JZERO "+longToString(cmdIndex+80));
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 3");
        addCommand("JNEG "+longToString(cmdIndex+43));
        addCommand("JUMP "+longToString(cmdIndex+12));
        addCommand("ADD 3");
        addCommand("JPOS "+longToString(cmdIndex+40));
        addCommand("JUMP "+longToString(cmdIndex+9));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JZERO "+longToString(cmdIndex+71));
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("ADD 3");
        addCommand("JNEG "+longToString(cmdIndex+34));
        addCommand("JUMP "+longToString(cmdIndex+3));
        addCommand("SUB 3");
        addCommand("JPOS "+longToString(cmdIndex+31));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JPOS "+longToString(cmdIndex+10));
        addCommand("STORE 4");
        addCommand("SUB 4");
        addCommand("SUB 4");
        addCommand("STORE 4");
        addCommand("LOAD 3");
        addCommand("SUB 3");
        addCommand("SUB 3");
        addCommand("STORE 3");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("STORE 4");
        addCommand("SUB 0");
        addCommand("STORE 6");
        addCommand("LOAD 4");
        addCommand("JZERO "+longToString(cmdIndex+48));
        addCommand("SHIFT 1");
        addCommand("SHIFT 2");
        addCommand("SUB 4");
        addCommand("JZERO "+longToString(cmdIndex+4));
        addCommand("LOAD 6"); 
        addCommand("ADD 3");
        addCommand("STORE 6"); 
        addCommand("LOAD 3");
        addCommand("SHIFT 2");
        addCommand("STORE 3"); 
	    addCommand("LOAD 4");
        addCommand("SHIFT 1");
        addCommand("STORE 4");
        addCommand("JUMP "+longToString(cmdIndex-14));
        addCommand("LOAD 3");                             
        addCommand("JNEG "+longToString(cmdIndex+5));
        addCommand("STORE 4");
        addCommand("LOAD "+longToString(mem2));
        addCommand("STORE 3");
        addCommand("JUMP "+longToString(cmdIndex+10));
        addCommand("SUB 3");
        addCommand("SUB 3");
        addCommand("STORE 4");
        addCommand("LOAD "+longToString(mem2));
        addCommand("SUB "+longToString(mem2));
        addCommand("SUB "+longToString(mem2));
        addCommand("STORE 3");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("STORE 4");
        addCommand("SUB 0");
        addCommand("STORE 6");
        addCommand("LOAD 4");
        addCommand("JZERO "+longToString(cmdIndex+15));
        addCommand("SHIFT 1");
        addCommand("SHIFT 2");
        addCommand("SUB 4");
        addCommand("JZERO "+longToString(cmdIndex+4));
        addCommand("LOAD 6");
        addCommand("ADD 3");
        addCommand("STORE 6"); 
        addCommand("LOAD 3");
        addCommand("SHIFT 2");
        addCommand("STORE 3"); 
	    addCommand("LOAD 4");
        addCommand("SHIFT 1");
        addCommand("STORE 4");
        addCommand("JUMP "+longToString(cmdIndex-14));
        addCommand("LOAD 6");
    }
}

void Expressions::div(char* a, char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(var1.name == "0") {
        addCommand("SUB 0");
    } else if(var2.name == "0") {
        addCommand("SUB 0");
    } else if(var2.name == "1") {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        }
    } else if(var2.name == "-1") {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        } 
        addCommand("STORE 3");
        addCommand("SUB 3");
        addCommand("SUB 3");
    } else if(var2.type == "NUM" && stringToLong(var2.name) > 0 && (stringToLong(var2.name) & (stringToLong(var2.name) - 1)) == 0 ) {
        long val = stringToLong(var2.name);
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        }
        while(val != 1) {
            addCommand("SHIFT 1");
            val /= 2;
        }
    } else {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        } 
        addCommand("JZERO "+longToString(cmdIndex+122)); 
        addCommand("STORE 4");
        addCommand("JNEG "+longToString(cmdIndex+60));
        addCommand("LOAD "+longToString(mem2)); 
        addCommand("JZERO "+longToString(cmdIndex+118));
        addCommand("STORE 6");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 6");
        addCommand("SUB 6");
        addCommand("STORE 6");
        addCommand("SUB 0");
        addCommand("STORE 5");
        addCommand("STORE 7");
        addCommand("INC");
        addCommand("SUB 4");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("ADD 4");
        addCommand("SHIFT 2");
        addCommand("JUMP "+longToString(cmdIndex-4));
        addCommand("ADD 4");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("LOAD 3"); 
        addCommand("SHIFT 1"); 
        addCommand("JZERO "+longToString(cmdIndex+26));    
        addCommand("STORE 3");  
        addCommand("LOAD 4");
        addCommand("SUB 3");
        addCommand("JNEG "+longToString(cmdIndex+7));
        addCommand("STORE 4");
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
        addCommand("INC");
	    addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex+4));
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
	    addCommand("STORE 5");
        addCommand("SUB 6");
        addCommand("JNEG "+longToString(cmdIndex+7));
        addCommand("STORE 5");
        addCommand("LOAD 7");
        addCommand("SHIFT 2");
        addCommand("INC");
        addCommand("STORE 7");
        addCommand("JUMP "+longToString(cmdIndex-23));
        addCommand("LOAD 7");
        addCommand("SHIFT 2");
        addCommand("STORE 7");
        addCommand("JUMP "+longToString(cmdIndex-27));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JPOS "+longToString(cmdIndex+71));
        addCommand("LOAD 5");
        addCommand("JZERO "+longToString(cmdIndex+6));
        addCommand("LOAD 7");
        addCommand("SUB 7");
        addCommand("SUB 7");
        addCommand("DEC");
        addCommand("JUMP "+longToString(cmdIndex+65));
        addCommand("LOAD 7");
        addCommand("SUB 7");
        addCommand("SUB 7");
        addCommand("JUMP "+longToString(cmdIndex+61));
        addCommand("LOAD "+longToString(mem2)); 
        addCommand("JZERO "+longToString(cmdIndex+59));
        addCommand("STORE 6");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 6");
        addCommand("SUB 6");
        addCommand("STORE 6");
        addCommand("SUB 0");
        addCommand("STORE 5");
        addCommand("STORE 7");
        addCommand("INC");
        addCommand("ADD 4");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 4");
        addCommand("SHIFT 2");
        addCommand("JUMP "+longToString(cmdIndex-4));
        addCommand("SUB 4");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("LOAD 3");
        addCommand("SHIFT 1");   
        addCommand("JZERO "+longToString(cmdIndex+26));  
        addCommand("STORE 3");   
        addCommand("LOAD 4");
        addCommand("ADD 3");
        addCommand("JPOS "+longToString(cmdIndex+7));
        addCommand("STORE 4");
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
        addCommand("DEC");
	    addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex+4));
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
	    addCommand("STORE 5");
        addCommand("ADD 6");
        addCommand("JPOS "+longToString(cmdIndex+7));
        addCommand("STORE 5");
        addCommand("LOAD 7");
        addCommand("SHIFT 2");
        addCommand("INC");
        addCommand("STORE 7");
        addCommand("JUMP "+longToString(cmdIndex-23));
        addCommand("LOAD 7");
        addCommand("SHIFT 2");
        addCommand("STORE 7");
        addCommand("JUMP "+longToString(cmdIndex-27));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JNEG "+longToString(cmdIndex+12));
        addCommand("LOAD 5");
        addCommand("JZERO "+longToString(cmdIndex+6));
        addCommand("LOAD 7");
        addCommand("SUB 7");
        addCommand("SUB 7");
        addCommand("DEC");
        addCommand("JUMP "+longToString(cmdIndex+6));
        addCommand("LOAD 7");
        addCommand("SUB 7");
        addCommand("SUB 7");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("LOAD 7");
    }
}

void Expressions::mod(char* a, char* b, int yylineno) {
    controllVariable(a,yylineno);
    controllVariable(b,yylineno);
    Variable var1 = variables.at(a);
    Variable var2 = variables.at(b);
    long mem2 = getMemory2Index(var2,yylineno);
    long mem1 = getMemory1Index(var1,yylineno);
    if(var1.name == "0") {
        addCommand("SUB 0");
    } else if(var2.name == "0") {
        addCommand("SUB 0");
    } else if(var2.name == "1") {
        addCommand("SUB 0");
    } else if(var2.name == "-1") {
        addCommand("SUB 0");
    } else {
        if(mem1 != 0) {
            addCommand("LOAD "+longToString(mem1));
        } 
        addCommand("JZERO "+longToString(cmdIndex+90));
        addCommand("STORE 4");
        addCommand("JNEG "+longToString(cmdIndex+45));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JZERO "+longToString(cmdIndex+86));        
        addCommand("STORE 6");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 6");
        addCommand("SUB 6");
        addCommand("STORE 6");
        addCommand("SUB 0");
        addCommand("STORE 5");
        addCommand("INC");
        addCommand("SUB 4");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("ADD 4");
        addCommand("SHIFT 2");
        addCommand("JUMP "+longToString(cmdIndex-4));
        addCommand("ADD 4");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("LOAD 3"); 
        addCommand("SHIFT 1");    
        addCommand("JZERO "+longToString(cmdIndex+18)); 
        addCommand("STORE 3");   
        addCommand("LOAD 4");
        addCommand("SUB 3");
        addCommand("JNEG "+longToString(cmdIndex+7));
        addCommand("STORE 4");
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
        addCommand("INC");
	    addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex+4));
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
	    addCommand("STORE 5");
        addCommand("SUB 6");
        addCommand("JNEG "+longToString(cmdIndex-17));
        addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex-19));
        addCommand("LOAD 5"); 
        addCommand("JZERO "+longToString(cmdIndex+49));
        addCommand("LOAD "+longToString(mem2)); 
        addCommand("JNEG "+longToString(cmdIndex+2));
        addCommand("SUB 0");
        addCommand("ADD 5");
        addCommand("JUMP "+longToString(cmdIndex+44));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JZERO "+longToString(cmdIndex+42));
        addCommand("STORE 6");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 6");
        addCommand("SUB 6");
        addCommand("STORE 6");
        addCommand("SUB 0");
        addCommand("STORE 5");
        addCommand("INC");
        addCommand("ADD 4");
        addCommand("JPOS "+longToString(cmdIndex+4));
        addCommand("SUB 4"); 
        addCommand("SHIFT 2");
        addCommand("JUMP "+longToString(cmdIndex-4));
        addCommand("SUB 4");
        addCommand("JUMP "+longToString(cmdIndex+2));
        addCommand("LOAD 3"); 
        addCommand("SHIFT 1");   
        addCommand("JZERO "+longToString(cmdIndex+18));  
        addCommand("STORE 3");   
        addCommand("LOAD 4");
        addCommand("ADD 3");
        addCommand("JPOS "+longToString(cmdIndex+7));
        addCommand("STORE 4");
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
        addCommand("DEC");
	    addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex+4));
        addCommand("LOAD 5");
        addCommand("SHIFT 2");
	    addCommand("STORE 5");
        addCommand("ADD 6");
        addCommand("JPOS "+longToString(cmdIndex-17));
        addCommand("STORE 5");
        addCommand("JUMP "+longToString(cmdIndex-19)); 
        addCommand("LOAD 5");
        addCommand("JZERO "+longToString(cmdIndex+5));
        addCommand("LOAD "+longToString(mem2));
        addCommand("JPOS "+longToString(cmdIndex+2));
        addCommand("SUB 0");
        addCommand("ADD 5");
    }
}
