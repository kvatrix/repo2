#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

bool hasMoreThan2Words(const string& line) {
    int wordCount = 0;
    for (char c : line) {
        if (c == ' ') {
            wordCount++;
        }
    }
    return wordCount > 1;
}



int countVowels(const string& word) {
    int vowelCount = 0;
    for (char c : word) {
        if (c == 'а' || c == 'е' || c == 'ё' || c == 'и' || c == 'о' || c == 'у' || c == 'ы' || c == 'э' || c == 'ю' || c == 'я' || c == 'А' || c == 'Е' || c == 'Ё' || c == 'И' || c == 'О' || c == 'У' || c == 'Ы' || c == 'Э' || c == 'Ю' || c == 'Я') {
            vowelCount++;
        }
    }
    return vowelCount;
}



int main() {
    setlocale(LC_ALL, "RU");
    system("chcp 1275");
    system("cls");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ifstream inFile("F1.txt");
    ofstream outFile("F2.txt");
   

    string line;
    while (getline(inFile, line)) {
        if (hasMoreThan2Words(line)) {
            outFile << line << endl;
        }
    }

   
    
    
    string sent;
    sent = line;

    string word;
    int maxVowels = 0;
    int wordNumber = 0;

    int currentPos = 0;
    int maxPos = 0;

    for (int i = 0; i <= sent.length(); i++) {
        if (sent[i] == ' ' || i == sent.length()) {
            word = sent.substr(currentPos, i - currentPos);
            currentPos = i + 1;
            int vowelCount = countVowels(word);

            if (vowelCount > maxVowels) {
                maxVowels = vowelCount;
                wordNumber = wordNumber + 1;
                maxPos = wordNumber - 1;
            }
        }
    }
    cout << "Номер слова с наибольшим кол-вом гласных: " << maxPos << endl;


    inFile.close();
    outFile.close();

    return 0;
}
