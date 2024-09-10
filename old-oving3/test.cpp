#include <algorithm> // for std::swap
#include <iostream>
#include <vector>

using namespace std;

struct SortTest {
    vector<int> vec;
    int checkSum;

    SortTest(vector<int> data, int checkSum) : vec(data), checkSum(checkSum) {}
};

bool isSorted(SortTest &testVector) {
    vector<int> &vec = testVector.vec;
    int sum = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        if (i < vec.size() - 1 && vec[i] > vec[i + 1])
            return false;

        sum += vec[i];
    }

    return sum == testVector.checkSum;
}

SortTest generateRandomVector(int n) {
    vector<int> vec(n);

    int range = 10000000;
    int sum = 0;

    for (int i = 0; i < n; i++) {
        vec[i] = rand() % range - range / 2;
        sum += vec[i];
    }

    return SortTest(vec, sum);
}

int partition(vector<int> &vec, int low, int high) {
    int pivot = vec[high];
    int i = low - 1;

    // Loop should run up to `high - 1`
    for (int j = low; j <= high - 1; j++) {
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quickSortMedian(vector<int> &vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high);
        quickSortMedian(vec, low, pi - 1);
        quickSortMedian(vec, pi + 1, high);
    }
}

void bubbleSort(vector<int> &vec, int low, int high) {
    bool swapped;
    for (int i = low; i <= high; i++) {
        swapped = false;
        for (int j = low; j < high - i; j++)
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
                swapped = true;
            }

        if (!swapped)
            break;
    }
}

int main() {
    vector<int> vec = {3, 6, 8, 10, 1, 2, 1};
    SortTest sortTest = generateRandomVector(1000);
    bubbleSort(sortTest.vec, 0, sortTest.vec.size() - 1);
    // for (size_t i = 0; i < sortTest.data.size(); i++) {
    //     cout << sortTest.data[i] << " ";
    // }

    cout << endl
         << (isSorted(sortTest)
                 ? "Sorted"
                 : "Not sorted")
         << endl;
    return 0;
}
