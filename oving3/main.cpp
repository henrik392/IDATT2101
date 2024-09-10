#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

const int MOD = 1e9 + 7;

using sort_function = void (*)(vector<int> &, int, int);
using sort_function_dual = void (*)(vector<int> &, int, int);

void runThresholdTestsOnAlgorithm(string algorithmName, sort_function sortFunction, int n);

void quickSort(vector<int> &vec, int low, int high);
void quickSortDualPivot(vector<int> &vec, int low, int high);
void quickSortInsertionHelper(vector<int> &vec, int low, int high);

const int THRESHOLD = 10;

int main() {
    srand(time(NULL));

    const int N = 50000;

    cout << "Oving 3" << endl
         << endl;

    runThresholdTestsOnAlgorithm("Quick Sort", quickSort, N);
    runThresholdTestsOnAlgorithm("Quick Sort med dual pivot", quickSortDualPivot, N);
    // runThresholdTestsOnAlgorithm("Quick Sort med dual pivot og InsertionHelper", quickSortInsertionHelper, N);

    return 0;
}

struct SortTest {
    vector<int> data;
    int checkSum;

    SortTest(vector<int> data, int checkSum) : data(data), checkSum(checkSum) {}
};

SortTest generateRandomSortTest(int n) {
    vector<int> vec(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        vec[i] = dist(gen);
        sum = (sum + vec[i] % MOD) % MOD;
    }

    return SortTest(vec, sum);
}

SortTest generateDupeSortTest(int n) {
    // Annenhvert tall er 42

    vector<int> vec(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        switch (i & 1) {
        case 0:
            vec[i] = 42;
            break;
        case 1:
            vec[i] = dist(gen);
            break;
        }
        sum = (sum + vec[i] % MOD) % MOD;
    }

    return SortTest(vec, sum);
}

SortTest generateSortedSortTest(int n) {
    // Sortert liste fra -n/2 til n/2

    vector<int> vec(n);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        vec[i] = i - n / 2;
        sum = (sum + vec[i] % MOD) % MOD;
    }

    return SortTest(vec, sum);
}

SortTest generateReverseSortTest(int n) {
    // Sortert liste fra n/2 til -n/2

    vector<int> vec(n);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        vec[i] = n / 2 - i;
        sum = (sum + vec[i] % MOD) % MOD;
    }

    return SortTest(vec, sum);
}

bool isSorted(SortTest sortTest) {
    vector<int> &vec = sortTest.data;
    int sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (i < vec.size() - 1 && vec[i] > vec[i + 1]) {
            cout << "Feil: " << i << " " << vec[i] << " " << vec[i + 1] << endl;
            return false;
        }

        sum = (sum + vec[i] % MOD) % MOD;
    }

    if (sum != sortTest.checkSum) {
        cout << "Feil sum: " << sum << " " << sortTest.checkSum << endl;
    }
    return sum == sortTest.checkSum;
}

int median3sort(vector<int> &vec, int low, int high) {
    int mid = (low + high) / 2;
    if (vec[low] > vec[high])
        swap(vec[low], vec[high]);
    if (vec[low] > vec[mid])
        swap(vec[low], vec[mid]);
    if (vec[mid] > vec[high])
        swap(vec[mid], vec[high]);

    return mid;
}

int partition(vector<int> &vec, int low, int high) {
    // cout << low << " " << high << endl;
    int pivotIndex = median3sort(vec, low, high);
    int pivotValue = vec[pivotIndex];
    swap(vec[pivotIndex], vec[high - 1]);

    int iLow = low;
    int iHigh = high - 1;
    while (true) {
        while (vec[++iLow] < pivotValue)
            ;
        while (vec[--iHigh] > pivotValue)
            ;
        if (iLow >= iHigh)
            break;

        swap(vec[iLow], vec[iHigh]);
    }

    swap(vec[iLow], vec[high - 1]);
    return iLow;
}

void quickSort(vector<int> &vec, int low, int high) {
    // low og high er inklusive

    if (high - low <= 2) {
        median3sort(vec, low, high);
        return;
    }

    int partitionIndex = partition(vec, low, high);
    quickSort(vec, low, partitionIndex - 1);
    quickSort(vec, partitionIndex + 1, high);
}

