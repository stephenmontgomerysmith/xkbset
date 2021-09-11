X11PREFIX=/usr/local
X11BASE=/usr/local

INSTALL_BIN=${DESTDIR}${X11PREFIX}/bin
INSTALL_MAN1=${DESTDIR}${X11PREFIX}/man/man1

INC_PATH=-I${X11BASE}/include
LIB_PATH=-L${X11BASE}/lib
LIBS=-lX11

all: xkbset xkbset.1

xkbset: getargs.o print.o usage.o main.o misc.o
	${CC} ${CFLAGS} ${LDFLAGS} ${INC_PATH} ${LIB_PATH} getargs.o print.o usage.o main.o misc.o -o $@ ${LIBS}

getargs.o: getargs.c xkbset.h
	${CC} ${CPPFLAGS} ${CFLAGS} ${INC_PATH} -c getargs.c

print.o: print.c xkbset.h
	${CC} ${CPPFLAGS} ${CFLAGS} ${INC_PATH} -c print.c

usage.o: usage.c xkbset.h
	${CC} ${CPPFLAGS} ${CFLAGS} ${INC_PATH} -c usage.c

main.o: main.c xkbset.h
	${CC} ${CPPFLAGS} ${CFLAGS} ${INC_PATH} -c main.c

misc.o: misc.c xkbset.h
	${CC} ${CPPFLAGS} ${CFLAGS} ${INC_PATH} -c misc.c

getargs.c: create_getargs config_getargs
	perl create_getargs

print.c: create_print config_print
	perl create_print

usage.c: create_usage config_usage
	perl create_usage

config_usage: config_getargs
	rm -f config_usage
	ln -s config_getargs config_usage

xkbset.1: xkbset.man
	cp xkbset.man xkbset.1

INSTALL=	install -c
INSTALL_DATA=	${INSTALL}
INSTALL_SCRIPT=	${INSTALL}
INSTALL_PROGRAM=${INSTALL} -s

install: all
	${INSTALL_PROGRAM} xkbset ${INSTALL_BIN}
	${INSTALL_SCRIPT} xkbset-gui ${INSTALL_BIN}
	${INSTALL_DATA} xkbset.1 ${INSTALL_MAN1}

clean:
	rm -f *.o getargs.c print.c usage.c *.core xkbset config_usage xkbset.1
