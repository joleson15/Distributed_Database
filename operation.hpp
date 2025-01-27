#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

enum class OP
{
    READ = 1,
    WRITE,
    BEGIN,
    END,
    DUMP,
    FAIL,
    RECOVER
};

class Operation
{

public:
    int tID = -1;
    int varID = -1;
    int val = -1;
    OP type;
    int site = -1;
    int timestamp = -1;

    Operation() {};

    void opREAD(string &line);
    void opWRITE(string &line);
    void opBEGIN(string &line);
    void opEND(string &line);
    void opFAIL(string &line);
    void opRECOVER(string &line);
    void opDUMP(string &line);
};


// enum class OP
// {
//     READ = 1,
//     WRITE,
//     BEGIN,
//     END,
//     DUMP,
//     FAIL,
//     RECOVER
// };

// class Operation
// {

// public:
//     int tID = -1;
//     int varID = -1;
//     int val = -1;
//     OP type;
//     int site = -1;
//     int timestamp = -1;

//     Operation() {};
// };

// Operation opREAD(std::string &line)
// {
//     Operation op;
//     int transactionID = 0;
//     int variableID = 0;
//     op.type = OP::READ;

//     int idx = line.find('T');
//     while (isdigit(line[++idx]))
//     {
//         transactionID = transactionID * 10 + (line[idx] - '0');
//     }
//     op.tID = transactionID;

//     idx = line.find('x');
//     while (isdigit(line[++idx]))
//     {
//         variableID = variableID * 10 + (line[idx] - '0');
//     }
//     op.varID = variableID;

//     return op;
// }

// Operation opWRITE(std::string &line)
// {

//     Operation op;
//     int transactionID = 0;
//     int variableID = 0;
//     int value = 0;
//     op.type = OP::WRITE;

//     int idx = line.find('T');
//     while (isdigit(line[++idx]))
//     {
//         transactionID = transactionID * 10 + (line[idx] - '0');
//     }
//     op.tID = transactionID;

//     idx = line.find('x');
//     while (isdigit(line[++idx]))
//     {
//         variableID = variableID * 10 + (line[idx] - '0');
//     }
//     op.varID = variableID;

//     while (isdigit(line[++idx]))
//     {
//         value = value * 10 + (line[idx] - '0');
//     }
//     op.val = value;

//     return op;
// }

// Operation opBEGIN(std::string &line)
// {

//     Operation op;
//     int transactionID = 0;
//     op.type = OP::BEGIN;

//     int idx = line.find('T');
//     while (isdigit(line[++idx]))
//     {
//         transactionID = transactionID * 10 + (line[idx] - '0');
//     }
//     op.tID = transactionID;

//     return op;
// }

// Operation opEND(std::string &line)
// {

//     Operation op;
//     int transactionID = 0;
//     op.type = OP::END;

//     int idx = line.find('T');
//     while (isdigit(line[++idx]))
//     {
//         transactionID = transactionID * 10 + (line[idx] - '0');
//     }
//     op.tID = transactionID;

//     return op;
// }

// Operation opFAIL(std::string &line)
// {

//     Operation op;
//     int siteNum = 0;
//     op.type = OP::FAIL;

//     int idx = line.find('(');
//     while (isdigit(line[++idx]))
//     {
//         siteNum = siteNum * 10 + (line[idx] - '0');
//     }
//     op.site = siteNum;

//     return op;
// }

// Operation opRECOVER(std::string &line)
// {

//     Operation op;
//     int siteNum = 0;
//     op.type = OP::RECOVER;

//     int idx = line.find('(');
//     while (isdigit(line[++idx]))
//     {
//         siteNum = siteNum * 10 + (line[idx] - '0');
//     }
//     op.site = siteNum;

//     return op;
// }

// Operation opDUMP(std::string &line)
// {

//     Operation op;
//     op.type = OP::DUMP;

//     return op;
// }