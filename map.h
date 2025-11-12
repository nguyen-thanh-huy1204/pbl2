#ifndef MAP_H
#define MAP_H

#include "Vector.h"
#include <stdexcept>

template <typename K, typename V>
class Map {
private:
    struct Pair { K key; V value; };
    Vector<Pair> elems;

public:
    Map() {}

    // insert or replace
    void insert(const K& k, const V& v) {
        for (int i = 0; i < elems.size(); ++i) {
            if (elems[i].key == k) { elems[i].value = v; return; }
        }
        Pair p; p.key = k; p.value = v; elems.push_back(p);
    }

    bool contains(const K& k) const {
        for (int i = 0; i < elems.size(); ++i) if (elems[i].key == k) return true;
        return false;
    }

    V& get(const K& k) {
        for (int i = 0; i < elems.size(); ++i) if (elems[i].key == k) return elems[i].value;
        throw std::runtime_error("Key not found");
    }

    const V& getConst(const K& k) const {
        for (int i = 0; i < elems.size(); ++i) if (elems[i].key == k) return elems[i].value;
        throw std::runtime_error("Key not found");
    }

    int size() const { return elems.size(); }

    Vector<K> keys() const {
        Vector<K> k;
        for (int i = 0; i < elems.size(); ++i) k.push_back(elems[i].key);
        return k;
    }

    void erase(const K& k) {
        for (int i = 0; i < elems.size(); ++i) {
            if (elems[i].key == k) { elems.erase(i); return; }
        }
    }
};

#endif
