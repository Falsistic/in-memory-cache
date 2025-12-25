#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int value;
    int cnt;
    Node* next;
    Node* prev;
    Node(int _key, int _value) {
        key = _key;
        value = _value;
        cnt = 1;
        next = NULL;
        prev = NULL;
    }
};

struct List {
    Node* head;
    Node* tail;
};

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
        updateFreq(temp);
        return temp -> value;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        if (cacheMap.find(key) != cacheMap.end()) {
            Node* temp = cacheMap[key];
            temp -> value = value;
            updateFreq(temp);
        }
        else {
            if (cacheMap.size() == capacity) {
                Node* temp = freqMap[minfreq].tail -> prev;
                cacheMap.erase(temp -> key);
                removeNode(temp);
                if (freqMap[minfreq].head -> next == freqMap[minfreq].tail) {
                    delete freqMap[minfreq].head;
                    delete freqMap[minfreq].tail;
                    freqMap.erase(minfreq);
                }
                delete temp;
            }
            Node* temp = new Node(key, value);
            cacheMap[key] = temp;
            minfreq = 1;
            addNode(temp, 1);
        }
    }

    void addNode(Node* ptr, int freq) {
        if (freqMap.find(freq) == freqMap.end()) {
            Node* head = new Node(-1, -1);
            Node* tail = new Node(-1, -1);
            head -> next = tail;
            tail -> prev = head;
            freqMap[freq] = {head, tail};
        }
        ptr -> next = freqMap[freq].head -> next;
        freqMap[freq].head -> next = ptr;
        ptr -> next -> prev = ptr;
        ptr -> prev = freqMap[freq].head;
    }

    void removeNode(Node* ptr) {
        ptr -> prev -> next = ptr -> next;
        ptr -> next -> prev = ptr -> prev;
    }

    void updateFreq(Node* ptr) {
        int oldfreq = ptr -> cnt;
        ptr -> cnt++;
        removeNode(ptr);
        if(freqMap[oldfreq].head -> next == freqMap[oldfreq].tail) {
            delete freqMap[oldfreq].head;
            delete freqMap[oldfreq].tail;
            freqMap.erase(oldfreq);
            if (minfreq == oldfreq) minfreq++;
        }
        addNode(ptr, ptr -> cnt);
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
