#include "../include/LRUCache.h"

LRUCache::LRUCache(size_t capacity){
    this->capacity = capacity;
}

bool LRUCache::contains(string query){

    return mp.find(query)!=mp.end();
}

vector<int> LRUCache::get(string query){

    if(mp.find(query)==mp.end()){

        return {};
    }

    auto it=mp[query];

    cache.splice(cache.begin(),cache,it);

    return it->second;
}

void LRUCache::put(

    string query,

    vector<int> results

){

    if(mp.find(query)!=mp.end()){

        cache.erase(mp[query]);

        mp.erase(query);
    }

    cache.push_front({query,results});

    mp[query]=cache.begin();

    if(cache.size()>capacity){

        auto last=cache.back();

        mp.erase(last.first);

        cache.pop_back();
    }
}

int LRUCache::size(){

    return cache.size();
}