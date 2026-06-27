#ifndef SPELLCORRECTOR_H
#define SPELLCORRECTOR_H

#include<iostream>
#include<vector>
#include<string>

using namespace std;

class SpellCorrector{

private:

    int editDistance(
        string a,
        string b
    );

public:

    string suggest(
        string word,
        vector<string> vocabulary
    );

};

#endif