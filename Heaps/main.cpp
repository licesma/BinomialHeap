#include <iostream>

#include "BinomialHeap.h"
#include "BinomialTree.h"

using namespace std;

int main()
{
    string heapCommand;
    cout<<"Press [(R)-> Random Heap, (E)->Empty Heap]\n";
    cin>>heapCommand;
    if (heapCommand == "R") {
        string orders;
        cout<<"Enter the orders of the Heap";
        cin>>orders;
        string segment;
        stringstream ss(orders);
        vector<string> orderStrings;
        vector<int> orderNums;
        while(getline(ss, segment, ',')) {
            orderStrings.push_back(segment);
        }
        for(auto seg: orderStrings) {
            orderNums.push_back(stoi(seg));
        }
        heap = BinomialHeap::create_random(orderNums);
        heap->print();
    }
    else {
        heap = new BinomialHeap();
    }

    string command = "R";
    while(command != "T") {
        if(command == "i") {
            string number;
            cout<<"Enter a two-digit number: ";
            cin>>number;
            heap->insert(stoi(number));
            heap->print();
        }
        else if (command == "m") {
            cout<<"The min is: "<<heap->getMin()<<"\n";
        }
        else if (command == "d") {
            heap->deleteMin();
            heap->print();
        }
        else if (command == "k") {
            heap->print(false);
            string id;
            string value;
            cout<<"Insert the id:";
            cin>>id;
            cout<<"\nInsert the value:";
            cin>>value;
            heap->decreaseKey(id, stoi(value));
            heap->print();
        }
        else {
        }
        cout<<"Press [(i)->insert, (m)->min, (d)->delete-min, (k)->decrease key]---[(T) to terminate]\n";
        cin>>command;
    }
    return 0;
}