void partitionDualPivot(vector<int> &vec, int l, int h, int &lp, int &rp) {
    if (vec[l] > vec[h])
        std::swap(vec[l], vec[h]);

    int j = l + 1;
    int g = h - 1, k = l + 1, p = vec[l], q = vec[h];
    while (k <= g) {
        if (vec[k] < p) {
            std::swap(vec[k], vec[j]);
            j++;
        } else if (vec[k] >= q) {
            while (vec[g] > q && k < g)
                g--;
            std::swap(vec[k], vec[g]);
            g--;
            if (vec[k] < p) {
                std::swap(vec[k], vec[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    std::swap(vec[l], vec[j]);
    std::swap(vec[h], vec[g]);

    lp = j;
    rp = g;
}

void __partitionDualPivot(vector<int> &vec, int low, int high, int &leftPartitionIndex, int &rightPartitionIndex) {
    cout << "2: " << low << " " << high << endl;
    // if (vec[low] > vec[high])
    //     swap(vec[low], vec[high]);

    int leftPivotIndex = low + (high - low) / 3;
    int rightPivotIndex = high - (high - low) / 3;
    if (vec[leftPivotIndex] > vec[rightPivotIndex])
        swap(vec[leftPivotIndex], vec[rightPivotIndex]);

    int leftPivotValue = vec[leftPivotIndex];
    int rightPivotValue = vec[rightPivotIndex];
    swap(vec[leftPivotIndex], vec[high - 1]);
    swap(vec[rightPivotIndex], vec[high]);

    // Inklusiv low og high
    int iLow = low;
    int iHigh = high - 2;
    while (true) {
        while (vec[iLow] < leftPivotValue) {
            iLow++;
        }
        // iLow er nå på en plass hvor vec[iLow] er lik eller større enn leftPivotValue
        // alt under iLow er mindre enn leftPivotValue

        while (vec[iHigh] >= leftPivotValue && iHigh > low) {
            iHigh--;
        }
        // iHigh er nå på en plass hvor vec[iHigh] er strengt mindre enn leftPivotValue
        // alt over iHigh er større eller lik leftPivotValue

        if (iLow >= iHigh) {
            // Dette vil alltid skje fordi enten den første eller den andre while-løkken må kjøre minst en gang
            break;
        }

        // Plassene til iLow og iHigh byttes om slik at alt under iLow er mindre enn leftPivotValue og alt over iHigh er større eller lik leftPivotValue
        swap(vec[iLow], vec[iHigh]);
    }

    // Bytter med temp plassen jeg har laget i high - 1,
    // siden iLow lander på et element som er større eller lik leftPivotValue er det greit å bytte denne ut med high - 1
    swap(vec[iLow], vec[high - 1]);

    leftPartitionIndex = iLow;

    // --- Right partition ---
    // Vi har lyst til å finne en plass hvor alt under er mindre enn eller lik rightPivotValue og alt over er strengt større enn rightPivotValue
    swap(vec[high], vec[leftPartitionIndex + 1]);
    iLow = leftPartitionIndex + 2;
    iHigh = high;
    while (true) {
        while (vec[iLow] <= rightPivotValue && iLow < high) {
            iLow++;
        }
        // iLow er nå på en plass hvor vec[iLow] er strengt større enn rightPivotValue
        // alt under iLow er mindre eller lik rightPivotValue

        while (vec[iHigh] > rightPivotValue) {
            iHigh--;
        }
        // iHigh er nå på en plass hvor vec[iHigh] er mindre eller lik rightPivotValue
        // alt over iHigh er større enn rightPivotValue

        if (iLow >= iHigh) {
            break;
        }

        swap(vec[iLow], vec[iHigh]);
    }

    // Bytter med temp plassen jeg har laget i high
    swap(vec[iHigh], vec[leftPartitionIndex + 1]);

    rightPartitionIndex = iHigh;
}

void quickSortDualPivot(vector<int> &vec, int low, int high) {
    if (low >= high) {
        return;
    }

    if (high - low <= 2) {
        median3sort(vec, low, high);
        return;
    }

    int leftPartitionIndex = 0;
    int rightPartitionIndex = 0;
    partitionDualPivot(vec, low, high, leftPartitionIndex, rightPartitionIndex);

    quickSortDualPivot(vec, low, leftPartitionIndex - 1);
    if (vec[leftPartitionIndex] != vec[rightPartitionIndex])
        quickSortDualPivot(vec, leftPartitionIndex + 1, rightPartitionIndex - 1);
    quickSortDualPivot(vec, rightPartitionIndex + 1, high);
}

void insertionSort(vector<int> &vec, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = vec[i];
        int j = i - 1;

        while (j >= low && vec[j] > key) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = key;
    }
}

void quickSortInsertionHelper(vector<int> &vec, int low, int high) {
    if (high - low <= 2) {
        median3sort(vec, low, high);
        return;
    }

    if (high - low <= THRESHOLD) {
        insertionSort(vec, low, high);
        return;
    }

    int leftPartitionIndex = 0;
    int rightPartitionIndex = 0;
    partitionDualPivot(vec, low, high, leftPartitionIndex, rightPartitionIndex);

    quickSortInsertionHelper(vec, low, leftPartitionIndex - 1);
    if (vec[leftPartitionIndex] != vec[rightPartitionIndex]) {
        quickSortInsertionHelper(vec, leftPartitionIndex + 1, rightPartitionIndex - 1);
    }
    quickSortInsertionHelper(vec, rightPartitionIndex + 1, high);
}

double getSpeedInMS(sort_function sortFunction, SortTest &sortTest) {
    auto start = chrono::high_resolution_clock::now();
    sortFunction(sortTest.data, 0, sortTest.data.size() - 1);
    auto finish = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::duration<double>>(finish - start).count() * 1000;
}

string vecIsSortedSymbol(bool correct) {
    return correct ? "✓" : "✗";
}

void printCell(double algorithmSpeed, bool isSorted) {
    cout << setprecision(2) << fixed << algorithmSpeed << " ms " << vecIsSortedSymbol(isSorted) << "\t";
}

void runThresholdTestsOnAlgorithm(string algorithmName, sort_function sortFunction, int n) {
    cout << "--------- " << algorithmName << " ---------" << endl
         << endl;

    cout
        << "quickSortRand"
        << "\t"
        << "quickSortDupes"
        << "\t"
        << "quickSortSorted"
        << "\t" << "quickSortReverse" << endl
        << endl;

    SortTest randomSortTest = generateRandomSortTest(n);
    SortTest dupeSortTest = generateDupeSortTest(n);
    SortTest sortedSortTest = generateSortedSortTest(n);
    SortTest reverseSortTest = generateReverseSortTest(n);

    printCell(getSpeedInMS(sortFunction, randomSortTest), isSorted(randomSortTest));
    printCell(getSpeedInMS(sortFunction, dupeSortTest), isSorted(dupeSortTest));
    printCell(getSpeedInMS(sortFunction, sortedSortTest), isSorted(sortedSortTest));
    printCell(getSpeedInMS(sortFunction, reverseSortTest), isSorted(reverseSortTest));

    cout << endl
         << endl;
}