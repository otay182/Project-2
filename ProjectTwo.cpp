#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

// -------------------------------
// Course object
// -------------------------------
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// -------------------------------
// BST node
// -------------------------------
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

// -------------------------------
// Binary Search Tree for courses
// -------------------------------
class CourseBST {
private:
    Node* root;

    // Helper: insert a course into the BST
    Node* insert(Node* node, const Course& course) {
        if (node == nullptr) {
            return new Node(course);
        }
        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    // Helper: in-order traversal to print sorted list
    void printInOrder(Node* node) const {
        if (node == nullptr) return;
        printInOrder(node->left);
        cout << node->course.courseNumber << ": " << node->course.title << endl;
        printInOrder(node->right);
    }

    // Helper: search for a course by number
    Node* search(Node* node, const string& courseNumber) const {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return node;

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }

public:
    CourseBST() : root(nullptr) {}

    // Insert a course into the tree
    void Insert(const Course& course) {
        root = insert(root, course);
    }

    // Print all courses in alphanumeric order
    void PrintCourseList() const {
        if (root == nullptr) {
            cout << "No course data loaded." << endl;
            return;
        }
        printInOrder(root);
    }

    // Find a course node by number
    Node* FindCourse(const string& courseNumber) const {
        return search(root, courseNumber);
    }

    // Print information for a single course
    void PrintCourseInfo(const string& courseNumber) const {
        Node* node = FindCourse(courseNumber);
        if (node == nullptr) {
            cout << "Course not found." << endl;
            return;
        }

        cout << node->course.courseNumber << ": " << node->course.title << endl;

        if (node->course.prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            cout << "Prerequisites:" << endl;
            for (const string& pNum : node->course.prerequisites) {
                Node* pNode = FindCourse(pNum);
                if (pNode != nullptr) {
                    cout << "  " << pNode->course.courseNumber
                        << ": " << pNode->course.title << endl;
                }
                else {
                    // If prerequisite not found in data, still show its number
                    cout << "  " << pNum << " (course data not found)" << endl;
                }
            }
        }
    }
};

// -------------------------------
// Load course data from file
// -------------------------------
bool LoadCourseFile(const string& fileName, CourseBST& bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << fileName << "\"." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split line by comma
        while (getline(ss, token, ',')) {
            // Trim whitespace (simple trim)
            size_t start = token.find_first_not_of(" \t\r\n");
            size_t end = token.find_last_not_of(" \t\r\n");
            if (start == string::npos) {
                tokens.push_back("");
            }
            else {
                tokens.push_back(token.substr(start, end - start + 1));
            }
        }

        // Expect at least course number and title
        if (tokens.size() < 2) {
            cout << "Format error: Line has fewer than 2 fields: " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.title = tokens[1];

        // Remaining tokens are prerequisites
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        bst.Insert(course);
    }

    file.close();
    return true;
}

// -------------------------------
// Print menu
// -------------------------------
void PrintMenu() {
    cout << "\n----------------------------------------\n";
    cout << "  ABCU Advising Assistance Program\n";
    cout << "----------------------------------------\n";
    cout << "1. Load course data file\n";
    cout << "2. Print all courses (alphanumeric)\n";
    cout << "3. Print course information\n";
    cout << "9. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Enter your choice: ";
}

int main() {
    CourseBST bst;
    bool dataLoaded = false;
    int choice = 0;

    while (choice != 9) {
        PrintMenu();
        cin >> choice;

        // Handle non-numeric input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a numeric option." << endl;
            continue;
        }

        if (choice == 1) {
            string fileName;
            cout << "Enter course data file name: ";
            cin >> fileName;

            if (LoadCourseFile(fileName, bst)) {
                cout << "Course data loaded successfully." << endl;
                dataLoaded = true;
            }
            else {
                cout << "Failed to load course data." << endl;
            }

        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Error: Please load data first (Option 1)." << endl;
            }
            else {
                cout << "\nCourse List (Alphanumeric):\n";
                bst.PrintCourseList();
            }

        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Error: Please load data first (Option 1)." << endl;
            }
            else {
                string courseNum;
                cout << "Enter course number: ";
                cin >> courseNum;
                cout << endl;
                bst.PrintCourseInfo(courseNum);
            }

        }
        else if (choice == 9) {
            cout << "Goodbye." << endl;

        }
        else {
            cout << "Invalid option. Please choose 1, 2, 3, or 9." << endl;
        }
    }
    return 0;
}
    