#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMERO_ELEMENTOS 10
#define NUMERO_PROCESOS 3

void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS]);

void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS])
{
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		for (int j = 0; j < NUMERO_ELEMENTOS; j++)
		{
			printf("[%3d] ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
}

int main(int argc, char** argv)

{

	int mirango, tag = 0;
	int vectorLongitudBloque[NUMERO_ELEMENTOS];
	int vectorDesplazamientoBloque[NUMERO_ELEMENTOS];
	int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS];
	MPI_Datatype matrizTriangularSuperior, matrizTriangularInferior;
	MPI_Request peticion;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);

	//creamos tipo matriz triangular superior
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		vectorLongitudBloque[i] = NUMERO_ELEMENTOS - i;
		vectorDesplazamientoBloque[i] = i * NUMERO_ELEMENTOS + i;
	}
	MPI_Type_indexed(NUMERO_ELEMENTOS, vectorLongitudBloque, vectorDesplazamientoBloque, MPI_INT, &matrizTriangularSuperior);
	MPI_Type_commit(&matrizTriangularSuperior);

	//creamos tipo matriz triangular inferior
	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
	{
		vectorLongitudBloque[i] = i + 1;
		vectorDesplazamientoBloque[i] = (NUMERO_ELEMENTOS * i);
	}
	MPI_Type_indexed(NUMERO_ELEMENTOS, vectorLongitudBloque, vectorDesplazamientoBloque, MPI_INT, &matrizTriangularInferior);
	MPI_Type_commit(&matrizTriangularInferior);

	if (mirango == 0)
	{
		//iniciamos la matriz con valores aleatorios
		srand((int)time(NULL));
		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
		{
			for (int j = 0; j < NUMERO_ELEMENTOS; j++)
			{
				matriz[i][j] = rand() % 11;
			}
		}
		printf("Matriz aleatoria original\n");
		MostrarMatriz(matriz);
		//enviamos la matriz superior al proceso 1
		MPI_Isend(matriz, 1, matrizTriangularSuperior, 1, tag, MPI_COMM_WORLD, &peticion);
		//y la matriz inferior al proceso 2
		MPI_Isend(matriz, 1, matrizTriangularInferior, 2, tag, MPI_COMM_WORLD, &peticion);
	}

	else
	{
		//inciamos la matriz con ceros
		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
		{
			for (int j = 0; j < NUMERO_ELEMENTOS; j++)
			{
				matriz[i][j] = 0;
			}
		}
		printf("Matriz original del proceso %d\n", mirango);
		MostrarMatriz(matriz);

		//elegimos que tipo de dato recibimos segun el proceso que seamos
		MPI_Datatype tipoRecibido;
		if (mirango == 1)
		{
			tipoRecibido = matrizTriangularSuperior;
		}
		else
		{
			tipoRecibido = matrizTriangularInferior;
		}
		//recibimos los datos y los mostramos
		MPI_Recv(matriz, 1, tipoRecibido, 0, tag, MPI_COMM_WORLD, &status);
		printf("Matriz final del proceso %d\n", mirango);
		MostrarMatriz(matriz);
	}
	MPI_Finalize();
}





//#define NUMERO_ELEMENTOS 6
//#define NUMERO_PROCESOS 3
//
//void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS]);
//
//
//int main(int argc, char *argv[])
//{
//	int mirango, tamano, tag = 0;
//	int numeroElementosEnviados = (NUMERO_ELEMENTOS + 1) * (NUMERO_ELEMENTOS / 2);
//	int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS];
//	int vectorLongitudBloque[NUMERO_ELEMENTOS];	
//	MPI_Aint vectorDesplazamientoBloque[NUMERO_ELEMENTOS];
//	MPI_Datatype vectorTipos[NUMERO_ELEMENTOS];
//	MPI_Datatype matrizTriangularInferior, matrizTriangularSuperior;
//	MPI_Request peticion;
//	MPI_Status estado;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
//	MPI_Comm_size(MPI_COMM_WORLD, &tamano);		
//	if (tamano != NUMERO_PROCESOS)
//	{
//		if (mirango == 0)
//		{
//			printf("Debe ejecutarse con %d procesos \n ", NUMERO_PROCESOS);
//			fflush(stdout);
//		}		
//		MPI_Abort(MPI_COMM_WORLD, 1);
//	}
//	else
//	{				
//		//Matriz triangular superior
//		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
//		{
//			vectorLongitudBloque[i] = NUMERO_ELEMENTOS - i;
//			vectorDesplazamientoBloque[i] = i * NUMERO_ELEMENTOS * sizeof(int); //(i + (NUMERO_ELEMENTOS * i)) * sizeof(int);
//			vectorTipos[i] = MPI_INT;
//		}
//		MPI_Type_struct(NUMERO_ELEMENTOS, vectorLongitudBloque, vectorDesplazamientoBloque, vectorTipos, &matrizTriangularSuperior);
//		MPI_Type_commit(&matrizTriangularSuperior);
//
//		//Enviamos matriz triangular inferior
//		for (int i = 0; i < NUMERO_ELEMENTOS; i++)
//		{
//			vectorLongitudBloque[i] = i + 1;
//			vectorDesplazamientoBloque[i] = (NUMERO_ELEMENTOS * i) * sizeof(int);
//			
//		}		
//		
//		MPI_Type_struct(NUMERO_ELEMENTOS, vectorLongitudBloque, vectorDesplazamientoBloque, vectorTipos, &matrizTriangularInferior);
//		MPI_Type_commit(&matrizTriangularInferior);				
//
//		if (mirango == 0)
//		{
//			//Rellenar la matriz con numeros aleatorios
//			srand((int)time(NULL));
//			int contador = 0;
//			for (int i = 0; i < NUMERO_ELEMENTOS; i++)
//			{
//				for (int j = 0; j < NUMERO_ELEMENTOS; j++)
//				{
//					matriz[i][j] = contador; // rand() % 101;
//					contador++;
//				}
//			}
//			
//			printf("Matriz original del proceso %d \n", mirango);
//			MostrarMatriz(matriz);			
//			
//			MPI_Isend(matriz, 1, matrizTriangularInferior, 2, tag, MPI_COMM_WORLD, &peticion);
//
//			MPI_Isend(matriz, 1, matrizTriangularSuperior, 1, tag, MPI_COMM_WORLD, &peticion);
//		}
//		else
//		{
//			for (int i = 0; i < NUMERO_ELEMENTOS; i++)
//			{
//				for (int j = 0; j < NUMERO_ELEMENTOS; j++)
//				{
//					matriz[i][j] = 0;
//				}
//			}
//			printf("Matriz original del proceso %d \n", mirango );
//			MostrarMatriz(matriz);
//			MPI_Recv(matriz, numeroElementosEnviados, MPI_INT, 0, tag, MPI_COMM_WORLD, &estado);
//			//MPI_Recv(elementosRecibidos, numeroElementosEnviados, MPI_INT, 0, tag, MPI_COMM_WORLD, &estado);
//			printf("Matriz final del proceso %d\n", mirango);
//			MostrarMatriz(matriz);
//		}
//	}	
//	MPI_Finalize();	
//	return 0;
//}
//
//
//void MostrarMatriz(int matriz[NUMERO_ELEMENTOS][NUMERO_ELEMENTOS])
//{
//	for (int i = 0; i < NUMERO_ELEMENTOS; i++)
//	{
//		for (int j = 0; j < NUMERO_ELEMENTOS; j++)
//		{
//			printf("[%3d] ", matriz[i][j]);
//		}
//		printf("\n");
//	}
//	printf("\n");
//	fflush(stdout);
//}
