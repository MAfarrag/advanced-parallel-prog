#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


int main(int argc, char *argv[])
{
    int i, myid, ntasks;
    int size = 10000000;
    int *message;
    int *receiveBuffer;
    MPI_Status status;

    MPI_Win rma_window; 

    double t0, t1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /* Allocate message */
    message = malloc(sizeof(int) * size);
    receiveBuffer = malloc(sizeof(int) * size);
    /* Initialize message */
    for (i = 0; i < size; i++)
        message[i] = myid;

    /* Create RMA window */
    MPI_Win_create(message, size *  sizeof(int), sizeof(int),
                   MPI_INFO_NULL, MPI_COMM_WORLD, &rma_window);

    /* Start measuring the time spend in communication */
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    /* TODO start */
    MPI_Win_fence(0, rma_window);
    /* Send and receive messages as defined in exercise */
    if (myid > 0) {
        MPI_Get(receiveBuffer, size, MPI_INT, myid - 1, 0, size, MPI_INT, 
                rma_window);
    }

    MPI_Win_fence(0, rma_window);

    if (myid > 0) {
        printf("Receiver: %d. first element %d.\n",
               myid, receiveBuffer[0]);
    }

    /* TODO end */

    /* Finalize measuring the time and print it out */
    t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    printf("Time elapsed in rank %2d: %6.3f\n", myid, t1 - t0);

    free(message);
    free(receiveBuffer);
    MPI_Finalize();
    return 0;
}
