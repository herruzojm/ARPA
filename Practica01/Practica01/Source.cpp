#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int mirango, tamano, numeroLeido, numeroRecibido, tag = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);		
	if (mirango == 0)
	{
		printf("Introduce un numero entero: ");		
		fflush(stdout);
		scanf_s("%d", &numeroLeido);
		fflush(stdout);
		printf("\n\n");
		printf("El numero introducido es n=%d \n", numeroLeido);
		printf("Enviando el dato al otro proceso\n");
		MPI_Send(&numeroLeido, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
		printf("Enviado\n\n");
		printf("Dame otro numero: ");
		fflush(stdout);
		scanf_s("%d", &numeroLeido);
		printf("\n\n");
		fflush(stdout);		
		printf("El segundo numero introducido es n=%d \n", numeroLeido);
		fflush(stdout);
		MPI_Send(&numeroLeido, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
	}
	else
	{
		printf("Voy a recibir el numero del otro proceso\n\n");		
		MPI_Status status;
		MPI_Recv(&numeroRecibido, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		printf("El numero recibido es n=%d \n", numeroRecibido);
		fflush(stdout);
		
		printf("Voy a recibir el segundo numero del otro proceso\n");		
		MPI_Recv(&numeroRecibido, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		printf("El segundo numero recibido es n=%d \n", numeroRecibido);
		fflush(stdout);
	}
	MPI_Finalize();	
	return 0;
}
