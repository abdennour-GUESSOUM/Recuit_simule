#----- Fichier genere automatiquement sous Linux ------
#----- lun. 03 juil. 2023 09:53:10 CEST

#-----
INCDIR=.
CCFLAGS=-W -Wall -I$(INCDIR)
LDFLAGS=
DEPOPT=-MM
CCC=cc
#-----

#-------------  Appli  --------------

TARGET=recuitFonctions

FILESrecuitFonctions=recuitFonctions.c \
	random.c \
	gnuplot.c

LDFLAGSrecuitFonctions=$(LDFLAGS) -lm

OBJECTSrecuitFonctions=$(FILESrecuitFonctions:.c=.o)

#-------------  Appli  --------------

all: $(TARGET)

#-------------  Appli  --------------

recuitFonctions : $(OBJECTSrecuitFonctions)
	$(CCC) $(OBJECTSrecuitFonctions) \
		-o recuitFonctions $(LDFLAGSrecuitFonctions)
#------------------------------------

.c.o :
	$(CCC) $(CCFLAGS) -c $< \
		-o $*.o
 
dep :
	@echo "======== Mise a jour des dependances : .depend ========"
	@rm -f .depend
	@for i in *.c ; do \
	echo $$i ; \
	$(CCC) $(DEPOPT) $(CCFLAGS) $$i > .tmpdepend ; \
	OBJNAME=`echo $$i | sed -e s%\\\.c%.o% ` ; \
	cat .tmpdepend | \
	sed -e s%`basename $$i .c`\\\.o%$$OBJNAME%         >> .depend ; \
	echo  >> .depend ; \
	done
	@rm -f .tmpdepend
 
CLEANING=rm -f *.o core a.out $(TARGET) .depend
CONSEIL=echo Penser a faire : make -f makefile dep
 
clear :
	@$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo
clean :
	$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo

sinclude .depend

