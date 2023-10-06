#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <unistd.h>

using namespace std;

struct ListNode {
    int index;
    ListNode *next;

    ListNode(int index) : index(index), next(nullptr) {}
};

struct TreeNode {
    char value;
    TreeNode *left;
    TreeNode *right;

    TreeNode(char value) : value(value), left(nullptr), right(nullptr) {}
};

int oppgave1(int n, int m) {
    // n = antall soldater, m = lengde på hopp

    // Creating circluar list, O(n)
    ListNode *head = new ListNode(1);
    ListNode *current = head;

    for (int i = 2; i <= n; i++) {
        current->next = new ListNode(i);
        current = current->next;
    }

    current->next = head;

    // Removing nodes from list until one left: O(m*(n-1)) = O(m*n), because n-1 nodes is removed and m nodes must be traveresed for each node to be deleted.

    int t = 0;
    ListNode *prev = current;
    current = head;
    while (current->next != current) {
        t++;

        if (t == m) {
            t = 0;
            prev->next = current->next;
            current = prev;
        }

        prev = current;
        current = current->next;
    }

    // Found the last soilder standing:
    return current->index;
}

bool oppgave2(const string &fileName) {
    map<char, char> closedToOpen = {
        {'}', '{'},
        {']', '['},
        {')', '('},
    };

    set<char> openBrackets = {'{', '[', '(', '"'};

    stack<char> lastOpenBrackets;

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return false;
    }

    bool inComment = false;
    bool inString = false;

    char c;
    while (file.get(c)) {
        if (inComment) {
            if (c == '\n')
                inComment = false;

            continue;
        }

        if (inString) {
            if (c == '"')
                inString = false;

            continue;
        }

        if (c == '/') {
            file.get(c);
            if (c == '/')
                inComment = true;

            continue;
        }

        if (c == '"') {
            inString = true;
            continue;
        }

        if (!lastOpenBrackets.empty() && lastOpenBrackets.top() == '"') {
            if (c == '"')
                lastOpenBrackets.pop();
            continue;
        }

        if (openBrackets.count(c)) {
            lastOpenBrackets.push(c);
        } else if (closedToOpen.count(c)) {
            if (lastOpenBrackets.empty() || lastOpenBrackets.top() != closedToOpen[c])
                return false;

            lastOpenBrackets.pop();
        }
    }

    file.close();
    return lastOpenBrackets.empty();
}

double verdiTilNode(TreeNode *node) {
    if (node->value == '+')
        return verdiTilNode(node->left) + verdiTilNode(node->right);
    else if (node->value == '-')
        return verdiTilNode(node->left) - verdiTilNode(node->right);
    else if (node->value == '*')
        return verdiTilNode(node->left) * verdiTilNode(node->right);
    else if (node->value == '/')
        return verdiTilNode(node->left) / verdiTilNode(node->right);
    else
        return node->value - '0';
}

string uttrykkFraNode(TreeNode *node) {
    // inorder
    if (node == nullptr)
        return "";

    return "(" + uttrykkFraNode(node->left) + node->value + uttrykkFraNode(node->right) + ")";

    // preorder
    string s = "";
    if (node->value == '+') {
        s += uttrykkFraNode(node->left) + " + " + uttrykkFraNode(node->right);
    } else if (node->value == '-') {
        s += uttrykkFraNode(node->left) + " - " + uttrykkFraNode(node->right);
    } else if (node->value == '*') {
        s += "(" + uttrykkFraNode(node->left) + ") * (" + uttrykkFraNode(node->right) + ")";
    } else if (node->value == '/') {
        s += "(" + uttrykkFraNode(node->left) + ") / (" + uttrykkFraNode(node->right) + ")";
    } else {
        s += node->value;
    }

    return s;
}

void oppgave3() {
    TreeNode *root = new TreeNode('/');

    TreeNode *teller = new TreeNode('*');

    teller->left = new TreeNode('2');

    teller->right = new TreeNode('-');
    teller->right->left = new TreeNode('1');
    teller->right->right = new TreeNode('3');

    TreeNode *nevner = new TreeNode('+');
    nevner->left = new TreeNode('4');

    nevner->right = new TreeNode('*');
    nevner->right->left = new TreeNode('2');
    nevner->right->right = new TreeNode('2');

    root->left = teller;
    root->right = nevner;

    cout << "Uttrykket til treet er: " << uttrykkFraNode(root) << endl;
    cout << "Verdien til treet er: " << verdiTilNode(root) << endl;
};

int main() {

    // Oppgave 1
    cout << "Oppgave 1:\n";
    cout << "Soilder number " << oppgave1(40, 3) << " is the last one standig." << endl;
    cout << "Soilder number " << oppgave1(40, 3) << " is the last one standig." << endl;

    // Oppgave 2, har også støtte for kommentarer
    cout << "\nOppgave 2:\n";
    cout << oppgave2("oving4/oppgave2test1.cpp") << endl;
    cout << oppgave2("oving4/oppgave2test2.cpp") << endl;

    cout << "\nOppgave3:\n";
    oppgave3();

    return 0;
}