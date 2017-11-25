#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void MostrarMatriz(float **matriz, int dimension);

int main(int argc, char *argv[])
{
	int rango, numeroProcesos, dimension, tag;
	float *filaEnviada, *filaRecibida;
	float **matriz_a, **matriz_b;
	double tiempo;
	MPI_Request peticion;
	MPI_Status estado;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rango);
	MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);

	//control para un numero de procesos minimo
	if (numeroProcesos == 1)
	{
		printf("Para ejecutar este ejercicio es neceario tener al menos dos procesos");
		fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, 0);
	}
	//leemos dimensiones de las matrices
	if (rango == 0)
	{
		if (argc == 2 && argv[0] == "-d")
		{
			dimension = (int)argv[1];
		}
		else
		{
			printf("Introduce la dimensión de la matriz: ");
			fflush(stdout);
			scanf_s("%d", &dimension);
			printf("\n");
			fflush(stdout);
		}		
		//iniciamos temporizador		
		tiempo = -MPI_Wtime();
	}
	
	//Mandar la dimension a todos
	MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//usaremos el tag como doble mecanismo: numero de fila
	//e indicador de terminar el proceso despues de calcular
	//una nueva fila	
	tag = dimension;
	if (numeroProcesos > dimension)
	{
		tag = numeroProcesos;
	}

	//Reservar memoria para las matrices
	filaEnviada = (float *)malloc(dimension * sizeof(float *));
	filaRecibida = (float *)malloc(dimension * sizeof(float *));

	matriz_a = (float **)malloc(dimension * sizeof(float *));
	matriz_b = (float **)malloc(dimension * sizeof(float *));
	float *Ma, *Mb;
	Ma = (float *)malloc(dimension * dimension * sizeof(float));
	for (int i = 0; i< dimension; i++)
	{
		matriz_a[i] = Ma + i * dimension;
	}
	Mb = (float *)malloc(dimension * dimension * sizeof(float));
	for (int i = 0; i< dimension; i++)
	{
		matriz_b[i] = Mb + i * dimension;
	}

	if (rango == 0)
	{
		//Iniciar las matrices
		srand((int)time(NULL));
		for (int i = 0; i<dimension; i++)
		{
			for (int j = 0; j<dimension; j++)
			{
				matriz_a[i][j] = (float)rand() / RAND_MAX;
				matriz_b[i][j] = (float)rand() / RAND_MAX;
			}
		}
		printf("Matrices iniciales\n");
		MostrarMatriz(matriz_a, dimension);
		MostrarMatriz(matriz_b, dimension);
	}

	//Distribuir la matriz B a todos
	MPI_Bcast(matriz_b[0], dimension * dimension, MPI_FLOAT, 0, MPI_COMM_WORLD);

	//Distribuir la matriz a por filas
	if (rango == 0)
	{
		//flag para comprobar que recibimos una fila
		int flag = 0;
		int filasEnviadas = 0;
		int filasRecibidas = 0;
		int ultimaFilaParaEnviar = dimension - 1;
		int procesoDestino = numeroProcesos - 1;					
		
		if (dimension > numeroProcesos - 1)
		{
			//Calentamiento
			//enviamos filas iniciales por orden decreciente de indice tanto en la 
			//matriz como en los procesos
			//nos detendremos cuando hayamos enviando una fila inicial a cada proceso
			//o nos queden las mismas filas que procesos de calculo; esas filas las 
			//enviaremos al final, en el enfriamiento
			for (ultimaFilaParaEnviar = dimension - 1; ultimaFilaParaEnviar >= numeroProcesos - 1; ultimaFilaParaEnviar--, procesoDestino--)
			{				
				MPI_Isend(matriz_a[ultimaFilaParaEnviar], dimension, MPI_FLOAT, procesoDestino, ultimaFilaParaEnviar, MPI_COMM_WORLD, &peticion);
				filasEnviadas++;
				if (filasEnviadas == numeroProcesos - 1)
				{					
					break;
				}
			}
			//comenzamos la recepción
			MPI_Irecv(filaRecibida, dimension, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &peticion);
			//Estacionario
			do 
			{
				MPI_Test(&peticion, &flag, &estado);
				if (flag == 1)
				{
					//hemos recibido una fila nueva
					flag = 0;
					//copiamos los valores a la matriz de resultados
					for (int j = 0; j < dimension; j++)
					{
						matriz_b[estado.MPI_TAG][j] = filaRecibida[j];
					}
					filasRecibidas++;
					//enviaremos una nueva fila al proceso que acaba de terminar si nos quedan
					//mas filas por enviar que el numero de procesos
					if (ultimaFilaParaEnviar > numeroProcesos - 1)
					{
						ultimaFilaParaEnviar--; 						
						MPI_Isend(matriz_a[ultimaFilaParaEnviar], dimension, MPI_FLOAT, estado.MPI_SOURCE, ultimaFilaParaEnviar, MPI_COMM_WORLD, &peticion);						
						filasEnviadas++;						
					}
					//si aun no hemos recibido todas las filas que hemos enviado,
					//seguimos esperando mas
					if (filasRecibidas != filasEnviadas)
					{
						MPI_Irecv(filaRecibida, dimension, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &peticion);
					}
				} 
			} while (filasRecibidas != filasEnviadas);			
		}
				
		//Enfriamiento
		//enviaremos las ultimas filas, tantas como el numero de procesos que
		//tenemos, que cerraran despues de este envio
		for (int i = 0; i < numeroProcesos - 1; i++)
		{
			//controlamos que no intentamos acceder a filas inexistentes de la 
			//matriz para los casos en que tengamos mas procesos que dimensiones
			//de las matrices
			if (i >= dimension)
			{				
				break;
			}
			MPI_Isend(matriz_a[i], dimension, MPI_FLOAT, i+1, i, MPI_COMM_WORLD, &peticion);			
		}	
		//comenzamos la recepcion de las ultimas filas
		MPI_Irecv(filaRecibida, dimension, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &peticion);
		//que terminaremos cuando hayamos recibidas tantas como dimensiones 
		//tenemos en las matrices
		while (filasRecibidas != dimension)
		{
			MPI_Test(&peticion, &flag, &estado);
			if (flag == 1)
			{
				//acabamos de recibir fila nueva
				tag = estado.MPI_TAG;
				flag = 0;
				filasRecibidas++;
				//copiamos el resultado a la matriz de resultado
				for (int j = 0; j < dimension; j++)
				{
					matriz_b[tag][j] = filaRecibida[j];
				}	
				//si aun no hemos recibido todas las filas que hemos enviado,
				//seguimos esperando mas
				if (filasRecibidas != dimension)
				{
					MPI_Irecv(filaRecibida, dimension, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &peticion);
				}
			}
		}
		//mostrar tiempo y resultado
		tiempo += MPI_Wtime();
		printf("Resultado\n");
		MostrarMatriz(matriz_b, dimension);
		printf("Tiempo necesario %f \n\n", tiempo);
		
	}
	else
	{
		//procesos de calculo		
		float resultado;		
		//si tenemos mas procesos que la dimension de la matriz,
		//al trabajar por filas, algunos de los procesos no se
		//usaran
		if (rango > dimension)
		{
			tag = 0; //cerrar proceso
		}		
		while (tag >= numeroProcesos-1)
		{
			//recibimos fila
			MPI_Recv(filaRecibida, dimension, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);			
			tag = estado.MPI_TAG;
			//calculamos el nuevo valor de la fila
			for (int i = 0; i < dimension; i++)
			{
				resultado = 0;
				for (int j = 0; j < dimension; j++)
				{
					resultado += filaRecibida[j] * matriz_b[j][i];
				}
				filaEnviada[i] = resultado;
			}			
			//y lo enviamos
			MPI_Isend(filaEnviada, dimension, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &peticion);
		}
	}

	MPI_Finalize();
	return 0;
}

void MostrarMatriz(float **matriz, int dimension)
{
	printf("{");
	for (int i = 0; i < dimension; i++)
	{
		printf("{");
		for (int j = 0; j < dimension; j++)
		{
			printf("%f", matriz[i][j]);
			if (j < dimension - 1)
			{
				printf(",");
			}
		}
		printf("},");
	}
	printf("}\n\n");
	fflush(stdout);
}

