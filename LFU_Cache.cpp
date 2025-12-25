#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int value;
    int cnt;
    long long expiry;
    Node* next;
    Node* prev;
    Node(int _key, int _value, long long _expiry) {
        key = _key;
        value = _value;
        cnt = 1;
        expiry = _expiry;
        next = NULL;
        prev = NULL;
    }
};

struct List {
    Node* head;
    Node* tail;
};

long long now() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}

class LFUCache {
public:
    int capacity;
    int minfreq;
    unordered_map<int, Node*> cacheMap;
    unordered_map<int, List> freqMap;

    LFUCache(int _capacity) {
        capacity = _capacity;
        minfreq = 0;
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

        updateFreq(temp);
        return temp->value;
    }

    void put(int key, int value, long long ttl_ms = -1) {
        if (capacity == 0) return;

        long long exp = (ttl_ms == -1) ? -1 : now() + ttl_ms;

        if (cacheMap.find(key) != cacheMap.end()) {
            Node* temp = cacheMap[key];
            temp->value = value;
            temp->expiry = exp;
            updateFreq(temp);
            return;
        }
        else {
            cleanupExpired();
    
            if (cacheMap.size() == capacity) {
                Node* lru = freqMap[minfreq].tail->prev;
                removeNode(lru);
                cacheMap.erase(ltu->key);
                delete lru;
            }
    
            Node* temp = new Node(key, value, exp);
            minfreq = 1;
            addNode(temp, 1);
            cacheMap[key] = temp;
        }
    }

    void addNode(Node* ptr, int freq) {
        if (freqMap.find(freq) == freqMap.end()) {
            Node* head = new Node(-1, -1, -1);
            Node* tail = new Node(-1, -1, -1);
            head->next = tail;
            tail->prev = head;
            freqMap[freq] = {head, tail};
        }
        
        ptr->next = freqMap[freq].head->next;
        ptr->prev = freqMap[freq].head;
        freqMap[freq].head->next->prev = ptr;
        freqMap[freq].head->next = ptr;
    }

    void removeNode(Node* ptr) {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
    }

    void updateFreq(Node* ptr) {
        int oldfreq = ptr->cnt;
        removeNode(ptr);

        if (freqMap[oldfreq].head->next == freqMap[oldfreq].tail) {
            delete freqMap[oldfreq].head;
            delete freqMap[oldfreq].tail;
            freqMap.erase(oldfreq);
            if (minfreq == oldfreq) minfreq++;
        }

        ptr->cnt++;
        addNode(ptr, ptr->cnt);
    }

    void cleanupExpired() {
        if (freqMap.find(minfreq) == freqMap.end()) return;

        Node* cur = freqMap[minfreq].tail->prev;
        while (cur != freqMap[minfreq].head) {
            Node* prev = cur->prev;
            if (cur->expiry != -1 && cur->expiry < now()) {
                removeNode(cur);
                cacheMap.erase(cur->key);
                delete cur;
            }
            cur = prev;
        }
    }

    ~LFUCache() {
        for (auto &it : freqMap) {
            Node* cur = it.second.head;
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }
    }
};
