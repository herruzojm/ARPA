#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	int mirango, numeroHijos, token = 3;
	MPI_Comm parentComm, interComm, intraComm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);

	MPI_Comm_get_parent(&parentComm);
	if (mirango == 0 && parentComm == MPI_COMM_NULL)
	{
		printf("Introduce el numero de hijos a lanzar: ");
		fflush(stdout);
		scanf_s("%d", &numeroHijos);
		printf("\n");
		printf("Lanzando %d hijos... \n", numeroHijos);
		fflush(stdout);
		
		MPI_Comm_spawn("C:\\Users\\JESUS\\Documents\\ARPA\\Practica08Hijo\\x64\\Debug\\Practica08Hijo.exe", MPI_ARGV_NULL, numeroHijos, MPI_INFO_NULL, mirango, MPI_COMM_WORLD, &interComm, MPI_ERRCODES_IGNORE);
		MPI_Intercomm_merge(interComm, 0, &intraComm);
		MPI_Bcast(&token, 1, MPI_INT, 0, intraComm);
	}
	MPI_Finalize();
	return 0;
}

