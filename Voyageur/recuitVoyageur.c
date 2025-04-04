
#define VISUMEILLEUR 1  /* Si 1, visu de la meilleure solution connue    */
                        /* Si 0, visu en continu de la solution courante */
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "random.h"
#include "gnuplot.h"
#include "geo.h"

#include "params.h"  /* Pour NBVILLES et COTECARTE */

#if NBVILLES==8

 Carte carte={{{5,2},{7,3},{8,5},{7,7},{5,8},{3,7},{2,5},{3,3}}, 8};

#elif NBVILLES==16

 Carte carte={{{5,2},{6,2},{7,3},{8,4},
               {8,5},{8,6},{7,7},{6,8},
               {5,8},{4,8},{3,7},{2,6},
               {2,5},{2,4},{3,3},{4,2}}, 16};

#elif NBVILLES==30

 Carte carte={{{9,8},{3,3},{4,1},{1,9},{7,6},
               {2,1},{0,0},{3,5},{4,6},{5,9},
               {3,9},{0,4},{8,5},{2,6},{6,1},
               {7,8},{5,2},{3,6},{6,5},{1,8},
               {7,1},{7,0},{7,3},{1,1},{3,1},
               {5,1},{6,0},{8,4},{1,4},{1,6}}, 30};
#else
 Carte carte;
#endif

typedef struct { int parcours[NBVILLES];
                 int nbVilles;
               } Chemin;

/*--------------- Des fonctions de gestion de Chemins ----------------*/

void genereChemin(Chemin *chemin)
{
 int nbVillesGenerees=0;

 chemin->nbVilles=NBVILLES;

 while (nbVillesGenerees!=chemin->nbVilles)
 {
   int entier=myRandomMinMax(0,chemin->nbVilles-1);

   /* Il faut rechercher si l'entier est deja present dans le parcours */

   int i=0,trouve=0;
   for(i=0;i<nbVillesGenerees;i++)
   {
    if (chemin->parcours[i]==entier) trouve=1;
   }
   if (!trouve) chemin->parcours[nbVillesGenerees++]=entier;
 }
}
/*--*/
double calculCoutChemin(Chemin chemin)
{
 double cout=0.0;
 int    i=0;

 const Ville *villeInitiale,*villePrecedente,*villeCourante;

 cout=0.0;                 /* Si 1 seule ville, cout: 0.0 */
 if (chemin.nbVilles!=1)   
 {
  villeInitiale=&carte.villes[chemin.parcours[0]];
  villePrecedente=villeInitiale;

  for(i=1;i<chemin.nbVilles;i++)
  {
   villeCourante=&carte.villes[chemin.parcours[i]];

   cout+=distanceVilles(villePrecedente,villeCourante);

   villePrecedente=villeCourante;
  }

  /* Si 2 villes, distance premiere/derniere Ville deja calculee ... */
  if (chemin.nbVilles!=2)     
  {   
   cout+=distanceVilles(villePrecedente,villeInitiale);
  }
 }

 return cout;
}
void dessineChemin(FILE* flot, Chemin chemin)
{
 int    i=0;

 beginPointsToGnuplot(flot,"linespoint");

 for(i=0;i<chemin.nbVilles-1;i++)
 {
  vectorGnuplot(flot, carte.villes[chemin.parcours[i]].x,
                      carte.villes[chemin.parcours[i]].y,
                      carte.villes[chemin.parcours[i+1]].x,
                      carte.villes[chemin.parcours[i+1]].y);
 }

 vectorGnuplot(flot, carte.villes[chemin.parcours[chemin.nbVilles-1]].x,
                     carte.villes[chemin.parcours[chemin.nbVilles-1]].y,
                     carte.villes[chemin.parcours[0]].x,
                     carte.villes[chemin.parcours[0]].y);

 endPointsToGnuplot(flot);
}
/*--*/
/*----------------mise en place des différents type de transformation */
void echange(Chemin *cheminY)
{
    int i1 = myRandomMinMax(0,cheminY->nbVilles-1);
    int i2 = myRandomMinMax(0,cheminY->nbVilles-1);
    while(i1==i2){
      i2=myRandomMinMax(0,cheminY->nbVilles-1);
    }


    int temp;
    temp = cheminY->parcours[i1];
    cheminY->parcours[i1]=cheminY->parcours[i2];
    cheminY->parcours[i2]=temp;
}

