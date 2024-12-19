#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here

    front = -1;
    rear = -1; //bos baslattik
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province  

    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        //dolu

    }else {

        if (front == -1){
            front = 0;

        }
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        data[rear] = province;
    }   

}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if(isEmpty()) {
        return -1;

    }else {
        int province = data[front];
        if (front == rear) {    
            front = -1;
            rear = -1;

        }else {
            front = (front + 1) % MAX_QUEUE_SIZE;

        }

        return province;
    }

}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here

    if (isEmpty()) {
        return -1;

    }else {
        return data[front];

    }
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here

    if (front == -1) {
        return true;
    }else {
        return false;
    }

}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here

    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        //dolu
    }else {
        if (front == -1){
            front = 0;
            rear = 0;

        }else {
            front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE; 

        }
        data[front] = province;
    }  
}



void Queue::printQueue() const {
    if (isEmpty()) {
        std::cout << "Queue is empty." << std::endl;
    } else {
        std::cout << "Queue contents: ";
        int i = front;
        while (i != rear) {
            std::cout << data[i] << " ";
            i = (i + 1) % MAX_QUEUE_SIZE;
        }
        std::cout << data[rear] << std::endl;
    }
}

bool Queue::contains(int province) {
    if (isEmpty()) {
        return false;
    }

    for (int i = front; i != rear; i = (i + 1) % MAX_QUEUE_SIZE) {
        if (data[i] == province) {
            return true;
        }
    }

    if (data[rear] == province) {
        return true;
    }
    return false;
}