# The following are for XFree86 on FreeBSD
X11PREFIX=/usr/X11R6
X11BASE=/usr/X11R6

INSTALL_BIN=${X11PREFIX}/bin
INSTALL_MAN1=${X11PREFIX}/man/man1

INC_PATH=-I${X11BASE}/include
LIB_PATH=-L${X11BASE}/lib
LIBS=-lX11
CC=cc ${INC_PATH} ${LIB_PATH} -W -Wall

all: xkbset xkbset.1

xkbset: getargs.o print.o usage.o main.o misc.o
	${CC} getargs.o print.o usage.o main.o misc.o -o $@ ${LIBS}

getargs.o: getargs.c xkbset.h
	${CC} -c getargs.c

print.o: print.c xkbset.h
	${CC} -c print.c

usage.o: usage.c xkbset.h
	${CC} -c usage.c

main.o: main.c xkbset.h
	${CC} -c main.c

misc.o: misc.c xkbset.h
	${CC} -c misc.c

getargs.c: create_getargs config_getargs
	perl create_getargs

print.c: create_print config_print
	perl create_print

usage.c: create_usage config_usage
	perl create_usage

config_usage:
	rm -f config_usage
	ln -s config_getargs config_usage

xkbset.1: xkbset.man
	cp xkbset.man xkbset.1

install: all
	install -c -s xkbset ${INSTALL_BIN}
	install -c xkbset-gui ${INSTALL_BIN}
	cp xkbset.1 ${INSTALL_MAN1}/.

clean:
	rm -f *.o getargs.c print.c usage.c *.core xkbset config_usage xkbset.1
