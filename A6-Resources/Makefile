#------------------------------------------------------------------------------
CPP=g++
SOURCE=lex.yy.c parser.tab.c ast.cc ast-eval.cc local-environment.cc ast-compile.cc icode.cc
MYPROGRAM=sclp
MYLIBRARY=$(CURDIR)
LDFLAGS=-Wl,-rpath=$(CURDIR)
CFLAGS=-std=c++0x
FLEX=flex
ALLHEADERS=allheaders.h
BISON=bison
#-----------------------------------------------------------------------------

all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)
	$(CPP) -include $(ALLHEADERS) -L$(MYLIBRARY) $(CFLAGS) $(LDFLAGS) $(SOURCE) -o $(MYPROGRAM) -lsclpshared -lfl -ly

lex.yy.c: scanner.l parser.tab.h
	$(FLEX) -l --yylineno scanner.l
parser.tab.c parser.tab.h : parser.y
	$(BISON) -d  parser.y

clean:
	rm -f $(MYPROGRAM)
