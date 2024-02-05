#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <chrono>
#include "HashTable.h"
#include "BST.h"

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
    bool operator==(const NodeItem& other) const {
        return word == other.word;
    }
    bool operator!=(const NodeItem& other) const {
        return word != other.word;
    }

};

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

void searchCreater(const vector<string>& fileNames, const int& total, HashTable<NodeItem>& hashTable, BinarySearchTree<NodeItem>& SearchTree, const NodeItem& NOT_FOUND) {

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

                structureUpdater(word, hashTable, fileNames[k], NOT_FOUND);
                structureUpdater(word, SearchTree, fileNames[k], NOT_FOUND);
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
void wordCountFinder( dataStructure& structure, const vector<string>& toBeFound, const NodeItem& NOT_FOUND, bool print ) {

    bool checkFound = true; //assigned true if any given word in the query is found
    NodeItem checkIn;   //contains the searched word's node
    vector<OutputFormat> output; //each index keeps the file information of individual file (each index corresponds to a line)

    for (int i = 0; i < toBeFound.size(); i++) { //Search all the words given in the query

        checkIn.word = toBeFound[i];
        checkIn = structure.find(checkIn);

        if (checkIn != NOT_FOUND && print) { //word exist

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
    else if(print){   //print out the output information vector

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

    searchCreater(fileNames, total, hashTable, SearchTree, NOT_FOUND );

    hashTable.info();

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
    wordCountFinder(hashTable, toBeFound, NOT_FOUND, true);
    wordCountFinder(SearchTree, toBeFound, NOT_FOUND, true);


    //SPEED COMPARISION
    int k = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        wordCountFinder(SearchTree, toBeFound, NOT_FOUND, false);
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << BSTTime.count() / k << "\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        wordCountFinder(hashTable, toBeFound, NOT_FOUND, false);
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << HTTime.count() / k << "\n";

    double speedup = static_cast<double>(BSTTime.count()) / HTTime.count();
    cout << endl << "Speed Up: " << speedup << endl;

    return 0;
}