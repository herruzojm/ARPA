
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMERO_ELEMENTOS 15

void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS]);
void MostrarColumna(int* columna);
void MostrarResultado(int* columna);

int main(int argc, char *argv[])
{
	int mirango;
	double tiempo;
	int matriz[2][NUMERO_ELEMENTOS][NUMERO_ELEMENTOS];
	int matrizResultado[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);

	if (mirango == 0)
	{
		//inicializar matriz		
		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
		{
			for (int j = 0; j < NUMERO_ELEMENTOS; j++)
			{
				matriz[0][i][j] = i + 1;
				matriz[1][i][j] = matriz[0][i][j] * 2;
			}
		}

		printf("Matriz Inicial 1: \n");
		MostrarMatriz(matriz[0]);
		printf("Matriz Inicial 2: \n");
		MostrarMatriz(matriz[1]);

		//temporizador		
		tiempo = -MPI_Wtime();		
	}
	//distribuir y recibir matriz
	MPI_Bcast(matriz, NUMERO_ELEMENTOS * NUMERO_ELEMENTOS * 2, MPI_INT, 0, MPI_COMM_WORLD);
	

	//sumar columna
	int columna[NUMERO_ELEMENTOS];
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		columna[i] = matriz[0][i][mirango] + matriz[1][i][mirango];
	}

	//enviar y recibir matriz
	MPI_Gather(columna, NUMERO_ELEMENTOS, MPI_INT, matrizResultado, NUMERO_ELEMENTOS, MPI_INT, 0, MPI_COMM_WORLD);

	if (mirango == 0)
	{
		//mostrar resultado
		tiempo += MPI_Wtime();
		printf("Teimpo necesario %f \n\n", tiempo);
		printf("Matriz Resultante: \n");
		MostrarMatriz(matrizResultado);		
	}

	MPI_Finalize();
	return 0;
}

void MostrarColumna(int* columna)
{
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		printf("[%3d] ", columna[i]);
	}
	printf("\n");
	fflush(stdout);
}


void MostrarResultado(int* columna)
{
	for (int i = 0; i < NUMERO_ELEMENTOS*NUMERO_ELEMENTOS; i++)
	{
		printf("[%3d] ", columna[i]);
	}
	printf("\n");
	fflush(stdout);
}

void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS])
{
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		for (int j = 0; j < NUMERO_ELEMENTOS; j++)
		{
			printf("[%3d] ", matriz[i][j]);
		}
		printf("\n");
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}
