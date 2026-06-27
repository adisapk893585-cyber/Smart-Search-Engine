#include "../include/SearchEngine.h"
#include <algorithm>
#include <cctype>

SearchEngine::SearchEngine()

: cache(10)

{

}

void SearchEngine::addDocument(Document document){

    documents.push_back(document);

    index.addDocument(document);


    vector<string> words=

        parser.parse(document.content);

    for(auto &word:words){

        trie.insert(word);

        if(vocabulary.insert(word).second){

            vocabularyList.push_back(word);
        }
    }
}

vector<SearchResult> SearchEngine::keywordSearch(string word){

    analytics.recordSearch(word);

    if(cache.contains(word)){

        analytics.recordCacheHit();

        vector<int> ids = cache.get(word);

        unordered_map<int,double> scores;

        for(int id : ids){

            scores[id] = 1.0;
        }

        return ranking.rank(scores);
    }

    analytics.recordCacheMiss();

    vector<int> ids = index.search(word);

    cache.put(word, ids);

    unordered_map<int,double> scores;

    int totalDocuments = documents.size();

    int documentFrequency = index.getDocumentFrequency(word);

    for(int id : ids){

        int frequency = index.getWordFrequency(word, id);

        int totalWords = parser.parse(documents[id].content).size();

        double score = ranking.calculateTFIDF(

            frequency,
            totalWords,
            totalDocuments,
            documentFrequency
        );

        scores[id] = score;
    }

    return ranking.rank(scores);
}

vector<string> SearchEngine::autoComplete(

    string prefix

){

    return trie.autoComplete(prefix);
}

string SearchEngine::spellSuggestion(string word){

    return spellCorrector.suggest(
        word,
        vocabularyList
    );
}

vector<Document> SearchEngine::getDocuments(){

    return documents;
}

void SearchEngine::loadDocuments(string folderPath){

    documents = FileManager::loadDocuments(folderPath);

    for(auto &doc : documents){

        index.addDocument(doc);

        vector<string> words = parser.parse(doc.content);

        for(auto &word : words){

            trie.insert(word);

            if(vocabulary.insert(word).second){

                vocabularyList.push_back(word);
            }
        }
    }
}

void SearchEngine::showAnalytics(){

    analytics.showDashboard();
}

vector<SearchResult> SearchEngine::booleanSearch(string query){

    size_t pos;

    unordered_map<int,double> scores;

    if((pos=query.find(" AND "))!=string::npos){

        string first=query.substr(0,pos);
        string second=query.substr(pos+5);

        vector<int> ids=index.andSearch(first,second);

        for(int id:ids){

            scores[id]=1.0;
        }
    }

    else if((pos=query.find(" OR "))!=string::npos){

        string first=query.substr(0,pos);
        string second=query.substr(pos+4);

        vector<int> ids=index.orSearch(first,second);

        for(int id:ids){

            scores[id]=1.0;
        }
    }

    else if((pos=query.find(" NOT "))!=string::npos){

        string first=query.substr(0,pos);
        string second=query.substr(pos+5);

        vector<int> ids=index.notSearch(first,second);

        for(int id:ids){

            scores[id]=1.0;
        }
    }

    return ranking.rank(scores);
}

string SearchEngine::generateSnippet(

    int documentId,

    string keyword

){

    string content = documents[documentId].content;

    string lowerContent = content;

    transform(
        lowerContent.begin(),
        lowerContent.end(),
        lowerContent.begin(),
        ::tolower
    );

    transform(
        keyword.begin(),
        keyword.end(),
        keyword.begin(),
        ::tolower
    );

    size_t pos = lowerContent.find(keyword);

    if(pos == string::npos){

        return "";
    }

    int start = max(0,(int)pos-20);

    int length = min(
        90,
        (int)content.size()-start
    );

    string snippet = content.substr(start,length);

    for(char &ch : snippet){

        if(ch=='\n'){

            ch=' ';
        }
    }

    if(start!=0){

        snippet="..."+snippet;
    }

    if(start+length<(int)content.size()){

        snippet+="...";
    }

    return snippet;
}

vector<SearchResult> SearchEngine::phraseSearch(string phrase){

    analytics.recordSearch(phrase);

    unordered_map<int,double> scores;

    transform(
        phrase.begin(),
        phrase.end(),
        phrase.begin(),
        ::tolower
    );

    for(auto &doc : documents){

        string content = doc.content;

        string lowerContent = content;

        transform(
            lowerContent.begin(),
            lowerContent.end(),
            lowerContent.begin(),
            ::tolower
        );

       

        if(lowerContent.find(phrase)!=string::npos){

            scores[doc.id]=1.0;
        }
    }

    return ranking.rank(scores);
}

vector<SearchResult> SearchEngine::fuzzySearch(string query){

    string suggestion = spellSuggestion(query);

    if(suggestion.empty()){

        return {};
    }

    return keywordSearch(suggestion);
}

vector<string> SearchEngine::getHistory(){

    return analytics.getHistory();
}


void SearchEngine::showTrending(){

    analytics.showTrending();
}