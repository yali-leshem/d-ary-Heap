#include <iostream>
#include <fstream>
#include <vector>

void dMaxHeapify(std::vector<int>& A, int i, int d, int count);
void buildHeap(std::vector<int>& A, int d, int len);
int heapExtractMax(std::vector<int>& A, int d, int& count);
void insert(std::vector<int>& A, int key, int d, int& count);
void increaseKey(std::vector<int>& A, int i, int key, int d);
void printHeap(const std::vector<int>& A, int len);
std::vector<int> resizeArray(const std::vector<int>& arr, int newSize);

int main(int argc, char* argv[]) {
    if (argc != 2) { // in case the command line arguements weren't given as request (a single filename in the directory)
        std::cerr << "File argument was not given as requested" << std::endl; 
        return 1;
    }

    std::ifstream fileStream(argv[1]); // opening file
    if (!fileStream.is_open()) { // When an error was found while opening file
        std::cerr << "File cannot be opened" << std::endl;
        return 1;
    }

    int num;
    std::vector<int> ptrArr; // using vector for storaging all elements

    while (fileStream >> num) { // pushing all numbers found in file
        ptrArr.push_back(num);
    }

    fileStream.close(); // closing file...

    int i = 0, count = ptrArr.size()-1, d;

    std::cout << "Enter the number of children for each node in the heap: ";
    std::cin >> d; // recieving as an input the required "d"

    buildHeap(ptrArr, d, count); // calling "buildHeap" function in order to sort the given heap elements 
    std::cout << "Heap was built successfully" << std::endl;

    char command;
    int key, element, max;

    while (true) { // loop continues until user decides to quit
        std::cout << "You have the options shown below: " << std::endl;
        std::cout << "Type 'Q' to quit activating functions on the heap" << std::endl;
        std::cout << "Type 'E' to extract the maximum of the heap" << std::endl;
        std::cout << "Type 'G' for inserting a new node to the heap" << std::endl;
        std::cout << "Type 'F' for increasing the key of a node" << std::endl;
        std::cout << "Type 'P' for printing the current heap" << std::endl; 

        if (!(std::cin >> command)) { // If a problem was found in user's input - break loop
            break;
        } 

        switch (command) {
            case 'Q': // When user wants to quit program
                std::cout << "Program is finished, as requested" << std::endl;
                return 0;
            case 'E':
                max = heapExtractMax(ptrArr, d, count); // extracting maximum
                std::cout << "Maximum was extracted as requested, and is " << max << std::endl; // printing the maximum element
                count--; // decreasing the count variable as now there's one less element
                break;
            case 'G':
                std::cout << "What key would you like to insert to the heap?" << std::endl;
                std::cin >> key; // recieveing the new node's value
                insert(ptrArr, key, d, count); // calling insert function
                std::cout << "Key was inserted to the heap successfully" << std::endl;
                count++; // increasing the count variable as now there's one more element
                break;
            case 'F':
                std::cout << "What element index would you like to increase (index)?" << std::endl;
                std::cin >> element; // recieveing the index of increased element
                std::cout << "What key would you like to increase to?" << std::endl;
                std::cin >> key; // recieveing the new value requested to increase the element to
                increaseKey(ptrArr, element, key, d); // calling function for increasing key
                std::cout << "Increased key successfully" << std::endl;
                break;
            case 'P':
                std::cout << "Printing current heap: " << std::endl; // printing current heap
                printHeap(ptrArr, count); // calling function for the print
                break;
            default: // In case the user didn't type any legal letter for one of the actions
                std::cout << "Command was not typed in the requested format" << std::endl;
        }
    }
    return 0;
}

void dMaxHeapify(std::vector<int>& A, int i, int d, int count) {
    int largest = i, child, temp;
    for (int j = 1; j <= d; j++) { // loop going through all d nodes
        child = d * i + j;
        if (child < count && A[child] > A[largest]) { 
            largest = child; // the 'if' condition make sure the largest value's index of all d-children AND the father will be stored in largest
        }
    }
    if (largest != i) { //When found a child with bigger value than it's fathers, exchange them
        temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        dMaxHeapify(A, largest, d, count); // calls recursively for the function for any more switches needed
    }
}

void buildHeap(std::vector<int>& A, int d, int len) {
    for (int i = (len / d) - 1; i >= 0; i--) { // function sorting from the first non-leaf node
        dMaxHeapify(A, i, d, len); // calling heapify in order to switch any unsorted nodes between a father node and it's d children ones
    }
}

int heapExtractMax(std::vector<int>& A, int d, int& count) {
    if (count <= 0) {
        std::cerr << "Heap underflow" << std::endl; //If there is no node to extract - quit program
        std::exit(1);
    }

    int max = A[0]; // extracting the maximum
    A[0] = A[count - 1];
    dMaxHeapify(A, 0, d, count - 1); // calling heapify in order to slide down the smallest value which has been placed as the root for re-sort
    return max; // returning the maximum value
}

void insert(std::vector<int>& A, int key, int d, int& count) {
    A = resizeArray(A, 1); // resizing array, and allocating a new one, which length's longer by one compartment
    A[count] = key; // inserting the new node value in the last compartment
    int index = count;
    int parent = index / d;
    while (index >= 0 && A[parent] < A[index]) { // loop continues as long as switches are needed between the new node and it's father's
        int temp = A[parent]; // exchanging new node and it's father's
        A[parent] = A[index];
        A[index] = temp;
        index = parent;
        parent = index / d; // updating the new node's current index and accordingly it's father's
    }
}

void increaseKey(std::vector<int>& A, int i, int key, int d) {
    if (key < A[i]) {
        std::cout << "The key is smaller than the current key" << std::endl; // When the new key value is smaller than the current one
    }
    A[i] = key; // increases value in requested index to the requested key value
    int parent = i / d;

    while (i >= 0 && A[parent] < A[i]) { // loop continues until new value of the node isn't needed to be exchanged with it's father's
        int temp = A[parent]; // exchanging new node's value and it's father's
        A[parent] = A[i];
        A[i] = temp;
        i = parent;
        parent = i / d; // updating the new node's value current index and accordingly it's father's
    }
}

void printHeap(const std::vector<int>& A, int len) {
    for (int i = 0; i < len; i++) {
        std::cout << A[i] << " "; // printing all heap elements
    }
    std::cout << std::endl;
}

std::vector<int> resizeArray(const std::vector<int>& arr, int newSize) {
    std::vector<int> newArr(arr.size() + newSize); // allocating a new memory space for new array
    std::copy(arr.begin(), arr.end(), newArr.begin()); // copying all previous elements to the first 'count' compartments of it
    return newArr; // returning the updated arrah
}
