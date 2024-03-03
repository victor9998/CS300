#include <algorithm>
#include <iostream>
#include <time.h>
#include <utility>

#include "CSVparser.hpp"

using namespace std;

// Forward declarations
double strToDouble(string str, char ch);

// Define a structure to hold course information
struct Course {
    string courseNumber; // Course number
    string courseTitle; // Name of the course
    string prerequisites; // Course number of the prerequisites (if any)
};

// Display the course information 
void displayCourse(Course course) {
    cout << course.courseNumber << ": " << course.courseTitle << " | " << " Prerequisites: "
        << course.prerequisites << " | " << endl;
    return;
}

// Display the searched course information
void displaySearchedCourse(Course course) {
    cout << course.courseNumber << ": " << course.courseTitle << endl;
    cout << "Prerequisites: " << course.prerequisites << endl;
    return;
}

// Load CSV file containing the courses into a container
vector<Course> loadCourses(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector to hold a collection of courses
    vector<Course> courses;

    // Initialize CSV parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // Loop to read the rows of the file
        for (int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of courses
            Course course;
            course.courseNumber = file[i][1];
            course.courseTitle = file[i][0];
            course.prerequisites = file[i][2];

            // Push the course to the end
            courses.push_back(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return courses;
}

// Partition the vector into two parts, low and high
int partition(vector<Course>& courses, int begin, int end) {
    // Set low and high equal to begin and end
    int low = begin;
    int high = end;

    // Pick the middle element as pivot point
    Course pivot = courses[low + (high - low) / 2];

    // While not done
    while (low <= high) {
        // Keep incrementing low index
        while (courses[low].courseNumber < pivot.courseNumber) {
            ++low;
        }
        // Keep decrementing high index
        while (pivot.courseNumber < courses[high].courseNumber) {
            --high;
        }
        // If there are zero or one elements remaining, all courses are partitioned. 
        if (low >= high) {
            return high;
        }
        // Else swap the low and high courses, move low and high closer
        else {
            swap(courses[low], courses[high]);
            ++low;
            --high;
        }
    }
    return high;
}

// Binary search to find a course
int search(vector<Course>& courses, int count, const string& key) {
    // Initializes variables
    int mid = 0;
    int low = 0;
    int high = count - 1;

    // While not done
    while (high >= low) {
        // Calculate the midpoint
        mid = (high + low) / 2;
        // Search the right half
        if (courses[mid].courseNumber <  key) {
            low = mid + 1; 
        }
        // Search the left half
        else if (courses[mid].courseNumber > key) {
            high = mid - 1;
        }
        // Return the course
        else {
            return mid;
        }
    }
    // Course not found
    return -1;
}

// Performs a quick sort on course title
void quickSort(vector<Course>& courses, int begin, int end) {
    // Set midpoint equal to 0
    int p = 0;
    // If begin is greater than or equal to end
    if (begin >= end) {
        return;
    }
    // Partition courses into low and high
    p = partition(courses, begin, end);
    // Recursively sort low partition
    quickSort(courses, begin, p);
    // Recursively sort high partition
    quickSort(courses, p + 1, end);
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    // Process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "Courses.csv";
    }

    // Define a vector to hold all the courses
    vector<Course> courses;

    // Name for the file that is going to be used
    string fileName;

    // Menu header and welcome message
    cout << "Welcome to the course planner!" << endl;
    cout << "MENU:" << endl;

    // Initializes the menu choice with 0
    int choice = 0;

    // While the choice is not 9 (exit), show the menu
    while (choice != 9) {
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;

        // Asks the user for their choice
        cout << "What would you like to do? ";
        cin >> choice;
        cout << endl;

        // Switch statement for the menu
        switch (choice) {

        // Asks the user for the file they would like to load and reads that file
        case 1:
            // Asks user for name of file and tells them what to input
            cout << "What file would you like to load? (Courses.csv) ";
            cin >> fileName;
            cout << endl;

            // If the users input is the file that is being used, load and read the data
            if (fileName == "Courses.csv") {
                // Complete the method call to load the courses
                courses = loadCourses(csvPath);

                cout << fileName << " was successfully loaded" << endl;
                cout << courses.size() << " courses read" << endl;
                cout << endl;
                break;
            }

            // If the users input did not match the file name
            else {
                cout << "There is no file with that name, please try again." << endl;
            }

            break;

        // Sorts and prints the courses in the data file
        case 2:
            // Quick sorts the courses read by course number
            quickSort(courses, 0, courses.size() - 1);

            // Tells the user that the courses were sorted 
            cout << courses.size() << " courses sorted" << endl;
            cout << endl;

            // For loop to display the courses and a list header
            cout << "COURSE LIST: " << endl;
            for (int i = 0; i < courses.size(); ++i) {
                displayCourse(courses[i]);
            }
            cout << endl;

            break;

        // Prints out the specified course information
        case 3:
        {
            string key;
            int index;

            // Quick sorts the courses read by course number first
            quickSort(courses, 0, courses.size() - 1);

            // Asks the user for the course they want to search 
            cout << "What course do you want to know about? (Enter Course Number) ";
            cin >> key;

            index = search(courses, courses.size(), key);

            // If course is found
            if (index != -1) {
                // Call the function to display the course and its prerequisites
                cout << endl;
                displaySearchedCourse(courses[index]);
                cout << endl;
            }

            // If the course if not found, display an error message
            else {
                cout << "There is no course with that number, please try again.";
                cout << endl;
            }

            break;
        }

        // Closes and ends the program
        case 9:     
            cout << "Thank you for using the course planner! " << endl;
            return 0;

        // Default case when the user inputs an incorrect choice
        default: 
            cout << "That is not a valid option, try again" << endl;
            cout << endl; 
            cin.clear();
            cin.ignore(1000, '\n');
            break;
        }
    }
}