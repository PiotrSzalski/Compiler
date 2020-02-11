#include <iostream>
#include <cstdlib>
#include "compiler.hpp"
#include <map>
#include <stack>

using namespace std;

map<string, Variable> variables;
vector<string> commands;
stack<long> jumps;
stack<long> whileLoop;
stack<ForLoop> forLoop;
stack<Array> arrays;
map<long,long> constToGen;
long memIndex = 8;
long cmdIndex = 0;

void Compiler::declareVar(string name, int line) {
    if(variables.find(name) != variables.end()) {
        error("Variable '" + name + "' already declared",line);
    } else {
       Variable var; 
       createVar(&var,name,"VAR");
       insertVar(name,var);
    }
}

void Compiler::declareTab(string name, string begin, string end, int line) {
    if(variables.find(name) != variables.end()) {
        error("Variable '" + name + "' already declared",line);
    } else {
        long bn = stringToLong(begin);
        long ed = stringToLong(end);
        if(bn > ed) {
            error("Negative number of elements in '" +name +"' array.",line);
        } else {
            Variable var; 
            long size = ed - bn + 2;
            createVar(&var,name,"TAB",size,bn);
            insertVar(name,var);
        }
    }
}

void error(string msg, int line) {
    cout<<"ERROR occured! Compilation stopped."<<endl;
    cout<<msg<<" Line: "<<line<<endl;
    exit(1);
}

void Compiler::read(char* name, int line) {
    addCommand("GET");
    assignVar(name,line);
}

void Compiler::write(char* name,int line) {
    controllVariable(name,line);
    Variable var = variables.at(name);
    if(var.type == "TAB") {
        Array arr = arrays.top();
        arrays.pop();
        if(arr.type == "NUM") {
            long index = stringToLong(arr.index);
            if(index < var.begin || index > var.begin + var.size - 2) { 
                error("Reference to array beyond range",line);
            }
            addCommand("LOAD "+longToString(var.memory+index-var.begin+1));
            addCommand("PUT");
        } else {
            Variable index = variables.at(arr.index);
            genConst(longToString(var.memory-var.begin+1));
            Variable v = variables.at(longToString(var.memory-var.begin+1));
            addCommand("LOAD "+longToString(v.memory));
            addCommand("ADD "+longToString(index.memory));
            addCommand("LOADI 0");
            addCommand("PUT");
        }
    } else {
        addCommand("LOAD "+longToString(var.memory));
        addCommand("PUT");
    }
}

void Compiler::assignVar(char* name, int line) {
    if(variables.find(name) == variables.end()) {
        error("Undeclarated variable '" +string(name) +"'.",line);
    }
    Variable var = variables.at(name);
    if(var.type == "ITE") {
        error("Attempt to loop's iterator modification.",line);
    }
    if(var.type == "TAB") {
        Array arr = arrays.top();
        arrays.pop();
        if(arr.type == "NUM") {
            long index = stringToLong(arr.index);
            if(index < var.begin || index > var.begin + var.size - 2) { 
                error("Reference to array beyond range",line);
            }
            addCommand("STORE "+longToString(var.memory+index-var.begin+1));
        } else {
            Variable index = variables.at(arr.index);
            genConst(longToString(var.memory-var.begin+1));
            Variable v = variables.at(longToString(var.memory-var.begin+1));
            addCommand("STORE 3");
            addCommand("LOAD "+longToString(v.memory));
            addCommand("ADD "+longToString(index.memory));
            addCommand("STORE "+longToString(var.memory));
            addCommand("LOAD 3");
            addCommand("STOREI "+longToString(var.memory));
        }
    } else {
        addCommand("STORE "+longToString(var.memory));
        variables.at(name).initialized = true;
    }
}

void Compiler::elseIf() {
    addCommand("JUMP $j");
    assignJump();
    jumps.push(commands.size()-1);
}

void createVar(Variable *var,string name, string type) {
    var->name = name;
    var->memory = memIndex;
    var->type = type;
    var->initialized = false;
    var->size = 1;
}

void createVar(Variable *var,string name, string type, long size, long begin) {
    var->name = name;
    var->memory = memIndex;
    var->type = type;
    var->initialized = false;
    var->size = size;
    var->begin = begin;
}

void insertVar(string name, Variable var) {
    variables.insert(make_pair(name, var));
    memIndex += var.size;
}

void Compiler::assignJump() {
    long index = jumps.top();
    jumps.pop();
    replace(commands.at(index),"$j",longToString(cmdIndex));
}

