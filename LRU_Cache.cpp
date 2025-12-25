#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int value;
    Node* next;
    Node* prev;
    Node(int _key, int _value) {
        key = _key;
        value = _value;
        next = NULL;
        prev = NULL;
    }
};

class LRUCache {
public:
    int capacity;
    unordered_map<int, Node*> cacheMap;
    Node* head;
    Node* tail;

    LRUCache(int _capacity) {
        capacity = _capacity;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head -> next = tail;
        tail -> prev = head;
    }
    
    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) return -1;
        Node* temp = cacheMap[key];
        removeNode(temp);
        insertAtHead(temp);
        return temp -> value;
    }
    
    void put(int key, int value) {
        if (capacity == 0) return;
        if (cacheMap.find(key) == cacheMap.end()) {
            if (cacheMap.size() == capacity) {
                Node* temp = tail -> prev;
                removeNode(temp);
                cacheMap.erase(temp -> key);
                delete temp;
            }
            Node* newNode = new Node(key, value);
            insertAtHead(newNode);
            cacheMap[key] = newNode;
        }
        else {
            Node* temp = cacheMap[key];
            temp -> value = value;
            removeNode(temp);
            insertAtHead(temp);
        }
    }

    void insertAtHead(Node* ptr) {
        ptr -> next = head -> next;
        head -> next = ptr;
        ptr -> next -> prev = ptr;
        ptr -> prev = head;
    }

    void removeNode(Node* ptr) {
        ptr -> prev -> next = ptr -> next;
        ptr -> next -> prev = ptr -> prev;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* next = curr -> next;
            delete curr;
            curr = next;
        }
    }
};
