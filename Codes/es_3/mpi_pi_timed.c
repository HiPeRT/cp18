#include <mpi.h> 
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
  double PI_25 = 3.141592653589793238462643;
  int id, p;
  double A,B, time_elapsed;
  int n;
  //Inizializzo la libreria MPI
  MPI_Init(&argc, &argv);
  //Richiedo il numero totale di processor elements
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  //Richiedo il mio ID tra questi
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  
  //AVVIO IL CRONOMETRO
  time_elapsed = MPI_Wtime();
  
  if (id == 0)
  {
    printf("Inserisci il numero di intervalli: ");
    scanf("%d",&n);
  }
  
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  double h = 1.0/(double)n;
  
  B=0.0;
  
  int i;
  
  int n_loc = n / p;
  
  int start = id*n_loc;
  int end = (id+1)*n_loc;
  
  for (i =start; i<end; i++)
    B = B + 4.0/(1.0 + ((double)i*h)*((double)i*h));
  
  B=B*h;
  
  MPI_Reduce(&B, &A, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  //FERMO IL CRONOMETRO
  time_elapsed = MPI_Wtime() - time_elapsed;
  
  if (id==0)
  {
    printf("Il valore di pi-greco calcolato: %g\n", A);
    printf("Precisione: %g\n", A-PI_25);
    printf("tempo totale: %lf\n", time_elapsed);
  }
  
  //Chiudo la libreria
  MPI_Finalize();
  return 0;
}