void genConst(string a) {
    if(variables.find(a) == variables.end()) {
        if(a == "1") {
            Variable var;
            var.name = "1";
            var.initialized = true;
            var.memory = 2;
            var.size = 1;
            var.type = "NUM";
            variables.insert(make_pair("1",var));
        } else if(a == "-1") {
            Variable var;
            var.name = "-1";
            var.initialized = true;
            var.memory = 1;
            var.size = 1;
            var.type = "NUM";
            variables.insert(make_pair("-1",var));
        } else {
            Variable var;
            createVar(&var,a,"NUM");
            insertVar(a,var);
            variables.at(a).initialized = true;
            constToGen.insert(make_pair(stringToLong(a),memIndex-1));
        }
    }
}

void Compiler::endWhile() {
    long ind = whileLoop.top();
    whileLoop.pop();
    addCommand("JUMP "+longToString(ind));
}

void Compiler::endDoWhile() {
    whileLoop.pop();
    long ind = whileLoop.top();
    whileLoop.pop();
    addCommand("JUMP "+longToString(ind));
    assignJump();
}

void Compiler::startUpFor(char* i, char* from, char* to, int line) {
    controllVariable(from,line);
    controllVariable(to,line);
    if(variables.find(i) != variables.end()) {
        error("Loop's iterator '" + string(i) + "' already declared.",line);
    }
    Variable var1 = variables.at(from);
    Variable var2 = variables.at(to);
    long mem2 = getMemory2Index(var2,line);
    long mem1 = getMemory1Index(var1,line);
    Variable iterator;
    createVar(&iterator,i,"ITE");
    insertVar(i,iterator);
    variables.at(i).initialized = true;
    string cond = "C" + longToString(cmdIndex);
    Variable condition;
    createVar(&condition,cond,"VAR");
    insertVar(cond,condition);
    variables.at(cond).initialized = true;
    ForLoop loop;
    loop.iterator = iterator.name;
    loop.condition = condition.name;
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    }
    addCommand("STORE "+longToString(iterator.memory));
    addCommand("LOAD "+longToString(mem2));
    addCommand("STORE "+longToString(condition.memory));
    addCommand("LOAD "+longToString(iterator.memory));
    loop.begin = cmdIndex;
    forLoop.push(loop);
    addCommand("SUB "+longToString(condition.memory));
    addCommand("JPOS $j");
    jumps.push(commands.size()-1);
}

void Compiler::endUpFor() {
    ForLoop loop = forLoop.top();
    forLoop.pop();
    Variable ite = variables.at(loop.iterator);
    addCommand("LOAD "+longToString(ite.memory));
    addCommand("INC");
    addCommand("STORE "+longToString(ite.memory));
    addCommand("JUMP "+longToString(loop.begin));
    assignJump();
    variables.erase(loop.iterator);
}

void Compiler::startDownFor(char* i, char* from, char* to, int line) {
    controllVariable(from,line);
    controllVariable(to,line);
    if(variables.find(i) != variables.end()) {
        error("Loop's iterator '" + string(i) + "' already declared",line);
    }
    Variable var1 = variables.at(from);
    Variable var2 = variables.at(to);
    long mem2 = getMemory2Index(var2,line);
    long mem1 = getMemory1Index(var1,line);
    Variable iterator;
    createVar(&iterator,i,"ITE");
    insertVar(i,iterator);
    variables.at(i).initialized = true;
    string cond = "C" + longToString(cmdIndex);
    Variable condition;
    createVar(&condition,cond,"VAR");
    insertVar(cond,condition);
    variables.at(cond).initialized = true;
    ForLoop loop;
    loop.iterator = iterator.name;
    loop.condition = condition.name;
    if(mem1 != 0) {
        addCommand("LOAD "+longToString(mem1));
    }
    addCommand("STORE "+longToString(iterator.memory));
    addCommand("LOAD "+longToString(mem2));
    addCommand("STORE "+longToString(condition.memory));
    addCommand("LOAD "+longToString(iterator.memory));
    loop.begin = cmdIndex;
    forLoop.push(loop);
    addCommand("SUB "+longToString(condition.memory));
    addCommand("JNEG $j");
    jumps.push(commands.size()-1);
}

void Compiler::endDownFor() {
    ForLoop loop = forLoop.top();
    forLoop.pop();
    Variable ite = variables.at(loop.iterator);
    addCommand("LOAD "+longToString(ite.memory));
    addCommand("DEC");
    addCommand("STORE "+longToString(ite.memory));
    addCommand("JUMP "+longToString(loop.begin));
    assignJump();
    variables.erase(loop.iterator);
}

void Compiler::checkIde(string name, int line) {
    if(variables.find(name) == variables.end()) {
        error("Undeclarated variable '" +string(name) +"'.",line);
    }
    Variable var = variables.at(name);
    if(var.type == "TAB") {
        error("Lack of reference to array '" +string(name) +"' element",line);
    }
}

