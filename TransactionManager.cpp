#include <iostream>
#include <map>
#include <list>
#include <unordered_set>
#include <stack>
#include <utility>

#include "TransactionManager.hpp"

using namespace std;

TransactionManager::TransactionManager(const list<Operation> ops)
{
    operations = ops;
    time = 0;
    initSites();
}

void TransactionManager::initSites()
{
    for (int i = 1; i < 11; i++)
    {
        allSites.push_back(Site(i));
    }
}

void TransactionManager::checkForConflict(map<int, map<int, int>> &transactionAccesses, map<int, map<int, int>> &conflicts, Operation op)
{
    // map<int, int> conflicts;

    auto &transactionConflicts = conflicts[op.tID];

    for (auto &tx : transactionAccesses)
    {

        int transactionID = tx.first;
        if (transactionID == op.tID)
            continue;

        map<int, int> &variableAccesses = tx.second;

        for (auto &targetVar : variableAccesses)
        {
            if (targetVar.first == op.varID)
            {
                // potential conflict
                if (op.type == OP::READ && targetVar.second == 0)
                    continue; // no conflict
                else
                {
                    if (op.type == OP::WRITE && targetVar.second == 0)
                    {
                        // RW
                        transactionConflicts[transactionID] = 0;
                    }
                    else if (op.type == OP::WRITE && targetVar.second == 1)
                    {
                        // WW
                        transactionConflicts[transactionID] = 1;
                    }
                    else
                    {
                        // WR
                        if (transactionMap[transactionID].curStatus == TransactionStatus::COMMITTED)
                            transactionConflicts[transactionID] = 2;
                        else
                        {
                            conflicts[transactionID][op.tID] = 0;
                            // transactionConflicts[transactionID] = 0;
                        }
                    }
                }
            }
        }
    }
}

bool TransactionManager::hasRWCycle(int startNode)
{
    // Visited set to track nodes in current DFS path
    std::unordered_set<int> visited;

    // Stack to perform DFS
    // Store: {current node, consecutive RW edge count}
    std::stack<std::pair<int, int>> dfsStack;

    dfsStack.push({startNode, 0});

    while (!dfsStack.empty())
    {
        int currentNode = dfsStack.top().first;
        int currentRWCount = dfsStack.top().second;
        dfsStack.pop();

        // If node already visited in current path, we have a cycle
        if (visited.count(currentNode) > 0 && currentRWCount >= 2)
        {
            return true;
        }

        visited.insert(currentNode);

        // Check outgoing edges from current node
        auto nodeConflicts = conflicts.find(currentNode);
        if (nodeConflicts != conflicts.end())
        {
            bool inTransactionGraph = false;
            for (const auto &edge : nodeConflicts->second)
            {
                if (transactionGraph.count(edge.first) || edge.first == startNode)
                {
                    inTransactionGraph = true;
                    int nextNode = edge.first;
                    int edgeType = edge.second;

                    // Track consecutive RW edges

                    int newRWCount = (edgeType == 0) ? currentRWCount + 1 : 0;
                    if (currentRWCount == 2)
                    {
                        newRWCount = 2;
                    }
                    
                    // If not visited, add to stack for further exploration
                    if (visited.count(nextNode) == 0)
                    {
                        dfsStack.push({nextNode, newRWCount});
                    }
                    // Also check if this node creates a cycle
                    else if (visited.count(nextNode) > 0 && newRWCount >= 2)
                    {
                        return true;
                    }
                }
            }
            if (!inTransactionGraph)
            {
                return false;
            }
        }
    }

    return false;
}

void TransactionManager::removeConflictEdges(int tid)
{
    conflicts.erase(tid);
}


void TransactionManager::beginTransaction(int tid, int beginTime)
{
    Transaction tr(tid, beginTime);
    transactionMap[tid] = tr;
    for (int i = 0; i < 10; i++)
    {
        Site *st = &allSites[i];
        st->readLog[beginTime] = st->committedData;
    }
}

void TransactionManager::SiteFail(int siteID)
{
    allSites[siteID - 1].fail(time);
    failureHistory[siteID - 1].push_back(time);
}

