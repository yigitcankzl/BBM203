#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    // TODO: Your code here

    top = -1; //bos baslatmak
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    // TODO: Your code here

    if (top<MAX_SIZE-1) {
        top++;
        data[top] = province;
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    // TODO: Your code here

    if (isEmpty()) {
        return -1;

    }else {
        int province = data[top];
        top--;
        return province; //pop cikardigini dondurur
    }

}

// Returns the top province without removing it
int Stack::peek() const {
    // TODO: Your code here

    if (isEmpty()) {
        return -1;

    }else {
        return data[top];

    }

}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    // TODO: Your code here

    if (top == -1) {
        return true;
    }else {
        return false;
    }
}

// Function to get the current size of the stack
int Stack::getSize() const {
    // TODO: Your code here

    return top + 1;
}
