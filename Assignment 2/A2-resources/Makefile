#------------------------------------------------------------------------------
CPP=g++
SOURCE=lex.yy.c parser.tab.c 
MYPROGRAM=sclp
MYLIBRARY=$(CURDIR)
LDFLAGS=-Wl,-rpath=$(CURDIR)
FLEX=flex
ALLHEADERS=allheaders.h
BISON=bison
#-----------------------------------------------------------------------------

all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)
	$(CPP) -include $(ALLHEADERS) -L$(MYLIBRARY) $(LDFLAGS) $(SOURCE) -o $(MYPROGRAM) -lsclpshared -lfl -ly

lex.yy.c: scanner.l parser.tab.h
	$(FLEX) -l --yylineno scanner.l
parser.tab.c parser.tab.h : parser.y
	$(BISON) -d  parser.y

clean:
	rm -f $(MYPROGRAM)