void Compiler::pushArrayIde(string name, string index, int line) {
    if(variables.find(name) == variables.end()) {
        error("Undeclarated variable '" +string(name) +"'.",line);
    }
    controllVariable(index,line);
    if(variables.at(index).type == "TAB") {
        error("Index identifier can not be array.",line);
    }
    if(variables.at(name).type != "TAB") {
        error("Variable '" +string(name) +"' is not array.",line);
    }
    Array arr;
    arr.type = "IDE";
    arr.index = index;
    arr.name = name;
    arrays.push(arr);
}

void Compiler::pushArrayNum(string name, string index, int line) {
    if(variables.find(name) == variables.end()) {
        error("Undeclarated variable '" +string(name) +"'.",line);
    }
    if(variables.at(name).type != "TAB") {
        error("Variable '" +string(name) +"' is not array.",line);
    }
    Array arr;
    arr.type = "NUM";
    arr.index = index;
    arr.name = name;
    arrays.push(arr);
}

void addCommand(string cmd) {
    commands.push_back(cmd);
    cmdIndex++;
}

long getMemory2Index(Variable var, int line) {
    long mem2 = var.memory;
	if(var.type == "TAB") {
		Array arr = arrays.top();
        arrays.pop();
        if(arr.type == "NUM") {
        	long index = stringToLong(arr.index);
            if(index < var.begin || index > var.begin + var.size - 2) { 
                error("Reference to array beyond range",line);
            }
        	mem2 = var.memory+index-var.begin+1;
    	} else {
        	Variable index = variables.at(arr.index);
        	genConst(longToString(var.memory-var.begin+1));
        	Variable v = variables.at(longToString(var.memory-var.begin+1));
        	addCommand("LOAD "+longToString(v.memory));
        	addCommand("ADD "+longToString(index.memory));
        	addCommand("LOADI 0");
        	addCommand("STORE "+longToString(var.memory));
        	mem2 = var.memory;
    	}
	}
    return mem2;
}

long getMemory1Index(Variable var, int line) {
    long mem1 = var.memory;
	if(var.type == "TAB") {
		Array arr = arrays.top();
        arrays.pop();
        if(arr.type == "NUM") {
        	long index = stringToLong(arr.index);
            if(index < var.begin || index > var.begin + var.size - 2) { 
                error("Reference to array beyond range",line);
            }
        	mem1 = var.memory+index-var.begin+1;
    	} else {
        	Variable index = variables.at(arr.index);
        	genConst(longToString(var.memory-var.begin+1));
        	Variable v = variables.at(longToString(var.memory-var.begin+1));
        	addCommand("LOAD "+longToString(v.memory));
        	addCommand("ADD "+longToString(index.memory));
       	 	addCommand("LOADI 0");
        	mem1 = 0;
    	}
	}
    return mem1;
}

void controllVariable(string name, int line) {
    if(variables.find(name) == variables.end()) {
        error("Undeclarated variable '" +string(name) +"'.",line);
    }
    Variable var = variables.at(name);
    if(var.type == "VAR" && var.initialized == false) {
        error("Uninitialized variable '" +string(name) +"'.",line);
    }
}

vector<string> generateConstants() {
    vector<string> comm;
    comm.push_back("SUB 0");
    if(constToGen.find(0) != constToGen.end()) {
        long mem = constToGen.at(0);
        comm.push_back("STORE "+longToString(mem));
        constToGen.erase(0);
    }
    comm.push_back("DEC");
    comm.push_back("STORE 1");
    comm.push_back("INC");
    comm.push_back("INC");
    comm.push_back("STORE 2");

    map<long, long>::iterator it;
    for (it = constToGen.begin(); it != constToGen.end(); it++) {
        long val = it->first;
        if(val < 0) {
            val = -val;
            string bin = decToBin(val);
            comm.push_back("LOAD 1");
            for(int i = 1; i < bin.length(); i++) {
                comm.push_back("SHIFT 2");
                if(bin[i] == '1') {
                    comm.push_back("DEC");
                }
            }
        } else if(val > 0) {
            string bin = decToBin(val);
            comm.push_back("LOAD 2");
            for(int i = 1; i < bin.length(); i++) {
                comm.push_back("SHIFT 2");
                if(bin[i] == '1') {
                    comm.push_back("INC");
                }
            }
        } else {
            comm.push_back("SUB 0");
        }
        comm.push_back("STORE "+longToString(it->second));
    } 
    return comm;
}

string longToString(long val) {
    ostringstream oss;
    oss << val;
    return oss.str();
}

long stringToLong(string val) {
    stringstream ss(val);
    long result;
    ss >> result;  
    return result;
}

void replace(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

string decToBin(long val) {
    string bin = "";
    while(val != 0) {
        bin = (val % 2 == 0 ? "0" : "1") + bin;
        val /= 2;
    }
    return bin;
}
