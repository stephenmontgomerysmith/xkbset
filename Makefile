# The following are for XFree86 on FreeBSD
INC_PATH=-I/usr/X11R6/include
LIB_PATH=-L/usr/X11R6/lib
LIBS=-lX11
CC=cc ${INC_PATH} ${LIB_PATH} -Wall
INSTALL_BIN=/usr/X11R6/bin
INSTALL_MAN1=/usr/X11R6/man/man1

all: xkbset xkbset.1.gz

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

xkbset.1.gz: xkbset.man
	gzip -c xkbset.man > xkbset.1.gz

install: all
	install -c -s xkbset ${INSTALL_BIN}
	install -c mskacc-gui ${INSTALL_BIN}
	cp xkbset.1.gz ${INSTALL_MAN}/.

clean:
	rm -f *.o getargs.c print.c usage.c *.core xkbset config_usage xkbset.1.gz
