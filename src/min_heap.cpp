#include "min_heap.hpp"
#include <iostream>

using namespace std;

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2; 
        
        if (heap[index].dist < heap[parent].dist) {
            swap(heap[index], heap[parent]);
            index = parent; 
        } else {
            break; 
        }
    }
}

void MinHeap::heapifyDown(int index) {
    int size = heap.size();
    while (true) {
        int leftChild = 2 * index + 1;  
        int rightChild = 2 * index + 2;
        int smallest = index;          

        if (leftChild < size && heap[leftChild].dist < heap[smallest].dist) {
            smallest = leftChild;
        }
        if (rightChild < size && heap[rightChild].dist < heap[smallest].dist) {
            smallest = rightChild;
        }
        
        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void MinHeap::push(int v, int dist) {
    HeapNode newNode = {v, dist};
    heap.push_back(newNode);     
    heapifyUp(heap.size() - 1);  
}

HeapNode MinHeap::pop() {
    if (isEmpty()) {
        cout << "Proba sciagniecia elementu z pustego kopca" << endl;
        return {-1, -1};
    }

    HeapNode root = heap[0]; 
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    return root; 
}