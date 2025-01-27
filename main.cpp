#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "site.hpp"
#include "operation.hpp"
#include "TransactionManager.hpp"
#include "transaction.hpp"

std::list<Operation> operations;

Operation parseLine(string line)
{
    Operation op;
    switch (line[0])
    {

    case 'R':
    {
        op.opREAD(line);
        break;
    }
    case 'W':
    {
        op.opWRITE(line);
        break;
    }
    case 'b':
    {
        op.opBEGIN(line);
        break;
    }
    case 'e':
    {
        op.opEND(line);
        break;
    }
    case 'f':
    {
        op.opFAIL(line);
        break;
    }
    case 'r':
    {
        op.opRECOVER(line);
        break;
    }
    case 'd':
    {
        op.opDUMP(line);
        break;
    }
    case 'D':
    {
        std::cout << "EXITING SESSION" << std::endl;
        exit(1);
        break;
    }
    default:
        // std::cout << "--nothing--" << std::endl;
        break;
    }
    return op;
}


int main(int argc, char *argv[])
{
    TransactionManager tm(operations);
    std::string line;
    int time = 0;
    if (argc == 2)
    {
        const char *filename = argv[1];
        std::ifstream file(filename);

        while (getline(file, line))
        {
            Operation curOP = parseLine(line);
            curOP.timestamp = time;
            tm.addOperation(curOP);
            tm.run();
            time++;
        }
    }
    else
    {
        while (std::cin)
        {
            getline(std::cin, line);
            Operation curOP = parseLine(line);
            curOP.timestamp = time;
            tm.addOperation(curOP);
            tm.run();
            time++;
        }
    }
    tm.endSimulation();
}
