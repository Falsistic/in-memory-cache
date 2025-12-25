#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int value;
    long long expiry;
    Node* next;
    Node* prev;
    Node(int k, int v, long long e) {
        key = k;
        value = v;
        expiry = e;
        next = NULL;
        prev = NULL;
    }
};

long long now() {
    return chrono::duration_cast<chrono::milliseconds>(
        chrono::steady_clock::now().time_since_epoch()
    ).count();
}

class LRUCache {
public:
    int capacity;
    unordered_map<int, Node*> cacheMap;
    Node* head;
    Node* tail;

    LRUCache(int cap) {
        capacity = cap;
        head = new Node(-1, -1, -1);
        tail = new Node(-1, -1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) return -1;

        Node* temp = cacheMap[key];

        if (temp->expiry != -1 && temp->expiry < now()) {
            removeNode(temp);
            cacheMap.erase(key);
            delete temp;
            return -1;
        }

        removeNode(temp);
        insertAtHead(temp);
        return temp->value;
    }

    void put(int key, int value) {
        put(key, value, -1);
    }

    void put(int key, int value, long long ttl_ms) {
        if (capacity == 0) return;

        long long exp = (ttl_ms == -1) ? -1 : now() + ttl_ms;

        if (cacheMap.find(key) != cacheMap.end()) {
            Node* temp = cacheMap[key];
            temp->value = value;
            temp->expiry = exp;
            removeNode(temp);
            insertAtHead(temp);
        }
        else {
            cleanupExpired();

            if (cacheMap.size() == capacity) {
                Node* lru = tail->prev;
                removeNode(lru);
                cacheMap.erase(lru->key);
                delete lru;
            }

            Node* temp = new Node(key, value, exp);
            insertAtHead(temp);
            cacheMap[key] = temp;
        }
    }

    void removeNode(Node* ptr) {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
    }

    void insertAtHead(Node* ptr) {
        ptr->next = head->next;
        head->next = ptr;
        ptr->next->prev = ptr;
        ptr->prev = head;
    }

    void cleanupExpired() {
        while (tail->prev != head) {
            Node* cur = tail->prev;
            if (cur->expiry != -1 && cur->expiry < now()) {
                removeNode(cur);
                cacheMap.erase(cur->key);
                delete cur;
            } else break;
        }
    }

    ~LRUCache() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
};
