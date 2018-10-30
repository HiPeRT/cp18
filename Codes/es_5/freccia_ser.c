#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// prototipi delle funzioni

void riempiMat (int, double *, int N);
void decLU (double *, double *m, int N);
void avanti (double *, double *, double *, int N);
void indietro (double *, double *, double *, int N);
void prodottoMM (double *, double *, double *m, int N);
void prodottoMV (double *, double *, double *, int N);
void copia_matr (double *, double *, int N);
void copia_vett (double *, double *, int N);


/*funzione che crea le matrici A, le vengono passati un indice k e il puntatore alla matrice da riempire; 
 *restituisce la matrice creata*/

void riempiMat (int k, double *A, int N){
  int i, j;
  srand(k);
  for (i=0; i<N; i++)
  {
    for (j=0; j<N; j++)
      A[i*N+j] = 1.0 / (double)rand();
    A[i*N+i] += 1.0 * N;
  } 
}


/*funzione per la decomposizione LU, le vengono passati i puntatori a due matrici, 
 *la prima è quella che diventerà la matrice U, mentre la seconda è una matrice vuota che diventerà la matrice L*/

void decLU (double *a, double *L, int N){
  int i, j, k;
  double m;
  for (i=0; i<N-1; i++)
    for (j=i+1; j<N; j++){
      m = a[j*N+i]/a[i*N+i];
      L[j*N+i] = m;
      for (k=i; k<N; k++)
	a[j*N+k] = a[j*N+k]-a[i*N+k]*m;
    }
    for (i=0; i<N; i++)
      for (j=0; j<N; j++)
	if (i==j) L[i*N+j] = 1;
	else if (i<j) L[i*N+j] = 0;
}

/*funzione per la risoluzione di sistemi in cui compare una matrice triangolare
 * inferiore (eliminazione in avanti), le vengono passati i puntatori alla matrice triangolare, 
 * al vettore noto e la vettore che costituirà la soluzione*/

void avanti (double *L, double *b, double *y, int N){
  int i, j;
  y[0] = b[0]/L[0];
  for (i=1; i<N; i++){
    y[i] = b[i];
    for (j=0; j<=i-1; j++)
      y[i] = y[i]-L[i*N+j]*y[j];
    y[i] = y[i]/L[i*N+i];}
}


/*funzione per la risoluzione di sistemi in cui compare una matrice triangolare superiore 
 * (eliminazione all'indietro), le vengono passati i puntatori alla matrice triangolare, 
 * al vettore noto e la vettore che costituirà la soluzione*/

void indietro (double *U, double *y, double *x, int N){
  int i, j;
  x[N-1] = y[N-1]/U[(N-1)*N+N-1];
  for (i=N-2; i>=0; i--){
    x[i] = y[i];
    for (j=i+1; j<N; j++)
      x[i] = x[i]-U[i*N+j]*x[j];
    x[i] = x[i]/U[i*N+i];}
}


/*funzione per il prodotto tra due matrici, le vengono passati i puntatori alle due matrici 
 *e a quella che costituirà la soluzione*/

void prodottoMM(double *c, double *Z, double *D, int N){
  int i, j, k;
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      D[i*N+j]=0;
    for(i=0; i<N; i++)
      for(j=0; j<N; j++)
	for(k=0; k<N; k++)
	  D[i*N+j]+=c[i*N+k]*Z[k*N+j];  
}

/*funzione per il prodotto tra matrice e vettore, le vengono passati i puntatori alla matrice
 *e ai due vettori*/
void prodottoMV(double *A, double *b, double *c, int N){
  int i, j;
    for(i=0; i<N; i++)
      for(j=0; j<N; j++)
	  c[i]+=A[i*N+j]*b[j];
}

	  
	  
/*funzione che copia una matrice all'interno di un'altra, le vengono passati i puntatori alle due matrici*/ 

void copia_matr(double *a, double *b, int N){
  int i, j;
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      a[i*N+j] = b[i*N+j];
}
	  
	  
/*funzione che copia un vettore all'interno di un altro, le vengono passati 
  *i puntatori ai due vettori*/ 

void copia_vett(double *a, double *b, int N){
  int i;
  for(i=0; i<N; i++)
    a[i] = b[i];
}
	  
	  
/*programma principale*/

