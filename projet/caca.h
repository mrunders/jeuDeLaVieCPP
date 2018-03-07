void master::ecouteur() {

    int messagereceived = 0;
    int flag = 0;

    MPI_Status status;
    MPI_Reuest request;

    for(;;) {

        flag = 0;
        MPI_Irecv(&messageriereceived, , MPI_INT, MPI_ANY_SOURCE, TAG_MASTER, MPI_COMM_WORLD, request);
        while(!flag) {

            MPI_Test(&request, &flag, &status);

            if(flag) {

                switch(messageriereceived) {

                    case (MSG_WORKER_END): 
                        break;

                    case (MSG_SOLUTION_FOUND):
                        solutionFound(status, MPI_SOURCE);
                       break;

                    case (MSg_JOB_FINISHED):
                        return;

                    default:
                        printf("listener manager received a message %d from [worker %d] problem \n", messagereceived,status, MPI_Source);
                        exit(0);
                }

                break;
            }
        }
    }
}