void inversion(Chemin *cheminY)
{
  int i1 = myRandomMinMax(0,cheminY->nbVilles-1);
  int i2 = myRandomMinMax(0,cheminY->nbVilles-1);

  while (i1 == i2)
  {
    i2 = myRandomMinMax(0, cheminY->nbVilles - 1);
  }
  if (i1 > i2)
  {
    int tmp = i1;
    i1 = i2;
    i2 = tmp;
  }
  // [5][6][7][8] -> [8][7][6][5] random 1 ++ random2 --
  while (i1 < i2)
  {
    int temp = cheminY->parcours[i1];
    cheminY->parcours[i1] = cheminY->parcours[i2];
    cheminY->parcours[i2] = temp;
    i1++;
    i2--;
  }
}

void translation(Chemin *cheminY)
{
  int index1 = myRandomMinMax(0, cheminY->nbVilles-1);
  int index2 = myRandomMinMax(0, cheminY->nbVilles-1);
  int index3 = myRandomMinMax(0, cheminY->nbVilles-1);

  while (index1 == index2 || index2 == index3 || index1 == index3)
  {
    if (index1 == index2)
      index2 = myRandomMinMax(0, cheminY->nbVilles-1);
    else if (index2 == index3)
      index3 = myRandomMinMax(0, cheminY->nbVilles-1);
    else if (index1 == index3)
      index3 = myRandomMinMax(0, cheminY->nbVilles-1);
  }

  if (index1 > index2)
  {
    int tmp = index1;
    index1 = index2;
    index2 = tmp;
  }
  if (index2 > index3)
  {
    int tmp = index2;
    index2 = index3;
    index3 = tmp;
  }
  if (index1 > index2)
  {
    int tmp = index1;
    index1 = index2;
    index2 = tmp;
  }

  // À partir d'ici, index1 < index2 < index3
  // Sauvegarder le segment [index1, index2] dans un tableau temporaire
  int taillesegment = index2 - index1 + 1;
  int temptab[taillesegment];

  for (int i = 0; i < taillesegment; i++)
  {
    temptab[i] = cheminY->parcours[index1 + i];
  }

  // Déplacer les éléments entre index2+1 et index3 vers la position index1
  int taillesegment2 = index3 - index2;

  for (int i = 0; i < taillesegment2; i++)
  {
    cheminY->parcours[index1 + i] = cheminY->parcours[index2 + 1 + i];
  }

  // Insérer le segment1 (temptab) sauvegardé à la nouvelle position
  int position = index1 + taillesegment2;

  for (int i = 0; i < taillesegment; i++)
  {
    cheminY->parcours[position + i] = temptab[i];
  }

}

void transformationChemin(Chemin *cheminY,
                          Chemin  cheminX, int amplitude)
{
 *cheminY = cheminX; /* *cheminY est au voisinage de cheminX suivant     */
                     /*  l'amplitude. Apres *cheminY =  cheminX, ne plus */
                     /*  modifier cheminX !!!                            */
  int nbBoucle=myRandomMinMax(1,amplitude);
  for(int i = 0;i< nbBoucle ; i++){
    //echange(cheminY);
    //inversion(cheminY);
    translation(cheminY);
    
  }
  
    
}

/*--------------- Parametres de controle du recuit --------------------*/

double T;                 /* Temperature T        */
double Ti;                /* Temperature initiale */
double Tf;                /* Temperature finale   */

int    amplitude;         /* Parametre d'amplitude dans transformation() */
double alpha;             /* Facteur de decroissance de la temperature   */

int    NbEssais;          /* Nb total de mouvements essayes              */
int    MaxRepetitions;    /* Nb. max de repetition a temp. constante     */

FILE* fdCout;
char* fileNameCout="Cout";

FILE *fdResults;
char* fileNameResults="Resultats";

FILE* fdGnuplotCout;
FILE* fdGnuplotChemin;

/*_________  Choix de la Fonction d'Evaluation   _________________________*/
#define F(x)     f(x)     /* la fonction de cout (ci-dessous)             */ 
#define FNAME    "f"      /* indiquer aussi son libelle (pour impressions)*/

/*_________  Etats du Recuit  ____________________________________________*/

Chemin x0;               /* Etat initial      */
Chemin x;                /* Solution courante */
Chemin y;                /* Solution voisine  */
Chemin xopt;             /* Solution optimale */
double fx, fy, fxopt;    /* Valeurs */

/*________  Fonctions Exemples  (Fonction de co�t) _______________________*/
double f(Chemin chemin)
{
 return calculCoutChemin(chemin) ;
}

/*________  Voisinage (modification configuration)  ______________________*/
void transformation(void) {  /* y est au voisinage de x suivant amplitude */
  transformationChemin(&y,x,amplitude);                   /* Doonnneeee ! */
}

