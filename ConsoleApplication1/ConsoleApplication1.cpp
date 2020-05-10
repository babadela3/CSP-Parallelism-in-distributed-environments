#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <iostream>

#include "Channel.h"
#include "ChannelUtility.h"
#include "Common.h"

#pragma warning(disable:4996)

using namespace std;
using namespace go;
using namespace go::internal;

int world_rank, world_size;
Channel channel1;

// Example Read/Write operations

void func_listening_1() {
    char str[] = "ping";
    if (world_rank == 1) {
        channel1 <= str;
    }
}

void main_listening_1() {
    
    func_listening_1();
    char message[10];

    channel1.run();

    if (world_rank == 0) {
        message <= channel1;
        cout << message << endl;
    }    
}


//Example SELECT STATEMENT

void example_2()
{
    Select
    {
        Case{world_rank == 1, [&]()
            {
                char message[] = "Received";
                channel1 <= message;
            }},
        Case{world_rank == 0, [&]()
            {
                for (int i = 0; i < world_size - 1; i++) {
                    char message[10];
                    message <= channel1;
                    cout << message << endl;
                }
            }},
        Default{[]()
            {
                Sleep(1000);
                char message[] = "Timeout";
                channel1 <= message;
            }}
    };
}

void example_3()
{
    Select
    {
        Case{world_rank > 0, [&]()
            {
                Sleep(1000 * world_rank);
                char message[] = "Done";
                channel1 <= message;
            }},
        Case{world_rank == 0, [&]()
            {
                for (int i = 0; i < world_size - 1; i++) {
                    char message[10];
                    message <= channel1;
                    cout << message << endl;
                }
            }}
    };
}

//Example WAIT FOR TASKS TO FINISH

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Find out rank, size
    
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    main_listening_1();
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (world_rank == 0) cout << endl;

    example_2();

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0) cout << endl;

    example_3();
    MPI_Barrier(MPI_COMM_WORLD);

    cout << "Finish:" << world_rank << endl;

    MPI_Finalize();
}