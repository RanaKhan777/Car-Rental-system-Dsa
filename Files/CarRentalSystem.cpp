#include "CarRentalSystem.h"
#include <iostream>
#include <queue>
#include <fstream>
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::seconds

using namespace std;

// Car constructor
CarRentalSystem::Car::Car(int carId, string carName, string carModel, int carYear, bool rented, string service, float carPrice, string carWarranty)
    : id(carId), name(carName), model(carModel), year(carYear), isRented(rented), serviceStatus(service), price(carPrice), warranty(carWarranty) {
}

// RentalRequest constructor
CarRentalSystem::RentalRequest::RentalRequest(int cId, int custId, string custName, float charge)
    : carID(cId), customerID(custId), customerName(custName), totalCharge(charge) {
}

// Transaction constructor
CarRentalSystem::Transaction::Transaction(int transId, int cId, float transAmount)
    : transactionID(transId), carID(cId), amount(transAmount) {
}

// CarNode constructor
CarRentalSystem::CarNode::CarNode(Car car) : data(car), next(nullptr) {}

// RentalRequestNode constructor
CarRentalSystem::RentalRequestNode::RentalRequestNode(RentalRequest request) : data(request), next(nullptr) {}

// TransactionNode constructor
CarRentalSystem::TransactionNode::TransactionNode(Transaction transaction) : data(transaction), next(nullptr) {}

// Helper function to add a car node to the linked list
void CarRentalSystem::addCarNode(CarNode*& head, const Car& car) {
    CarNode* newNode = new CarNode(car);
    newNode->next = head;
    head = newNode;
}

// Helper function to remove a car node from the linked list
void CarRentalSystem::removeCarNode(CarNode*& head, int carID) {
    CarNode* prev = nullptr, * curr = head;

    while (curr != nullptr && curr->data.id != carID) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        cout << "Car not found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    if (prev == nullptr) { // Removing the head node
        head = curr->next;
    }
    else {
        prev->next = curr->next;
    }

    delete curr;
    cout << "Car removed successfully!" << endl;
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

// Helper function to find a car node by its ID
CarRentalSystem::CarNode* CarRentalSystem::findCarNode(CarNode* head, int carID) {
    while (head != nullptr) {
        if (head->data.id == carID) return head;
        head = head->next;
    }
    return nullptr;
}

