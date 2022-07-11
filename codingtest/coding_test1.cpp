/*
2022.06.29 백준 1546번 Array
평균 조작하는 문제
점수를 받아 새로운 점수로 바꾸고 새 평균을 출력하여 본인 평균 조작하기
작성자 - chosungmin
*/
#include <iostream>
#include <vector>

using namespace std;

float score_max(float score_list[], int test_amount){
    float max = 0;
    for(int i=0; i<test_amount; i++){
      if(max < score_list[i]) max = score_list[i];
    }

    return max;
}

void change_score(float score_list[], int test_amount){
    float m = score_max(score_list, test_amount);
    for(int i=0; i<test_amount; i++){
        score_list[i] = (score_list[i] / m) * 100;
    }
}

float average_score(float score_list[],int test_amount){
    float total=0;
    for(int i=0; i<test_amount; i++){
        total += score_list[i];
    }
    float average = total/test_amount;
    return average;
}

int main(){
    int test_amount;
    cin >> test_amount;
    int score;

    float score_list[test_amount];

    for(int i=0; i<test_amount; i++){
        cin >> score_list[i];
    }
    
    change_score(score_list, test_amount);
    printf("%f", average_score(score_list, test_amount));
}