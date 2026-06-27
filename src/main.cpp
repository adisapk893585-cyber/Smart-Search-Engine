#include<iostream>
#include<vector>
#include <iomanip>
#include <chrono>

#include "../include/SearchEngine.h"

using namespace std;

void printMenu(){

    cout<<"\n=============================================\n";
    cout<<"          SMART SEARCH ENGINE\n";
    cout<<"=============================================\n\n";

    cout<<"1. Search\n";
    cout<<"2. Auto Complete\n";
    cout<<"3. Spell Suggestion\n";
    cout<<"4. Analytics\n";
    cout<<"5. Query History\n";
    cout<<"6. Trending Searches\n";
    cout<<"7. Exit\n";

    cout<<"\nChoice : ";
}

int main(){

    SearchEngine engine;

    engine.loadDocuments("data/Documents");

    cout<<"\nDocuments Loaded Successfully!\n";

    int choice;

    while(true){

        printMenu();

        cin>>choice;

        cin.ignore();

        if(choice==1){

            string query;

            cout<<"\nSearch : ";

            getline(cin,query);

            engine.addHistory(query);

            auto start = chrono::high_resolution_clock::now();

            vector<SearchResult> result;

            if(

                query.find(" AND ") != string::npos ||

                query.find(" OR ") != string::npos ||

                query.find(" NOT ") != string::npos

            ){

                result = engine.booleanSearch(query);
            }

            else if(query.find(' ') != string::npos){
                result = engine.phraseSearch(query);
            }

            else{

                result = engine.keywordSearch(query);
            }


            string suggestion = engine.spellSuggestion(query);
            string searchWord = query;
            
            if(result.empty()){

                vector<SearchResult> fuzzy =
                    engine.fuzzySearch(query);

                if(!fuzzy.empty()){


                    cout << "\nNo exact match found.\n";
                    cout << "Showing results for : "
                        << suggestion
                        << "\n\n";

                    result = fuzzy;

                    searchWord = suggestion;
                }
            }
            
            if(result.empty()){

                cout << "\nNo Results Found.\n";

                if(
                    suggestion != "" &&
                    suggestion != query
                ){

                    cout << "\nDid you mean : "
                        << suggestion
                        << " ?\n";
                }

                

            }
            else{

                cout << "\n";
                cout << string(80, '=') << endl;
                cout << "                     SEARCH RESULTS" << endl;
                cout << string(80, '=') << endl;

                cout << left
                     << setw(8)  << "Rank"
                     << setw(40) << "Document"
                     << setw(10) << "Score"
                     << endl;

                cout << string(80, '-') << endl;

                vector<Document> docs = engine.getDocuments();

                int rank = 1;

                for(auto &res : result){

                    cout << left
                         << setw(8)  << rank++
                         << setw(40) << (docs[res.documentId].title + ".txt")
                         << setw(10) << fixed << setprecision(4) << res.score
                         << endl;

                    cout << "\nPreview:\n";
                    cout << string(80, '-') << endl;

                    cout << engine.generateSnippet(
                        res.documentId,
                        searchWord
                    ) << endl;

                    cout << string(80, '-') << endl;
                }

            }
            auto end = chrono::high_resolution_clock::now();

            auto duration =
                chrono::duration_cast<chrono::microseconds>(
                    end - start
                );

           cout << "\n";
           cout << string(80, '-') << endl;
           cout << "Query         : " << query << endl;
           cout << "Results Found : " << result.size() << endl;
           cout << "Search Time   : "
               << fixed << setprecision(3)
               << duration.count() / 1000.0
               << " ms" << endl;
           cout << string(80, '-') << endl;
           }

        else if(choice==2){

            string prefix;

            cout<<"\nPrefix : ";

            getline(cin,prefix);

            vector<string> words=

                engine.autoComplete(prefix);

            if(words.empty()){

                cout<<"\nNo Suggestions.\n";
            }
            else{

                cout<<"\nSuggestions\n";
                cout<<"---------------------\n";

                for(auto &word:words){

                    cout<<word<<endl;
                }
            }
        }

        else if(choice==3){

            string word;

            cout<<"\nEnter Word : ";

            getline(cin,word);

            string suggestion=

                engine.spellSuggestion(word);

            if(suggestion.empty()){

                cout<<"\nNo Suggestion Available.\n";
            }
            else{

                cout<<"\nDid you mean : "

                    <<suggestion

                    <<" ?\n";
            }
        }

        else if(choice==4){

            engine.showAnalytics();
        }

        else if(choice==5){

            vector<string> history = engine.getHistory();

            cout << "\n=========================================\n";
            cout << "          RECENT SEARCHES\n";
            cout << "=========================================\n\n";

            if(history.empty()){

                cout << "No Search History.\n";
            }
            else{

                for(int i = history.size()-1; i >= 0; i--){

                    cout << history[i] << endl;
                }
            }

            cout << "\n=========================================\n";
        }
        else if(choice==6){

            engine.showTrending();
        }
        else if(choice==7){

            cout<<"\nThank You For Using Smart Search Engine.\n";

            break;
        }

        else{

            cout<<"\nInvalid Choice.\n";
        }
    }

    return 0;
}


