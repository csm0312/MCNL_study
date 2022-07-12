/*
2022-07-07
백준 1712번 손익분기점 문제
A = 매년 임대료, 재산세, 보험료, 급여 등 고정 비용
B = 재료비와 인건비 등 총 가변 비용
C = 물건 판매비용
해당 정보를 활용하여 손익분기점을 찾는 프로그램
해결 방법: 판매비용에서 가변 비용을 뺀 비용이 고정 비용을 넘었을 경우 손익분기점이 된다.
*/
#include <iostream>

using namespace std;

int main(){
    ios_base :: sync_with_stdio(false); 
    cin.tie(NULL); 
    cout.tie(NULL);

    long a, b, c;
    int i=0;
    cin >> a >> b >> c;

    if(a < 0 || b < 0 || c < 0){
        cout << "They are not natural number\n";
        return 0;
    }
    // 판매 비용이 가변 비용보다 작을 경우는 이익이 날 수 없기에
    if(b >= c){
        cout << "-1\n";
        return 0;
    }

    cout << a/(c-b) + 1;

    return 0;
}