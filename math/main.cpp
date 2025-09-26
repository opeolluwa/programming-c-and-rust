#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

void exercise1() {
    const double pi = 3.14159;
    double radius;

    cout << "Enter the radius of the circle: ";
    cin >> radius;

    double area = pi * radius * radius;
    cout << "The area of the circle is: " << area << endl;
}

void exercise2() {
    const double pi = 3.141592653589793238;
    double radius;
    int precision;

    cout << "Enter the radius of the circle: ";
    cin >> radius;

    cout << "Enter the number of decimal digits to display: ";
    cin >> precision;

    double area = pi * radius * radius;

    cout << fixed << setprecision(precision);
    cout << "The area of the circle is: " << area << endl;
}

void exercise3() {
    int totalInches;
    cout << "Enter the number of inches: ";
    cin >> totalInches;

    int feet = totalInches / 12;
    int inches = totalInches % 12;

    cout << totalInches << " inches is equal to "
         << feet << " feet and " << inches << " inches." << endl;
}

void exercise4() {
    double h_inches;
    int d_feet, d_inches;
    double angle_degrees;

    cout << "Enter your eye height (in inches): ";
    cin >> h_inches;

    cout << "Enter distance from tree - feet part: ";
    cin >> d_feet;
    cout << "Enter distance from tree - inches part: ";
    cin >> d_inches;

    cout << "Enter the angle to the top of the tree (in degrees): ";
    cin >> angle_degrees;

    double d_totalFeet = d_feet + d_inches / 12.0;
    double h_feet = h_inches / 12.0;
    double angle_radians = angle_degrees * M_PI / 180.0;

    double tree_height = h_feet + d_totalFeet * tan(angle_radians);

    cout << "The height of the tree is: " << tree_height << " feet." << endl;
}

int main() {
    int choice;

    do {
        cout << "\n===== Menu =====\n";
        cout << "1. Compute area of a circle\n";
        cout << "2. Area of a circle (with precision control)\n";
        cout << "3. Convert inches to feet and inches\n";
        cout << "4. Compute height of a tree\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cout << endl;

        switch (choice) {
            case 1: exercise1(); break;
            case 2: exercise2(); break;
            case 3: exercise3(); break;
            case 4: exercise4(); break;
            case 0: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
