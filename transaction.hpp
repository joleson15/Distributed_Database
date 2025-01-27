#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include <map>
#include <vector>

#include "site.hpp"

using namespace std;;

enum class TransactionStatus
{
    RUNNING,
    WAITING,
    ABORTED,
    COMMITTED
};

class Transaction
{
public:
    int startTime = -1;
    int commitTime = -1;
    int transId = -1;
    int firstWriteTime = -1;
    bool toAbort = false;
    vector<Site *> sitesUsed;
    TransactionStatus curStatus;
    vector<int> variablesWrittenTo;
    Transaction(){}
    Transaction(int id, int time);
    
};