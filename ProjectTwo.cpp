//Name: ProjectTwo.ccp
//Author: Taylor Stapus

#include <iostream>
#include <fstream>

#include "CSVparser.hpp"

using namespace std;

//Structure to hold course info
struct Course {
	string courseName;
	string courseNumber;
	vector<string> coursePrereq;

	Course() {}
};

//Structure for tree node
struct Node {
	Course course;
	Node* left; 
	Node* right;

	//defualt contructor 
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//initialize with a course
	Node(Course aCourse) :
		Node() {
		this->course = aCourse;
	}
};

// Binary Search Tree class definition

// CLass containing data members and methods
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void printCourseInfo(Node* node, string courseNumber);
	

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	void PrintCourseInfo(string courseNumber);
	int NumCoursePrereq(Course course);
};

//Deafualt Coonstructor 
BinarySearchTree::BinarySearchTree() {
	//initialize housekeeping variable
	root = nullptr;
}

//In Order function
void BinarySearchTree::InOrder() {
	// call inOrder function and pass root
	this->inOrder(root);
}

//Insert a course
void BinarySearchTree::Insert(Course course) {
	// if root equal to nullptr
	if (root == nullptr) {
		//root is equal to a new node course
		root = new Node(course);
	}
	else {
		//add Node root and bid
		this->addNode(root, course);
	}
}

// Print course info
void BinarySearchTree::PrintCourseInfo(string courseNumber) {
	this->printCourseInfo(root, courseNumber);
}

//Total of Prereqs
int BinarySearchTree::NumCoursePrereq(Course course) {
	int totalPrereq = 0;
	for (unsigned int i = 0; i < course.coursePrereq.size(); i++) {
		if (course.coursePrereq.at(i).length() > 0) {
			totalPrereq++;
		}
		return totalPrereq;
	}



}


//addNode function
void BinarySearchTree::addNode(Node* node, Course course) {
	//if node is larger then add to lfet
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		//if no left node
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		// else recurse down the left node
		else {
			this->addNode(node->left, course);
		}
	}

	else {
		//if no right node
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		
		else {
			//recurse down the right node
			this->addNode(node->right, course);
		}
	}
}

//inOder function
void BinarySearchTree::inOrder(Node* node) {
	//if node is not equal to nullptr
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ", " << node->course.courseName << ", " << endl;
		inOrder(node->right);
	}
	return;
}

//printCourseInfo function
void BinarySearchTree::printCourseInfo(Node* current, string courseNumber) {
	while (current != nullptr) {
		if (current->course.courseNumber.compare(courseNumber) == 0) {
			cout << endl << current->course.courseNumber << ", " << current->course.courseName << endl;
			unsigned int size = NumCoursePrereq(current->course);
			cout << "Prerequisites: ";

			// if prereq exists then print
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << current->course.coursePrereq.at(i);
				if (i != size - 1) {
					cout << ", ";
				}
			}

			//if there are no prereqs then tell user and return
			if (i == 0) {
				cout << "No prerequisites" << endl;
				return;
			}
		}

		else if (courseNumber.compare(current->course.courseNumber) < 0) {
			current = current->left;
		}

		else {
			current = current->right;
		}

		cout << "Course" << courseNumber << " not found" << endl;
	}

}

//Load courses from file
bool loadCourses(string csvPath, BinarySearchTree* bst) {
	//Open course file
	try {
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				//vector to hold data
				vector<string> courseInfo;
				string courseData;

				getline(courseFile, courseData);
				while (courseData.length() > 0) {

					//get substring of each course data and add to vector
					unsigned int comma = courseData.find(',');
					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					else {
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}

				//insert into Binary Tree
				Course course;
				course.courseNumber = courseInfo[0];
				course.courseName = courseInfo[1];

				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.coursePrereq.push_back(courseInfo[i]);
				}
				bst->Insert(course);
			}
			courseFile.close();
			return true;
		}
	}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}

//Main
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}

	// Define a binary search tree to hold all bids
	BinarySearchTree* bst = nullptr;

	//make user choice a string
	string choice = "0";
	int userChoice = choice[0] - '0';

	//menu

	while (userChoice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Load Courses" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course Info" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		//check if user coice is valid
		if (choice.length() == 1) {
			userChoice = choice[0] - '0';
		}
		else {
			userChoice = 0;
		}
		bool success = 1;

		//users selection
		switch (userChoice) {

		case 1:
			// sets bst to new Binary Search tree
			if (bst == nullptr) {
				bst = new BinarySearchTree();
			}
			// ask user of input
			if (csvPath.length() == 0) {
				cout << "Enter File: " << endl;
				cin >> csvPath;

			}
			//file opened successfully
			success = loadCourses(csvPath, bst);
			if (success) {
				cout << "Courses have been loaded.\n" << endl;
			}
			else {
				cout << "File is not found.\n" << endl;
			}
			csvPath = "";
			break;

		case 2:
			//call InOrder function
			bst->InOrder();
			break;

		case 3:
			//search course and print course info if found
			if (bst != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "Enter Course ID: ";
					cin >> courseId;
					for (auto& userChoice : courseId) {
						userChoice = toupper(userChoice);
					}
				}
				bst->PrintCourseInfo(courseId);
				cout << endl;
			}
			break;


		default:
			if (userChoice != 9) {
				cout << "Invalid Choice\n" << endl;
				break;
			}

		}
		cout << "Good bye." << endl;
		return 0;
	}
}
