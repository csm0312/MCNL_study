/*
2020-07-09
백준 17212번 달나라 토끼를 위한 구매대금 지불 도우미
참고 : https://kangeee.tistory.com/m/123
*/
#include <iostream>

using namespace std;
int dp[100001];

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int price;
    cin >> price;
    dp[1] = 1, dp[2] = 1, dp[3] = 2, dp[4] = 2, dp[5] = 1, dp[6] = 2, dp[7] = 1;

    for(int i=8; i<=price;i++){
        if(i%7 == 0) dp[i] = i/7;
        else dp[i] = min({ dp[i - 7],dp[i - 5],dp[i - 2],dp[i - 1] }) + 1;
    }
    cout << dp[price];

    return 0;
}