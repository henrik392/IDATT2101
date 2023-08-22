#include <iostream>

using namespace std;

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++)
    {
        int a;
        cin >> a;

        v[i] = a;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "a_" << i << ": " << v[i] << endl;
    }

    return 0;
}