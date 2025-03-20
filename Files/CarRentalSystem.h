#ifndef CARRENTALSYSTEM_H
#define CARRENTALSYSTEM_H

#include <iostream>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

// CarRentalSystem class definition
class CarRentalSystem {
public:
    // Car structure definition
    struct Car {
        int id;
        std::string name;
        std::string model;
        int year;
        bool isRented;
        std::string serviceStatus;
        float price;
        std::string warranty;

        Car(int carId, std::string carName, std::string carModel, int carYear, bool rented, std::string service, float carPrice, std::string carWarranty);
    };

    // RentalRequest structure definition
    struct RentalRequest {
        int carID;
        int customerID;
        std::string customerName;
        float totalCharge;

        RentalRequest(int cId, int custId, std::string custName, float charge);
    };

    // Transaction structure definition
    struct Transaction {
        int transactionID;
        int carID;
        float amount;

        Transaction(int transId, int cId, float transAmount);
    };

    // Linked list node structure for cars, rental requests, and transactions
    struct CarNode {
        Car data;
        CarNode* next;

        CarNode(Car car);
    };

    struct RentalRequestNode {
        RentalRequest data;
        RentalRequestNode* next;

        RentalRequestNode(RentalRequest request);
    };

    struct TransactionNode {
        Transaction data;
        TransactionNode* next;

        TransactionNode(Transaction transaction);
    };

    // Data heads for linked lists
    CarNode* carsHead = nullptr;
    RentalRequestNode* rentalRequestsHead = nullptr;
    TransactionNode* transactionsHead = nullptr;

    std::queue<RentalRequest> rentalRequestsQueue; // Queue for rental requests

    // Car Management
    void addCar();
    void sellCar(int carID);
    void updateCarStatus(int carID, bool isRented);
    void displayCars();
    bool carExists(int carID);

    // Rental Operations
    void processRentalRequest(int carID);
    void returnRental(int carID);
    void displayRentalRequests();
    void deleteRentalRequest(int rentalID);

    // Transaction Management
    void recordTransaction(int carID, float amount);
    void displayTransactions();
    void deleteTransactionRecord(int transactionID);

    // Service Management
    void serviceCar(int carID, const std::string& serviceType);

    // Warranty System
    void warrantyExpirationCheck();

    // File Handling for saving and loading data
    void saveCarsToFile();
    void saveRentalRequestsToFile();
    void saveTransactionsToFile();

    // Helper Functions for Linked List
    void addCarNode(CarNode*& head, const Car& car);
    void removeCarNode(CarNode*& head, int carID);
    CarNode* findCarNode(CarNode* head, int carID);
};

#endif
