#include "../include/Ranking.h"

#include<cmath>
#include<algorithm>

double Ranking::calculateTF(

    int frequency,

    int totalWords

){

    if(totalWords==0)
        return 0;

    return (double)frequency/totalWords;
}

double Ranking::calculateIDF(

    int totalDocuments,

    int documentFrequency

){

    return log(

        (double)(totalDocuments+1)

        /

        (documentFrequency+1)

    )+1;
}

double Ranking::calculateTFIDF(

    int frequency,

    int totalWords,

    int totalDocuments,

    int documentFrequency

){

    double tf=

        calculateTF(

            frequency,

            totalWords
        );

    double idf=

        calculateIDF(

            totalDocuments,

            documentFrequency
        );

    return tf*idf;
}

vector<SearchResult> Ranking::rank(

    unordered_map<int,double> scores

){

    vector<SearchResult> result;

    for(auto &it:scores){

        result.push_back(

            SearchResult(

                it.first,

                it.second
            )
        );
    }

    sort(

        result.begin(),

        result.end()
    );

    return result;
}