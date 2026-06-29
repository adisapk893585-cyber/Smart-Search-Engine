#include "../include/InvertedIndex.h"

InvertedIndex::InvertedIndex(){

}

void InvertedIndex::addDocument(Document document){

    vector<string> words = parser.parse(document.content);

    for(auto &word : words){

        index[word][document.id]++;
    }
}

vector<int> InvertedIndex::search(string word){

    vector<int> result;

    word = parser.toLowerCase(word);

    if(index.find(word)==index.end()){

        return result;
    }

    for(auto &it : index[word]){

        result.push_back(it.first);
    }

    return result;
}

unordered_map<int,int> InvertedIndex::getPostingList(string word){

    word = parser.toLowerCase(word);

    if(index.find(word)==index.end()){

        return {};
    }

    return index[word];
}

bool InvertedIndex::contains(string word){

    word = parser.toLowerCase(word);

    return index.find(word)!=index.end();
}

int InvertedIndex::vocabularySize(){

    return index.size();
}

int InvertedIndex::getDocumentFrequency(string word){

    word = parser.toLowerCase(word);

    if(index.find(word)==index.end()){

        return 0;
    }

    return index[word].size();
}

int InvertedIndex::getWordFrequency(string word,int documentId){

    word = parser.toLowerCase(word);

    if(index.find(word)==index.end()){
        return 0;
    }

    if(index[word].find(documentId)==index[word].end()){
        return 0;
    }

    return index[word][documentId];
}

vector<int> InvertedIndex::andSearch(

    string first,

    string second

){

    vector<int> answer;

    vector<int> a = search(first);

    vector<int> b = search(second);

    unordered_set<int> s(b.begin(),b.end());

    for(int id : a){

        if(s.count(id)){

            answer.push_back(id);
        }
    }

    return answer;
}

vector<int> InvertedIndex::orSearch(string first,string second){

    unordered_set<int> result;

    vector<int> a = search(first);

    vector<int> b = search(second);

    for(int id : a){

        result.insert(id);
    }

    for(int id : b){

        result.insert(id);
    }

    return vector<int>(

        result.begin(),

        result.end()
    );
}

vector<int> InvertedIndex::notSearch(string first,string second){

    vector<int> answer;

    vector<int> a = search(first);

    vector<int> b = search(second);

    unordered_set<int> remove(b.begin(),b.end());

    for(int id : a){

        if(!remove.count(id)){

            answer.push_back(id);
        }
    }

    return answer;
}