#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct Course {
    string name;
    double grade;
    double creditHours;
    double gradePoints;
};

int main() {
    int numCourses;

    cout << "==========================================" << endl;
    cout << "          CGPA CALCULATOR                 " << endl;
    cout << "==========================================" << endl;

    cout << "\nEnter the number of courses: ";
    cin >> numCourses;

    if (numCourses <= 0) {
        cout << "Invalid number of courses. Exiting." << endl;
        return 1;
    }

    vector<Course> courses(numCourses);
    double totalCredits = 0;
    double totalGradePoints = 0;

    cin.ignore(); // Clear newline from buffer

    for (int i = 0; i < numCourses; i++) {
        cout << "\n--- Course " << (i + 1) << " ---" << endl;

        cout << "Enter course name: ";
        getline(cin, courses[i].name);

        cout << "Enter grade (0.0 - 4.0): ";
        while (!(cin >> courses[i].grade) || courses[i].grade < 0.0 || courses[i].grade > 4.0) {
            cout << "Invalid grade. Please enter a value between 0.0 and 4.0: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << "Enter credit hours: ";
        while (!(cin >> courses[i].creditHours) || courses[i].creditHours <= 0) {
            cout << "Invalid credit hours. Please enter a positive value: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        courses[i].gradePoints = courses[i].grade * courses[i].creditHours;
        totalCredits += courses[i].creditHours;
        totalGradePoints += courses[i].gradePoints;

        cin.ignore(); // Clear newline for next getline
    }

    double semesterGPA = (totalCredits > 0) ? (totalGradePoints / totalCredits) : 0.0;
    double cgpa = semesterGPA; // For single semester; extend for multiple semesters

  
    cout << "\n========================================================" << endl;
    cout << "                    COURSE SUMMARY                " << endl;
    cout << "========================================================" << endl;
    cout << left
         << setw(20) << "Course"
         << setw(10) << "Grade"
         << setw(15) << "Credit Hours"
         << setw(15) << "Grade Points"
         << endl;
    cout << "-----------------------------------------------------" << endl;

    for (const auto& c : courses) {
        cout << left
             << setw(20) << c.name
             << setw(10) << fixed << setprecision(2) << c.grade
             << setw(15) << c.creditHours
             << setw(15) << c.gradePoints
             << endl;
    }

    cout << "==================================" << endl;
    cout << "Total Credits    : " << totalCredits << endl;
    cout << "Total Grade Pts  : " << fixed << setprecision(2) << totalGradePoints << endl;
    cout << "Semester GPA     : " << fixed << setprecision(2) << semesterGPA << endl;
    cout << "CGPA             : " << fixed << setprecision(2) << cgpa << endl;
    cout << "==================================" << endl;

    // Letter grade interpretation
    cout << "\nCGPA Interpretation: ";
    if (cgpa >= 3.7)      cout << "A+ / Excellent" << endl;
    else if (cgpa >= 3.3) cout << "A  / Very Good" << endl;
    else if (cgpa >= 3.0) cout << "B+ / Good" << endl;
    else if (cgpa >= 2.7) cout << "B  / Above Average" << endl;
    else if (cgpa >= 2.3) cout << "C+ / Average" << endl;
    else if (cgpa >= 2.0) cout << "C  / Satisfactory" << endl;
    else if (cgpa >= 1.0) cout << "D  / Below Average" << endl;
    else                  cout << "F  / Fail" << endl;

    return 0;
}