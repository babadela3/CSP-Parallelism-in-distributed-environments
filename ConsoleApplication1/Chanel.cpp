#include"Channel.h"
#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include "Common.h"

using namespace std;

Channel::Channel()
{
}

void operator<=(const Channel& c1, char* str) {
	MPI_Send(str, 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
}

char* operator<=(char* str, const Channel& c1) {
	if (world_rank == 0) {
		MPI_Recv(str, 100, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	return str;
}

void Channel::run()
{
	if (world_size < 2) {
		fprintf(stderr, "World size must be greater than 1 \n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
}