#include <iostream>
#include <cmath>
#include <cstdarg>
using namespace std;

float triangleArea(float a, float b, float c) {
    float s = (a + b + c) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

float polygonArea(int n, ...) {
    va_list args;
    va_start(args, n);
    
    float sum = 0;
    float side;
    
    for (int i = 0; i < n; i++) {
        side = va_arg(args, double);
        sum += side;
    }
    
    va_end(args);
    
    float s = sum / 2;
    float result = s;
    
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        result *= (s - va_arg(args, double));
    }
    
    return sqrt(result);
}

int main() {
    float side1 = 3, side2 = 4, side3 = 5;
    float area1 = triangleArea(side1, side2, side3);
    cout << "Площадь треугольника: " << area1 << endl;
    
    float area2 = polygonArea(6, 1, 1, 1, 1, 1);
    cout << "Площадь 5-угольника: " << area2 << endl;
    
    float area3 = polygonArea(8, 2, 2, 3, 4, 5, 6, 4, 3);
    cout << "Площадь 8-угольника: " << area3 << endl;
    
    return 0;
}