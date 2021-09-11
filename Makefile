# Use Gnu Make

# The following are for XFree86 on FreeBSD
INC_PATH=-I/usr/X11R6/include
LIB_PATH=-L/usr/X11R6/lib
LIBS=-lX11
CC=cc ${INC_PATH} ${LIB_PATH} -Wall


CREATEDOBJ=getargs.o print.o usage.o
CREATEDSRC=${CREATEDOBJ:.o=.c}
OBJ=${CREATEDOBJ} main.o misc.o

all: xkbset man

xkbset: ${OBJ}
	${CC} ${OBJ} -o $@ ${LIBS}

${OBJ}: %.o : %.c xkbset.h
	${CC} -c $<

${CREATEDSRC}: %.c : create_% config_%
	perl create_${@:.c=}

config_usage:
	rm -f config_usage
	ln -s config_getargs config_usage

man:
	gzip -c xkbset.man > xkbset.1.gz

install: all
	install -c -s xkbset /usr/local/bin
	install -c mskacc-gui /usr/local/bin
	cp xkbset.1.gz /usr/X11R6/man/man1/.

clean:
	rm -f ${OBJ} ${CREATEDSRC} *.core xkbset config_usage xkbset.1.gz
