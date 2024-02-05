#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

////Code Used From Cs300 9-Hashtables2 pp (some parts are updated)
////HASHTABLE USING LINEAR PROBING

template <class HashedObj>
class HashTable
{

public:

    explicit HashTable(HashedObj& notFound, int size = 53);
    HashTable(const HashTable& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
        array(rhs.array), currentSize(rhs.currentSize) { }
    HashedObj& find(HashedObj& x);

    void insert(const HashedObj& x);
    void remove(const HashedObj& x);
    void info();
    HashedObj* elementUpdate( HashedObj& x);

    enum EntryType { ACTIVE, EMPTY, DELETED };

private:

    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj& e = HashedObj(),
            EntryType i = EMPTY) : element(e), info(i) { }
    };

    vector<HashEntry> array;
    int currentSize;
    HashedObj ITEM_NOT_FOUND;

    bool isActive(int currentPos) const;
    int findPos(const HashedObj& x) const;
    void rehash();

};

    bool isPrime(int n)
    {
        if (n == 2 || n == 3)
            return true;

        if (n == 1 || n % 2 == 0)
            return false;

        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0)
                return false;

        return true;
    }

    int nextPrime(int n)
    {
        if (n % 2 == 0)
            n++;

        for (; !isPrime(n); n += 2)
            ;

        return n;
    }

    template <class HashedObj>
    void HashTable<HashedObj>::info(){
        
        double load = double(currentSize) / array.size();
        cout <<endl<< "After preprocessing, the unique word count is "<<currentSize<<". Current load ratio is "<<load<<endl;
    }

    template<class HashedObj>
    int Hash(HashedObj& key, int tableSize)
    {
        int hashVal = 0;

        string word = key.word;

        for (int i = 0; i < word.length(); i++)
            hashVal = 37 * hashVal + word[i];

        hashVal = hashVal % tableSize;

        if (hashVal < 0)
            hashVal = hashVal + tableSize;

        return(hashVal);
    }

    /**
     * Construct the hash table.
     */
     /**
      * Construct the hash table.
      */
    template <class HashedObj>
    HashTable<HashedObj>::HashTable(HashedObj& notFound,
        int size)
        : ITEM_NOT_FOUND(notFound), array(nextPrime(size))
    {
        currentSize = 0;
    }

    /**
     * Method that performs linear probing resolution.
     * Return the position where the search for x terminates.
     */
    template <class HashedObj>
    int HashTable<HashedObj>::findPos(const HashedObj& x) const
    {
        int collisionNum = 0;
        int currentPos = Hash(x, array.size());

        while (array[currentPos].info != EMPTY &&
            array[currentPos].element != x)
        {   

            currentPos += ++collisionNum;  //add the difference
            if (currentPos >= array.size())              // perform the mod
                currentPos -= array.size();                // if necessary

        }
        return currentPos;
    }


    template <class HashedObj>
    HashedObj* HashTable<HashedObj>::elementUpdate(HashedObj& x)
    {
        int currentPos = findPos(x);
        return &array[currentPos].element;

    }

    /**
      * Return true if currentPos exists and is active.
      */
    template <class HashedObj>
    bool HashTable<HashedObj>::isActive(int currentPos) const
    {
        return array[currentPos].info == ACTIVE;
    }

    /**
     * Remove item x from the hash table.
     *  x has to be in the table
     */
    template <class HashedObj>
    void HashTable<HashedObj>::remove(const HashedObj& x)
    {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            array[currentPos].info = DELETED;

        currentSize -= 1;
    }

    /**
     * Find item x in the hash table.
     * Return the matching item, or ITEM_NOT_FOUND, if not found.
     */
    template <class HashedObj>
    HashedObj& HashTable<HashedObj>::find(HashedObj& x)

    {
        int currentPos = findPos(x);
        if (isActive(currentPos))
            return array[currentPos].element;

        return   ITEM_NOT_FOUND;
    }

    /**
      * Insert item x into the hash table. If the item is
      * already present, then do nothing.
      */
    template <class HashedObj>
    void HashTable<HashedObj>::insert(const HashedObj& x)
    {
        // Insert x as active
        int currentPos = findPos(x);

        if ( isActive (currentPos))
            return;

        array[currentPos] = HashEntry(x, ACTIVE);

        double load = double(++currentSize) / array.size();
        // enlarge the hash table if necessary
        if (load >= 0.8)
            rehash();
    }


    /**
     * Expand the hash table.
     */
    template <class HashedObj>
    void HashTable<HashedObj>::rehash()
    {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize(nextPrime(2 * oldArray.size()));
        for (int j = 0; j < array.size(); j++)
            array[j].info = EMPTY;

        // Copy table over
        currentSize = 0;
        for (int i = 0; i < oldArray.size(); i++)
            if (oldArray[i].info == ACTIVE)
                insert(oldArray[i].element);
    }

    #endif //_HASHTABLE_H