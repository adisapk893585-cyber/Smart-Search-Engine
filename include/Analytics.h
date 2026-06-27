#ifndef ANALYTICS_H
#define ANALYTICS_H

#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>

using namespace std;

class Analytics{

private:

    unordered_map<string,int> searchCount;

    int totalSearches;

    int cacheHits;

    int cacheMisses;

    vector<string> queryHistory;

public:

    Analytics();

    void recordSearch(string query);

    void recordCacheHit();

    void recordCacheMiss();

    void showDashboard();

    void addHistory(string query);

    vector<string> getHistory();

    void showTrending();

};

#endif