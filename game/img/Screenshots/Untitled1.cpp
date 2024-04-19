#include<iostream>
#include<math.h>
using namespace std;

long long tong( int a , int b){
    int s = a + b;
    return s;
}
long long hieu ( int a , int b){
    return a - b;
}
long long tich(int a , int b){
    return a * b;
}
long long chiadu(int a , int b){
    return a % b ;
}
long long thuong ( int a , int b){
    return a / b;
}
void nhappheptoan(char &pheptoan){
    do{
        cin >> pheptoan;
        cout << " nhap lai phep toan " << " " ;
}while( pheptoan != '+' && pheptoan != '-' && pheptoan != '*' && pheptoan != '/' );
}
//void thuchienpheptoan ( char& pheptoan , int a  , int b){
//    if(pheptoan == '+' ){
  //      tong(a , b);
//    }
//    else if ( pheptoan == '-' ){
  //      hieu(a , b );
//    }
//    else if ( pheptoan == '*'){
//        tich(a , b);
//    }
//    else if ( pheptoan == '%'){
//        chiadu(a , b);
//    }
//    else if ( pheptoan == '/' ){
//        thuong(a , b);
//    }
//}
int main(){

    int a , b;
    cin >> a >> b;
    char pheptoan ;
    nhappheptoan(pheptoan);
//    thuchienpheptoan( pheptoan , a , b);
//    cout << thuchienpheptoan( pheptoan , a , b)
    cout << pheptoan ;
}


