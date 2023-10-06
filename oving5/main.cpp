#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

using namespace std;

class HashTableLinked {
  private:
    struct LinkedNode {
        string key;
        int value;
        LinkedNode *next = nullptr;
    };
    int size;
    int elements = 0;
    int collisions = 0;
    LinkedNode *table;

    int hashFunction(string key) {
        int hash = 0;
        for (size_t i = 0; i < key.length(); i++) {
            hash += key[i];
        }
        return hash % size;
    };

  public:
    HashTableLinked(int size) : size(size) {
        table = new LinkedNode[size];
    }

    void insert(string key, int value) {
        elements++;
        int index = hashFunction(key);
        LinkedNode *node = new LinkedNode;

        if (table[index].next != nullptr) {
            collisions++;
            cout << "There has been a collision between \"" << key << "\" and \"" << table[index].next->key << "\"" << endl;
        }

        node->key = key;
        node->value = value;
        node->next = table[index].next;
        table[index].next = node;
    }

    int get(string key) {
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

    double getLoadFactor() {
        return (double)elements / size;
    }

    int getCollisions() {
        return collisions;
    }

    double getCollisionsPerElement() {
        return (double)collisions / elements;
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

class HashTable {
  protected:
    struct Entry {
        int key;
        int value;
        bool isOccupied = false;
    };

    Entry *table;
    int size;

    virtual int hashFunction(int key) {
        static int PRIME = 31;

        return abs((key * PRIME) % size);
    };

    int collisions = 0;

  public:
    HashTable(int size) : size(size) {
        table = new Entry[size];
    }

    virtual void insert(int key, int value) {
        cout << "key: " << key << endl
             << "value: " << value << endl;
        cout << "Insert is not defined for this hash table.\n";
    };

    virtual int get(int key) {
        cout << "key: " << key << endl;
        cout << "Get is not defined for this hash table.\n";
        return -1;
    };

    int getCollisions() const {
        return collisions;
    }

    virtual ~HashTable() {
        delete[] table;
    }
};

class LinearProbingHashTable : public HashTable {
  public:
    LinearProbingHashTable(int size) : HashTable(size) {}

    void insert(int key, int value) override {
        int index = hashFunction(key);
        while (table[index].isOccupied) {
            collisions++;
            index = (index + 1) % size;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
    };

    int get(int key) override {
        int index = hashFunction(key);
        while (table[index].isOccupied) {
            if (table[index].key == key) {
                return table[index].value;
            }
            index = (index + 1) % size;
        }

        return -1;
    };
};

class DoubleHashingHashTable : public HashTable {
  private:
    int hashFunction2(int key) {
        static int PRIME = 37;

        return PRIME - (key % PRIME);
    };

  public:
    DoubleHashingHashTable(int size) : HashTable(size) {}

    void insert(int key, int value) override {
        int index = hashFunction(key);
        int step = hashFunction2(key);

        while (table[index].isOccupied) {
            collisions++;
            index = (index + step) % size;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
    };

    int get(int key) override {
        int index = hashFunction(key);
        int step = hashFunction2(key);

        while (table[index].isOccupied) {
            if (table[index].key == key) {
                return table[index].value;
            }
            index = (index + step) % size;
        }

        return -1;
    };
};

void oppgave1() {
    HashTableLinked table(173);

    ifstream file("oving5/navn.txt");

    if (!file.is_open()) {
        cout << "Could not open file." << endl;
        return;
    }

    string name;
    while (getline(file, name)) {
        if (!name.empty()) {
            table.insert(name, 1);
        }
    }

    file.close();

    cout << "Lastfaktor: " << table.getLoadFactor() << endl;
    cout << "Kollisjoner: " << table.getCollisions() << endl;
    cout << "Kollisjoner per person: " << table.getCollisionsPerElement() << endl;

    cout << table.get("Knut") << endl;
    cout << table.get("Erik") << endl;
    cout << table.get("Henrik Halvorsen Kvamme") << endl;
    cout << table.get("Nils Petter Farstad") << endl;
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

void testHashTable(HashTable &hashTable, double fillPercent, int m, vector<int> &nums) {
    int elements = m * fillPercent;

    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    for (int i = 0; i < elements; i++) {
        int num = nums[i];
        hashTable.insert(num, 1);
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "The hashtable used " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms to insert " << fillPercent * 100 << "% of m" << endl;
    cout << "Collisions: " << hashTable.getCollisions() << endl;
}

void oppgave2() {
    // min: 10000000;
    static int NEAREST_PRIME = 10000019;
    // static int NEAREST_PRIME = 999959;
    int m = NEAREST_PRIME;
    // m = 65537;

    vector<int> nums = generateRandomUniqueNums(m);

    // LinearProbingHashTable linearProbingHashTable(m);
    // DoubleHashingHashTable doubleHashingHashTable(m);

    double fillPercentages[] = {
        0.5,
        0.8,
        0.9,
        0.99,
        1};

    cout << "m = " << nums.size() << endl;

    cout << "Linear probing:" << endl;
    for (double fillPercentage : fillPercentages) {
        if (fillPercentage == 1)
            continue; // Programmet bruker for lang tid

        LinearProbingHashTable linearProbingHashTable(m);
        testHashTable(linearProbingHashTable, fillPercentage, m, nums);
        cout << endl;
    }

    cout << "\nDouble hashing:" << endl;
    for (double fillPercentage : fillPercentages) {
        DoubleHashingHashTable doubleHashingHashTable(m);
        testHashTable(doubleHashingHashTable, fillPercentage, m, nums);
        cout << endl;
    }
}

int main() {
    oppgave1();

    cout << endl;

    oppgave2();

    return 0;
}