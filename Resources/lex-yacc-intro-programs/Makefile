#PARSE= left-recursion-removed
PARSE= exp4
SCAN = exp

exp: lex.yy.o y.tab.o
	gcc -o exp lex.yy.o y.tab.o -ly -ll

y.tab.o y.tab.h: $(PARSE).y
	yacc -dv $(PARSE).y
	gcc -c y.tab.c

lex.yy.o : $(SCAN).l y.tab.h
	lex $(SCAN).l
	gcc -c lex.yy.c

clean:
	rm -f lex.yy.* y.tab.* y.output exp a.out
