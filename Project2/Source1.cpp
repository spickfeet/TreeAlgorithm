#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
using namespace std;


// Способ с массивом

const int ALPHABET_SIZE = 26;

struct TrieNodeArray {
    TrieNodeArray* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNodeArray() : isEndOfWord(false) {
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
};

// Функция для подсчета используемой памяти
size_t calculateMemoryUsage(TrieNodeArray* node) {
    if (!node) return 0;
    size_t size = sizeof(*node); // Память для текущего узла
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        size += calculateMemoryUsage(node->children[i]); // Рекурсивно добавляем детей
    }
    return size;
}



TrieNodeArray* root = nullptr;  // Глобальная переменная для корня

TrieNodeArray* getNode() {
    return new TrieNodeArray();
}

void insert(TrieNodeArray* root, const string& key) {
    TrieNodeArray* pCrawl = root;
    for (char c : key) {
        int index = c - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
}

// Общее количество узлов (без корня)
int countTotalNodes(TrieNodeArray* node) {
    if (!node) return 0;
    int count = 0;  // Не считаем текущий узел если это корень
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count += countTotalNodes(node->children[i]);
    }
    return node == root ? count : count + 1;  // Пропускаем корень
}

// Количество слов (листовых вершин)
int countLeafWords(TrieNodeArray* node) {
    if (!node) return 0;
    int count = node->isEndOfWord ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count += countLeafWords(node->children[i]);
    }
    return count;
}

// Количество внутренних вершин (не корень и не листья)
int countInternalNodes(TrieNodeArray* node, bool isRoot = true) {
    if (!node) return 0;

    bool hasChildren = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            hasChildren = true;
            break;
        }
    }

    int count = (!isRoot && hasChildren) ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count += countInternalNodes(node->children[i], false);
    }
    return count;
}

// Количество ветвлений (узлов с >1 ребенком, кроме корня)
int countBranchingNodes(TrieNodeArray* node, bool isRoot = true) {
    if (!node) return 0;

    int childCount = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) childCount++;
    }

    int count = (!isRoot && childCount > 1) ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        count += countBranchingNodes(node->children[i], false);
    }
    return count;
}

bool hasAnyWord(TrieNodeArray* node) {
    if (!node) return false;
    if (node->isEndOfWord) return true;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hasAnyWord(node->children[i])) return true;
    }
    return false;
}

pair<int, int> countRealBranchings(TrieNodeArray* node, bool isRoot = true) {
    if (!node) return { 0, 0 };

    int childCount = 0;
    vector<TrieNodeArray*> activeChildren;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            childCount++;
            activeChildren.push_back(node->children[i]);
        }
    }

    bool isRealBranch = false;
    if (childCount > 1 && !isRoot) {
        int wordPaths = 0;
        for (auto child : activeChildren) {
            if (child->isEndOfWord) wordPaths++;
            if (hasAnyWord(child)) wordPaths++;
            if (wordPaths >= 2) {
                isRealBranch = true;
                break;
            }
        }
    }

    int sum = 0, count = 0;
    if (isRealBranch) {
        sum = childCount;
        count = 1;
    }

    for (auto child : activeChildren) {
        auto childStats = countRealBranchings(child, false);
        sum += childStats.first;
        count += childStats.second;
    }

    return { sum, count };
}

double calculateAvgBranching(TrieNodeArray* root) {
    auto stats = countRealBranchings(root);
    return (stats.second == 0) ? 0.0 :
        static_cast<double>(stats.first) / stats.second;
}

// Способ с листом

class TrieNode;

struct ListNode {
    char ch;
    TrieNode* next;
    ListNode* nextListNode;

    ListNode(char c) : ch(c), next(nullptr), nextListNode(nullptr) {}
};

class TrieNode {
public:
    ListNode* head;
    bool isEndOfWord;

    TrieNode() : head(nullptr), isEndOfWord(false) {}

    TrieNode* getChild(char ch) {
        ListNode* current = head;
        while (current) {
            if (current->ch == ch) {
                return current->next;
            }
            current = current->nextListNode;
        }
        return nullptr;
    }

    void addChild(char ch) {
        if (!head) {
            head = new ListNode(ch);
            head->next = new TrieNode();
        }
        else {
            ListNode* current = head;
            while (current->nextListNode) {
                if (current->ch == ch) {
                    return;
                }
                current = current->nextListNode;
            }
            current->nextListNode = new ListNode(ch);
            current->nextListNode->next = new TrieNode();
        }
    }
};

class Trie {
private:

    int countChars(TrieNode* node) {
        if (!node) return 0;

        int count = 0;
        ListNode* current = node->head;
        while (current) {
            count += 1 + countChars(current->next);
            current = current->nextListNode;
        }
        return count;
    }

    int countWords(TrieNode* node) {
        if (!node) return 0;

        int count = node->isEndOfWord ? 1 : 0;
        ListNode* current = node->head;
        while (current) {
            count += countWords(current->next);
            current = current->nextListNode;
        }
        return count;
    }

