#include <mpi.h>
#include <stdio.h>

#define NUMERO_ESCRITURAS 4

int main(int argc, char *argv[])
{
	int mirango;
	int posicion;
	int size;
	int datosParaEscribir[NUMERO_ESCRITURAS];
	int datosLeidos[NUMERO_ESCRITURAS];
	MPI_File fichero;
	MPI_Status estado;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	posicion = mirango*NUMERO_ESCRITURAS;


	for (int i = 0; i < NUMERO_ESCRITURAS; i++)
	{
		datosParaEscribir[i] = mirango + 48;
	}
	printf("Soy el proceso %d y a partir de la posicion %d voy a grabar %d veces el valor %d \n", mirango, posicion, NUMERO_ESCRITURAS, mirango);
	fflush(stdout);

	//Crear fichero y abrirlo para escritura
	MPI_File_open(MPI_COMM_WORLD, "e:/datos.txt", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fichero);

	//Crear vista de fichero
	MPI_File_set_view(fichero, posicion, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);

	//Escribir
	MPI_File_write_at(fichero, posicion, datosParaEscribir, NUMERO_ESCRITURAS, MPI_INT, &estado);

	////Cerrar fichero
	//MPI_File_close(&fichero);


	////Crear fichero y abrirlo para lectura
	//MPI_File_open(MPI_COMM_WORLD, "e:/datos.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fichero);

	////Crear vista de fichero
	//MPI_File_set_view(fichero, posicion, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);

	//Leer
	MPI_File_read_at(fichero, posicion, datosLeidos, NUMERO_ESCRITURAS, MPI_INT, &estado);

	//Cerrar fichero
	MPI_File_close(&fichero);

	//Mostrar datos
	printf("Soy el proceso %d Empezando en la posicion %d he leido los valores \n", mirango, posicion);
	fflush(stdout);
	for (int i = 0; i < NUMERO_ESCRITURAS; i++)
	{
		printf("[ %c ]", datosLeidos[i]);
	}
	printf("\n");
	fflush(stdout);

	MPI_Finalize();
	return 0;
}



//int main(int argc, char *argv[])
//{
//	int mirango;	
//	int posicion;
//	int size;
//	int datosParaEscribir[NUMERO_ESCRITURAS];
//	int datosLeidos[NUMERO_ESCRITURAS];
//	MPI_File fichero;
//	MPI_Status estado;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	posicion =  mirango*NUMERO_ESCRITURAS;
//
//
//	for(int i  = 0; i < NUMERO_ESCRITURAS; i++)
//	{
//		datosParaEscribir[i] = mirango + 48;				
//	}
//	printf("Soy el proceso %d y a partir de la posicion %d voy a grabar %d veces el valor %d \n", mirango, posicion, NUMERO_ESCRITURAS, mirango);
//	fflush(stdout);
//
//	//Crear fichero y abrirlo para escritura
//	MPI_File_open(MPI_COMM_WORLD, "e:/datos.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fichero);
//	
//	//Crear vista de fichero
//	MPI_File_set_view(fichero, posicion, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
//
//	//Escribir
//	MPI_File_write_at(fichero, posicion, datosParaEscribir, NUMERO_ESCRITURAS, MPI_INT, &estado);
//
//	//Cerrar fichero
//	MPI_File_close(&fichero);
//
//
//	//Crear fichero y abrirlo para lectura
//	MPI_File_open(MPI_COMM_WORLD, "e:/datos.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fichero);
//
//	//Crear vista de fichero
//	MPI_File_set_view(fichero, posicion, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
//
//	//Leer
//	MPI_File_read_at(fichero, posicion, datosLeidos, NUMERO_ESCRITURAS, MPI_INT, &estado);
//		
//	//Cerrar fichero
//	MPI_File_close(&fichero);
//
//	//Mostrar datos
//	printf("Soy el proceso %d Empezando en la posicion %d he leido los valores \n", mirango, posicion);
//	fflush(stdout);
//	for (int i = 0; i < NUMERO_ESCRITURAS; i++)
//	{
//		printf("[ %d ] \n", datosLeidos[i]);		
//	}
//	printf("\n");
//	fflush(stdout);
//
//	MPI_Finalize();	
//	return 0;
//}