/*________  Modification temperature  ____________________________________*/
double g(void) {
#if 1
 return(T * alpha);       /* On decroit la temperature en utilisant T * alpha */
#else
 return(T-alpha);       /* On decroit la temperature en utilisant T - alpha */
#endif
}

/*------ visu du Cout -------- */
void visualiserCout(FILE *fd, char *fileName, int affichageObligatoire)
{
 static int i=0;

 char type[]="linespoints";

 if (i%10==0 || affichageObligatoire)
 {
#if 0 
  fprintf(fd,"plot \"%s\" with %s\n",fileName,type);
  fflush(fd);
#else

  FILE *fdFileName;

  fdFileName=fopen(fileName,"r");
  if (fdFileName==NULL) {
    perror("fopen (dans visualiserCout)");
    fprintf(fd,"plot \"%s\" with %s\n",fileName,type);
    fflush(fd);
  }
  else {

   long ind; double val;

   fprintf(fd,"plot '-' with %s\n",type);

   fscanf(fdFileName,"%ld %lf",&ind,&val);
   if (ferror(fdFileName)) perror("fscanf (dans visualiserCout)");

   while (!feof(fdFileName) && !ferror(fdFileName))
   {
    fprintf(fd,"%ld %f\n",ind,val);
    fflush(fd);
    fscanf(fdFileName,"%ld %lf",&ind,&val);
    if (ferror(fdFileName)) perror("fscanf (dans visualiserCout)");
   }

   fprintf(fd,"e\n");
   fflush(fd);

   fclose(fdFileName);

  }
#endif

 }
 i++;
}

void ecrireCout(FILE *fdCout, int abscisse, double cout)
{
 if (fdCout==NULL) return;

 fprintf(fdCout,"%d %f\n",abscisse,cout);
 fflush(fdCout);
}

/*------ Sauvegarde fichier Resultats -------- */
void PrintParameters(FILE *fd)
{
 if (fd==NULL) return;

 fprintf(fd,"---> Parametres\n");
 fprintf(fd,"%-15s %s\n"   ,"FNAME",FNAME);
 fprintf(fd,"%-15s %s\n","x0","...");
 fprintf(fd,"%-15s %-.2f\n","FNAME(x0)",F(x0));
 fprintf(fd,"%-15s %-.2f\n","Ti",Ti);
 fprintf(fd,"%-15s %-.2f\n","Tf",Tf);
 fprintf(fd,"%-15s %-.2f\n","alpha",alpha);
 fprintf(fd,"%-15s %d\n","amplitude",amplitude);
 fprintf(fd,"%-15s %d\n"   ,"MaxRepetitions",MaxRepetitions);

 fprintf(fd,"<---\n");
 fflush(fd);
}        

void PrintTitleLine(FILE *fd)
{
 int n=11;

 if (fd==NULL) return;

 fprintf(fd,"%-*s",n,"T");
 fprintf(fd,"%-*s",n,"NbEssais");
 fprintf(fd,"%-*s",n," x");
 fprintf(fd,"%-*s",n," f(x)");
 fprintf(fd,"%-*s",n," xopt");
 fprintf(fd,"%-*s",n," f(xopt)");

 fprintf(fd,"\n");
 fflush(fd);
}

void PrintALine(FILE *fd)
{
 int n=11;

 if (fd==NULL) return;

 fprintf(fd,"%-*.2f",n,T);
 fprintf(fd,"%-*d",n,NbEssais);
 fprintf(fd,"%-*s",n," ...");
 fprintf(fd,"%- *.2f",n,fx);
 fprintf(fd,"%-*s",n," ...");
 fprintf(fd,"%- *.2f",n,fxopt);
 fprintf(fd,"\n");

 fflush(fd);
}

void Visu(Chemin x, int affichageObligatoire)
{
 (void)x;
 visualiserCout(fdGnuplotCout,fileNameCout,affichageObligatoire);
 dessineChemin(fdGnuplotChemin,x);
}

void EcrireCoutEtVisu(double fx, Chemin x,int affichageObligatoire)
{
 (void)fx;(void)x;(void)affichageObligatoire;
 ecrireCout(fdCout,NbEssais,fx);
 Visu(x,affichageObligatoire);
}

void FermetureFlots(void)
{
 if (fdResults!=NULL) fclose(fdResults);

 fclose(fdCout);
 closeGnuplot(fdGnuplotCout);
 closeGnuplot(fdGnuplotChemin);
}

