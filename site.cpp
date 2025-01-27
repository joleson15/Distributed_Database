#include "site.hpp"

#include <iostream>

Site::Site(const int id) : siteID(id), status(SiteStatus::UP) {init();}

void Site::init(){

    for(int i = 1; i <= 20; i++){
        if((i % 10 + 1) == (siteID)){
            committedData[i] = 10 * i;
            lastCommitTimes[i] = 0;
        }
    }

    for(int i = 2; i <= 20; i++){
        if(i % 2 == 0){
            committedData[i] = 10 * i;
            lastCommitTimes[i] = 0;
        }
    }

}

void Site::fail(int failureTime){
    status = SiteStatus::DOWN;
    failtime = failureTime;
}

void Site::recover(){
    status = SiteStatus::UP;
}

int Site::getID()
{
    return siteID;
}
