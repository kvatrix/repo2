#include <iostream>
using namespace std;
 
 
 
void hanoi(int quantity, int from, int to, int middle_buf) {                              
    if (quantity != 0)
    {
        
        hanoi(quantity-1, from, middle_buf, to);
 
        cout << from << " -> " << to << endl;
 
        hanoi(quantity-1, middle_buf, to, from);
    }
}
 
 
 
int main()
{
        setlocale(LC_ALL,"rus");
        
    int start, end, middle, quantity;
 
    cout << "Номер первой башни:" << endl;
    cin >> start;
 
    cout << "Номер последней башни:" << endl;
    cin >> end;
 
    cout << "Номер промежуточной башни:" << endl;
    cin >> middle;
 
    cout << "Количество дисков:" << endl;
    cin >> quantity;
 
    hanoi(quantity, start, end, middle);
return 0;
}