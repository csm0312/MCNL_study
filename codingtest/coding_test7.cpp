/*
2022-07-18
백준 1316번 그룹 단어 확인 문제
문자를 받아서 연속된 단어인지 확인 하는 문제
해결방법: 반복문을 통해서 문자가 연속된 단어인지 확인한다.
*/

#include <iostream>
#include <string>

using namespace std;

int main(){
    ios_base :: sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;
    int count = n;
    string str;
    for(int i=0; i<n; i++){
        cin >> str;
        bool flag = true;
        for(int j=0; j<str.length(); j++){
            for(int k=0; k<str.length(); k++){
                    if(str[j] != str[j-1] && str[j] == str[k]){
                        count--;
                        flag = false;
                        break;
                    }
                if(!flag) break;
            }
        }
    }
    cout << count;

    return 0;
}