// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// struct for a course
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

// struct for a node in binary search tree
struct TreeNode {
	Course course;
	TreeNode* left;
	TreeNode* right;

	TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// BST class
class BST {
private:
	TreeNode* root;

	void insertRecursively(TreeNode*& node, Course course) {
		if (node == nullptr) {
			node = new TreeNode(course);
		}
		else if (course.courseNumber < node->course.courseNumber) {
			insertRecursively(node->left, course);
		}
		else {
			insertRecursively(node->right, course);
		}
	}

	void destroyTree(TreeNode* node) {
		if (node != nullptr) {
			destroyTree(node->left);
			destroyTree(node->right);
			delete node;
		}
	}

	TreeNode* search(TreeNode* node, const string& courseNumber) {
		if (node == nullptr || node->course.courseNumber == courseNumber) {
			return node;
		}
		if (courseNumber < node->course.courseNumber) {
			return search(node->left, courseNumber);
		}
		else {
			return search(node->right, courseNumber);
		}
	}

	void printAllCourses(TreeNode* node) {
		if (node != nullptr) {
			printAllCourses(node->left);
			cout << "Course Number: " << node->course.courseNumber << ", "
				<< "Course Name: " << node->course.courseName << endl;
			if (!node->course.prerequisites.empty()) {
				cout << "Prerequisites: ";
				for (const string& prereq : node->course.prerequisites) {
					cout << prereq << " ";
				}
				cout << endl;
			}
			printAllCourses(node->right);
		}
	}
public:

	BST() : root(nullptr) {}

	~BST() {
		destroyTree(root);
	}

	// insert course into BST
	void insert(Course course) {
		insertRecursively(root, course);
	}

	// search for course in BST
	TreeNode* search(const string& courseNumber) {
		return search(root, courseNumber);
	}

	// print every course in alphanumeric order
	void printAllCourses() {
		printAllCourses(root);
	}
};


// read file and load course into BST

BST* loadCoursesFromFile(const string& filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		cout << "Error: Unable to open file." << endl;
		return nullptr;
	}

	BST* bst = new BST();
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		string courseNumber, courseName;
		vector<string> prerequisites;

		getline(ss, courseNumber, ',');
		getline(ss, courseName, ',');

		string prereq;
		while (getline(ss, prereq, ',')) {
			prerequisites.push_back(prereq);
		}

		Course course = { courseNumber, courseName, prerequisites };
		bst->insert(course);
	}

	file.close();
	return bst;
}

// print course info and display menu

void printCourseInfo(BST* bst, const string& courseNumber) {
	TreeNode* node = bst->search(courseNumber);
	if (node != nullptr) {
		cout << "Course Number: " << node->course.courseNumber << endl;
		cout << "Course Name: " << node->course.courseName << endl;
		if (!node->course.prerequisites.empty()) {
			cout << "Prerequisites: ";
			for (const string& prereq : node->course.prerequisites) {
				cout << prereq << " ";
			}
			cout << endl;
		}
		else {
			cout << "No prerequisites." << endl;
		}
	}
	else {
		cout << "Course not found." << endl;
	}
}

int main() {
	BST* bst = nullptr;
	int choice;
	string filePath;

	do {
		// Display menu
		cout << "Welcome to the course planner." << endl;
		cout << "1. Load data structure" << endl;
		cout << "2. Print course list." << endl;
		cout << "3. Print course." << endl;
		cout << "9. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "Enter file name: ";
			cin >> filePath;
			bst = loadCoursesFromFile(filePath);
			if (bst != nullptr) {
				cout << "Data loaded successfully." << endl;
			}
			break;
		case 2:
			if (bst != nullptr) {
				bst->printAllCourses();
			}
			else {
				cout << "Please load data first." << endl;
			}
			break;
		case 3:
			if (bst != nullptr) {
				string courseNumber;
				cout << "Enter course number: ";
				cin >> courseNumber;
				printCourseInfo(bst, courseNumber);
			}
			else {
				cout << "Please load data first." << endl;
			}
			break;
		case 9:
			cout << "Exiting program." << endl;
			break;
		default:
			cout << "Invalid option. Please try again." << endl;
			break;
		}
	} while (choice != 9);

	delete bst;
	return 0;
}