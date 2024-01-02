#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

bool authenticateAdmin() {
    string adminID, password;
    cout << "\nEnter Admin ID: ";
    cin >> adminID;

    cout << "\nEnter Password: ";
    char ch;
    while (true) {
        ch = _getch(); // Read a character without echoing it
        if (ch == '\r' || ch == '\n') // Check for Enter key
            break;
        password += ch; // Append to password string
        cout << '*'; // Print '*' for each character
    }

    // Check if the entered credentials match the predefined ones
    return (adminID == "admin123" && password == "123");
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", localTime);

    return buffer;
}

class ElectricityBill {
private:
    int customerID;
    string customerName;
    int unitsConsumed;
    float billAmount;
    int month; // New variable to store month
    int year;  // New variable to store year

    float calculateGST() {
        // GST calculation (let's assume GST is 18%)
        return billAmount * 0.18;
    }

    float calculateTax() {
        // Tax calculation (let's assume tax is 5%)
        return billAmount * 0.05;
    }

public:
    void setData(string name, int units, int id, int m, int y) {
        customerName = name;
        unitsConsumed = units;
        customerID = id; // Set customer ID
        month = m; // Set month
        year = y;  // Set year

        // Get current date and time
        string currentTime = getCurrentDateTime();

        // Calculate the bill
        calculateBill();

        // Save the bill to the file
        saveBillToFile(currentTime); // Pass current time to saveBillToFile()
    }

    void calculateBill() {
        // Bill calculation logic
        if (unitsConsumed <= 100) {
            billAmount = unitsConsumed * 1.20;
        } else if (unitsConsumed <= 300) {
            billAmount = 100 * 1.20 + (unitsConsumed - 100) * 2.00;
        } else {
            billAmount = 100 * 1.20 + 200 * 2.00 + (unitsConsumed - 300) * 3.00;
        }
        float gst = calculateGST();
        float tax = calculateTax();
        billAmount += gst + tax;
    }

    void saveBillToFile(string currentTime) {
        ofstream outFile("electricity_bills.txt", ios::app);
        if (outFile.is_open()) {
            // Write bill details to the file along with the current date and time
            outFile << "\nCustomer ID: " << customerID << endl;
            outFile << "Customer Name: " << customerName << endl;
            outFile << "Units Consumed: " << unitsConsumed << " units" << endl;
            outFile << "Bill Amount: ₹" << billAmount << endl;
            outFile << "Month: " << month << endl; // Include month in the file
            outFile << "Year: " << year << endl;   // Include year in the file
            outFile << "Date and Time of the bill: " << currentTime << endl;
            outFile << "Breakdown:" << endl;
            outFile << "  - Electricity Charge: ₹" << billAmount - calculateGST() - calculateTax() << endl;
            outFile << "  - GST (18%): ₹" << calculateGST() << endl;
            outFile << "  - Tax (5%): ₹" << calculateTax() << endl;
            outFile.close();
            cout << "Bill information saved to file.\n";
        } else {
            cout << "Unable to open file.\n";
        }
    }

    void viewCustomerBillByID(int searchID, int viewMonth, int viewYear) {
    ifstream inFile("electricity_bills.txt");
    bool found = false;
    string line;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.find("Customer ID: " + to_string(searchID)) != string::npos) {
                found = true;
                // Display customer information
                for (int i = 0; i < 3; ++i) { // Assuming 3 lines for customer information
                    getline(inFile, line);
                    cout << line << endl;
                }
                getline(inFile, line); // Skip Month line
                int billMonth = stoi(line.substr(line.find(":") + 2));
                cout << line << endl;
                
                getline(inFile, line); // Skip Year line
                int billYear = stoi(line.substr(line.find(":") + 2));
                cout << line << endl;

                if (billMonth == viewMonth && billYear == viewYear) {
                    // Display bill details if the month and year match
                    for (int i = 0; i < 4; ++i) { // Assuming 4 lines for bill details
                        getline(inFile, line);
                        cout << line << endl;
                    }
                    break;
                }
            }
        }
        inFile.close();
        if (!found) {
            cout << "Customer ID not found or no bill available for this ID in the specified month and year.\n";
        }
    } else {
        cout << "Unable to open file.\n";
    }
}


};

// Main function
int main() {
    ElectricityBill bill;
    string name;
    int choice, units, customerID, chosenMonth, chosenYear;

    cout << "\nELECTRICITY BILL CALCULATOR\n";

    if (!authenticateAdmin()) {
        cout << "Invalid Admin ID or Password. Access denied.\n";
        return 1; // Exit the program if authentication fails
    }

    do {
        cout << "\n1. Calculate Bill\n";
        cout << "2. View Customer Bill by ID\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Customer ID: ";
                while (!(cin >> customerID)) {
                    cout << "Invalid input! Please enter a numeric Customer ID: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter Customer Name: ";
                cin.ignore();
                getline(cin, name);
                while (!all_of(name.begin(), name.end(), ::isalpha)) {
                    cout << "Invalid input! Please enter alphabetic characters for Customer Name: ";
                    getline(cin, name);
                }

                cout << "Enter Units Consumed: ";
                while (!(cin >> units)) {
                    cout << "Invalid input! Please enter numeric Units Consumed: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter Month (1-12): ";
                cin >> chosenMonth;

                cout << "Enter Year: ";
                while (!(cin >> chosenYear) || to_string(chosenYear).length() != 4) {
                    cout << "Invalid input! Please enter a 4-digit Year: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (units < 0) {
                    cout << "Invalid input! Units consumed cannot be negative.\n";
                    break;
                }
                bill.setData(name, units, customerID, chosenMonth, chosenYear); // Pass customer details to setData()
                break;

            case 2:
                int searchID, viewMonth, viewYear;
                cout << "Enter Customer ID: ";
                cin >> searchID;
                cout << "Enter Month to view (1-12): ";
                cin >> viewMonth;
                cout << "Enter Year to view: ";
                cin >> viewYear;
                bill.viewCustomerBillByID(searchID, viewMonth, viewYear);
                break;

            case 3:
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
