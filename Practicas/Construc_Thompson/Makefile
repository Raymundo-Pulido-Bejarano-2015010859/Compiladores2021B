PROY=ConsThomp.out
DSTPATH=/usr/local/bin
CFLAGS=-Wall
COBJ=-c
LINKF=-lm 
CC=gcc
ORIGINFILES=ConsThomp.c ./Stack/Stack.c ./ArbolBin/TreeBin.c ./RegexToPostfix/RegexPost.c ./StackTreeBin/Stack.c ./AFN/AFN.c
OBJECTFILES=$(ORIGINFILES:.c=.o)
DIREC=./Stack/*.o ./ArbolBin/*.o ./RegexToPostfix/*.o ./StackTreeBin/*.o ./AFN/*.o
all:${PROY}

# % es cualquier archivo en la carpeta
# $< regresa la primer dependencia
%{OBJECTFILES}:${ORIGINFILES}
	${CC} ${COBJ} $< ${CFLAGS}

# $^ Regresa la lista completa de dependencias completa
# $@ Regresa el nombre de la etiqueta
${PROY}:${OBJECTFILES}
	${CC} $^ -o $@ ${LINKF}
	@mkdir -p AFN_PNG
	@mkdir -p AFN_FORMAT
	@cp Makefile_dot ./AFN_PNG/Makefile


.PHONY:clean

install:all
	@if [ ! -d ${DSTPATH} ]; then\
		sudo mkdir ${DSTPATH}
	fi
	@sudo cp ${PROY} ${DSTPATH}
uninstall:
	@sudo rm ${DSTPATH}/${PROY}
clean:
	@rm -f *.o *.gv *.png AFN*.txt ${PROY} ${DIREC}
	@mkdir -p AFN_PNG
	@mkdir -p AFN_FORMAT
	@rm -r AFN_PNG
	@rm -r AFN_FORMAT
	
