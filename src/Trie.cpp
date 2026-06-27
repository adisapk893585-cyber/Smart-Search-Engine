#include "../include/Trie.h"

TrieNode::TrieNode(){

    isEnd=false;

    for(int i=0;i<26;i++){

        child[i]=NULL;
    }
}

Trie::Trie(){

    root=new TrieNode();
}

void Trie::insert(string word){

    TrieNode* curr=root;

    for(char ch:word){

        if(ch<'a' || ch>'z')
            continue;

        int idx=ch-'a';

        if(curr->child[idx]==NULL){

            curr->child[idx]=new TrieNode();
        }

        curr=curr->child[idx];
    }

    curr->isEnd=true;
}

bool Trie::search(string word){

    TrieNode* curr=root;

    for(char ch:word){

        if(ch<'a' || ch>'z')
            continue;

        int idx=ch-'a';

        if(curr->child[idx]==NULL){

            return false;
        }

        curr=curr->child[idx];
    }

    return curr->isEnd;
}

bool Trie::startsWith(string prefix){

    TrieNode* curr=root;

    for(char ch:prefix){

        if(ch<'a' || ch>'z')
            continue;

        int idx=ch-'a';

        if(curr->child[idx]==NULL){

            return false;
        }

        curr=curr->child[idx];
    }

    return true;
}

void Trie::dfs(
    TrieNode* node,
    string current,
    vector<string>& words
){

    if(node->isEnd){

        words.push_back(current);
    }

    for(int i=0;i<26;i++){

        if(node->child[i]){

            dfs(
                node->child[i],
                current+char(i+'a'),
                words
            );
        }
    }
}

vector<string> Trie::autoComplete(string prefix){

    TrieNode* curr=root;

    vector<string> words;

    for(char ch:prefix){

        if(ch<'a' || ch>'z')
            continue;

        int idx=ch-'a';

        if(curr->child[idx]==NULL){

            return words;
        }

        curr=curr->child[idx];
    }

    dfs(
        curr,
        prefix,
        words
    );

    return words;
}