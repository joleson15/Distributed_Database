

#pragma once
#include <iostream>
#include <map>
#include <utility>

enum class SiteStatus{UP = 1, DOWN, FAILED, RECOVERED};

class Site{

    private:
        int siteID;

    public:
        SiteStatus status;
        int failtime = -1;

        //each site contains data
        std::map<int, int> committedData;
        std::map<int, int> lastCommitTimes;  //pair<value, last_commit_time
        std::map<int, std::map<int, int>> snapshots; //use this for timestamps 
        std::map<int, std::map<int, int>> readLog; //use this for writes that we later commit

        Site(){}
        Site(const int id);

        void init();
        
        void fail(int failtime);
        void recover();
        void read();
        int getID();
};