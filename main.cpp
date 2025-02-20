#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    std::cout << "Hello, World!" << std::endl;
    if(argc < 4) {
        cout << "not enough arguments provided" << endl;
        exit(-1);
    }
    char* numItempsString = argv[1];
    char* rangeString = argv[2];
    char* seedString = argv[3];

    int numItems = atoi(numItempsString);
    int range = atoi(rangeString);
    int seed = atoi(seedString);

    cout << "inputed values as ints: " << numItems << " " << range << " " << seed << endl;

    int pipeVar[2];
    pid_t pid;
    if(pipe(pipeVar) == -1) {
        cout << "pipe creation failed" << endl;
        exit(-1);
    }

    pid = fork();

    if(pid < 0) {
        cout << "fork failure" << endl;
        exit(-1);
    }

    if (pid > 0) { //parent process
        close(pipeVar[0]); //close read end of pipe
        srand(seed); //seed random number generator
        int randomNum = 0;
        for(int i = 0; i < numItems; ++i) {
            randomNum = rand() % range;
            cout << randomNum << " " << i << endl;
        }


        close(pipeVar[1]);

    }

    else if (pid == 0) { //child process
        close(pipeVar[1]); //close write end of pipe





        close(pipeVar[0]);


    }




    return 0;
}
