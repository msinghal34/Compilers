

all: lex.yy.c mysclp

lex.yy.c: myScript.l
	lex myScript.l

mysclp: lex.yy.c 
	g++ lex.yy.c -include allheaders.h -L. -Wl,-rpath=. -o yoursclp -lsclpshared -lfl -ly 