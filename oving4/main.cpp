#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <vector>

using namespace std;

void oppgave1();
void oppgave2();

int main() {
    oppgave1();

    cout << endl;

    oppgave2();

    return 0;
}

template <typename KeyType>
class HashTable {
  protected:
    int size;
    int elements = 0;
    int collisions = 0;

    virtual int hashFunction(const KeyType &key) const = 0;

  public:
    HashTable(int size) : size(size) {}

    virtual void insert(const KeyType &key, int value) = 0;
    virtual int get(const KeyType &key) const = 0;

    double getLoadFactor() const {
        return static_cast<double>(elements) / size;
    }

    int getCollisions() const {
        return collisions;
    }

    double getCollisionsPerElement() const {
        return static_cast<double>(collisions) / elements;
    }

    virtual ~HashTable() = default;
};

class HashTableLinked : public HashTable<string> {
  private:
    struct LinkedNode {
        string key;
        int value;
        LinkedNode *next = nullptr;
    };
    LinkedNode *table;

    int hashFunction(const string &key) const override {
        int hash = 0;
        for (char c : key) {
            hash = (7 * hash + c) % size;
        }
        return hash;
    }

  public:
    HashTableLinked(int size) : HashTable<string>(size) {
        table = new LinkedNode[size];
    }

    void insert(const string &key, int value) override {
        elements++;
        int index = hashFunction(key);
        LinkedNode *node = new LinkedNode{key, value, table[index].next};

        if (table[index].next != nullptr) {
            collisions++;
            cout << "There has been a collision between \"" << key << "\" and \"" << table[index].next->key << "\"" << endl;
        }

        table[index].next = node;
    }

    int get(const string &key) const override {
        int index = hashFunction(key);
        LinkedNode *node = table[index].next;
        while (node != nullptr) {
            if (node->key == key) {
                return node->value;
            }
            node = node->next;
        }
        return -1;
    }

    ~HashTableLinked() {
        for (int i = 0; i < size; ++i) {
            LinkedNode *current = table[i].next;
            while (current != nullptr) {
                LinkedNode *temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
};

class DoubleHashingHashTable : public HashTable<int> {
  private:
    struct Entry {
        int key;
        int value;
        bool isOccupied = false;
    };

    Entry *table;

    int hashFunction(const int &key) const override {
        static const int PRIME = 31;
        return abs((key * PRIME) % size);
    }

    int hashFunction2(const int &key) const {
        static const int PRIME = 37;
        return PRIME - (key % PRIME);
    }

  public:
    DoubleHashingHashTable(int size) : HashTable<int>(size) {
        table = new Entry[size];
    }

    void insert(const int &key, int value) override {
        elements++;
        int index = hashFunction(key);
        int step = hashFunction2(key);

        while (table[index].isOccupied) {
            collisions++;
            index = (index + step) % size;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
    }

    int get(const int &key) const override {
        int index = hashFunction(key);
        int step = hashFunction2(key);

        while (table[index].isOccupied) {
            if (table[index].key == key) {
                return table[index].value;
            }
            index = (index + step) % size;
        }

        return -1;
    }

    ~DoubleHashingHashTable() {
        delete[] table;
    }
};

template <typename T>
void testHashTable(HashTable<T> &hashTable, const double fillPercent, int m, vector<T> &elements) {
    const int NUM_ELEMENTS = m * fillPercent;

    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        T element = elements[i];
        hashTable.insert(element, 1);
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Hashtabellen brukte " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms for å legg inn " << fillPercent * 100 << "% av m" << endl;
    cout << "Kollisjoner: " << hashTable.getCollisions() << endl;
    cout << "Kollisjoner per element: " << hashTable.getCollisionsPerElement() << endl;
    cout << "Lastfaktor: " << hashTable.getLoadFactor() << endl;
}

void oppgave1() {
    HashTableLinked table(173);

    ifstream file("navn.txt");

    if (!file.is_open()) {
        cout << "Could not open file." << endl;
        return;
    }

    vector<string> names;
    string name;
    while (getline(file, name)) {
        if (!name.empty()) {
            names.push_back(name);
        }
    }

    file.close();

    testHashTable<string>(table, 1, names.size(), names);

    bool found_all = true;
    for (string name : names) {
        found_all = found_all && table.get(name) == 1;
    }

    if (found_all) {
        cout << "Alle navn ble funnet i tabellen" << endl;
    } else {
        cout << "Ikke alle navn ble funnet i tabellen" << endl;
    }
}

vector<int> generateRandomUniqueNums(int n) {
    vector<int> nums;
    int prevNum = 0;
    for (int i = 0; i < n; i++) {
        prevNum += rand() % 100 + 1;
        nums.push_back(prevNum);
    }

    random_device rd;
    mt19937 g(rd());

    shuffle(nums.begin(), nums.end(), g);

    return nums;
}

void oppgave2() {
    // min: 10000000;
    const int NEAREST_PRIME = 10000019;
    int m = NEAREST_PRIME;
    // m = 65537;

    vector<int> nums = generateRandomUniqueNums(m);

    // LinearProbingHashTable linearProbingHashTable(m);
    // DoubleHashingHashTable doubleHashingHashTable(m);

    const double FILL_PERCENTAGE = 0.75;

    cout << "m = " << nums.size() << endl;

    cout << "\n------ Double hashing: ------" << endl;
    DoubleHashingHashTable doubleHashingHashTable(m);
    testHashTable<int>(doubleHashingHashTable, FILL_PERCENTAGE, m, nums);
    cout << endl;

    cout << "\n------ Standard c++ map<int>: ------" << endl;
    const int NUM_ELEMENTS = m * FILL_PERCENTAGE;
    map<int, int> map;

    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int element = nums[i];
        map.insert({element, 1});
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Standard c++ map<int> brukte " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms for å legg inn " << FILL_PERCENTAGE * 100 << "% av m" << endl;
}
