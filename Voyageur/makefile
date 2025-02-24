# Fichier makefile pour ais
#--------------------------

CC=cc

TARGET=recuitVoyageur
FILES=recuitVoyageur.c geo.c random.c gnuplot.c
OBJECTS=$(FILES:.c=.o)
CLEANING=rm -f $(OBJECTS) $(TARGET) core a.out

#-------------------------------------------------------------

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lm

#-------------------------------------------------------------

recuitVoyageur.o: params.h recuitVoyageur.c
	$(CC) -c -W -Wall recuitVoyageur.c -o recuitVoyageur.o

geo.o: params.h geo.c
	$(CC) -c -W -Wall geo.c -o geo.o

#-------------------------------------------------------------

.c.o :
	$(CC) -c -W -Wall $< -o $*.o

clean:
	$(CLEANING)
clear:
	$(CLEANING)