    int countInternalNodes(TrieNode* node, bool isRoot = true) {
        if (!node || !node->head) return 0;

        int count = isRoot ? 0 : 1;
        ListNode* current = node->head;

        while (current) {
            count += countInternalNodes(current->next, false);
            current = current->nextListNode;
        }

        return count;
    }

    int countBranchingNodes(TrieNode* node) {
        if (!node || !node->head) return 0;

        int count = 0;
        if (node->head && node->head->nextListNode && node != root) {
            count = 1;
        }

        ListNode* current = node->head;
        while (current) {
            count += countBranchingNodes(current->next);
            current = current->nextListNode;
        }
        return count;
    }

    void calculateBranchingPaths(TrieNode* node, int& totalPaths) {
        if (!node || !node->head) return;

        int paths = 0;
        ListNode* current = node->head;
        while (current) {
            paths++;
            current = current->nextListNode;
        }
        if (paths > 1 && node != root) {
            totalPaths += paths;
        }

        current = node->head;
        while (current) {
            calculateBranchingPaths(current->next, totalPaths);
            current = current->nextListNode;
        }
    }
public:
    TrieNode* root;

    int countTrieNodes(TrieNode* node) {
        if (!node) return 0;
        int count = 1;
        ListNode* current = node->head;
        while (current) {
            count += countTrieNodes(current->next);
            current = current->nextListNode;
        }
        return count;
    }

    int countListNodes(TrieNode* node) {
        if (!node) return 0;
        int count = 0;
        ListNode* current = node->head;
        while (current) {
            count += 1 + countListNodes(current->next);
            current = current->nextListNode;
        }
        return count;
    }

    void printTree() {
        printNode(root, "", true);
    }

