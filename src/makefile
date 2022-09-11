SRC=lib
INCLUDE=include
CFLAGS = -I$(INCLUDE) -Wimplicit-function-declaration -Wall -Wno-unused-result -O3 
CFLAGS_DEBUG = -g -O0
LDLIBS = -lmenu -lncurses

all: floodit #util

util: geramapa verifica anima

# heuristicas: floodit_ga

#need to compile liprogressbar to use, or libstatusbar 
# floodit: -lmenu -lncurses floodit.c  #install "sudo apt install libncurses5-dev libncursesw5-dev"
floodit: floodit.o libprogressbar.so mapa.o floodit_ga.o #install "sudo apt install libncurses5-dev libncursesw5-dev"

%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

floodit.o: floodit.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o floodit.o

# floodit_ga: lib/mapa.h mapa.c floodit_ga.c

geramapa: lib/mapa.h mapa.c geramapa.c

verifica: lib/mapa.h mapa.c verifica.c

anima: lib/mapa.h mapa.c anima.c

clean:
	rm -f floodit geramapa verifica anima *.o
	rm -f floodit_ga
