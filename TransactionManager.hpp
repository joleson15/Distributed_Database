#pragma once
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <deque>
#include <unordered_set>

#include "transaction.hpp"
#include "site.hpp"
#include "operation.hpp"

using namespace std;

class TransactionManager
{
private:
    int time;
    list<Operation> operations;
    map<int, Transaction> transactionMap;
    deque<pair<int, int>> waitingTransactionReads;
    vector<Site> allSites;
    map<int, vector<int>> failureHistory;
    map<int, map<int, int>> transactionAccesses;
    map<int, map<int, int>> conflicts;
    unordered_set<int> transactionGraph;

public:
    TransactionManager(const list<Operation> ops);
    Operation currentOP;
    void initSites();
    void checkForConflict(map<int, map<int, int>> &transactionAccesses, map<int, map<int, int>> &conflicts, Operation op);
    bool checkCycle(int tid);
    bool hasRWCycle(int startNode);
    void removeConflictEdges(int tid);
    void beginTransaction(int tid, int beginTime);
    void SiteFail(int siteID);
    void SiteRecover(int siteID);
    void transactionRead(int tid, int var, Operation op);
    void transactionWrite(int tid, int var, int val, Operation op);
    void transactionEnd(int tid, int endTime);
    void addOperation(Operation op);
    void endSimulation();
    void dump();
    void run();
};