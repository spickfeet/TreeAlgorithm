//#include <string>
//#include <iostream>
//#include <vector>
//#include <chrono>
//using namespace std;
//
//const int ALPHABET_SIZE = 26;
//
//
//struct TrieNodeArray {
//    TrieNodeArray* children[ALPHABET_SIZE];
//    bool isEndOfWord;
//
//    TrieNodeArray() : isEndOfWord(false) {
//        for (int i = 0; i < ALPHABET_SIZE; i++)
//            children[i] = nullptr;
//    }
//};
//
//// Функция для подсчета используемой памяти
//size_t calculateMemoryUsage(TrieNodeArray* node) {
//    if (!node) return 0;
//    size_t size = sizeof(*node); // Память для текущего узла
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        size += calculateMemoryUsage(node->children[i]); // Рекурсивно добавляем детей
//    }
//    return size;
//}
//
//
//
//TrieNodeArray* root = nullptr;  // Глобальная переменная для корня
//
//TrieNodeArray* getNode() {
//    return new TrieNodeArray();
//}
//
//void insert(TrieNodeArray* root, const string& key) {
//    TrieNodeArray* pCrawl = root;
//    for (char c : key) {
//        int index = c - 'a';
//        if (!pCrawl->children[index]) {
//            pCrawl->children[index] = getNode();
//        }
//        pCrawl = pCrawl->children[index];
//    }
//    pCrawl->isEndOfWord = true;
//}
//
//// Общее количество узлов (без корня)
//int countTotalNodes(TrieNodeArray* node) {
//    if (!node) return 0;
//    int count = 0;  // Не считаем текущий узел если это корень
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        count += countTotalNodes(node->children[i]);
//    }
//    return node == root ? count : count + 1;  // Пропускаем корень
//}
//
//// Количество слов (листовых вершин)
//int countLeafWords(TrieNodeArray* node) {
//    if (!node) return 0;
//    int count = node->isEndOfWord ? 1 : 0;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        count += countLeafWords(node->children[i]);
//    }
//    return count;
//}
//
//// Количество внутренних вершин (не корень и не листья)
//int countInternalNodes(TrieNodeArray* node, bool isRoot = true) {
//    if (!node) return 0;
//
//    bool hasChildren = false;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        if (node->children[i]) {
//            hasChildren = true;
//            break;
//        }
//    }
//
//    int count = (!isRoot && hasChildren) ? 1 : 0;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        count += countInternalNodes(node->children[i], false);
//    }
//    return count;
//}
//
//// Количество ветвлений (узлов с >1 ребенком, кроме корня)
//int countBranchingNodes(TrieNodeArray* node, bool isRoot = true) {
//    if (!node) return 0;
//
//    int childCount = 0;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        if (node->children[i]) childCount++;
//    }
//
//    int count = (!isRoot && childCount > 1) ? 1 : 0;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        count += countBranchingNodes(node->children[i], false);
//    }
//    return count;
//}
//
//bool hasAnyWord(TrieNodeArray* node) {
//    if (!node) return false;
//    if (node->isEndOfWord) return true;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        if (hasAnyWord(node->children[i])) return true;
//    }
//    return false;
//}
//
//pair<int, int> countRealBranchings(TrieNodeArray* node, bool isRoot = true) {
//    if (!node) return { 0, 0 };
//
//    int childCount = 0;
//    vector<TrieNodeArray*> activeChildren;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        if (node->children[i]) {
//            childCount++;
//            activeChildren.push_back(node->children[i]);
//        }
//    }
//
//    bool isRealBranch = false;
//    if (childCount > 1 && !isRoot) {
//        int wordPaths = 0;
//        for (auto child : activeChildren) {
//            if (child->isEndOfWord) wordPaths++;
//            if (hasAnyWord(child)) wordPaths++;
//            if (wordPaths >= 2) {
//                isRealBranch = true;
//                break;
//            }
//        }
//    }
//
//    int sum = 0, count = 0;
//    if (isRealBranch) {
//        sum = childCount;
//        count = 1;
//    }
//
//    for (auto child : activeChildren) {
//        auto childStats = countRealBranchings(child, false);
//        sum += childStats.first;
//        count += childStats.second;
//    }
//
//    return { sum, count };
//}
//
//double calculateAvgBranching(TrieNodeArray* root) {
//    auto stats = countRealBranchings(root);
//    return (stats.second == 0) ? 0.0 :
//        static_cast<double>(stats.first) / stats.second;
//}
//
//
//
//int Test() {
//    setlocale(LC_ALL, "Russian");
//    string keys[] = {"qw"};
//    int n = sizeof(keys) / sizeof(keys[0]);
//
//    auto start = chrono::high_resolution_clock::now();
//    root = getNode();
//    for (int i = 0; i < n; i++) {
//        insert(root, keys[i]);
//    }
//    auto end = chrono::high_resolution_clock::now();
//
//    size_t memory_used = calculateMemoryUsage(root);
//    cout << "Затраченное время: " << chrono::duration<double>(end - start).count()
//        << " секунд" << endl;
//    cout << "Использовано памяти: " << memory_used << " байт" << endl;
//    cout << "1. Общее количество узлов: " << countTotalNodes(root) << endl;
//    cout << "2. Количество слов: " << countLeafWords(root) << endl;
//    cout << "3. Количество внутренних вершин: " << countInternalNodes(root) << endl;
//    cout << "4. Количество ветвлений: " << countBranchingNodes(root) << endl;
//    cout << "5. Среднее количество путей: " << calculateAvgBranching(root) << endl;
//
//    return 0;
//}