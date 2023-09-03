#include <chrono>
#include <iostream>

using namespace std;

int multMetode1(int n, int x);
int multMetode2(int n, int x);
void tidtaking(int (*funksjon)(int, int), int n, int x);

int main() {
    cout << "Oving 2" << endl;
    int n_arr[] = {100, 1000, 10000, 100000};
    int x = 7;

    cout << "x = " << x << endl
         << endl;

    for (int n : n_arr) {
        cout << "n = " << n << endl;

        cout << "Metode 1: " << endl;
        tidtaking(multMetode1, n, x);
        cout << endl;

        cout << "Metode 2: " << endl;
        tidtaking(multMetode2, n, x);
        cout << endl;

        cout << "Kontroll: " << n * x << endl
             << endl;

        cout << "----------------------------------------" << endl
             << endl;
    }

    return 0;
}

int multMetode1(int n, int x) {
    if (n == 1)
        return x;

    return multMetode1(n - 1, x) + x;
}

int multMetode2(int n, int x) {
    if (n == 1)
        return x;

    if (n & 1)
        return multMetode2(n / 2, x + x) + x;

    return multMetode2(n / 2, x + x);
}

void tidtaking(int (*funksjon)(int, int), int n, int x) {
    int runder = 0;
    int verdi;

    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();

    do {
        verdi = funksjon(n, x);
        finish = chrono::high_resolution_clock::now();
        runder++;
    } while (chrono::duration_cast<chrono::duration<double>>(finish - start).count() < 1.0);

    cout << "Verdi: " << verdi << endl;
    auto elapsed = chrono::duration_cast<chrono::duration<double, std::micro>>(finish - start);
    cout << "Tid: " << round(elapsed.count() / runder * 100) / 100 << " mikrosekunder per runde" << endl;
}