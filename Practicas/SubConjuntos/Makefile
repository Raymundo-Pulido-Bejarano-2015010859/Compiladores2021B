PROY=Subconjuntos.out
DSTPATH=/usr/local/bin
CFLAGS=-Wall
COBJ=-c
LINKF=-lm 
CC=gcc
ORIGINFILES=$(wildcard *.c) ./AFN/AFN.c ./Lista/List.c ./EstadosSubC/EstadosInt.c
OBJECTFILES=$(ORIGINFILES:.c=.o)

all:${PROY}

# % es cualquier archivo en la carpeta
# $< regresa la primer dependencia
%{OBJECTFILES}:${ORIGINFILES}
	${CC} ${COBJ} $< ${CFLAGS}

# $^ Regresa la lista completa de dependencias completa
# $@ Regresa el nombre de la etiqueta
${PROY}:${OBJECTFILES}
	${CC} $^ -o $@ ${LINKF}
	@mkdir -p AFD_Format
	@mkdir -p AFD_PNG



.PHONY:clean

install:all
	@if [ ! -d ${DSTPATH} ]; then\
		sudo mkdir ${DSTPATH}
	fi
	@sudo cp ${PROY} ${DSTPATH}
uninstall:
	@sudo rm ${DSTPATH}/${PROY}
clean:
	@rm -f *.o ${PROY} ${OBJECTFILES}
	@mkdir -p AFD_Format
	@mkdir -p AFD_PNG
	@rm -r AFD_Format
	@rm -r AFD_PNG
	@rm AFN_Recibido.txt	
