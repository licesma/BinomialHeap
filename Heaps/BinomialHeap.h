//
// Created by Esteban on 19/09/24.
//

#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H
#include <list>

#include "BinomialTree.h"


class BinomialHeap {

private:
    BinomialTree *minTree;
    std::list<BinomialTree*> *trees;
public:
    unordered_map<string, BinomialTree*> *keyMap;
    BinomialHeap() {
        minTree = nullptr;
        trees = new std::list<BinomialTree*>();
        keyMap = new unordered_map<string, BinomialTree*>();
    }

    explicit BinomialHeap(std::list<BinomialTree*> *nodes) {
        minTree = nullptr;
        this->trees = nodes;
        keyMap = new unordered_map<string, BinomialTree*>();
        setMin();
        setMap();
    }

    void print(bool shortVersion = true) {
        vector<vector<vector<char>>> childrenMatrix;
        for(auto *tree: *this->trees) {
            childrenMatrix.push_back(tree->getMatrixRepresentation(shortVersion));
        }

        int i, maxDepth = 0;
        for( const auto& matrixRep: childrenMatrix) {
            int size = matrixRep.size();
            maxDepth = std::max(maxDepth, size);
        }

        for( i = 0; i <= maxDepth; i++ ) {
            vector<char> row;
            for( auto matrixRep: childrenMatrix) {
                if(i < matrixRep.size()) {
                    row.insert(row.end(), matrixRep[i].begin(), matrixRep[i].end());
                }
                else {
                    vector<char> white(matrixRep[0].size(), ' ');
                    row.insert(row.end(), white.begin(), white.end());
                }
            }
            cout<<std::string(row.begin(), row.end())<<"\n";
        }
    }


private: explicit BinomialHeap(int number) {
        minTree = nullptr;
        this->trees = new std::list({new BinomialTree(number)});
        setMin();
    }


    void setMin() {
            for(BinomialTree* tree: *this->trees){
                if(this->minTree == nullptr || tree->value < this->minTree->value ) {
                    this->minTree = tree;
                }
            }
    }

    void setMap() {
        for(BinomialTree* tree: *this->trees){
            keyMap->insert(tree->keyMap->begin(), tree->keyMap->end());
        }
}

public: void merge(const BinomialHeap *other) {
        auto *newTrees = new std::list<BinomialTree*>;
        auto itOne = this->trees->begin(), itTwo = other->trees->begin();
        while(itOne != this->trees->end() || itTwo != other->trees->end()) {
            BinomialTree* treeOne = itOne != this->trees->end() ? *itOne : nullptr;
            BinomialTree* treeTwo = itTwo != other->trees->end() ? *itTwo : nullptr;
            int orderOne = treeOne != nullptr ? treeOne->order : INT_MAX;
            int orderTwo = treeTwo != nullptr ? treeTwo->order : INT_MAX;

            if(treeOne != nullptr || treeTwo != nullptr) {
                if(orderOne == orderTwo) {
                    BinomialTree* merged = BinomialTree::merge(treeOne, treeTwo);
                    newTrees->push_back(merged);
                    ++itOne;
                    ++itTwo;
                }
                else if(orderOne < orderTwo) {
                    if(!newTrees->empty() && newTrees->back()->order == orderOne) {
                        BinomialTree* merged = BinomialTree::merge(newTrees->back(), treeOne);
                        newTrees->pop_back();
                        newTrees->push_back(merged);
                    }else {
                        newTrees->push_back(treeOne);
                    }
                    ++itOne;
                }
                else { //orderTwo < orderOne
                    if(!newTrees->empty() && newTrees->back()->order == orderTwo) {
                        BinomialTree* merged = BinomialTree::merge(newTrees->back(), treeTwo);
                        newTrees->pop_back();
                        newTrees->push_back(merged);
                    }else {
                        newTrees->push_back(treeTwo);
                    }
                    ++itTwo;
                }
            }
        }

        BinomialTree* minTree = nullptr;
        for(BinomialTree* tree: *newTrees) {
            if(minTree == nullptr) {
                minTree = tree;
            }
            else {
                if(tree->value < minTree->value) {
                    minTree = tree;
                }
            }
        }

        this->trees = newTrees;
        this->setMin();
        this->setMap();
    }

    int getMin() {
        if(minTree != nullptr) {
            return minTree->value;
        }

        throw  std::domain_error("Cannot get min from an empty heap.");
    }

    void deleteMin() {
        if(minTree != nullptr) {
            this->trees->remove(minTree);
            for(BinomialTree* c : *minTree->children) {
                c->parent = nullptr;
            }
            auto newHeap = new BinomialHeap(minTree->children);
            this->minTree = nullptr;
            this->merge(newHeap);
        }
        else {
            throw  std::domain_error("Cannot get min from an empty heap.");
        }

    }


    void insert(int number) {
        auto newHeap = new BinomialHeap(number);
        this->merge(newHeap);
    }

    void decreaseKey(string id, int value) {
        BinomialTree* target = this->keyMap->at(id);
        target->value = value;
        while(target->parent != nullptr && target->parent->value > target->value) {
            BinomialTree* parent = target->parent;

                auto aux= new BinomialTree(0);
                aux->children = target->children;
                aux->order = target->order;
                target->children = parent->children;
                list<BinomialTree *>::iterator eraseIt;
                for (auto it = target->children->begin(); it != target->children->end(); ++it) {
                    if(*it == target) {
                        eraseIt = target->children->erase(it);
                        target->children->insert(eraseIt, parent);
                        break;
                    }
                }

                if(parent->parent != nullptr) {
                    BinomialTree* gParent = parent->parent;
                    for (auto it = gParent->children->begin(); it != gParent->children->end(); ++it) {
                        if(*it == parent) {
                            eraseIt = gParent->children->erase(it);
                            gParent->children->insert(eraseIt, target);
                            break;
                        }
                    }
                }else {
                    for (auto it = this->trees->begin(); it != this->trees->end(); ++it) {
                        if(*it == parent) {
                            eraseIt = this->trees->erase(it);
                            this->trees->insert(eraseIt, target);
                            break;
                        }
                    }

                }


                target->order = parent->order;
                target->parent = parent->parent;

                parent->children = aux->children;
                parent->order = aux->order;
                for(auto child: *target->children) {
                    child->parent = target;
                }
                for(auto child: *parent->children) {
                    child->parent = parent;
                }

                this->setMin();
        }
}

    static BinomialHeap* create_random(vector<int> orders) {
        sort(orders.begin(), orders.end());

        auto trees = new list<BinomialTree*>();
        for(int order: orders) {
            trees->push_back(BinomialTree::create_random(order));
        }
        return new BinomialHeap(trees);
    }
};



#endif //BINOMIALHEAP_H
