/**************************************************************
 * 백준 18258번 - 큐 2
 * push X: 정수 X를 큐에 넣는 연산이다.
 * pop: 큐에서 가장 앞에 있는 정수를 빼고, 그 수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
 * size: 큐에 들어있는 정수의 개수를 출력한다.
 * empty: 큐가 비어있으면 1, 아니면 0을 출력한다.
 * front: 큐의 가장 앞에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
 * back: 큐의 가장 뒤에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
 * 
 * queue 의 특성을 이해하고 라이브러리 함수를 활용하여 문제를 해결한다.
***************************************************************/

#include <iostream>
#include <queue>

using namespace std;

int main(){
    ios_base :: sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int N, num;
    string str;
    queue<int> q;

    cin >> N;

    for(int i=0; i<N; i++){
        cin >> str;

        if(str == "push"){
            cin >> num;
            q.push(num);
        }

        else if(str == "pop"){
            if(!q.empty()){
                cout << q.front() << '\n';
                q.pop();
            }else
                cout << -1 << '\n';
        }

        else if(str == "size"){
            cout << q.size() << '\n';
        }

        else if (str == "empty"){
            cout << q.empty() << '\n';
        }

        else if (str == "front"){
            if(!q.empty()) cout << q.front() << '\n';
            else cout << -1 << '\n';
        }

        else if (str == "back"){
            if(!q.empty()) cout << q.back() << '\n';
            else cout << -1 << '\n';
        }
    }
}