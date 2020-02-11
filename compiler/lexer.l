%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include <vector>
#include "parser_y.hpp"

%}

%x COMMENT
%option noyywrap

%%

"["                 { BEGIN(COMMENT); }
<COMMENT>"]"        { BEGIN(INITIAL); }
<COMMENT>"["        { printf("Comments can not be nested \n"); exit(1);}
<COMMENT>(\n|.)

"DECLARE"		return _declare;
"BEGIN"		    return _begin;
"END"		    return _end;

"ASSIGN"		return _assign;

"IF"		    return _if;
"THEN"		    return _then;
"ELSE"		    return _else;
"ENDIF"		    return _endif;

"WHILE"		    return _while;
"DO"		    return _do;
"ENDWHILE"	    return _endwhile;
"ENDDO"	        return _enddo;

"FOR"		    return _for;
"FROM"		    return _from;
"TO"		    return _to;
"DOWNTO"	    return _downto;
"ENDFOR"	    return _endfor;

"WRITE"		    return _wri;
"READ"		    return _red;

"EQ"            return _eq;
"LE"            return _l;
"GE"            return _g;
"LEQ"           return _le;
"GEQ"           return _ge;
"NEQ"           return _ne;

"PLUS"          return _add;
"MINUS"         return _sub;
"TIMES"         return _mul;
"DIV"           return _div;
"MOD"           return _mod;
")"             return _rightBracket;
"("             return _leftBracket;

-?[0-9]+        { yylval.str = (char *)strdup(yytext); return _number; }
[_a-z]+         { yylval.str = (char *)strdup(yytext); return _identifier; }

","             { return _comma; }
":"             { return _colon; }
";"             { return _semicolon; }

[ \t\r\n]+

.               {}

%%
