#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <chrono>
#include "HashTable.h"
#include "BST.h"
#include <cstdlib>

using namespace std;

struct item {
    string name;
    int count = 0;
};

struct NodeItem {

    string word;
    vector<item> doc;

    bool operator<(const NodeItem& other) const {
        return word < other.word;
    }
    bool operator>(const NodeItem& other) const {
        return word > other.word;
    }
    bool operator>=(const NodeItem& other) const {
        return word >= other.word;
    }
    bool operator<=(const NodeItem& other) const {
        return word <= other.word;
    }
    bool operator==(const NodeItem& other) const {
        return word == other.word;
    }
    bool operator!=(const NodeItem& other) const {
        return word != other.word;
    }

};

NodeItem binarySearch(const vector<NodeItem>& vec, const NodeItem& toBeFound, const NodeItem& NOT_FOUND, int min, int max) {

    if (max >= min) {
        int mean = min + (max - min) / 2;
        if (vec[mean] == toBeFound) {
            return vec[mean];
        }
        if (vec[mean] > toBeFound) {
            return binarySearch(vec, toBeFound, NOT_FOUND, min, mean - 1);
        }
        return binarySearch(vec, toBeFound, NOT_FOUND, mean + 1, max);
    }
    return NOT_FOUND;
}

//SORTING FUNCTIONS
void insertionSort(vector <NodeItem>& a)
{
    int j;
    // loop over the passes
    for (int p = 1; p < a.size(); p++)
    {
        NodeItem tmp = a[p];
        // loop over the elements
        for (j = p; j > 0 && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

inline int leftChild(int i)
{
    return 2 * i + 1;
}

void percDown(vector<NodeItem>& a, int i, int n)
{
    int child;
    NodeItem tmp;

    for (tmp = a[i]; leftChild(i) < n; i = child)
    {
        child = leftChild(i);
        if (child != n - 1 && a[child] < a[child + 1])
            child++;
        if (a[child] > tmp)
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}

void heapsort(vector<NodeItem>& a)
{
    // buildHeap
    for (int i = a.size() / 2; i >= 0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() - 1; j > 0; j--)
    {
        swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}

void merge(vector<NodeItem>& a, vector<NodeItem>& tmpArray, int leftPos, int rightPos, int rightEnd)
{
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;
    // Main loop
    while (leftPos <= leftEnd && rightPos <= rightEnd)
        if (a[leftPos] <= a[rightPos])
            tmpArray[tmpPos++] = a[leftPos++];
        else
            tmpArray[tmpPos++] = a[rightPos++];

    while (leftPos <= leftEnd)    // Copy rest of first half
        tmpArray[tmpPos++] = a[leftPos++];

    while (rightPos <= rightEnd)  // Copy rest of right half
        tmpArray[tmpPos++] = a[rightPos++];

    // Copy tmpArray back
    for (int i = 0; i < numElements; i++, rightEnd--)
        a[rightEnd] = tmpArray[rightEnd];
}

void mergeSort(vector<NodeItem>& a, vector<NodeItem>& tmpArray, int left, int right)
{
    if (left < right)
    {
        int center = (left + right) / 2;
        mergeSort(a, tmpArray, left, center);
        mergeSort(a, tmpArray, center + 1, right);
        merge(a, tmpArray, left, center + 1, right);
    }
}

void mergeSort(vector<NodeItem>& a)
{
    vector<NodeItem> tmpArray(a.size());

    mergeSort(a, tmpArray, 0, a.size() - 1);
}

inline void swap(NodeItem& obj1, NodeItem& obj2)
{
    NodeItem tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

void insertionSort(vector<NodeItem>& a, int left, int right)
{
    for (int p = left + 1; p <= right; p++)
    {
        NodeItem tmp = a[p];
        int j;

        for (j = p; j > left && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

const NodeItem& median3(vector<NodeItem>& a, int left, int right)
{   
    int center = (left + right) / 2;
    if (a[center] < a[left])
        swap(a[left], a[center]);
    if (a[right] < a[left])
        swap(a[left], a[right]);
    if (a[right] < a[center])
        swap(a[center], a[right]);

    // Place pivot at position right - 1
    swap(a[center], a[right - 1]);
    return a[right - 1];
}

void quicksortm(vector<NodeItem>& a, int left, int right)
{
    if (left + 10 <= right)
    {
        NodeItem pivot = median3(a, left, right);
        // Begin partitioning
        int i = left, j = right - 1;
        for (; ; )
        {
            while (a[++i] < pivot) {}

            while (pivot < a[--j]) {}

            if (i < j)
                swap(a[i], a[j]);
            else
                break;
        }
        swap(a[i], a[right - 1]);   // Restore pivot

        quicksortm(a, left, i - 1);       // Sort small elements
        quicksortm(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertionSort(a, left, right);
}

void quicksortM(vector<NodeItem>& a)
{
    quicksortm(a, 0, a.size() - 1);
}

int partitionf(vector<NodeItem>& a, int left, int right) {
   NodeItem c = a[left];
   int k = right;
   for (int i = right; i > left; i--) {
       if (a[i] > c)
           swap(a[i], a[k--]);
       swap(a[left], a[k]);
   }
    return k;
}

void quickSortF(vector<NodeItem>& a, int left, int right)
{
    if (left < right) {
        int i = partitionf(a, left, right);
        quickSortF(a, left, i - 1);
        quickSortF(a, i + 1, right);
    }
}

void quicksortF(vector<NodeItem>& a)
{
    quickSortF(a, 0, a.size() - 1);
}

int partitionr(vector<NodeItem>& a, int left, int right)
{
    NodeItem c = a[right];
    int i = (left - 1);
    for (int j = left; j <= right - 1; j++) {
        if (a[j] <= c) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[right]);
    return (i + 1);
}

int partitionR(vector<NodeItem>& a, int left, int right) {
    srand(time(NULL));
    int random = left + rand() % (right - left);
    swap(a[random], a[right]);
    return partitionr(a, left, right);
}

void quickSortR(vector<NodeItem>& a, int left, int right) {
    if (left < right) {
        int i = partitionR(a, left, right);
        quickSortR(a, left, i - 1);
        quickSortR(a, i + 1, right);
    }
}

void quicksortR(vector<NodeItem>& a)
{
    quickSortR(a, 0, a.size() - 1);
}

//END OF SORTING FUNCTIONS

template <typename dataStructure>
void structureUpdater(const string& word, dataStructure& structure, const string& fileName, const NodeItem& NOT_FOUND) {

    NodeItem checkIn;
    checkIn.word = word;
    checkIn = structure.find(checkIn);

    if (checkIn == NOT_FOUND) { //if word is not in structure add it

        //add new word to NodeItem struct
        NodeItem newWord;
        newWord.word = word;

        //create new item struct for the given filename and push it to new NodeItem struct
        item newDoc;
        newDoc.name = fileName;
        newDoc.count = 1;

        newWord.doc.push_back(newDoc);
        structure.insert(newWord); //insert new word to structure

    }
    else {   //if word is in the structure update the node

        NodeItem* ptr = structure.elementUpdate(checkIn); //pointer to update the node
        bool documentCheck = false;
        for (int i = 0; i < checkIn.doc.size(); i++) { //if the node already has the document name increment the count of corresponding word

            if (checkIn.doc[i].name == fileName) {

                documentCheck = true;
                ptr->doc[i].count++;
            }
        }
        if (!documentCheck) {  //if document name is not in the node add it to the corresponding word's node

            item newDoc;
            newDoc.name = fileName;
            newDoc.count = 1;
            ptr->doc.push_back(newDoc);
        }
    }
}

void searchCreater(const vector<string>& fileNames, const int& total, HashTable<NodeItem>& hashTable, BinarySearchTree<NodeItem>& SearchTree, const NodeItem& NOT_FOUND, vector<NodeItem>& sortVector) {

    for (int k = 0; k < total; k++) { //read all the text files into search structure one by one

        string line;
        ifstream file(fileNames[k]);

        while (getline(file, line)) {   //add every word to search structure

            // Convert the word to lowercase and non alpha characters to space
            for (int i = 0; i < line.length(); ++i) {

                line[i] = tolower(line[i]);
                if (!isalpha(line[i]))
                    line[i] = ' ';
            }

            istringstream iss(line);
            string word;

            while (iss >> word) {

                //FORM HASHTABLE AND SEARCHTREE
                structureUpdater(word, hashTable, fileNames[k], NOT_FOUND);
                structureUpdater(word, SearchTree, fileNames[k], NOT_FOUND);

                //FORM THE SORTVECTOR
                bool exist = false;
                for (int j = 0; j < sortVector.size(); j++) {

                    if (sortVector[j].word == word ) {  //WORD EXIST IN THE VECTOR

                        exist = true;
                        bool documentCheck = false;
                        for (int m = 0; m < sortVector[j].doc.size(); m++) { //if the node already has the document name increment the count of corresponding word

                            if (sortVector[j].doc[m].name == fileNames[k]) {

                                documentCheck = true;
                                (sortVector[j].doc[m]).count++;
                            }
                        }
                        if (!documentCheck) {  //if document name is not in the node add it to the corresponding word's node

                            item newDoc;
                            newDoc.name = fileNames[k];
                            newDoc.count = 1;
                            (sortVector[j].doc).push_back(newDoc);
                        }

                        break;
                    }
                }
                if (!exist) {

                    //add new word to NodeItem struct
                    NodeItem newWord;
                    newWord.word = word;

                    //create new item struct for the given filename and push it to new NodeItem struct
                    item newDoc;
                    newDoc.name = fileNames[k];
                    newDoc.count = 1;

                    (newWord.doc).push_back(newDoc);
                    sortVector.push_back(newWord); //insert new word to structure

                }
            }
        }
    }
}

struct OutputFormat {

    string txt;
    vector<item> words;
};

void outputUpdater(const NodeItem& checkIn, vector<OutputFormat>& output, const string& word) {

    for (int j = 0; j < checkIn.doc.size(); j++) { //add each txt info in the node to output information vector

        OutputFormat of; //dummy struct value to add new values to output information vector
        item it;

        bool isIn = false;
        for (int i = 0; i < output.size(); i++) {    //check if txt name is already added to output vector

            if (output[i].txt == checkIn.doc[j].name) { //if name of txt is already in output information vector add the searched new word to same index
                isIn = true;

                it.name = checkIn.word;
                it.count = checkIn.doc[j].count;

                output[i].words.push_back(it);
            }
        }
        if (!isIn) {    //Add txt name and its word in case it isn't in the output vector

            of.txt = checkIn.doc[j].name;
            it.name = checkIn.word;
            it.count = checkIn.doc[j].count;

            of.words.push_back(it);
            output.push_back(of);
        }
    }

}

template <typename dataStructure>
void wordCountFinder(dataStructure& structure, const vector<string>& toBeFound, const NodeItem& NOT_FOUND, bool print, bool vectorCheck, vector<NodeItem>& vecStructure) {

    bool checkFound = true; //assigned true if any given word in the query is found
    NodeItem checkIn;   //contains the searched word's node
    vector<OutputFormat> output; //each index keeps the file information of individual file (each index corresponds to a line)

    for (int i = 0; i < toBeFound.size(); i++) { //Search all the words given in the query

        checkIn.word = toBeFound[i];
        if (vectorCheck) {
            checkIn = binarySearch(vecStructure, checkIn, NOT_FOUND, 0, vecStructure.size() - 1);
        }
        else {
            checkIn = structure.find(checkIn);
        }

        if (checkIn != NOT_FOUND ) { //word exist

            outputUpdater(checkIn, output, toBeFound[i]); //update the output vector accordingly if result is going to be printed else skip this part
        }
        else {
            checkFound = false;
            break;
        }
    }

    if (!checkFound && print) {

        cout << "No document contains the given query" << endl;
    }
    else if (print) {   //print out the output information vector

        for (int n = 0; n < output.size(); n++) {

            cout << "in Document " << output[n].txt << ", ";
            for (int y = 0; y < output[n].words.size(); y++) {

                cout << output[n].words[y].name << " found " << to_string(output[n].words[y].count) << " times";
                if (y + 1 == output[n].words.size()) {
                    cout << "." << endl;
                }
                else {
                    cout << ", ";
                }
            }
        }
    }
}

int main() {

    //get total input txt number
    int total;
    cout << "Enter number of input files: ";
    cin >> total;
    vector<string> fileNames;
    string fileName;

    //get names of txts'
    for (int i = 1; i <= total; i++) {

        cout << "Enter " << to_string(i) << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName);
    }

    NodeItem NOT_FOUND;
    NOT_FOUND.word = "NOT_FOUND";

    BinarySearchTree<NodeItem> SearchTree(NOT_FOUND);
    HashTable<NodeItem> hashTable(NOT_FOUND);
    vector<NodeItem> sortVector;

    searchCreater(fileNames, total, hashTable, SearchTree, NOT_FOUND, sortVector);
    hashTable.info();

    //VECTORS FOR DIFFERENT SORTING ALGORTIHMS AND THEIR TIME
    int k = 100;
    clock_t begin_time = clock();

    vector<NodeItem> quick = sortVector;
    begin_time = clock();
    quicksortM(quick);
    double qsort = float(clock() - begin_time);

    vector<NodeItem> rquick = sortVector;
    begin_time = clock();
    quicksortR(rquick);
    double qrsort = float(clock() - begin_time);

    vector<NodeItem> fquick = sortVector;
    begin_time = clock();
    quicksortF(fquick);
    double qfsort = float(clock() - begin_time);

    vector<NodeItem> insertion = sortVector;
    begin_time = clock();
    insertionSort(insertion);
    double isort = float(clock() - begin_time);

    vector<NodeItem> merge = sortVector;
    begin_time = clock();
    mergeSort(merge);
    double msort = float(clock() - begin_time);

    vector<NodeItem> heap = sortVector;
    begin_time = clock();
    heapsort(heap);
    double hsort = float(clock() - begin_time);


    //search for queried words in both data structures
    string line;

    cout << endl << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, line);

    vector<string> toBeFound;

    // Convert the line to lowercase and non alpha characters to space
    for (int i = 0; i < line.length(); ++i) {

        line[i] = tolower(line[i]);
        if (!isalpha(line[i]))
            line[i] = ' ';
    }
    istringstream iss(line);
    string word;

    while (iss >> word) {

        for (int i = 0; i < word.length(); ++i) {
            word[i] = tolower(word[i]);
        }
        toBeFound.push_back(word);
    }

    wordCountFinder(hashTable, toBeFound, NOT_FOUND, true, false, insertion);
    wordCountFinder(SearchTree, toBeFound, NOT_FOUND, true, false, insertion);
    wordCountFinder(SearchTree, toBeFound, NOT_FOUND, true, true, insertion);
    
    for (int i = 0; i < k; i++)
        wordCountFinder(SearchTree, toBeFound, NOT_FOUND, false, false, insertion);
    double endBST = float(clock() - begin_time);
    cout << "\nBinary Search Tree Time: " << endBST/ k << "\n";
    begin_time = clock();
    for (int i = 0; i < k; i++)
        wordCountFinder(hashTable, toBeFound, NOT_FOUND, false, false, insertion);
    double endHT = float(clock() - begin_time);
    cout << "Hash Table Time: " << endHT / k << "\n";
    begin_time = clock();
    for (int i = 0; i < k; i++)
        wordCountFinder(SearchTree, toBeFound, NOT_FOUND, false, true, insertion);
    double endBS = float(clock() - begin_time);
    cout << "Binary Search Time: " << endBS / k << "\n";

    cout << "\nQuick Sort(median) Time: " << qsort << "\n";
    cout << "Quick Sort(random) Time: " << qrsort << "\n";
    cout << "Quick Sort(first) Time: " << qfsort << "\n";
    cout << "Merge Sort Time: " << msort << "\n";
    cout << "Heap Sort Time: " << hsort << "\n";
    cout << "Insertion Sort Time: " << isort << "\n";

    cout << "\nSpeed Up BST/HST: " << endBST/endHT << "\n";
    cout << "Speed Up Merge Sort/Quick Sort(Median): " << msort / qsort << "\n";
    cout << "Speed Up Heap Sort/Quick Sort(Median): " << hsort / qsort << "\n";
    cout << "Speed Up Insertion Sort/Quick Sort(Median): " << isort / qsort << "\n";

    cout << "\nSpeed Up Binary Search / Binary Search Tree Time: " << endBS /endBST << "\n";
    cout << "Speed Up Binary Search / Hash Table Time: " << endBS / endHT << "\n";

    return 0;
}