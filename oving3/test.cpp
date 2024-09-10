#include <algorithm> // for std::swap
#include <iostream>
#include <vector>

using namespace std;

struct SortTest {
    vector<int> data;
    int checkSum;

    SortTest(vector<int> data, int checkSum) : data(data), checkSum(checkSum) {}
};

bool isSorted(SortTest &testVector) {
    vector<int> &vec = testVector.data;
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

void partitionDualPivot(vector<int> &vec, int low, int high, int &leftPartitionIndex, int &rightPartitionIndex) {
    int pivot1 = vec[low + (high - low) / 3];
    int pivot2 = vec[high - (high - low) / 3];
    if (pivot1 > pivot2)
        swap(pivot1, pivot2);

    vector<int> low;
    vector<int> mid;
    vector<int> high;

    for (int i = low; i <= high; i++) {
        if (vec[i] < pivot1) {
            low.push_back(vec[i]);
        } else if (vec[i] > pivot2) {
            high.push_back(vec[i]);
        } else {
            mid.push_back(vec[i]);
        }
    }

    for (int i = 0; i < low.size(); i++) {
        vec[i] = low[i];
    }
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

int main() {
    vector<int> vec = {3, 6, 8, 10, 1, 2, 1};
    SortTest sortTest = generateRandomVector(1000);
    quickSortDualPivot(sortTest.data, 0, sortTest.data.size() - 1);

    cout << endl
         << (isSorted(sortTest)
                 ? "Sorted"
                 : "Not sorted")
         << endl;
    return 0;
}
