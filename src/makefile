CFLAGS = -g -Wall -Wno-unused-result -O3 

all: floodit util

util: geramapa verifica anima

# heuristicas: floodit_ga

#need to compile liprogressbar to use, or libstatusbar 
# floodit: -lmenu -lncurses floodit.c  #install "sudo apt install libncurses5-dev libncursesw5-dev"
floodit: mapa.h mapa.c floodit_ga.h floodit_ga.c floodit.c -lmenu -lncurses libprogressbar.so #install "sudo apt install libncurses5-dev libncursesw5-dev"

# floodit_ga: mapa.h mapa.c floodit_ga.c

geramapa: mapa.h mapa.c geramapa.c

verifica: mapa.h mapa.c verifica.c

anima: mapa.h mapa.c anima.c

clean:
	rm -f floodit geramapa verifica anima *.o
	rm -f floodit_ga
