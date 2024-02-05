#include <iostream>
#include <vector>
#include "AvlTree.h"
#include "AvlTree.cpp"
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

struct item {
    string name;
    int count;
};

struct NodeItem {

    string word;
    vector<item> doc;

    //operators to compare in AvlTree class.
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

void treeUpdater(const string& word, AvlTree<NodeItem>& SearchTree, const string& fileName, const NodeItem& NOT_FOUND) {

    NodeItem checkIn;
    checkIn.word = word;
    checkIn = SearchTree.find(checkIn);

    if (checkIn == NOT_FOUND) { //if word is not in tree add it

        //add new word to NodeItem struct
        NodeItem newWord;
        newWord.word = word;

        //create new item struct for the given filename and push it to new NodeItem struct
        item newDoc;
        newDoc.name = fileName;
        newDoc.count = 1;

        newWord.doc.push_back(newDoc);
        SearchTree.insert(newWord); //insert new word to search tree

    }
    else {   //if word is in the tree update the node

        NodeItem* ptr = SearchTree.elementUpdate(checkIn); //pointer to update the node
        bool documentCheck = false;
        for (int i = 0; i < checkIn.doc.size(); i++) { //if node already has the document name increment the count of corresponding word

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

void treeCreater(const vector<string>& fileNames, const int& total, AvlTree<NodeItem>& SearchTree, const NodeItem& NOT_FOUND) {

    for (int k = 0; k < total; k++) { //read all the text files into tree one by one

        string line;
        ifstream file(fileNames[k]);

        while (getline(file, line)) {   //add every word to search tree

            istringstream iss(line);
            string word;
            while (iss >> word) {
                // Convert the word to lowercase

                for (int i = 0; i < word.length(); ++i) {

                    word[i] = tolower(word[i]);
                }

                treeUpdater(word, SearchTree, fileNames[k], NOT_FOUND);
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

void wordCountFinder(const AvlTree<NodeItem>& SearchTree, const vector<string>& toBeFound, const NodeItem& NOT_FOUND) {

    bool checkFound = false; //assigned true if any given word in the query is found in AVL Tree
    NodeItem checkIn;   //contains the searched word's node
    vector<OutputFormat> output; //each index keeps the file information of individual file (each index corresponds to a line)

    for (int i = 0; i < toBeFound.size(); i++) { //Search all the words given in the query

        checkIn.word = toBeFound[i];
        checkIn = SearchTree.find(checkIn);

        if (checkIn != NOT_FOUND) { //word exist in AVL Tree

            checkFound = true;
            outputUpdater(checkIn, output, toBeFound[i]); //update the output vector accordingly
        }
    }

    if (!checkFound) {

        cout << "No document contains the given query" << endl;
    }
    else {   //print out the output information vector

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

    //create an AVL Tree using the txt inputs
    NodeItem NOT_FOUND;
    NOT_FOUND.word = "NOT_FOUND";
    AvlTree<NodeItem> SearchTree(NOT_FOUND);

    treeCreater(fileNames, total, SearchTree, NOT_FOUND);  //takes the created empty AVL tree and actually implements it


    //search for queried words in the AVL Tree
    string line;
    bool takeInput = true;
    do {
        cout << endl << "Enter queried words in one line: ";
        getline(cin, line);

        if (line == "") {   //to ignore the case where getline reads blankspace as an input without asking to console
            getline(cin, line);

        }
        vector<string> toBeFound;
        istringstream css(line); //string stream created to check if user wants to remove element or exit the query loop
        string check;
        css >> check;

        if (check == "REMOVE") {

            css >> check;   //take the word user wants to remove
            for (int i = 0; i < check.length(); ++i) {
                check[i] = tolower(check[i]);
            }
            NodeItem toBeRemoved;
            toBeRemoved.word = check;
;
            SearchTree.remove(toBeRemoved);
            cout <<check <<" has been REMOVED"<<endl;
        }
        else if (check == "ENDOFINPUT") {

            takeInput = false;
        }
        else {
            istringstream iss(line);
            string word;
            while (iss >> word) {

                for (int i = 0; i < word.length(); ++i) {
                    word[i] = tolower(word[i]);
                }
                toBeFound.push_back(word);
            }
            wordCountFinder(SearchTree, toBeFound, NOT_FOUND);
        }
    } while (takeInput);

    return 0;
}