void TransactionManager::SiteRecover(int siteID)
{
    // status = up
    // search for waiting transactions that need to read site's data
    allSites[siteID - 1].recover();
    for (auto waitingTransaction = waitingTransactionReads.begin(); waitingTransaction != waitingTransactionReads.end(); waitingTransaction++)
    {
        if (waitingTransaction->first == -1)
        {
            continue;
        }
        Transaction tr = transactionMap[waitingTransaction->first];
        int varCheck = waitingTransaction->second;
        bool upContinuously = true;
        for (auto failTime : failureHistory[siteID - 1])
        {
            if (failTime > allSites[siteID - 1].lastCommitTimes[varCheck] && failTime < tr.startTime)
            {
                upContinuously = false;
            }
        }
        if (upContinuously)
        {
            int readVal = allSites[siteID - 1].committedData[varCheck];
            std::cout << "x" << varCheck << ": " << readVal << std::endl;
            // waitingTransactionReads.erase(waitingTransaction);
            waitingTransaction->first = -1;
        }
    }
}

void TransactionManager::transactionRead(int tid, int var, Operation op)
{
    Transaction *tr = &transactionMap[tid];
    bool replicated = (var % 2 == 0);
    transactionAccesses[tid][var] = 0;
    checkForConflict(transactionAccesses, conflicts, op);

    if (tr->curStatus == TransactionStatus::ABORTED)
        return;

    bool hasBeenRead = false;
    int readVal;
    if (replicated)
    {
        // in the case of replicated variable, if a site recovers after it fails
        // then the replicated vairables of that site cannot be read until a commit
        // happens after it recovers

        for (int i = 0; i < 10; i++)
        {
            if (hasBeenRead)
            {
                break;
            }

            Site *st = &allSites[i];
            if (st->status == SiteStatus::DOWN)
            {
                continue;
            }

            // case where no commit has occured on a site after its failed and
            // recovered
            if (st->failtime > st->lastCommitTimes[var] || st->lastCommitTimes[var] > tr->startTime)
            {
                continue;
            }

            readVal = st->readLog[tr->startTime][var];
            hasBeenRead = true;
        }
    }
    else
    {
        int siteNum = (var % 10) + 1;
        if (allSites[siteNum - 1].status != SiteStatus::DOWN)
        {
            readVal = allSites[siteNum - 1].readLog[tr->startTime][var];
            hasBeenRead = true;
        }
    }

    if (hasBeenRead)
    {
        std::cout << "x" << var << ": " << readVal << std::endl;
    }
    else
    {
        tr->curStatus = TransactionStatus::WAITING;
        std::pair<int, int> waitOp(tid, var);
        waitingTransactionReads.push_back(waitOp);
        std::cout << "T" << tid << " waits for var x" << var << std::endl;
    }
}

void TransactionManager::transactionWrite(int tid, int var, int val, Operation op)
{
    Transaction *tr = &transactionMap[tid];
    // if tr = active ... continue
    bool replicated = (var % 2 == 0);
    bool notWritten = true;
    transactionAccesses[tid][var] = 1;
    checkForConflict(transactionAccesses, conflicts, op);

    if (!replicated)
    {
        int siteNum = (var % 10) + 1;
        if ((allSites[siteNum].status == SiteStatus::DOWN))
        {
            return; // something else here (what happens when we try to write to a site that is down)
        }
        else
        {
            Site *st = &allSites[siteNum - 1];

            st->snapshots[tid][var] = val; // update variable with value
            tr->variablesWrittenTo.push_back(var);
            if (tr->firstWriteTime == -1)
            {
                tr->firstWriteTime = time;
            }
            printf("T%d writes to site %d\n", tid, siteNum);
        }
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            Site *st = &allSites[i];

            if (st->status == SiteStatus::DOWN)
            {
                continue;
            }

            st->snapshots[tid][var] = val;
            tr->variablesWrittenTo.push_back(var);
            if (tr->firstWriteTime == -1)
            {
                tr->firstWriteTime = time;
            }
            printf("T%d writes to site %d\n", tid, i + 1);
        }
    }
}

void TransactionManager::dump()
{
    for (auto site : allSites)
    {
        printf("site %d - ", site.getID());
        string comma = "";
        for (auto val : site.committedData)
        {
            cout << comma << " ";
            printf("x%d: %d", val.first, val.second);
            comma = ",";
        }
        cout << endl;
    }
}

