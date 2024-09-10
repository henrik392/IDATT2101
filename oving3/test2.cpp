#include <functional>
#include <iostream>
#include <random>
#include <vector>

class SortVerification {
  public:
    int checksum = 0;
    int *_arr;
    int _arrSize;

    SortVerification(int arr[], int size) : _arr(arr), _arrSize(size) {
        for (int i = 0; i < size; i++) {
            checksum += _arr[i];
        }
    }

    bool runTests() {
        if (!isSorted()) {
            std::cout << "Error: Array is not sorted!" << std::endl;
            return false;
        }

        if (!checksumIsCorrect()) {
            std::cout << "Error: Checksum has changed!" << std::endl;
            return false;
        }

        return true;
    }

  private:
    bool isSorted() const {
        int prevVal = _arr[0];

        for (int i = 0; i < _arrSize; i++) {
            if (prevVal > _arr[i]) {
                return false;
            }

            prevVal = _arr[i];
        }

        return true;
    }

    bool checksumIsCorrect() const {
        int tempChecksum = 0;

        for (int i = 0; i < _arrSize; i++) {
            tempChecksum += _arr[i];
        }

        if (tempChecksum != checksum) {
            return false;
        }

        return true;
    }
};

class TestSort {
  public:
    static bool test(const std::function<void(int[], int, int)> &func) {
        bool testFailed = false;

        if (!testPrevSorted(func)) {
            std::cout << "Failed to sort previously sorted array" << std::endl;
            testFailed = true;
        }

        if (!testDuplicates(func)) {
            std::cout << "Failed to sort array with duplicates" << std::endl;
            testFailed = true;
        }

        return !testFailed;
    }

  private:
    static bool testPrevSorted(const std::function<void(int[], int, int)> &func) {
        const int arrLen = 50;
        int arr[arrLen];

        for (int i = 0; i < arrLen; i++) {
            arr[i] = i;
        }

        SortVerification ver(arr, std::size(arr));
        func(arr, 0, arrLen - 1);
        return ver.runTests();
    }

    static bool testDuplicates(const std::function<void(int[], int, int)> &func) {
        const int arrLen = 50;
        int arr[arrLen];

        int duplicates[] = {2, 3, 5, 30, 31, 32, 39, 40, 45, 46, 49};

        int duplicateIndex = 0;
        for (int i = 0; i < arrLen; i++) {
            if (i > duplicates[duplicateIndex]) {
                duplicateIndex++;
            }
            arr[i] = duplicates[duplicateIndex];
        }

        SortVerification ver(arr, std::size(arr));
        func(arr, 0, arrLen - 1);
        return ver.runTests();
    }
};

int median3sort(int *arr, int v, int h) {
    int m = (v + h) / 2;

    if (arr[v] > arr[m]) {
        std::swap(arr[v], arr[m]);
    }

    if (arr[m] > arr[h]) {
        std::swap(arr[m], arr[h]);
        if (arr[v] > arr[m]) {
            std::swap(arr[v], arr[m]);
        }
    }
    return m;
}

int split(int *arr, int v, int h) {
    int iv;
    int ih;
    int m = median3sort(arr, v, h);
    int dv = arr[m];

    std::swap(arr[m], arr[h - 1]);
    for (iv = v, ih = h - 1;;) {
        while (arr[++iv] < dv)
            ;
        while (arr[--ih] > dv)
            ;
        if (iv >= ih)
            break;
        std::swap(arr[iv], arr[ih]);
    }
    std::swap(arr[iv], arr[h - 1]);
    return iv;
}

void quicksort(int *arr, int v, int h) {
    if (h - v > 2) {
        int divpos = split(arr, v, h);
        quicksort(arr, v, divpos - 1);
        quicksort(arr, divpos, h);
    } else {
        median3sort(arr, v, h);
    }
}

int dualPartition(int *arr, int l, int h, int *lp) {
    if (arr[l] > arr[h])
        std::swap(arr[l], arr[h]);

    int j = l + 1;
    int g = h - 1, k = l + 1, p = arr[l], q = arr[h];
    while (k <= g) {
        if (arr[k] < p) {
            std::swap(arr[k], arr[j]);
            j++;
        } else if (arr[k] >= q) {
            while (arr[g] > q && k < g)
                g--;
            std::swap(arr[k], arr[g]);
            g--;
            if (arr[k] < p) {
                std::swap(arr[k], arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;

    std::swap(arr[l], arr[j]);
    std::swap(arr[h], arr[g]);

    *lp = j;
    return g;
}

void dualPivotQuicksort(int *arr, int l, int h) {
    if (l < h) {
        int leftPartition;
        int rightPartition;

        rightPartition = dualPartition(arr, l, h, &leftPartition);
        dualPivotQuicksort(arr, l, leftPartition - 1);
        dualPivotQuicksort(arr, leftPartition + 1, rightPartition - 1);
        dualPivotQuicksort(arr, rightPartition + 1, h);
    }
}

void populateVec(std::vector<int> &vec) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist;
    for (auto &val : vec) {
        val = dist(gen);
    }
}

int main() {
    const int arrSize = 5000000;
    auto arr = std::make_unique<std::vector<int>>(arrSize);
    populateVec(*arr);
    TestSort::test(dualPivotQuicksort);
    SortVerification ver(arr->data(), arr->size());
    dualPivotQuicksort(arr->data(), 0, arrSize - 1);
    ver.runTests();

    std::vector<int> vec(10);
    std::cout << vec.size();
    populateVec(vec);

    for (auto &val : vec) {
        std::cout << val << " ";
    }

    std::cout << std::endl;

    dualPivotQuicksort(vec.data(), 0, vec.size() - 1);

    for (auto &val : vec) {
        std::cout << val << " ";
    }

    std::cout << std::endl;

    return 0;
}