/*
2022-07-09
백준 2846번
길을 입력받아 오르막길인지 확인하고 오르막길이면 높이를 확인하고 최대 높이를 출력한다
i번째 보다 i+1 한 것이 더 큰 경우를 오르막으로 인지하고 시작점과 끝점을 변수에 저장하여 비교 후 최대 높이 출력!
*/
#include <iostream>
#include <algorithm>
using namespace std;

int p[1000];
int main(){
    int n;
    int start=0, finish=0, max_rise=0;
    cin >> n;
    for(int i=0; i<n; i++){
        cin >> p[i];
    }
    for(int i=0; i<n-1; i++){
        if(p[i] < p[i+1]){ //오르막인 경우
            finish++;
            max_rise = max(max_rise, p[finish]-p[start]);
        }else{ // 오르막이 아닌 경우
            finish = i+1;
            start = i+1;
        }
    }
    cout << max_rise;
    return 0;
}