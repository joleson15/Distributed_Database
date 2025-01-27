#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "operation.hpp"

void Operation::opREAD(std::string &line)
{
    int transactionID = 0;
    int variableID = 0;
    type = OP::READ;

    int idx = line.find('T');
    while (isdigit(line[++idx]))
    {
        transactionID = transactionID * 10 + (line[idx] - '0');
    }
    tID = transactionID;

    idx = line.find('x');
    while (isdigit(line[++idx]))
    {
        variableID = variableID * 10 + (line[idx] - '0');
    }
    varID = variableID;
}

void Operation::opWRITE(std::string &line)
{

    int transactionID = 0;
    int variableID = 0;
    int value = 0;
    type = OP::WRITE;

    int idx = line.find('T');
    while (isdigit(line[++idx]))
    {
        transactionID = transactionID * 10 + (line[idx] - '0');
    }
    tID = transactionID;

    idx = line.find('x');
    while (isdigit(line[++idx]))
    {
        variableID = variableID * 10 + (line[idx] - '0');
    }
    varID = variableID;

    if(line[++idx] != ' ')
    {
        --idx;
    }

    while (isdigit(line[++idx]))
    {
        value = value * 10 + (line[idx] - '0');
    }
    val = value;
}

void Operation::opBEGIN(std::string &line)
{

    int transactionID = 0;
    type = OP::BEGIN;

    int idx = line.find('T');
    while (isdigit(line[++idx]))
    {
        transactionID = transactionID * 10 + (line[idx] - '0');
    }
    tID = transactionID;
}

void Operation::opEND(std::string &line)
{

    int transactionID = 0;
    type = OP::END;

    int idx = line.find('T');
    while (isdigit(line[++idx]))
    {
        transactionID = transactionID * 10 + (line[idx] - '0');
    }
    tID = transactionID;
}

void Operation::opFAIL(std::string &line)
{

    int siteNum = 0;
    type = OP::FAIL;

    int idx = line.find('(');
    while (isdigit(line[++idx]))
    {
        siteNum = siteNum * 10 + (line[idx] - '0');
    }
    site = siteNum;
}

void Operation::opRECOVER(std::string &line)
{

    int siteNum = 0;
    type = OP::RECOVER;

    int idx = line.find('(');
    while (isdigit(line[++idx]))
    {
        siteNum = siteNum * 10 + (line[idx] - '0');
    }
    site = siteNum;
}

void Operation::opDUMP(std::string &line)
{
    type = OP::DUMP;
}