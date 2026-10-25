//============================================================================
// Name        : LinkedList.cpp
// Author      : Your Name
// Version     : 1.0
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;

// Forward declaration
double strToDouble(string str, char ch);

// Structure to hold bid information
struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;

    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// LinkedList class definition
//============================================================================

class LinkedList {

private:
    // Internal node structure
    struct Node {
        Bid bid;
        Node* next;

        Node() : next(nullptr) {}
        Node(Bid aBid) : bid(aBid), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList();
    virtual ~LinkedList();

    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

// Default constructor
LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destructor ? frees all nodes
LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

// Append a bid to the end of the list
void LinkedList::Append(Bid bid) {
    Node* newNode = new Node(bid);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

// Prepend a bid to the front of the list
void LinkedList::Prepend(Bid bid) {
    Node* newNode = new Node(bid);

    if (head != nullptr) {
        newNode->next = head;
    }

    head = newNode;

    if (tail == nullptr) {
        tail = newNode;
    }

    size++;
}

// Print all bids in the list
void LinkedList::PrintList() {
    Node* current = head;

    while (current != nullptr) {
        cout << current->bid.bidId << ": "
            << current->bid.title << " | "
            << current->bid.amount << " | "
            << current->bid.fund << endl;

        current = current->next;
    }
}

// Remove a bid by ID
void LinkedList::Remove(string bidId) {

    // Special case: head matches
    if (head != nullptr && head->bid.bidId == bidId) {
        Node* temp = head;
        head = head->next;

        if (temp == tail) {
            tail = nullptr;
        }

        delete temp;
        size--;
        return;
    }

    Node* current = head;

    // Traverse list looking ahead at current->next
    while (current != nullptr && current->next != nullptr) {

        if (current->next->bid.bidId == bidId) {
            Node* temp = current->next;
            current->next = temp->next;

            if (temp == tail) {
                tail = current;
            }

            delete temp;
            size--;
            return;
        }

        current = current->next;
    }
}

// Search for a bid by ID
Bid LinkedList::Search(string bidId) {
    Node* current = head;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        current = current->next;
    }

    return Bid(); // empty bid
}

// Return number of bids in list
int LinkedList::Size() {
    return size;
}

//============================================================================
// Helper functions
//============================================================================

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | "
        << bid.amount << " | " << bid.fund << endl;
}

Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

void loadBids(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);

    try {
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            list->Append(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

//============================================================================
// Main
//============================================================================

int main(int argc, char* argv[]) {

    string csvPath, bidKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;
    LinkedList bidList;
    Bid bid;

    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);
            break;

        case 2:
            ticks = clock();
            loadBids(csvPath, &bidList);
            cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 3:
            bidList.PrintList();
            break;

        case 4:
            ticks = clock();
            bid = bidList.Search(bidKey);
            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 5:
            bidList.Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;
    return 0;
}

