#include <chrono>
#include <iostream>

using namespace std;

void testMethods();

double eksponentMetode1(double x, int n);
double eksponentMetode2(double x, int n);
double eksponentMetode3(double x, int n);

typedef double (*eksponent_method)(double, int);

void tidtaking(eksponent_method method, double x, int n);

int main() {
    cout << "Oving 2\n\n";
    testMethods();

    int n_arr[] = {100, 1000, 10000, 100000};
    double x = 1.002;

    cout << "x = " << x << endl
         << endl;

    for (int n : n_arr) {
        cout << "n = " << n << endl;

        cout << "Metode 1: " << endl;
        tidtaking(eksponentMetode1, x, n);
        cout << endl;

        cout << "Metode 2: " << endl;
        tidtaking(eksponentMetode2, x, n);
        cout << endl;

        cout << "Metode 3: " << endl;
        tidtaking(eksponentMetode3, x, n);
        cout << endl;

        cout << "Kontroll: " << pow(x, n) << endl
             << endl;

        cout << "----------------------------------------" << endl
             << endl;
    }

    return 0;
}

void verifyMethod(eksponent_method method, string methodName, double x, int n, double ans) {
    double res = method(x, n);
    printf("%s(%f, %d) = %f is ", methodName.c_str(), x, n, res);
    if (res == ans) {
        cout << "Correct" << endl;
    } else {
        cout << "Incorrect" << endl;
    }
}

void testMethods() {
    cout << "Verifying methods:" << endl;
    double x = 5.0;
    int n = 11;
    double ans = 48828125;
    printf("%f^%d = %f\n", x, n, ans);

    verifyMethod(eksponentMetode1, "eksponentMetode1", x, n, ans);
    verifyMethod(eksponentMetode2, "eksponentMetode2", x, n, ans);
    verifyMethod(eksponentMetode3, "eksponentMetode3", x, n, ans);

    cout << "----------------------------------------" << endl
         << endl;
}

double eksponentMetode1(double x, int n) {
    if (n == 1)
        return x;

    return x * eksponentMetode1(x, n - 1);
}

double eksponentMetode2(double x, int n) {
    if (n == 1)
        return x;

    if (n & 1)
        return x * eksponentMetode2(x * x, (n - 1) / 2);

    return eksponentMetode2(x * x, n / 2);
}

double eksponentMetode3(double x, int n) {
    return pow(x, n);
}

void tidtaking(eksponent_method method, double x, int n) {
    int runder = 0;
    double verdi;

    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();

    do {
        verdi = method(x, n);
        finish = chrono::high_resolution_clock::now();
        runder++;
    } while (chrono::duration_cast<chrono::duration<double>>(finish - start).count() < 1.0);

    cout << "Verdi: " << verdi << endl;
    auto elapsed = chrono::duration_cast<chrono::duration<double, std::micro>>(finish - start);
    cout << "Tid: " << round(elapsed.count() / runder * 100) / 100 << " mikrosekunder per runde" << endl;
}