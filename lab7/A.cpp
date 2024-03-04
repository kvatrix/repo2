#include <iostream>
using namespace std;



int countOdd(int mas[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (mas[i] % 2 != 0) {
            count++;
        }
    }
    return count;
}


int countOdd(double mas[], int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (static_cast<int>(mas[i]) % 2 != 0) {
            count++;
        }
    }
    return count;
}




int main() {
    int mas[] = {1, 2, 3, 4, 5};
    double mas_d[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    
    int Count = countOdd(mas, 5);
    int Count_d = countOdd(mas_d, 5);
    
    cout << "Number of odd integers: " << Count << endl;
    cout << "Number of odd doubles: " << Count_d << endl;
    
    return 0;
}