void TransactionManager::transactionEnd(int tid, int endTime)
{
    // vector of things done
    //  records[tid] --> will give all things done by this transaction
    Transaction *tr = &transactionMap[tid];

    if (tr->curStatus == TransactionStatus::ABORTED)
    {
        std::cout << "T" << tid << " aborts" << std::endl;
        return;
    }

    bool canCommit = true;

    // check first commit wins rule
    for (auto site : allSites)
    {
        for (auto v : tr->variablesWrittenTo)
        {
            if (site.lastCommitTimes[v] > tr->startTime)
            {
                canCommit = false;
            }
        }
    }

    for (auto writeVar : tr->variablesWrittenTo)
    {
        bool replicated = (writeVar % 2 == 0);
        if (replicated)
        {
            for (auto site : allSites)
            {

                if ((tr->firstWriteTime != -1) && (site.failtime > tr->firstWriteTime))
                {
                    tr->curStatus = TransactionStatus::ABORTED;
                    std::cout << "T" << tid << " aborts" << std::endl;
                    return;
                }
            }
        }
        else
        {
            int siteNum = (writeVar % 10) + 1;
            // check only sites that transaction has accessed
            //  for the case of non-replicated variables
            if ((tr->firstWriteTime != -1) && (allSites[siteNum - 1].failtime > tr->firstWriteTime))
            {
                tr->curStatus = TransactionStatus::ABORTED;
                std::cout << "T" << tid << " aborts" << std::endl;
                return;
            }
        }
    }

    // check for RW-Cycle
    if (hasRWCycle(tid))
    {
        canCommit = false;
        removeConflictEdges(tid);
    }


    if (canCommit)
    {
        // update all sites
        for (int i = 0; i < 10; i++)
        {
            Site *st = &allSites[i];
            if (st->status == SiteStatus::UP)
            {
                if (st->snapshots.find(tid) == st->snapshots.end())
                {
                    continue;
                }

                for (auto updateVar : st->snapshots[tid])
                {
                    st->committedData[updateVar.first] = updateVar.second;
                }

                for (auto var : tr->variablesWrittenTo)
                {
                    st->lastCommitTimes[var] = time;
                }
            }
        }
        std::cout << "T" << tid << " commits" << std::endl;
        tr->curStatus = TransactionStatus::COMMITTED;
        transactionGraph.insert(tid);
    }
    else
    {
        tr->curStatus = TransactionStatus::ABORTED;
        std::cout << "T" << tid << " aborts" << std::endl;
        return;
    }
}

void TransactionManager::run()
{

    while (!operations.empty())
    {
        currentOP = operations.front();
        operations.pop_front();
        time++;
        switch (currentOP.type)
        {
        case OP::BEGIN:
        {
            // std::cout << "begin T" << currentOP.tID << std::endl;
            beginTransaction(currentOP.tID, currentOP.timestamp);
        }
        break;

        case OP::READ:
        {
            // std::cout << "Read T" << currentOP.tID << " x" << currentOP.varID << std::endl;
            transactionRead(currentOP.tID, currentOP.varID, currentOP);
        }
        break;

        case OP::WRITE:
        {
            // std::cout << "Write T" << currentOP.tID << " x" << currentOP.varID << " " << currentOP.val << std::endl;
            transactionWrite(currentOP.tID, currentOP.varID, currentOP.val, currentOP);
        }
        break;

        case OP::FAIL:
        {
            // std::cout << "Site " << currentOP.site << " Failed" << std::endl;
            SiteFail(currentOP.site);
        }
        break;

        case OP::RECOVER:
        {
            SiteRecover(currentOP.site);
            // std::cout << "Site " << currentOP.site << " Recovered" << std::endl;
        }
        break;

        case OP::END:
        {
            // std::cout << "end T" << currentOP.tID << std::endl;
            transactionEnd(currentOP.tID, time);
        }
        break;

        case OP::DUMP:
        {
            // std::cout << "dump()" << std::endl;
            dump();
        }
        break;

        default:
            std::cout << "no operation" << std::endl;
            exit(1);
        }
    }
}

void TransactionManager::addOperation(Operation op)
{
    operations.push_back(op);
}

void TransactionManager::endSimulation()
{
    for (auto tr : transactionMap)
    {
        if ((tr.second.curStatus != TransactionStatus::ABORTED) && ((tr.second.curStatus != TransactionStatus::COMMITTED)))
        {
            std::cout << "T" << tr.first << " aborts" << std::endl;
        }
    }
}