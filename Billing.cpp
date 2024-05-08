
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>

using namespace std;

class Item {
public:
    string name;
    int rate;
    int quantity;

    Item(string n = "", int r = 0, int q = 0) : name(n), rate(r), quantity(q) {}
};

class Supermarket {
private:
    vector<Item> inventory;
    const string fileName = "supermarket_inventory.txt";

    // Method to load inventory from file
    void loadInventory() {
        ifstream in(fileName);
        if (!in) {
            cerr << "Error opening file for reading: " << fileName << endl;
            return;
        }
        inventory.clear();
        string line;
        while (getline(in, line)) {
            istringstream ss(line);
            string name;
            int rate, quantity;
            ss >> name >> rate >> quantity;
            inventory.emplace_back(name, rate, quantity);
        }
        in.close();
    }

    // Method to save inventory to file
    void saveInventory() {
        ofstream out(fileName);
        if (!out) {
            cerr << "Error opening file for writing: " << fileName << endl;
            return;
        }
        for (const auto& item : inventory) {
            out << item.name << " " << item.rate << " " << item.quantity << endl;
        }
        out.close();
    }

public:
    Supermarket() {
        loadInventory();
    }

    void addItem() {
        string itemName;
        int itemRate, itemQuantity;

        cout << "Enter item name: ";
        cin >> itemName;
        cout << "Enter item rate: ";
        while (!(cin >> itemRate) || itemRate < 0) {
            cout << "Invalid input. Please enter a positive rate: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter item quantity: ";
        while (!(cin >> itemQuantity) || itemQuantity < 0) {
            cout << "Invalid input. Please enter a positive quantity: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        inventory.emplace_back(itemName, itemRate, itemQuantity);
        saveInventory();

        cout << "Item added successfully!" << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }

    void printBill() {
        int itemCount;
        cout << "How many different items do you want to purchase? ";
        while (!(cin >> itemCount) || itemCount <= 0) {
            cout << "Invalid input. Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        vector<pair<string, int>> itemsToPurchase;

        for (int i = 0; i < itemCount; i++) {
            string itemName;
            int quantity;

            cout << "Enter name of item " << (i + 1) << ": ";
            cin >> itemName;
            cout << "Enter quantity for " << itemName << ": ";
            while (!(cin >> quantity) || quantity <= 0) {
                cout << "Invalid input. Please enter a positive quantity: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            itemsToPurchase.push_back(make_pair(itemName, quantity));
        }

        int totalCost = 0;
        for (const auto& itemRequest : itemsToPurchase) {
            const auto& itemName = itemRequest.first;
            const auto& requestedQuantity = itemRequest.second;

            bool found = false;
            for (auto& item : inventory) {
                if (item.name == itemName) {
                    if (item.quantity >= requestedQuantity) {
                        int cost = item.rate * requestedQuantity;
                        item.quantity -= requestedQuantity;
                        totalCost += cost;
                        found = true;

                        cout << "Purchased " << requestedQuantity << " of " << item.name
                             << " at rate " << item.rate << " each. Total cost: "
                             << cost << endl;
                    } else {
                        cout << "Insufficient quantity for " << itemName << ". Available: " << item.quantity << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "Item not found: " << itemName << endl;
            }
        }

        cout << "Total cost for the entire purchase: " << totalCost << endl;

        saveInventory(); // Save updated inventory
        this_thread::sleep_for(chrono::seconds(3));
    }
};

int main() {
    Supermarket supermarket;

    bool exitProgram = false;
    while (!exitProgram) {
        int choice;
        cout << "Welcome to the Supermarket Billing System" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Print Bill" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                supermarket.addItem();
                break;
            case 2:
                supermarket.printBill();
                break;
            case 3:
                cout << "Exiting the program. Goodbye!" << endl;
                exitProgram = true;
                break;
            default:
                break; // Should never reach here due to validation
        }

        this_thread::sleep_for(chrono::seconds(2));
    }

    return 0;
}
