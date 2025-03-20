#include "CarRentalSystem.h"
#include <iostream>
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::seconds
#include <cstdlib>
#include <string>
bool start = true;
using namespace std;

//function for loading screen
#include <iostream>
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::milliseconds

using namespace std;

void loadingscreen() {
    const int width = 40;  // Width of the loading bar
    const int total = 100; // Total percentage

    cout << "\n\n\n\n\n"; // Add vertical spacing to center the loading bar

    for (int progress = 0; progress <= total; progress += 2) {
        // Calculate the number of blocks to display
        int numBlocks = (progress * width) / total;

        // Print spaces to center the bar horizontally
        cout << string((60 - width) / 2, ' ');

        // Draw the loading bar
        cout << "[";
        for (int i = 0; i < numBlocks; i++) {
            cout << "=";  // Filled part
        }
        for (int i = numBlocks; i < width; i++) {
            cout << " ";  // Empty part
        }
        cout << "] " << progress << "%\r"; // Update percentage on the same line
        cout.flush(); // Flush the output to update the console

        // Simulate loading time
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    cout << endl << "\nLoading complete!" << endl;
    this_thread::sleep_for(chrono::milliseconds(50));
}

// Function to clear screen
void clearscreen() {
    if (start == true) {
        loadingscreen();
        start = false;
    }
    else {
        //cout << "Press any key to continue: ";
        cin.get();
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayMainMenu() {
    clearscreen();
    cout << "\n*************************************************\n";
    cout << "           Welcome to the Car Rental System     \n";
    cout << "*************************************************\n";
    cout << "1. Car Management\n";
    cout << "2. Rental Operations\n";
    cout << "3. Service Management\n";
    cout << "4. Warranty System\n";
    cout << "5. Display Cars\n";
    cout << "6. Display Rental Requests\n";
    cout << "7. Display Transactions\n";
    cout << "8. Delete Record\n";
    cout << "9. Exit\n";
    cout << "*************************************************\n";
    cout << "Enter your choice: ";
}

void displayCarManagementMenu() {
    cout << "\n*** Car Management ***\n";
    cout << "1. Add Car\n";
    cout << "2. Sell Car\n";
    cout << "3. Update Car Status\n";
    cout << "4. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void displayRentalOperationsMenu() {
    cout << "\n*** Rental Operations ***\n";
    cout << "1. Process Rental Request\n";
    cout << "2. Return Car\n";
    cout << "3. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void displayServiceManagementMenu() {
    cout << "\n*** Service Management ***\n";
    cout << "1. Service a Car\n";
    cout << "2. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void displayWarrantySystemMenu() {
    cout << "\n*** Warranty System ***\n";
    cout << "1. Check Warranty Expiration\n";
    cout << "2. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void displayDeleteRecordMenu() {
    cout << "\n*** Delete Record ***\n";
    cout << "1. Delete Car Record\n";
    cout << "2. Delete Rental Record\n";
    cout << "3. Delete Transaction Record\n";
    cout << "4. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

int main() {
    CarRentalSystem system;
    int choice;

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
        case 1: { // Car Management
            int carChoice;
            displayCarManagementMenu();
            cin >> carChoice;

            switch (carChoice) {
            case 1:
                system.addCar();
                break;
            case 2: {
                int carID;
                cout << "Enter Car ID to sell: ";
                cin >> carID;
                system.sellCar(carID);
                break;
            }
            case 3: {
                int carID;
                bool isRented;
                cout << "Enter Car ID to update status: ";
                cin >> carID;
                cout << "Enter new status (1 for rented, 0 for available): ";
                cin >> isRented;
                system.updateCarStatus(carID, isRented);
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice! Returning to main menu." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
                break;
            }
            break;
        }

        case 2: { // Rental Operations
            int rentalChoice;
            displayRentalOperationsMenu();
            cin >> rentalChoice;

            switch (rentalChoice) {
            case 1: {
                int carID;
                cout << "Enter Car ID to rent: ";
                cin >> carID;
                system.processRentalRequest(carID);
                break;
            }
            case 2: {
                int carID;
                cout << "Enter Car ID to return: ";
                cin >> carID;
                system.returnRental(carID);
                break;
            }
            case 3:
                break;
            default:
                cout << "Invalid choice! Returning to main menu." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
                break;
            }
            break;
        }

        case 3: { // Service Management
            int serviceChoice;
            displayServiceManagementMenu();
            cin >> serviceChoice;

            switch (serviceChoice) {
            case 1: {
                int carID;
                string serviceType;
                cout << "Enter Car ID to service: ";
                cin >> carID;
                cin.ignore();  // Clear input buffer
                cout << "Enter Service Type (e.g., Tuning, Wash, Repair): ";
                getline(cin, serviceType);
                system.serviceCar(carID, serviceType);
                break;
            }
            case 2:
                break;
            default:
                cout << "Invalid choice! Returning to main menu." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
                break;
            }
            break;
        }

        case 4: { // Warranty System
            int warrantyChoice;
            displayWarrantySystemMenu();
            cin >> warrantyChoice;

            switch (warrantyChoice) {
            case 1:
                system.warrantyExpirationCheck();
                break;
            case 2:
                break;
            default:
                cout << "Invalid choice! Returning to main menu." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
                break;
            }
            break;
        }

        case 5: { // Display Cars
            system.displayCars();
            //cout << "\nPress any key to continue: ";
            cin.get();
            break;
        }

        case 6: { // Display Rental Requests
            system.displayRentalRequests();
            break;
        }

        case 7: { // Display Transactions
            system.displayTransactions();
            break;
        }

        case 8: { // Delete Record
            int deleteChoice;
            displayDeleteRecordMenu();
            cin >> deleteChoice;

            switch (deleteChoice) {
            case 1: {
                int carID;
                cout << "Enter Car ID to delete: ";
                cin >> carID;
                system.sellCar(carID); // Selling the car is equivalent to deleting it
                break;
            }
            case 2: {
                int rentalID;
                cout << "Enter Rental ID to delete: ";
                cin >> rentalID;
                system.deleteRentalRequest(rentalID);
                break;
            }
            case 3: {
                int transactionID;
                cout << "Enter Transaction ID to delete: ";
                cin >> transactionID;
                system.deleteTransactionRecord(transactionID);
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice! Returning to main menu." << endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
                break;
            }
            break;
        }

        case 9: { // Exit Program
            cout << "Exiting program... Thank you for using the Car Rental System!" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
            return 0;
        }

        default:
            cout << "Invalid choice! Please try again." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));  // 3-second delay
            break;
        }
    }

    return 0;
}
