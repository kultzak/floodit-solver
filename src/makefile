CFLAGS = -g -Wall -Wno-unused-result -O3 

all: floodit util heuristicas

util: geramapa verifica anima

heuristicas: floodit_h1 floodit_h2 floodit_h3 floodit_h4 floodit_h5

floodit: mapa.h mapa.c floodit.c -lmenu -lncurses libprogressbar.so #install "sudo apt install libncurses5-dev libncursesw5-dev"
#need to compile liprogressbar to use, or libstatusbar 
# floodit: -lmenu -lncurses floodit.c  #install "sudo apt install libncurses5-dev libncursesw5-dev"

floodit_h1: mapa.h mapa.c floodit_h1.c

floodit_h2: mapa.h mapa.c floodit_h2.c

floodit_h3: mapa.h mapa.c floodit_h3.c

floodit_h4: mapa.h mapa.c floodit_h4.c

floodit_h5: mapa.h mapa.c floodit_h5.c

geramapa: mapa.h mapa.c geramapa.c

verifica: mapa.h mapa.c verifica.c

anima: mapa.h mapa.c anima.c

clean:
	rm -f floodit geramapa verifica anima *.o
	rm -f floodit_h1
	rm -f floodit_h2
	rm -f floodit_h3
	rm -f floodit_h4
	rm -f floodit_h5
