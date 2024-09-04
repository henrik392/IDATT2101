#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using sort_function = void (*)(vector<int> &, int, int);

void runThresholdTestsOnAlgorithm(sort_function sortFunction, int n);

void quickSortMedian(vector<int> &vec, int low, int high);
void quickSort3Way(vector<int> &vec, int low, int high);
void quickSort3WayBubbleHelper(vector<int> &vec, int low, int high);
void quickSort3WayInsertionHelper(vector<int> &vec, int low, int high);

int threshold = 10;

int main() {
    srand(time(NULL));

    // vector<int> N = {100, 1000, 10000, 100000, 1000000};
    static int n = 1000000;

    cout << "Oving 3" << endl
         << endl;

    // runThresholdTestsOnAlgorithm(quickSortMedian, n);

    // cout << endl;

    // runThresholdTestsOnAlgorithm(quickSort3Way, n);

    cout << "\n\nbubbleThreshold = " << threshold << " for bobblesortering:" << endl
         << endl;

    runThresholdTestsOnAlgorithm(quickSort3WayBubbleHelper, n);

    cout << "\ninsertionThreshold = " << threshold << " for insettingssortering:" << endl
         << endl;

    runThresholdTestsOnAlgorithm(quickSort3WayInsertionHelper, n);

    return 0;
}

struct SortTest {
    vector<int> data;
    int checkSum;

    SortTest(vector<int> data, int checkSum) : data(data), checkSum(checkSum) {}
};

SortTest generateRandomSortTest(int n) {
    vector<int> vec(n);

    int range = 10000;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        vec[i] = rand() % range - range / 2;
        sum += vec[i];
    }

    return SortTest(vec, sum);
}

SortTest generateDupeSortTest(int n) {
    vector<int> vec(n);

    int range = 10000;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        switch (i & 1) {
        case 0:
            vec[i] = 42;
            break;
        case 1:
            vec[i] = rand() % range - range / 2;
            break;
        }
        sum += vec[i];
    }

    return SortTest(vec, sum);
}

SortTest generateSortedSortTest(int n) {
    vector<int> vec(n);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        vec[i] = i - n / 2;
        sum += vec[i];
    }

    return SortTest(vec, sum);
}

bool isSorted(SortTest sortTest) {
    vector<int> &vec = sortTest.data;
    int sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (i < vec.size() - 1 && vec[i] > vec[i + 1])
            return false;

        sum += vec[i];
    }

    return sum == sortTest.checkSum;
}

int medianOfThree(vector<int> &vec, int low, int high) {
    int mid = low + (high - low) / 2;
    if (vec[low] > vec[high])
        swap(vec[low], vec[high]);
    if (vec[low] > vec[mid])
        swap(vec[low], vec[mid]);
    if (vec[mid] > vec[high])
        swap(vec[mid], vec[high]);

    return mid;
}

int partition(vector<int> &vec, int low, int high) {
    int pivotIndex = medianOfThree(vec, low, high);
    swap(vec[pivotIndex], vec[high]);
    int pivot = vec[high];

    int i = low;
    for (int j = low; j <= high - 1; j++) {
        if (vec[j] < pivot) {
            swap(vec[i], vec[j]);
            i++;
        }
    }

    swap(vec[i], vec[high]);
    return i;
}

void threeWayPartition(vector<int> &vec, int low, int high, int &lt, int &gt) {
    int pivotIndex = medianOfThree(vec, low, high);
    swap(vec[pivotIndex], vec[high]);
    int pivot = vec[high];

    lt = low;
    gt = high;
    int i = low;

    while (i <= gt) {
        if (vec[i] < pivot) {
            swap(vec[lt++], vec[i++]);
        } else if (vec[i] > pivot) {
            swap(vec[i], vec[gt--]);
        } else {
            i++;
        }
    }
}

void bubbleSort(vector<int> &vec, int low, int high) {
    bool swapped;
    for (int i = low; i <= high; i++) {
        swapped = false;
        for (int j = low; j < high - i + low; j++)
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
                swapped = true;
            }

        if (!swapped)
            break;
    }
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

void quickSortMedian(vector<int> &vec, int low, int high) {
    if (low >= high)
        return;

    int pi = partition(vec, low, high);
    quickSortMedian(vec, low, pi - 1);
    quickSortMedian(vec, pi + 1, high);
}

void quickSort3Way(vector<int> &vec, int low, int high) {
    if (low >= high)
        return;

    int lt, gt;
    threeWayPartition(vec, low, high, lt, gt);
    quickSort3Way(vec, low, lt - 1);
    quickSort3Way(vec, gt + 1, high);
}

void quickSort3WayBubbleHelper(vector<int> &vec, int low, int high) {
    if (low >= high)
        return;

    if (high - low <= threshold) {
        bubbleSort(vec, low, high);
        return;
    }

    int lt, gt;
    threeWayPartition(vec, low, high, lt, gt);
    quickSort3WayBubbleHelper(vec, low, lt - 1);
    quickSort3WayBubbleHelper(vec, gt + 1, high);
}

void quickSort3WayInsertionHelper(vector<int> &vec, int low, int high) {
    if (low >= high)
        return;

    if (high - low <= threshold) {
        insertionSort(vec, low, high);
        return;
    }

    int lt, gt;
    threeWayPartition(vec, low, high, lt, gt);
    quickSort3WayInsertionHelper(vec, low, lt - 1);
    quickSort3WayInsertionHelper(vec, gt + 1, high);
}

double getSpeedInMS(sort_function sortFunction, SortTest &sortTest) {
    auto start = chrono::high_resolution_clock::now();
    sortFunction(sortTest.data, 0, sortTest.data.size() - 1);
    auto finish = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::duration<double>>(finish - start).count() * 1000;
}

string vecIsSortedSymbol(bool correct) {
    return correct ? " ✓" : " ✗";
}

void printCell(sort_function sortFunction, SortTest &sortTest) {
    cout << "\t\t" << setprecision(6) << fixed << getSpeedInMS(sortFunction, sortTest) << vecIsSortedSymbol(isSorted(sortTest));
}

void runThresholdTestsOnAlgorithm(sort_function sortFunction, int n) {
    cout << "threshold"
         << "\t\t"
         << "quickSortRand"
         << "\t\t"
         << "quickSortDupes"
         << "\t\t"
         << "quickSortSorted" << endl
         << endl;

    // for (threshold = 1; threshold < n / 100; threshold *= 2) {
    SortTest randomSortTest = generateRandomSortTest(n);
    SortTest dupeSortTest = generateDupeSortTest(n);

    cout << left << threshold << right;
    printCell(sortFunction, randomSortTest);
    printCell(sortFunction, dupeSortTest);
    printCell(sortFunction, randomSortTest);
    cout << endl;
    // }
}

// void runNTestsOnAlgorithm(vector<int> N, sort_function sortFunction, string helperName = "") {
//     cout << "n"
//          << "\t\t"
//          << "quickSortRand"
//          << "\t\t"
//          << "quickSortDupes"
//          << "\t\t"
//          << "quickSortSorted" << endl
//          << endl;

//     for (int n : N) {
//         SortTest randomSortTest = generateRandomSortTest(n);
//         SortTest dupeSortTest = generateDupeSortTest(n);

//         cout << left << n << right;
//         printCell(sortFunction, randomSortTest);
//         printCell(sortFunction, dupeSortTest);
//         printCell(sortFunction, randomSortTest);
//         cout << endl;
//     }
// }