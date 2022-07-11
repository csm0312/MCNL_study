#include <iostream>
using namespace std;

int city[200];
int trip[1000];

void init(int n){
    for(int i=0; i<n; i++){
        city[i] = i;
    }
}

int find(int x){
    if(city[x] == x) return x;
    else return find(city[x]);
}

void Union(int x, int y){
    x = find(x);
    y = find(y);

    if(x!= y){
        if(x<y) city[y] = x;
        else city[x] = y;
    }
}

int main(){
    int city_size, trip_size;
    cin >> city_size;
    cin >> trip_size;
    init(city_size);
    
    int connect;
    for(int i=0; i<city_size; i++){
        for(int j=0; j<city_size; j++){
            cin >> connect;
            if(connect == 1) Union(i,j);
        }
    }

    int here, dest;
    bool canTrip = true;
    cin >> here;
    for(int i=1; i<trip_size; i++){
        cin >> dest;
        if(find(here) != find(dest)) canTrip = false;
        here = dest;
    }

    if(canTrip) printf("YES");
    else printf("NO");

    return 0;
}