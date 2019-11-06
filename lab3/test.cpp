#include <iostream>

using namespace std;


int main(){
    float vec[15];
    for(int i = 0; i < 15; ++i){
        vec[i] = i + 0.1f;
    }
    cout << sizeof(vec) << endl;
}