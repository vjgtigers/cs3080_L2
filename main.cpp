#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include <numeric>
#include <vector>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
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

    //cout << "inputed values as ints: " << numItems << " " << range << " " << seed << endl;

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
            //cout << randomNum << " " << i << endl;
            write(pipeVar[1], &randomNum, sizeof(randomNum));
        }

        wait(NULL);
        cout << "Parent ID: (" << getpid() << ") number of items written into the pipes are: " << numItems << endl;
        close(pipeVar[1]);

        return 0; //explicit exit for parent
    }

    else if (pid == 0) { //child process
        close(pipeVar[1]); //close write end of pipe
        int fetchedNum;
        vector<int> numbers; //easiest way to perform stat calculations at the end
        for(int i = 0; i < numItems; ++i) {
            read(pipeVar[0], &fetchedNum, sizeof(fetchedNum));
            //cout << fetchedNum << endl;
            numbers.push_back(fetchedNum);//could also have specific variables for min/max/sum/count
            //but the numbers could be needed later in the program so I chose to store them in a vector
            //until the end where all the statistics are only calculated once
        }



        cout << "Child ID: (" << getpid() << ") number of items received: " << numItems<<", min: " << *min_element(numbers.begin(), numbers.end()) << ", max: " << *max_element(numbers.begin(), numbers.end()) << ", avg: " << (accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size()) << endl;
        close(pipeVar[0]);

    return 0; //explicit exit for child
    }




    return 0;
}
