#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <vector>

using namespace std;

struct HeapNode {
    int v;   
    int dist; 
};

class MinHeap {
private:
    vector<HeapNode> heap; 

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MinHeap() {}

    bool isEmpty() const { 
        return heap.empty(); 
    }

    void push(int v, int dist);

    HeapNode pop();
};

#endif