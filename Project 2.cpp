/* 
    Author: Jeremiah Brown
    Date: 2/23/25
    Scenario: The academic advisors in the Computer Science department at ABCU are very happy with the planning and design you have completed
    for the advising assistance software. Now, they would like you to write the code for the application so the department advisors can use it
    to help when they talk with students.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Structure to store course details
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node structure for BST
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BST {
private:
    Node* root;

    // Helper function to insert a course into the BST
    void insert(Node*& node, Course course) {
        if (!node) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    // Helper function to print courses in alphanumeric order
    void printInOrder(Node* node) {
        if (node) {
            printInOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            printInOrder(node->right);
        }
    }

    // Helper function to search for a course in the BST
    Course* search(Node* node, const string& courseNumber) {
        if (!node) return nullptr;
        if (node->course.courseNumber == courseNumber) return &node->course;
        if (courseNumber < node->course.courseNumber) return search(node->left, courseNumber);
        return search(node->right, courseNumber);
    }

public:
    BST() : root(nullptr) {}

    // Public method to insert a course into the BST
    void insert(Course course) {
        insert(root, course);
    }

    // Public method to print all courses in alphanumeric order
    void printAllCourses() {
        printInOrder(root);
    }

    // Public method to search for a course
    Course* search(const string& courseNumber) {
        return search(root, courseNumber);
    }
};

// Function to load course data from a file into the BST
void loadCourses(const string& filename, BST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {  // If the file cannot be opened, return
        cout << "Error: Unable to open file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {   // While csv is being read, populate the bst
        stringstream ss(line);
        string courseNumber, courseTitle, prerequisite;
        vector<string> prerequisites;

        getline(ss, courseNumber, ',');
        getline(ss, courseTitle, ',');
        while (getline(ss, prerequisite, ',')) {
            prerequisites.push_back(prerequisite);
        }

        bst.insert({ courseNumber, courseTitle, prerequisites });
    }

    file.close();
    cout << "Course data loaded successfully!" << endl;
}

int main()
{
    BST bst;
    int choice;
    string filename, courseNumber;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    do {
        // Display menu options
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            // Check if data has already been pulled from file
            // To prevent duplication of data
            if (dataLoaded) {
                cout << "Data already loaded!";
            }
            else {
                // Load course data from file
                filename = "CS 300 ABCU_Advising_Program_Input.csv";
                loadCourses(filename, bst);
                dataLoaded = true;
            }            
            break;
        case 2:
            // Print all courses in alphanumeric order
            if (!dataLoaded) {
                cout << "Error: Load course data first (Option 1)." << endl;
            }
            else {
                bst.printAllCourses();
            }
            break;
        case 3:
            // Search and display details of a specific course
            if (!dataLoaded) {
                cout << "Error: Load course data first (Option 1)." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                // Convert course number to uppercase for case-insensitive comparison
                for (char& c : courseNumber) c = toupper(c);
                Course* course = bst.search(courseNumber);
                if (course) {
                    cout << course->courseNumber << ", " << course->courseTitle << endl;
                    if (!course->prerequisites.empty()) {
                        cout << "Prerequisites: ";
                        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                            cout << course->prerequisites[i];
                            if (i < course->prerequisites.size() - 1) {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    }
                }
                else {
                    cout << "Course not found!" << endl;
                }
            }
            break;
        case 9:
            // Exit program
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 9);

    return 0;
}