#ifndef LRUCACHE_H
#define LRUCACHE_H

#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<unordered_map>

using namespace std;

class LRUCache{

private:

    size_t capacity;

    list<pair<string,vector<int>>> cache;

    unordered_map<
        string,
        list<pair<string,vector<int>>>::iterator
    > mp;

public:

    LRUCache(size_t capacity);

    bool contains(string query);

    vector<int> get(string query);

    void put(
        string query,
        vector<int> results
    );

    int size();

};

#endif