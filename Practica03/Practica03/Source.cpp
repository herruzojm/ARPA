#include <mpi.h>
#include <stdio.h>

#define NUMERO_ELEMENTOS 4

int main(int argc, char *argv[])
{
	int mirango, x, y, resultado;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);

	resultado = 0;
	int vector1[NUMERO_ELEMENTOS];
	int vector2[NUMERO_ELEMENTOS];

	if (mirango == 0)
	{
		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
		{
			vector1[i] = i + 1;
			vector2[i] = i * 2;
			resultado += vector1[i] * vector2[i];
		}		
	}
	printf("Enviando datos \n");
	fflush(stdout);
	MPI_Scatter(vector1, 1, MPI_INT, &x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Enviado x con valor %d \n", x);
	fflush(stdout);
	MPI_Scatter(vector2, 1, MPI_INT, &y, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//y = 1;
	printf("Enviado y con valor %d \n", y);
	fflush(stdout);
	x = x*y;
	MPI_Reduce(&x, &y, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (mirango == 0)
	{
		printf("El resultado deberia ser %d y es %d \n", resultado, y);
		fflush(stdout);
	}
	
	MPI_Finalize();	
	return 0;
}
