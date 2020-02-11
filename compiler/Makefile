.PHONY = all clean

all: parser_y.cpp lexer_l.cpp compiler.o expressions.o conditions.o
	g++ -o kompilator parser_y.cpp lexer_l.cpp Compiler.cpp Expressions.cpp Conditions.cpp
	rm -f *.o parser_y.* lexer_l.cpp

parser_y.cpp: parser.y
	bison -o parser_y.cpp -d parser.y

lexer_l.cpp: lexer.l
	flex -o lexer_l.cpp lexer.l
    
compiler.o: Compiler.cpp
	g++ -c Compiler.cpp -o compiler.o

expressions.o: Expressions.cpp
	g++ -c Expressions.cpp -o expressions.o

conditions.o: Conditions.cpp
	g++ -c Conditions.cpp -o conditions.o
	
clean:
	rm -f *.o parser_y.* lexer_l.cpp kompilator
