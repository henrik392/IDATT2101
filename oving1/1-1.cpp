#include <iostream>
#include <chrono>

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

    auto start = chrono::high_resolution_clock::now();

    int bestProfit = 0;
    int globalMin = 0;
    int minDay = 0;
    int buyAtDay = 0;
    int sellAtDay = 0;
    int price = 0;
    for (int i = 0; i < n; i++)
    {
        int priceChange;
        cin >> priceChange;

        price += priceChange;

        if (price < globalMin)
        {
            globalMin = price;
            minDay = i;
        }

        if (price - globalMin > bestProfit)
        {
            bestProfit = price - globalMin;
            buyAtDay = minDay;
            sellAtDay = i;
        }

        bestProfit = max(bestProfit, price - globalMin);
    }

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "You make the best profit by buying at day " << buyAtDay << " and selling at day " << sellAtDay << ".\nYour profit is then " << bestProfit << endl;

    cout << "Time taken with n=" << n << " is " << duration.count() << " microseconds." << endl;

    return 0;
}