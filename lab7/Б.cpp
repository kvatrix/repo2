#include <iostream>
#include <string>
#include <sstream>

using namespace std;

        int countWords(string str) {
    istringstream iss(str);
    int count = 0;
    string word;
    
    while (iss >> word) {
        count++;
    }
    
        return count;
}

int countWords(char* str) {
        return countWords(string(str));
}



int main() {
    
    string str;
    getline(cin, str); 
    
    int count = countWords(str);
    cout << "Количесвто слов в тексте: " << count << endl;
    
    return 0;
}