    void printNode(TrieNode* node, const std::string& prefix, bool isLast) {
        if (!node) return;

        if (node == root) {
            std::cout << prefix << (isLast ? "`-- " : "|-- ") << "[root]\n";
        }

        ListNode* current = node->head;
        while (current) {
            std::string newPrefix = prefix + (isLast ? "    " : "|   ");

            std::cout << newPrefix;
            std::cout << (current->nextListNode ? "|-- " : "`-- ");
            std::cout << current->ch;
            if (current->next->isEndOfWord) {
                std::cout << " (end)";
            }
            std::cout << "\n";

            printNode(current->next, newPrefix, !current->nextListNode);

            current = current->nextListNode;
        }
    }
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            TrieNode* child = current->getChild(ch);
            if (!child) {
                current->addChild(ch);
                child = current->getChild(ch);
            }
            current = child;
        }
        current->isEndOfWord = true;
    }

    bool search(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            TrieNode* child = current->getChild(ch);
            if (!child) {
                return false;
            }
            current = child;
        }
        return current->isEndOfWord;
    }

    // 1. Общее количество символов
    int totalChars() {
        return countChars(root);
    }

    // 2. Количество слов (листовых вершин в дереве)
    int wordCount() {
        return countWords(root);
    }

    // 3. Количество внутренних вершин
    int internalNodeCount() {
        return countInternalNodes(root);
    }

    // 4. Количество ветвлений (внутренних вершин из которых более одного пути)
    int branchingNodeCount() {
        return countBranchingNodes(root);
    }

    // 5. Среднее количество путей в вершинах ветвлений
    double averageBranchingPaths() {
        int branchCount = branchingNodeCount();
        int totalPaths = 0;
        calculateBranchingPaths(root, totalPaths);
        return branchCount > 0 ? static_cast<double>(totalPaths) / branchCount : 0.0;
    }
};
void generateWords(std::vector<std::string>& words, int minLen, int maxLen, int n) {
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    std::uniform_int_distribution<int> charDist(0, alphabet.size() - 1);

    std::uniform_int_distribution<int> rand(0, 100);

    words.clear();

    while (n > 0) {
        std::string word;
        int lineLen = 0;
        while (lineLen == 0 || (rand(rng) >= 15 && n != 0)) {
            word += alphabet[charDist(rng)];
            lineLen++;
            n--;
            if (n % 50 == 0) {
                break;
            }
        }
        words.push_back(word);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");


    std::vector<std::string> words = {};
    generateWords(words, 4, 8, 1000);

    //int currentN = 0;
    //Trie trie;
    //int j = 0;
    //auto start_time = std::chrono::high_resolution_clock::now();
    //while (true)
    //{
    //    trie.insert(words[j]);
    //    currentN += words[j].length();
    //    if (currentN % 50 == 0) break;
    //    j++;
    //}
    //auto end_time = std::chrono::high_resolution_clock::now();
    //std::cout << std::endl << "---------------- n = " << currentN << " ----------------" << std::endl;
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    //std::cout << "Время постройки дерева: " << duration.count() << " микросекунд\n";

    //std::cout << std::endl << "Подсчет памяти" << std::endl;

    //int totalTrieNodes = trie.countTrieNodes(trie.root);
    //int totalListNodes = trie.countListNodes(trie.root);

    ////std::cout << "totalTrieNodes = " << totalTrieNodes << std::endl;
    ////std::cout << "totalListNodes = " << totalListNodes << std::endl;

    ////std::cout << "sizeof(TrieNode)) = " << sizeof(TrieNode) << std::endl;
    ////std::cout << "sizeof(ListNode) = " << sizeof(ListNode) << std::endl;

    //size_t totalMemory = (totalTrieNodes * sizeof(TrieNode)) + (totalListNodes * sizeof(ListNode));
    //std::cout << "Память: " << totalMemory << " байт (~"
    //    << totalMemory / 1024.0 << " KB)\n";

    //std::cout << "\nПараметры дерева:\n";
    //std::cout << "1. Общее количество узлов (символов): " << trie.totalChars() << std::endl;
    //std::cout << "2. Количество слов (листовых вершин): " << trie.wordCount() << std::endl;
    //std::cout << "3. Количество внутренних вершин. " << trie.internalNodeCount() << std::endl;
    //std::cout << "4. Количество ветвлений (внутренних вершин из которых более одного пути). " << trie.branchingNodeCount() << std::endl;
    //std::cout << "5. Среднее количество путей в вершинах ветвлений. " << trie.averageBranchingPaths() << std::endl;
    //
    //std::cout << "\nДерево:\n";
    //trie.printTree();

    // Массив
    
    cout<<endl <<"***************************** Способ 1: массив ***********************************"<<endl;
    for (size_t i = 1; i <= 10; i++)
    {
        int currentN = 0;
        root = getNode();
        int j = 0;
        auto start_time = std::chrono::high_resolution_clock::now();
        while (true)
        {
            insert(root,words[j]);
            currentN += words[j].length();
            if (currentN == i * 50) break;
            j++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout << std::endl << "---------------- n = " << currentN << " ----------------" << std::endl;
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        std::cout << "Время постройки дерева: " << duration.count() << " микросекунд\n";

        std::cout << std::endl << "Подсчет памяти" << std::endl;

        size_t totalMemory = calculateMemoryUsage(root);
        std::cout << "Память: " << totalMemory << " байт (~"
            << totalMemory / 1024.0 << " KB)\n";

        std::cout << "\nПараметры дерева:\n";
        std::cout << "1. Общее количество узлов (символов): " << countTotalNodes(root) << std::endl;
        std::cout << "2. Количество слов (листовых вершин): " << countLeafWords(root) << std::endl;
        std::cout << "3. Количество внутренних вершин. " << countInternalNodes(root) << std::endl;
        std::cout << "4. Количество ветвлений (внутренних вершин из которых более одного пути). " << countBranchingNodes(root) << std::endl;
        std::cout << "5. Среднее количество путей в вершинах ветвлений. " << calculateAvgBranching(root) << std::endl;
    }
    // Список
    cout << endl << "***************************** Способ 2: список ***********************************" << endl;
    for (size_t i = 1; i <= 10; i++)
    {
        int currentN = 0;
        Trie trie;
        int j = 0;
        auto start_time = std::chrono::high_resolution_clock::now();
        while (true)
        {
            trie.insert(words[j]);
            currentN += words[j].length();
            if (currentN == i * 50) break;
            j++;
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout << std::endl << "---------------- n = " << currentN << " ----------------" << std::endl;
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        std::cout << "Время постройки дерева: " << duration.count() << " микросекунд\n";

        std::cout << std::endl << "Подсчет памяти" << std::endl;

        int totalTrieNodes = trie.countTrieNodes(trie.root);
        int totalListNodes = trie.countListNodes(trie.root);

        //std::cout << "totalTrieNodes = " << totalTrieNodes << std::endl;
        //std::cout << "totalListNodes = " << totalListNodes << std::endl;

        //std::cout << "sizeof(TrieNode)) = " << sizeof(TrieNode) << std::endl;
        //std::cout << "sizeof(ListNode) = " << sizeof(ListNode) << std::endl;

        size_t totalMemory = (totalTrieNodes * sizeof(TrieNode)) + (totalListNodes * sizeof(ListNode));
        std::cout << "Память: " << totalMemory << " байт (~"
            << totalMemory / 1024.0 << " KB)\n";

        std::cout << "\nПараметры дерева:\n";
        std::cout << "1. Общее количество узлов (символов): " << trie.totalChars() << std::endl;
        std::cout << "2. Количество слов (листовых вершин): " << trie.wordCount() << std::endl;
        std::cout << "3. Количество внутренних вершин. " << trie.internalNodeCount() << std::endl;
        std::cout << "4. Количество ветвлений (внутренних вершин из которых более одного пути). " << trie.branchingNodeCount() << std::endl;
        std::cout << "5. Среднее количество путей в вершинах ветвлений. " << trie.averageBranchingPaths() << std::endl;
    }
    
    
    
    
    //std::cout << "\nДерево:\n";
    //trie.printTree();
    //return 0;

}