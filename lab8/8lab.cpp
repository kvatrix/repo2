#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

struct avto {
	string surname;
	string name;
	string otchestvo;
	string num_avto; //	номер автомобиля
	string num_pass; //	номер техпаспорта

	void info(string s, string n, string o, string a, string p) {
		surname = s;
		name = n;
		otchestvo = o;
		num_avto = a;
		num_pass = p;
	}
};



int main()
{
	setlocale(LC_ALL, "RU");
	system("chcp 1275");
	system("cls");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int quantity; // кол-во владельцев авто
	avto mas[50];
	cout << "Введите кол-во владельцев авто ";
	cin >> quantity;
	for (int i = 0; i < quantity; i++) {
		string s, n, o, a, p;
		
		cout << "Введите ФИО, номер автомобиля, номер техпаспорта владельца авто ";
		cin >> s >> n >> o >> a >> p;
		mas[i].info(s, n, o, a, p);
	}
	


	bool f = false;
	int zfam;
	string fam;
	cout << "Введите фамилию человека, перед которым нужно вписать еще двоих: ";
	cin >> fam;
	for (int i = 0; i < quantity; i++) {
		if (mas[i].surname == fam) {
			zfam = i;
			f = true;
		}
	}
	if (f == true) {

		for (int j = 0; j < 2; j++) {
			for (int i = quantity; i > zfam; i--) {
				mas[i] = mas[i - 1];
			}
			string s1, n1, o1, a1, p1;
			cout << "Введите ФИО, номер автомобиля, номер техпаспорта нового владельца авто " << endl;
			cin >> s1 >> n1 >> o1 >> a1 >> p1;
			mas[zfam].info(s1, n1, o1, a1, p1);
			quantity++;
		}
	}
	


		   //удаление
	int a;
	int b; //номер для удаления
	int kmas[50];
	cout << "Введите сколько хотите вычеркнуть людей из списка: ";
	cin >> a;
	for (int i = 0; i < a; i++) {
		cout << endl << "Введите номер человека, которого хотите исключить из списка: ";
		cin >> b;
		kmas[i] = b;
	}

	ofstream f1("f1.txt");
	
	bool g = false;
	for (int i = 0; i < quantity; i++) {


		for (int j = 0; j < quantity; j++) {

			if (i == kmas[j]) {
				g == true;
			}
			
		}
			if (g != true) {
		f1 << mas[i].surname << " " << mas[i].name << " " << mas[i].otchestvo << " " << mas[i].num_avto << " " << mas[i].num_pass << endl;
		}
		else
		g == false;

	}
}
