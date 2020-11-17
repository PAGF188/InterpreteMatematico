CC= gcc -Wall
HEADER_FILES_DIR = ./headerFiles
INCLUDES = -I $(HEADER_FILES_DIR)
MAIN= matematica
SRCS = Errores.c sintacticoSemantico.tab.c main.c 
DEPS = $(HEADER_FILES_DIR)/Definiciones.h $(HEADER_FILES_DIR)/Errores.h
OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	#compilacion
	$(CC) -lm -o $(MAIN) $(OBJS)

precompila:
	#precompilacion
	flex lexico.l
	bison -d sintacticoSemantico.y
	mv sintacticoSemantico.tab.h ./headerFiles/Definiciones.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(INCLUDES)

cleanall: clean
	rm -f $(MAIN)

clean:
	rm -f *.o *~
	rm lex.yy.c
	rm $(HEADER_FILES_DIR)/Definiciones.h
	rm sintacticoSemantico.tab.c
