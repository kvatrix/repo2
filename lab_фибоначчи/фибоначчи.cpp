#include <iostream>
using namespace std;

int main() {
    int n, numone = 0, numtwo = 1, nextn;
    
    cout << "Введите кол-во чисел: ";
    cin >> n;
    cout << "Последовательность Фибоначчи:" << endl;
    
    for(int i = 1; i <= n; ++i) {
        
        
        if(i == 1) {
            cout << numone << " ";
            continue;
        }
        if(i == 2) {
            cout << numtwo << " ";
            continue;
        }
        
        
        
        nextn = numone + numtwo;
        numone = numtwo;
        numtwo = nextn;
        cout << nextn << " ";
    }
    return 0;
}
