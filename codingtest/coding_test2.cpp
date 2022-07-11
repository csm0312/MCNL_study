/*
2022.07.02 coding_test 1717번 문제
union find를 이용하여 두 숫자가 같은 집합 안에 있는지 찾아내는 프로그램

1. command가 0일 경우 - 뒤에 숫자 2개를 연결한다. (union)
2. command가 1일 경우 - 뒤에 숫자가 같은 집합인지 판별한다. (find)
작성자 - cho sungmin
*/
#include <iostream>
#include <cstdio>

using namespace std;

int *parent;

void init(){
    for(int i=0; i<=sizeof(parent); i++){
        parent[i] = i;
    }
}

int find(int x){
    if(parent[x] == x)
        return x;
    else
        return find(parent[x]);
}

void Union(int x, int y){
    x = find(x);
    y = find(y);

    if(x!= y){
        if(x<y) parent[y] = x;
        else parent[x] = y;
    }
}

void union_find(int command[]){
    if(command[0] == 0){
        Union(command[1], command[2]);
    }else{
        command[1] = find(command[1]);
        command[2] = find(command[2]);

        if(command[1] == command[2]){
            printf("YES\n");
        }else{
            printf("NO\n");
        }
    }
}


int main(){
    int n, m;
    cin >> n;
    cin >> m;
    
    parent = (int *)malloc(sizeof(int)* n);
    init();

    int command[3];
    for(int i=0; i<m; i++){
        for(int j=0; j<3; j++){
            cin >> command[j];
        }
        union_find(command);
    }

    free(parent);
}