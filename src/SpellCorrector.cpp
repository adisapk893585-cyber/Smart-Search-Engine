#include "../include/SpellCorrector.h"

#include<vector>
#include<algorithm>


int SpellCorrector::editDistance(

    string a,

    string b

){

    int n=a.size();
    int m=b.size();

    vector<vector<int>> dp(

        n+1,

        vector<int>(m+1)
    );

    for(int i=0;i<=n;i++)
        dp[i][0]=i;

    for(int j=0;j<=m;j++)
        dp[0][j]=j;

    for(int i=1;i<=n;i++){

        for(int j=1;j<=m;j++){

            if(a[i-1]==b[j-1]){

                dp[i][j]=dp[i-1][j-1];
            }
            else{

                dp[i][j]=1+

                    min(

                        dp[i-1][j],

                        min(

                            dp[i][j-1],

                            dp[i-1][j-1]
                        )
                    );
            }
        }
    }

    return dp[n][m];
}

string SpellCorrector::suggest(

    string word,

    vector<string> vocabulary

){

    int bestDistance=1000000;

    string answer="";

    for(auto &candidate:vocabulary){

        int dist=

            editDistance(

                word,

                candidate
            );

        if(dist<bestDistance){

            bestDistance=dist;

            answer=candidate;
        }
    }

    if(bestDistance<=2){

        return answer;
    }

    return "";
}