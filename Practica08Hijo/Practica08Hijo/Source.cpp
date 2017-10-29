#include <mpi.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	int mirango;
	MPI_Comm parentComm, intraComm;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_get_parent(&parentComm);

	if (parentComm == MPI_COMM_NULL)
	{
		printf("Soy el hijo %d pero no tengo padre!!\n", mirango);
		fflush(stdout);
	}
	else
	{
		printf("[HIJO %d] Soy el hijo %d y acabo de ser creado \n", mirango, mirango);
		fflush(stdout);
		MPI_Intercomm_merge(parentComm, 1, &intraComm);
		int token;
		MPI_Bcast(&token, 1, MPI_INT, 0, intraComm);
		printf("[HIJO %d] Papa nos pasa el valor: %d \n", mirango, token);
		fflush(stdout);
		int tokenHijoMenor = 5;
		MPI_Bcast(&tokenHijoMenor, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (mirango == 0)
		{
			printf("[HIJO %d] Soy el menor y mando a mis hermanos el %d \n", mirango, tokenHijoMenor);
			fflush(stdout);
		}
		else
		{
			printf("[HIJO %d] Mi hermano menor me envia un %d \n", mirango, tokenHijoMenor);
			fflush(stdout);
		}
	}
	MPI_Finalize();
	return 0;
}