int main(int argc, char *argv[]){
double **A, **B, **C, *bb, *a, *b, *c,*AAA, *bb1, *y, *k, *Z, *z, 
       *HH, *hh, *Acap, *As, *bcap, *X, *XX, *Xs, *L, *cc, *bs, *D, *d, *xx,
       *bb_t, **bb_es;
int i, j, block, N, np;
clock_t start, diff;

N = atoi(argv[1]);
np = atoi(argv[2]);


  //TIMING
  start = clock();

/* alloco lo spazio per il puntatore alle matrici A[i], 
  *per le matrici A[i], B[i], C[i] e le creo*/
//Sono tutti i blocchi della diagonale. np piu' la s
a =(double *)malloc(N*N*(np+1)*sizeof(double));
//Sono solo i puntatori alle singole matrici.
A =(double **)malloc((np+1)*sizeof(double*));
b =(double *)malloc(N*N*(np)*sizeof(double));
B =(double **)malloc((np)*sizeof(double*));
c =(double *)malloc(N*N*(np)*sizeof(double));
C =(double **)malloc((np)*sizeof(double*));
bs =(double *)malloc(N*(np+1)*sizeof(double));
memset(bs, 0, N*(np+1)*sizeof(double));
bb_es =(double **)malloc((np+1)*sizeof(double*));
bb1 = (double *)malloc((np+1)*N*sizeof(double));
for (i=0; i<(np+1)*N; i++)
  bb1[i] = 1.;
for (i=0; i<np; i++)
{
  A[i] = &(a[i*N*N]);
  riempiMat(i+1, A[i], N);
  B[i] = &(b[i*N*N]);
  riempiMat(i+2, B[i], N);
  C[i] = &(c[i*N*N]);
  riempiMat(i+3, C[i], N);
  bb_es[i] = &(bs[i*N]);
  prodottoMV(A[i],bb1, bb_es[i], N);
  prodottoMV(B[i],bb1, bb_es[i], N);
}


/*inoltre alloco lo spazio e creo la matrice A[np] e il vettore di uno bb1*/

A[np] = &(a[np*N*N]);
riempiMat(np+1, A[np], N);
bb_es[np]=&(bs[np*N]);

for (i=0; i<np; i++)
  prodottoMV(C[i],bb1, bb_es[np], N);

prodottoMV(A[np],bb1, bb_es[np], N);

/*copio la matrice A[0] all'interno della matrice a, la matrice B[0] in b, 
  *la matrice C[0] in c, il vettore bb1 in bb e alloco lo spazio per una matrice HH e un vettore h*/
/*allocano spazio per le matrici Y e Z e per i vettori y, k e z*/
/*risolvono i sistemi LY=B, aZ=Y con i metodi di eliminazione in avanti e all'indietro*/
a = (double *)malloc(N*N*sizeof(double));
b = (double *)malloc(N*N*sizeof(double));
c = (double *)malloc(N*N*sizeof(double));
bb = (double *)malloc(N*sizeof(double));
bb_t =(double *)malloc(N*(np+1)*sizeof(double));
AAA = (double *)malloc(N*N*sizeof(double));
HH = (double *)malloc(N*N*sizeof(double));
hh = (double *)malloc(N*sizeof(double));
L = (double *)malloc(N*N*sizeof(double));
y = (double *)malloc(N*sizeof(double));
k = (double *)malloc(N*sizeof(double));
//A^-1 * B
Z = (double *)malloc(N*N*sizeof(double));
z = (double *)malloc(N*sizeof(double));
D = (double *)malloc(N*N*sizeof(double));
d = (double *)malloc(N*sizeof(double));
memset(HH, 0, N*N*sizeof(double));
memset(hh, 0, N*sizeof(double));
//copia_vett(bb, bb1);
for (block=0; block<np; block++)
{
copia_matr(a, A[block], N);
copia_matr(b, B[block], N);
copia_matr(c, C[block], N);
copia_vett(bb, bb_es[block], N);

decLU(a,L, N);

for(i=0; i<N; i++)
{
  for (j=0; j<N; j++)
    k[j] = b[j*N+i];
    avanti(L, k, y, N);
    indietro(a, y, z, N);
    for (j=0; j<N; j++)
      Z[j*N+i] = z[j];
  
}

/*risolvono i sistemi Ly=bb e az=y*/

avanti(L, bb, y, N);
indietro(a, y, z, N);

/*allocano spazio per una matrice D e un vettore d e svolgono il prodotto 
*matrice per matrice tra c e Z e inseriscono il risultato in D*/    

prodottoMM(c, Z, D, N);
    
/*riempiono il vettore d di 0 e svolgono il prodotto matrice-vettore tra C e z inserendolo nel vettore d*/

memset(d, 0, N*sizeof(double));
  
prodottoMV(c ,z, d, N);
    
/*viene inviata tramite una reduce la matrice D da ogni processore al processore 0,
* che inserisce dentro alla matrice HH la loro somma, la stessa cosa per il vettore d*/

for(i=0; i<N; i++)
{
  for (j=0; j<N; j++)
    HH[i*N+j]+=D[i*N+j];
  hh[i]+=d[i];
}

copia_vett(&(bb_t[N*block]), bb, N);

}

/*allocano lo spazio per un vattore Xs*/
Xs =  (double *)malloc(N*sizeof(double));

/*se sono il processore 0 alloco spazio per le matrici As, Acap e per i vettori bs, bcap */

As = (double *)malloc(N*N*sizeof(double));
bs = (double *)malloc(N*sizeof(double));
Acap = (double *)malloc(N*N*sizeof(double));
bcap = (double *)malloc(N*sizeof(double));

/*copio la matrice A[nproc] in As e b[nproc] in bs*/

copia_matr(As, A[np], N);
copia_vett(bs, bb_es[np], N);
      
/*calcolo la matrice Acap e il vettore bcap*/

for(i=0; i<N; i++)
  for(j=0; j<N; j++)
    Acap[i*N+j] = As[i*N+j] - HH[i*N+j];
  for(i=0; i<N; i++)
    bcap[i] = bs[i] - hh[i];

/*svolgo la decomposizione LU della matrice Acap e uso i metodi dell'eliminazione 
  *in avanti e all'indietro per risolvere l'equazione Acap x = bcap*/

decLU(Acap, L, N);
avanti(L, bcap, y, N);
indietro(Acap, y, Xs, N);
    
/*tutti i processori(compreso lo 0) ricevono il vettore Xs, allocano spazio per un 
  * vettore X, vi inseriscono il prodotto matrice-vettore tra b e Xs, allocano lo spazio
  * per il vettore cc e lo riempiono*/
    
    
X = (double *)malloc(N*sizeof(double));
XX = (double *)malloc(N*(np+1)*sizeof(double));
cc = (double *)malloc((np+1)*N*sizeof(double));    
xx = (double *)malloc(N*sizeof(double));
memset(X, 0, (np+1)*N*sizeof(double));

for (block=0; block<np; block++)
{
	copia_matr(b, B[block], N);  
	for(i=0; i<N; i++)
	  for(j=0; j<N; j++)
	    X[i]+=b[i*N+j]*Xs[j];
	  

	
	for (i=0; i<N; i++) cc[i+N*block] = bb_t[i+N*block] - X[i];
	
	/*decomposizione LU e metodi dell'eliminazione in avanti e all'indietro per la 
	*risoluzione del sistema axx = cc*/
	
	
	copia_matr(AAA, A[0], N);	
	decLU(AAA, L, N);
	avanti(L, &(cc[N*block]), y, N);
	indietro(AAA, y, xx, N);

	/* se sono il processore 0 copio nel vettore XX la mia soluzione xx, ricevo le 
	*soluzioni dagli altri processori e le inserisco all'interno del vettore XX; infine vi copio il vettore Xs*/
        copia_vett(&(XX[N*block]), xx, N);
}
  for(i=0; i<N; i++) XX[np*N+i] = Xs[i];

  
     //TIMING
   diff = clock() - start;
  
     int msec = diff * 1000 / CLOCKS_PER_SEC;
   printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
  
	/*stampa il risultato e dealloca tutta la memoria*/
// 	for (i=0; i<N*(np+1); i++) {
// 	  printf( " X = %lf  ", XX[i]);
//   if((i+1)%N==0)
//     printf("\n");
// 	}
// 	free (a);
// 	free (b);
// 	free (c);
// 	free (A);
// 	free (B);
// 	free (C);
// 	free (Acap);
// 	free (As);
// 	free (bcap);
// 	free (bs);
// 	free (XX);
// 	free (HH);
// 	free (hh);
// 	free (bb);
// 	free (bb_t);
// 	free (AAA);
// 	free (bb1);
// 	free (y);
// 	free (k);
// 	free (Z);
// 	free (z);
// 	free (X);
// 	free (Xs);
// 	free (L);
// 	free (cc);
// 	free (D);
// 	free (d);
}
