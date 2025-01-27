#include <iostream>
#include <list>
#include <unordered_map>
#include <map>

#include "transaction.hpp"

using namespace std;

Transaction::Transaction(int id, int time)
{
    startTime = time;
    transId = id;
    curStatus = TransactionStatus::RUNNING;
}
