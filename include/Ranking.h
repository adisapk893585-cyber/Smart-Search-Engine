#ifndef RANKING_H
#define RANKING_H

#include<iostream>
#include<vector>
#include<unordered_map>

#include "SearchResult.h"

using namespace std;

class Ranking{

public:

    double calculateTF(

        int frequency,

        int totalWords

    );

    double calculateIDF(

        int totalDocuments,

        int documentFrequency

    );

    double calculateTFIDF(

        int frequency,

        int totalWords,

        int totalDocuments,

        int documentFrequency

    );

    vector<SearchResult> rank(

        unordered_map<int,double> scores

    );

};

#endif