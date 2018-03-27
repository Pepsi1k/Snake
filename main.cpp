#include <iostream>
#include <clocale>
#include <Windows.h>

using namespace std;

int main(){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    for (int i = 0; i < 10; i++)
        cout << "text";
        
    system("pause");
    return 0;
}