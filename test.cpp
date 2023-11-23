#include <iostream>

using namespace std;

int main() {
    string str;
    str = "Hello World!";
    str += string("Hello!", 3);

    cout << str;
    return 0;
}