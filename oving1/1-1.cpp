#include <iostream>

using namespace std;

int main()
{
    if (getenv("DEBUG_MODE") && (string)getenv("DEBUG_MODE") == "1337")
    {
        (void)!freopen("input.txt", "r", stdin);
        (void)!freopen("output.txt", "w", stdout);
    }

    int n;
    cin >> n;

    int bestProfit = 0;
    int globalMin = 0;
    int price = 0;
    for (int i = 0; i < n; i++)
    {
        int priceChange;
        cin >> priceChange;

        price += priceChange;

        globalMin = min(globalMin, price);

        bestProfit = max(bestProfit, price - globalMin);
    }

    cout << bestProfit << endl;

    return 0;
}