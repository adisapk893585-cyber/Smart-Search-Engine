#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include<string>

using namespace std;

class SearchResult{

public:

    int documentId;

    double score;

    SearchResult(){

        documentId=-1;

        score=0;
    }

    SearchResult(

        int id,

        double score

    ){

        documentId=id;

        this->score=score;
    }

    bool operator<(

        const SearchResult &other

    ) const{

        return score>other.score;
    }

};

#endif