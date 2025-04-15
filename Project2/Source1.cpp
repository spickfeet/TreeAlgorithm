#include <iostream>
#include <vector>
#include <chrono>

struct ListNode {
    char ch;
    TrieNode* next;
    ListNode* nextListNode;

    ListNode(char c) : ch(c), next(nullptr), nextListNode(nullptr) {}
};

class TrieNode {
public:
    ListNode* head; // ”казатель на начало списка
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
    TrieNode* root;

public:
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
};