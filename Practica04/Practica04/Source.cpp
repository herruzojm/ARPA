#include <mpi.h>
#include <stdio.h>

#define DIMENSIONES 2
#define FILAS 3
#define COLUMNAS 5

void MostrarMatriz(int matriz[FILAS][COLUMNAS]);

int main(int argc, char *argv[])
{
	double tiempo;
	int tamaño_dimensiones[DIMENSIONES] = {FILAS, COLUMNAS};
	int periocidad[DIMENSIONES] = {0, 0};
	int coordenadas[DIMENSIONES];
	int rango, x, y, numero_elementos;
	int matriz1[FILAS][COLUMNAS]; 
	int matriz2[FILAS][COLUMNAS];
	MPI_Comm comunicador;
	
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango);

	MPI_Cart_create(MPI_COMM_WORLD, DIMENSIONES, tamaño_dimensiones, periocidad, 0, &comunicador);	
	MPI_Cart_coords(comunicador, rango, DIMENSIONES, coordenadas);
	x = coordenadas[0];
	y = coordenadas[1];
	numero_elementos = FILAS * COLUMNAS;

	if (rango == 0)
	{
		//inicializar matriz		
		for (int i = 0; i < FILAS; i++)
		{
			for (int j = 0; j < COLUMNAS; j++)
			{
				matriz1[i][j] = i + 1;
				matriz2[i][j] = matriz1[i][j] * 2;
			}
		}

		printf("Matriz Inicial 1: \n");
		MostrarMatriz(matriz1);
		printf("Matriz Inicial 2: \n");
		MostrarMatriz(matriz2);

		//temporizador		
		tiempo = -MPI_Wtime();
	}
	MPI_Bcast(matriz1, numero_elementos, MPI_INT, 0, comunicador);
	MPI_Bcast(matriz2, numero_elementos, MPI_INT, 0, comunicador);
	
	printf("Soy el proceso %2d y mis coordenadas son [%d][%d]. Voy a sumar %d y %d \n", rango, x, y, matriz1[x][y], matriz2[x][y]);
	fflush(stdout);
	matriz1[x][y] = matriz1[x][y] + matriz2[x][y];

	MPI_Gather(&matriz1[x][y], 1, MPI_INT, matriz1, 1, MPI_INT, 0, comunicador);
		
	

	if (rango == 0)
	{
		//mostrar resultado
		tiempo += MPI_Wtime();
		printf("Teimpo necesario %f \n\n", tiempo);
		printf("Matriz Resultante: \n");
		MostrarMatriz(matriz1);		
	}

	MPI_Finalize();	
	return 0;
}


void MostrarMatriz(int matriz[FILAS][COLUMNAS])
{
	for (int i = 0; i < FILAS; i++)
	{
		for (int j = 0; j < COLUMNAS; j++)
		{
			printf("[%3d] ", matriz[i][j]);
		}
		printf("\n");
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}