// Car Management
void CarRentalSystem::addCar() {
    int carID, carYear;
    string carName, carModel, warranty;
    float carPrice;

    cout << "Enter Car ID: ";
    cin >> carID;

    if (carExists(carID)) {
        cout << "Car with this ID already exists!" << endl;
        return;
    }

    cin.ignore();  // This is crucial to avoid skipping the next getline
    cout << "Enter Car Name: ";
    getline(cin, carName);
    cout << "Enter Car Model: ";
    getline(cin, carModel);
    cout << "Enter Car Year: ";
    cin >> carYear;
    cout << "Enter Car Price: ";
    cin >> carPrice;
    cin.ignore(); // To clear the remaining newline character from the input buffer
    cout << "Enter Car Warranty: ";
    getline(cin, warranty);

    addCarNode(carsHead, Car(carID, carName, carModel, carYear, false, "", carPrice, warranty));
    cout << "Car added successfully!" << endl;
    saveCarsToFile();  // Save to file after adding
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

void CarRentalSystem::sellCar(int carID) {
    removeCarNode(carsHead, carID);
    saveCarsToFile();  // Save after selling the car
}

void CarRentalSystem::updateCarStatus(int carID, bool isRented) {
    CarNode* carNode = findCarNode(carsHead, carID);

    if (carNode) {
        carNode->data.isRented = isRented;
        cout << "Car status updated successfully!\nPress any key to continue : " << endl;
        saveCarsToFile();  // Save the updated status
        cin.get();  // Replaced getch() with cin.get()
    }
    else {
        cout << "Car not found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
}

void CarRentalSystem::displayCars() {
    if (!carsHead) {
        cout << "No cars available!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    CarNode* current = carsHead;
    while (current) {
        const auto& car = current->data;
        cout << "ID: " << car.id << ", Name: " << car.name
            << ", Model: " << car.model << ", Year: " << car.year
            << ", Price: " << car.price << ", Warranty: " << car.warranty << endl;
        current = current->next;
    }
    cout << "\nPress any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

bool CarRentalSystem::carExists(int carID) {
    return findCarNode(carsHead, carID) != nullptr;
}

// Rental Operations using Queue for Rental Requests
void CarRentalSystem::processRentalRequest(int carID) {
    CarNode* carNode = findCarNode(carsHead, carID);

    if (!carNode || carNode->data.isRented) {
        cout << "Car is unavailable or does not exist!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    int customerID;
    string customerName;
    float rentalPrice;

    cout << "Enter Customer ID: ";
    cin >> customerID;
    cin.ignore();
    cout << "Enter Customer Name: ";
    getline(cin, customerName);
    cout << "Enter Rental Price: ";
    cin >> rentalPrice;

    RentalRequest rentalRequest(carID, customerID, customerName, rentalPrice);

    rentalRequestsQueue.push(rentalRequest); // Add rental request to queue

    carNode->data.isRented = true;
    cout << "Rental request processed successfully!" << endl;
    saveRentalRequestsToFile();  // Save rental requests after processing
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

void CarRentalSystem::displayRentalRequests() {
    if (rentalRequestsQueue.empty()) {
        cout << "No rental requests found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    queue<RentalRequest> tempQueue = rentalRequestsQueue; // Temporary queue to display requests
    while (!tempQueue.empty()) {
        const auto& rentalRequest = tempQueue.front();
        cout << "Car ID: " << rentalRequest.carID
            << ", Customer ID: " << rentalRequest.customerID
            << ", Name: " << rentalRequest.customerName
            << ", Total Charge: " << rentalRequest.totalCharge << endl;
        cout << "\nPress any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        tempQueue.pop();
    }
    cout << "\nAll viewed.\nPress any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

void CarRentalSystem::deleteRentalRequest(int rentalID) {
    queue<RentalRequest> tempQueue;

    bool found = false;
    while (!rentalRequestsQueue.empty()) {
        RentalRequest rentalRequest = rentalRequestsQueue.front();
        rentalRequestsQueue.pop();

        if (rentalRequest.carID == rentalID) {
            found = true;
            cout << "Rental request deleted successfully!" << endl;
            cout << "Press any key to continue: ";
            cin.get();  // Replaced getch() with cin.get()
        }
        else {
            tempQueue.push(rentalRequest); // Keep requests that do not match
        }
    }

    rentalRequestsQueue = tempQueue;  // Replace the original queue with the modified one
    if (!found) {
        cout << "Rental request not found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
}

// Transaction Management
void CarRentalSystem::recordTransaction(int carID, float amount) {
    static int transactionID = 1;
    Transaction transaction(transactionID++, carID, amount);

    TransactionNode* newNode = new TransactionNode(transaction);
    newNode->next = transactionsHead;
    transactionsHead = newNode;

    cout << "Transaction recorded successfully!" << endl;
    saveTransactionsToFile();  // Save transaction after recording
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

void CarRentalSystem::displayTransactions() {
    if (!transactionsHead) {
        cout << "No transactions found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    TransactionNode* current = transactionsHead;
    while (current) {
        const auto& transaction = current->data;
        cout << "Transaction ID: " << transaction.transactionID
            << ", Car ID: " << transaction.carID
            << ", Amount: " << transaction.amount << endl;
        current = current->next;
    }
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

void CarRentalSystem::deleteTransactionRecord(int transactionID) {
    TransactionNode* prev = nullptr, * curr = transactionsHead;

    while (curr != nullptr && curr->data.transactionID != transactionID) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        cout << "Transaction record not found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
        return;
    }

    if (prev == nullptr) { // Removing the head node
        transactionsHead = curr->next;
    }
    else {
        prev->next = curr->next;
    }

    delete curr;
    cout << "Transaction record deleted successfully!" << endl;
    cout << "Press any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

// Service Management: Service a car
void CarRentalSystem::serviceCar(int carID, const string& serviceType) {
    CarNode* carNode = findCarNode(carsHead, carID);

    if (carNode) {
        carNode->data.serviceStatus = serviceType;
        cout << "Car serviced with " << serviceType << " successfully!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
    else {
        cout << "Car not found!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
}

// Warranty System: Check for expired warranties
void CarRentalSystem::warrantyExpirationCheck() {
    for (CarNode* carNode = carsHead; carNode != nullptr; carNode = carNode->next) {
        // Here you can add the logic to check warranty expiration based on any criteria.
        cout << "Checking warranty for car: " << carNode->data.name << endl;
    }
    cout << "\nChecked.\nPress any key to continue: ";
    cin.get();  // Replaced getch() with cin.get()
}

// Return Rental: Return a rented car
void CarRentalSystem::returnRental(int carID) {
    CarNode* carNode = findCarNode(carsHead, carID);

    if (carNode && carNode->data.isRented) {
        carNode->data.isRented = false;
        cout << "Car has been returned successfully!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
    else {
        cout << "Car not rented or does not exist!" << endl;
        cout << "Press any key to continue: ";
        cin.get();  // Replaced getch() with cin.get()
    }
}

// File Handling for saving all records to file
void CarRentalSystem::saveCarsToFile() {
    ofstream outFile("cars.txt", ios::app);  // Open in append mode

    if (outFile.is_open()) {
        CarNode* current = carsHead;
        while (current) {
            const auto& car = current->data;
            outFile << car.id << "," << car.name << "," << car.model << "," << car.year << ","
                << car.isRented << "," << car.serviceStatus << "," << car.price << ","
                << car.warranty << endl;
            current = current->next;
        }
        outFile.close();
        cout << "Cars saved to file successfully!" << endl;
    }
    else {
        cout << "Error saving cars to file!" << endl;
    }
}

void CarRentalSystem::saveRentalRequestsToFile() {
    ofstream outFile("rental_requests.txt", ios::app);  // Open in append mode

    if (outFile.is_open()) {
        queue<RentalRequest> tempQueue = rentalRequestsQueue;
        while (!tempQueue.empty()) {
            const auto& rentalRequest = tempQueue.front();
            outFile << rentalRequest.carID << "," << rentalRequest.customerID << ","
                << rentalRequest.customerName << "," << rentalRequest.totalCharge << endl;
            tempQueue.pop();
        }
        outFile.close();
        cout << "Rental requests saved to file successfully!" << endl;
    }
    else {
        cout << "Error saving rental requests to file!" << endl;
    }
}

void CarRentalSystem::saveTransactionsToFile() {
    ofstream outFile("transactions.txt", ios::app);  // Open in append mode

    if (outFile.is_open()) {
        TransactionNode* current = transactionsHead;
        while (current) {
            const auto& transaction = current->data;
            outFile << transaction.transactionID << "," << transaction.carID << ","
                << transaction.amount << endl;
            current = current->next;
        }
        outFile.close();
        cout << "Transactions saved to file successfully!" << endl;
    }
    else {
        cout << "Error saving transactions to file!" << endl;
    }
}
