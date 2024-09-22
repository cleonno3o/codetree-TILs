#include <iostream>
using namespace std;

int main() {
    int a = 1;
    int b = 2;
    int c = 3;
    a = b = c = 3;
    cout << a << ' ' << b << ' ' << c;
    return 0;
}