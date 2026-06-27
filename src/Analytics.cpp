#include "../include/Analytics.h"

#include<algorithm>

Analytics::Analytics(){

    totalSearches=0;

    cacheHits=0;

    cacheMisses=0;
}

void Analytics::recordSearch(string query){

    totalSearches++;

    searchCount[query]++;
}

void Analytics::recordCacheHit(){

    cacheHits++;
}

void Analytics::recordCacheMiss(){

    cacheMisses++;
}

void Analytics::showDashboard(){

    cout<<"\n=========================================\n";
    cout<<"          SEARCH ANALYTICS\n";
    cout<<"=========================================\n\n";

    cout<<"Total Searches : "<<totalSearches<<endl;

    cout<<"Cache Hits     : "<<cacheHits<<endl;

    cout<<"Cache Misses   : "<<cacheMisses<<endl;

    cout<<"\nMost Searched Queries\n";
    cout<<"-----------------------------------------\n";

    vector<pair<int,string>> queries;

    for(auto &it:searchCount){

        queries.push_back({

            it.second,

            it.first
        });
    }

    sort(

        queries.rbegin(),

        queries.rend()
    );

    int cnt=0;

    for(auto &it:queries){

        cout<<it.second

            <<" : "

            <<it.first

            <<" searches\n";

        cnt++;

        if(cnt==5)
            break;
    }

    cout<<"\n=========================================\n";
}


void Analytics::addHistory(string query){

    queryHistory.push_back(query);

    if(queryHistory.size()>10){

        queryHistory.erase(queryHistory.begin());
    }
}

vector<string> Analytics::getHistory(){

    return queryHistory;
}


void Analytics::showTrending(){

    vector<pair<int,string>> trending;

    for(auto &x : searchCount){

        trending.push_back({
            x.second,
            x.first
        });
    }

    sort(
        trending.rbegin(),
        trending.rend()
    );

    cout << "\n=========================================\n";
    cout << "        TRENDING SEARCHES\n";
    cout << "=========================================\n\n";

    int cnt = 0;

    for(auto &x : trending){

        cout
            << cnt + 1
            << ". "
            << x.second
            << " ("
            << x.first
            << " searches)"
            << endl;

        cnt++;

        if(cnt==5)
            break;
    }

    cout << "\n=========================================\n";
}