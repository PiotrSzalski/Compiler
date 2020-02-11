#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;

typedef struct {
	string name;
	string type;
	long memory;
	bool initialized;
	long begin;
	long size;
} Variable;

typedef struct {
    string type;
	string name;
	string index;
} Array;

typedef struct {
    string iterator;
	string condition;
    long begin;
} ForLoop;


extern map<string, Variable> variables;
extern vector<string> commands;
extern stack<long> jumps;
extern stack<long> whileLoop;
extern stack<ForLoop> forLoop;
extern stack<Array> arrays;
extern map<long,long> constToGen;
extern long memIndex;
extern long cmdIndex;

string longToString(long val);
long stringToLong(string val);
void addCommand(string cmd);
void replace(string& str, const string& from, const string& to);
void createVar(Variable *var,string name, string type);
void createVar(Variable *var,string name, string type, long size, long begin);
void insertVar(string name, Variable var);
void genConst(string a);
long getMemory1Index(Variable var, int line);
long getMemory2Index(Variable var, int line);
vector<string> generateConstants();
string decToBin(long val);
void controllVariable(string name, int line);
void error(string msg, int line);

class Compiler {
    public:
        void declareVar(string name, int line);
        void declareTab(string name, string begin, string end, int line);
        void assignVar(char* name, int line);
        void checkIde(string name, int line);
        void pushArrayIde(string name, string index, int line);
        void pushArrayNum(string name, string index, int line);
        void read(char* name,int line);
        void write(char* name, int line);
        void assignJump();
        void elseIf();
        void endWhile();
        void endDoWhile();
        void startUpFor(char* i, char* from, char* to, int line);
        void startDownFor(char* i, char* from, char* to, int line);
        void endUpFor();
        void endDownFor();
};

class Expressions {
    public:
        void singleVal(char* a, int yylineno);
        void add(char* a, char* b, int yylineno);
        void sub(char* a, char* b, int yylineno);
        void div(char* a, char* b, int yylineno);
        void mod(char* a, char* b, int yylineno);
        void mul(char* a, char* b, int yylineno);
};

class Conditions {
    public:
        void eq(char* a,char* b, int yylineno);
        void notEq(char* a,char* b, int yylineno);
        void less(char* a,char* b, int yylineno);
        void greater(char* a,char* b, int yylineno);
        void lessEq(char* a,char* b, int yylineno);
        void greaterEq(char* a,char* b, int yylineno);
};
