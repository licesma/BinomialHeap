//
// Created by Esteban on 19/09/24.
//

#ifndef BINOMIALTREE_H
#define BINOMIALTREE_H
#include <list>
#include <iostream>
#include <sstream>
#include <random>
using namespace std;
inline int KEY_LENGTH = 4;
class BinomialTree {
public:
    int value;
    int order;
    BinomialTree* parent;
    unordered_map<string, BinomialTree*> *keyMap;
    string key;
    std::list<BinomialTree*> *children;

    explicit BinomialTree(int value):
        value {value},
        order {0},
        parent {nullptr}
    {
        string uniqueKey;
        for( int i = 0; i < KEY_LENGTH; i++) {
            uniqueKey.append(1,getRandom(65,90));
        }
        this->key = uniqueKey;
        this->keyMap = new unordered_map<string, BinomialTree*>({{uniqueKey, this}});
        this->children = new std::list<BinomialTree*>();
        }


    vector<vector<char>> getMatrixRepresentation( bool shortVersion = true) {
        vector<bool> lc;
        vector<vector<char>> result;

        getMatrixRepresentationHelper(this, lc, result, shortVersion);
        int longestRow = 0;
        for(const auto& row: result) {
            int size = row.size();
            longestRow = max(longestRow, size);
        }

        for(int i = 0; i < result.size(); i++) {
            vector<char> &row = result[i];
            if(i == 0) {
                row.insert(row.end(), longestRow - row.size(), '-');
            }
            else {
                row.insert(row.end(), longestRow - row.size(), ' ');
            }
        }
        return result;
    }
     void print(bool shortVersion = true) {
        std::vector<bool> lc;
        printHelper(this, lc, shortVersion);
    }
    void printf(bool shortVersion = true) {
        auto mat = this->getMatrixRepresentation();
        for(auto row: mat) {
            string str(row.begin(), row.end());
            cout<<str<<"\n";
        }
    }

private:
public:
    static int MIN_RANDOM;
    static int MAX_RANDOM;
    static  BinomialTree* merge(BinomialTree* treeOne, BinomialTree* treeTwo) {
        if(treeOne->order != treeTwo->order) {
            throw  std::invalid_argument("Cannot merge trees of different orders.");
        }
        int valueOne = treeOne->value, valueTwo = treeTwo->value;
        return valueOne < valueTwo ? mergeOrdered(treeOne, treeTwo) : mergeOrdered(treeTwo, treeOne);
    }

    static BinomialTree* create_random(int order) {
        return create_random_helper(order);
    }

private:
    void printHelper(BinomialTree *tree, std::vector<bool> lastChild, bool shortVersion = true) {
        int spacePadding = lastChild.size();
        std::vector<char> lastChildPaddingBlock(4,' '), paddingBlock(3,' '), paddingV;
        paddingBlock.push_back('|');

        for(int i = 0; i < spacePadding; i++) {
            if(lastChild[i] && i != spacePadding-1) {
                paddingV.insert(paddingV.end(),lastChildPaddingBlock.begin(), lastChildPaddingBlock.end());
            }
            else {
            paddingV.insert(paddingV.end(),paddingBlock.begin(), paddingBlock.end());
            }
        }


        std::string padding(paddingV.begin(), paddingV.end());
        if(!shortVersion)  std::cout<<padding<<"\n";
        std::cout<<padding<<"--"<<tree->value<<"\n";
        lastChild.push_back(false);
        int childrenSize = tree->children->size();
        int count = 1;
        for (auto it = tree->children->begin(); it != tree->children->end(); ++it) {
            if (count == childrenSize) {
                lastChild.pop_back();
                lastChild.push_back(true);
            }
            printHelper(*it, lastChild, shortVersion);
            count++;
        }
    }

    void getMatrixRepresentationHelper(BinomialTree *tree, vector<bool>  lastChild, vector<vector<char>> &result, bool shortVersion = true) {
        int spacePadding = lastChild.size();
        std::vector<char> lastChildPaddingBlock(4,' '), paddingBlock(3,' '), paddingV;
        paddingBlock.push_back('|');

        for(int i = 0; i < spacePadding; i++) {
            if(lastChild[i] && i != spacePadding-1) {
                paddingV.insert(paddingV.end(),lastChildPaddingBlock.begin(), lastChildPaddingBlock.end());
            }
            else {
                paddingV.insert(paddingV.end(),paddingBlock.begin(), paddingBlock.end());
            }
        }


        std::string padding(paddingV.begin(), paddingV.end());
        stringstream ss;
        ss<<padding<<"---"<<tree->value;
        if(!shortVersion) ss<<"("<<tree->key<<")";
        string row = ss.str();
        vector<char> vec(row.begin(), row.end());
        result.push_back(vec);
        lastChild.push_back(false);
        int childrenSize = tree->children->size();
        int count = 1;
        for (auto it = tree->children->begin(); it != tree->children->end(); ++it) {
            if (count == childrenSize) {
                lastChild.pop_back();
                lastChild.push_back(true);
            }
            getMatrixRepresentationHelper(*it, lastChild, result, shortVersion);
            count++;
        }

    }

    static BinomialTree* create_random_helper (int order) {
        if(order == 0) {
            return new BinomialTree(BinomialTree::getRandom());
        }

        BinomialTree* firstChild = create_random_helper(order-1);
        BinomialTree* secondChild = create_random_helper(order-1);
        return BinomialTree::merge(firstChild, secondChild);

    }

    static int getRandom(int a = 1, int b = 99) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(a,b);

        return(dist(rng));
    }

    static BinomialTree* mergeOrdered(BinomialTree* smallerTree, BinomialTree* largerTree) {
        smallerTree->children->push_back(largerTree);
        smallerTree->keyMap->insert(largerTree->keyMap->begin(), largerTree->keyMap->end());
        smallerTree->order += 1;
        largerTree->parent = smallerTree;
        return smallerTree;
    }
};

#endif //BINOMIALTREE_H
