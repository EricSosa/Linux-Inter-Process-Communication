#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define INPUT_END 1
#define OUTPUT_END 0

using namespace std;

int main(int argc, char **argv)
{
    int z;
    string sinp;
    string ninp;
    string linp;
    string cinp;

    opterr = 0;

    while ((z = getopt(argc, argv, "s:n:l:c:")) != -1)
        switch (z)
        {
        case 's':
            sinp = optarg;
            sinp = "-s " + sinp;
            break;
        case 'n':
            ninp = optarg;
            ninp = "-n " + ninp;
            break;
        case 'l':
            linp = optarg;
            linp = "-l " + linp;
            break;
        case 'c':
            cinp = optarg;
            cinp = "-c " + cinp;
            break;
        case '?':
            std::cerr << "Error: unknown option: " << optopt << std::endl;
            return 1;
        default:
            return 0;
        }

    char *argw[6];

    argw[0] = (char *)"rgen";
    argw[1] = (char *)sinp.c_str();
    argw[2] = (char *)ninp.c_str();
    argw[3] = (char *)linp.c_str();
    argw[4] = (char *)cinp.c_str();
    argw[5] = nullptr;

    char *argp[2];

    argp[0] = (char *)"intersections.py";
    argp[1] = nullptr;

    char *argcpp[2];

    argcpp[0] = (char *)"short-path";
    argcpp[1] = nullptr;

    int rgentopyton[2];
    pipe(rgentopyton);
    int pytontoc[2];
    pipe(pytontoc);

    std::vector<pid_t> kids;

    pid_t kidrgen = fork();

    if (kidrgen < 0)
    {
        cerr << "Error: forking" << endl;
        return 1;
    }
    if (kidrgen == 0)
    {
        // redirect stdout to the pipe
        dup2(rgentopyton[INPUT_END], STDOUT_FILENO);
        close(rgentopyton[OUTPUT_END]);
        close(rgentopyton[INPUT_END]);
        execv("rgen", argw);
    }

    kids.push_back(kidrgen);

    pid_t kidpyton = fork();

    if (kidpyton < 0)
    {
        cerr << "Error: forking" << endl;
        return 1;
    }
    if (kidpyton == 0)
    {
        // redirect stdout to the pipe
        dup2(rgentopyton[OUTPUT_END], STDIN_FILENO);
        close(rgentopyton[OUTPUT_END]);
        close(rgentopyton[INPUT_END]); // Close this too!
        dup2(pytontoc[INPUT_END], STDOUT_FILENO);
        close(pytontoc[OUTPUT_END]);
        close(pytontoc[INPUT_END]); // Close this too!
        execv("intersections.py", argp);
    }

    kids.push_back(kidpyton);

    pid_t kidc = fork();

    if (kidc < 0)
    {
        cerr << "Error: forking" << endl;
        return 1;
    }
    if (kidc == 0)
    {
        // redirect stdout to the pipe
        dup2(pytontoc[OUTPUT_END], STDIN_FILENO);
        close(pytontoc[OUTPUT_END]);
        close(pytontoc[INPUT_END]);
        execv("short-path", argcpp);
    }

    kids.push_back(kidc);

    pid_t driver = fork();

    if (driver < 0)
    {
        cerr << "Error: forking" << endl;
        return 1;
    }
    if (driver == 0)
    {
        dup2(pytontoc[INPUT_END], STDOUT_FILENO);
        close(pytontoc[OUTPUT_END]);
        close(pytontoc[INPUT_END]);

        //int status;
        while (1)
        {
            string line;
            getline(cin, line);
            if (cin.eof())
            {
                for (pid_t k : kids)
                    kill(k, SIGTERM);
                break;
            }
            if (line.size() > 0)
            {
                cout << line << endl;
            }
        }
    }
    int status;
    if (waitpid(kidrgen, &status, 0) || waitpid(kidpyton, &status, 0) || waitpid(kidc, &status, 0))
    {
        for (pid_t k : kids)
            kill(k, SIGTERM);
    }
    return 0;
}
