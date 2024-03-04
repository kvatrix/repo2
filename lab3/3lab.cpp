#include <iostream>
using namespace std;


int main()
{
    const int n = 5;
    int mas[n][n];
    
    
    
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
        
       
        
        if(i<j)
            mas[i][n-1-j]=0;
        else
            mas[i][n-1-j]=1;
        }
        
        
    }
    
    
    for(int i=0;i<5;i++){
        for(int j=0;j<3;j++) {
        
        if(i<j)
            mas[i][j]=0;
        else
            mas[i][j]=1;
        }
    }
    
    
    //вывод
    
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++) {
            cout<<mas[i][j];
        }
        cout<<endl;
    }
    
    return 0;
}
