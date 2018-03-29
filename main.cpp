#include <iostream>

using namespace std;

int main()
{

    
    int var = 10;
    int *ptr = &var;
    int **pptr = &ptr;

    cout << "var:    " << var << endl;
    cout << "&var:   " << &var << endl;
    cout << "\n\n";

    cout << "*ptr:   " << *ptr << endl;
    cout << "&ptr:   " << &ptr << endl;
    cout << "ptr:    " << ptr << endl;
    cout << "\n\n";

    cout << "*pptr:   " << *pptr << endl;
    cout << "&pptr:   " << &pptr << endl;
    cout << "pptr:    " << pptr << endl;
    cout << "**pptr:  " << *(*pptr) << endl;

        system("pause");
    return 0;
}