#include <mpi.h>
#include <stdio.h>

int CalcularFactorial(int numero);


int main(int argc, char *argv[])
{
	int mirango;
	int numero = -1;
	int factorial;
	int tag = 0;
	int flag = 0;
	MPI_Request peticion;
	MPI_Status estado;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	
	while (numero != 0)
	{
		if (mirango == 0)
		{
			//Leemos número del usuario
			printf("Introduzca número (0 para salir)\n");
			fflush(stdout);
			scanf_s("%d", &numero);
			fflush(stdout);
			//Enviamos los datos al proceso 1 sin esperar el resultado
			MPI_Isend(&numero, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &peticion);
			//Si el usuario nos dio el cero, no tenemos nada mas que hacer
			if (numero != 0)
			{				
				//Mostramos mensaje de espera
				printf("\n Calculando factorial, espere por favor \n");
				fflush(stdout);
				//Ahora esperamos el resultado del proceso 1 y mostramos el resultado
				do {
					MPI_Test(&peticion, &flag, &estado);
				} while (flag != 1);
				MPI_Recv(&factorial, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &estado);
				printf("\n El resultado es %d \n\n\n", factorial);
				fflush(stdout);
			}			
		}
		else
		{	//Recibimos datos y calculamos el factorial		
			MPI_Recv(&numero, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &estado);
			factorial = CalcularFactorial(numero);
			//Enviamos el resultado al proceso 0 y continuamos la ejecución			
			MPI_Isend(&factorial, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &peticion);
		}
	}	

	MPI_Finalize();	
	return 0;
}

int CalcularFactorial(int numero)
{
	int resultado = 1;
	for (int i = 2; i <= numero; i++)
	{
		resultado *= i;
	}
	return resultado;
}
