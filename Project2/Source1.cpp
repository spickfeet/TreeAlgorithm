#include <iostream>
#include <vector>
#include <chrono>
#include <random>

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
void generateWords(std::vector<std::string>& words, int minLen, int maxLen, int count) {
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    std::uniform_int_distribution<int> charDist(0, alphabet.size() - 1);

    words.clear();

    for (int i = 0; i < count; ++i) {
        int len = lenDist(rng);
        std::string word;
        for (int j = 0; j < len; ++j) {
            word += alphabet[charDist(rng)];
        }
        words.push_back(word);
    }
}
int main() {
    setlocale(LC_ALL, "Russian");


    std::vector<std::string> words = {};
    generateWords(words, 4, 8, 500);



    for (size_t i = 1; i <= 10; i++)
    {
        std::cout << std::endl << "---------------- n = " << i * 50 <<" ----------------" << std::endl;
        Trie trie;
        auto start_time = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < 50 * i; j++)
        {
            trie.insert(words[j]);
        }

        auto end_time = std::chrono::high_resolution_clock::now();
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