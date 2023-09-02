#include <iostream>

using namespace std;

int main()
{
    freopen("input.txt", "w", stdout);

    int n = 10000;
    
    cout << n << endl;

    int max = 10;
    int min = -10;

    for (int i = 0; i < n; i++)
    {
        cout << rand() % (max - min + 1) + min << endl;
    }
}