/*________  Initialisation  ___________________________________________*/
int main(void)
{
 char rep;

 initRandom();

#if NBVILLES!=8 && NBVILLES!=16 && NBVILLES!=30
 genereCarte(&carte,COTECARTE);
#endif

/*
 printf("f(x)=%s\n", FNAME );

 printf("Etat initial (x0) ?\n");
 scanf("%lf", &x0);
*/

 genereChemin(&x0);  /* Configuration initiale */

 printf("TInit ?\n");
 scanf("%lf",&Ti);

 printf("TFin ?\n");
 scanf("%lf",&Tf);

 printf("Alpha ?\n");
 scanf("%lf",&alpha);

 printf("Ampli (un int) ?\n");
 scanf("%d",&amplitude);

 printf("MaxRepetitions ?\n");
 scanf("%d",&MaxRepetitions);

 PrintParameters(stdout);

 do {
     printf("Sauvegarde des resultats dans un fichier? (o/n)\n");
     scanf("%c",&rep);
     while (rep=='\n') { scanf("%c",&rep); }
 }while (rep!='o' && rep!='n' && rep!='O' && rep!='N');

 if (rep=='n' || rep=='N') fdResults=NULL;
 else {
       fdResults= fopen(fileNameResults, "w");
       if (fdResults==NULL) {
         fprintf(stderr,
                 "Probleme sur fopen(\"%s\",\"w\")\n",fileNameResults);
       }
 }

 /*  si on veut avoir un en-tete d'identification */    
 PrintParameters(fdResults);
 PrintTitleLine(fdResults);   

 fdCout=fopen(fileNameCout,"w");  /* Ouverture du fichier pour les couts */
 if (fdCout==NULL) {
   fprintf(stderr,"Probleme sur fopen(\"%s\",\"w\")\n",fileNameCout);
   fprintf(stderr,"=> Arret du programme\n");
   fclose(fdResults);
   exit(EXIT_FAILURE);
 }

 fdGnuplotCout=openGnuplot(NULL);            /* pipe + fork pour visu    */
 if (fdGnuplotCout==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdResults);
   fclose(fdCout);
   exit(EXIT_FAILURE);
 }

 fdGnuplotChemin=openGnuplot(NULL);         /* pipe + fork pour visu    */
 if (fdGnuplotCout==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdResults);
   fclose(fdCout);
   closeGnuplot(fdGnuplotCout);
   exit(EXIT_FAILURE);
 }

/*__ Recuit Simule  ___________________________________________*/

    x        = xopt  = x0 ;         /* Configuration initiale */
    fx       = fxopt = f(x0) ;         /* Cout initial           */
     
    T        = Ti ;                 /* Temperature initiale   */
    NbEssais = 0;
 
    EcrireCoutEtVisu(fx,x,1);
    PrintALine(fdResults); /* Sauvegarde configuration initiale       */
   
    while (T>=Tf) {                            /* 1er critere d'arret */

       int    rep;    /* Nb de repetitions a temperature constante    */
       double p, Df;  /* p: pour tirage aleatoire, Df: pour Delta f   */

       for(rep=0; rep<MaxRepetitions; rep++){ /* 2eme critere d'arret */

          transformation();                   /* transformation => y, voisin de x */
          fy =F(y);

          Df =fy-fx;                      /* Nouveau - Ancien         */
          
          if (Df<0) {                    /* Descente !!              */
                x=y;                /* y devient l'etat courant */
                fx =fy;                                
                if (fx<fxopt){               /* Mise a jour optimum ?    */
                    xopt=x;
                    fxopt=fx;
#if VISUMEILLEUR==1
                    EcrireCoutEtVisu(fxopt,xopt,1);
#endif
                }                
          } 
          else {                         /* Remontee : acceptee ??   */
		 p = myRandom01() ;
                 if (p<exp(-(Df/T))) {
                    x=y;           /* y devient l'etat courant */
                    fx=fy;                
                 }                
          }

          NbEssais++;

#if VISUMEILLEUR!=1
          EcrireCoutEtVisu(fx,x,0);
#endif
          PrintALine(fdResults);   /* Sauvegarde resulats courants */
       } 
        
       T =g();                        /* modifier la temperature */

       usleep(10);
    } /* end while */

 printf("-------------------->\n");
 printf("Temperature a la fin de l'algorithme=%f\n",T);
 printf("Cout optimal (fxopt)=%f\n",fxopt);
 printf("<--------------------\n");
 Visu(xopt,1);

 FermetureFlots();

 exit(EXIT_SUCCESS);
}