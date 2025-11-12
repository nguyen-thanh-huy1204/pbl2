#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Vector.h"
#include <stdexcept>
#include <string>

using namespace std;

template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        bool occupied = false;
        bool deleted = false;
        Entry() : occupied(false), deleted(false) {}
    };

    Vector<Entry> table;
    int capacity;
    int count;

    int hash(const K& key) const {
        // Polynomial rolling hash cho string
        unsigned long h = 0;
        for (char c : key) h = (h * 31 + c) % capacity;
        return h % capacity;
    }

    void rehash() {
        int oldCap = capacity;
        capacity *= 2;
        Vector<Entry> oldTable = table;
        table.clear();
        for (int i = 0; i < capacity; ++i) table.push_back(Entry());
        count = 0;

        for (int i = 0; i < oldCap; ++i) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

public:
    HashTable(int cap = 16) : capacity(cap), count(0) {
        table = Vector<Entry>();
        for (int i = 0; i < capacity; ++i) table.push_back(Entry());
    }

    void insert(const K& key, const V& value) {
        if (count * 2 >= capacity) rehash(); // load factor 0.5
        int idx = hash(key);
        while (table[idx].occupied && !table[idx].deleted && table[idx].key != key) {
            idx = (idx + 1) % capacity; // Linear probing
        }
        if (!table[idx].occupied || table[idx].deleted) {
            table[idx].occupied = true;
            table[idx].deleted = false;
            table[idx].key = key;
            table[idx].value = value;
            count++;
        } else { // key trùng -> replace
            table[idx].value = value;
        }
    }

    bool contains(const K& key) const {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) return true;
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        return false;
    }

    V& get(const K& key) {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) return table[idx].value;
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        throw runtime_error("Key not found");
    }

    const V& getConst(const K& key) const {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) return table[idx].value;
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        throw runtime_error("Key not found");
    }

    void erase(const K& key) {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) {
                table[idx].deleted = true;
                count--;
                return;
            }
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
    }

    int size() const { return count; }

    Vector<K> keys() const {
        Vector<K> k;
        for (int i = 0; i < capacity; ++i)
            if (table[i].occupied && !table[i].deleted) k.push_back(table[i].key);
        return k;
    }
};

#endif
