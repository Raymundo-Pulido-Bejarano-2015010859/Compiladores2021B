PROY=MArbol.out
DSTPATH=/usr/local/bin
CFLAGS=-Wall
COBJ=-c
LINKF=-lm 
CC=gcc
ORIGINFILES=MArbol.o ./Stack/Stack.c ./ArbolBin/TreeBin.c ./RegexToPostfix/RegexPost.c ./StackTreeBin/Stack.c ./AFD/AFD.c ./TabSig/TabSig.c
OBJECTFILES=$(ORIGINFILES:.c=.o)
DIREC=./Stack/*.o ./ArbolBin/*.o ./RegexToPostfix/*.o ./StackTreeBin/*.o ./AFD/*.o  ./TabSig/*.o
all:${PROY}

# % es cualquier archivo en la carpeta
# $< regresa la primer dependencia
%{OBJECTFILES}:${ORIGINFILES}
	${CC} ${COBJ} $< ${CFLAGS}

# $^ Regresa la lista completa de dependencias completa
# $@ Regresa el nombre de la etiqueta
${PROY}:${OBJECTFILES}
	${CC} $^ -o $@ ${LINKF}


.PHONY:clean

install:all
	@if [ ! -d ${DSTPATH} ]; then\
		sudo mkdir ${DSTPATH}
	fi
	@sudo cp ${PROY} ${DSTPATH}
uninstall:
	@sudo rm ${DSTPATH}/${PROY}
clean:
	@rm -f *.o  ${PROY} ${DIREC}
	
	
