#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

class Product {
private:
    int id;
    string name;
    double price;
    int quantity;

public:
    Product(int productId = 0, const string& productName = "", double productPrice = 0.0, int productQuantity = 0)
        : id(productId), name(productName), price(productPrice), quantity(productQuantity) {}

    // Input validation
    bool isValid() const {
        return id >= 0 && !name.empty() && price >= 0.0 && quantity >= 0;
    }

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters with validation
    bool setName(const string& newName) {
        if (!newName.empty()) {
            name = newName;
            return true;
        }
        return false;
    }

    bool setPrice(double newPrice) {
        if (newPrice >= 0.0) {
            price = newPrice;
            return true;
        }
        return false;
    }

    bool setQuantity(int newQuantity) {
        if (newQuantity >= 0) {
            quantity = newQuantity;
            return true;
        }
        return false;
    }

    void displayProduct() const {
        cout << "ID: " << setw(4) << right << id
             << " | Name: " << setw(20) << left << name
             << " | Price: $" << setw(8) << fixed << setprecision(2) << price
             << " | Quantity: " << setw(4) << quantity << endl;
    }

    double getSubtotal() const {
        return price * quantity;
    }

    // For file handling
    string toFileString() const {
        return to_string(id) + "," + name + "," + to_string(price) + "," + to_string(quantity);
    }

    static Product fromFileString(const string& fileString) {
        size_t pos1 = fileString.find(',');
        size_t pos2 = fileString.find(',', pos1 + 1);
        size_t pos3 = fileString.find(',', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            throw invalid_argument("Invalid file string format");
        }

        int id = stoi(fileString.substr(0, pos1));
        string name = fileString.substr(pos1 + 1, pos2 - pos1 - 1);
        double price = stod(fileString.substr(pos2 + 1, pos3 - pos2 - 1));
        int quantity = stoi(fileString.substr(pos3 + 1));

        return Product(id, name, price, quantity);
    }
};

class ShoppingCart {
private:
    vector<Product> products;
    const string filename = "products.txt";

    int findProductIndex(int id) const {
        for (size_t i = 0; i < products.size(); i++) {
            if (products[i].getId() == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    bool productExists(int id) const {
        return findProductIndex(id) != -1;
    }

    void printHeader(const string& title) const {
        cout << "\n" << string(50, '=') << endl;
        cout << title << endl;
        cout << string(50, '=') << endl;
    }

    void printSection(const string& title) const {
        cout << "\n" << title << endl;
        cout << string(title.length(), '-') << endl;
    }

    int getNextProductId() const {
        if (products.empty()) {
            return 1;
        }
        int maxId = 0;
        for (const auto& product : products) {
            if (product.getId() > maxId) {
                maxId = product.getId();
            }
        }
        return maxId + 1;
    }

    bool validatePositiveInput(double value, const string& fieldName) const {
        if (value < 0) {
            cout << "Error: " << fieldName << " cannot be negative." << endl;
            return false;
        }
        return true;
    }

    bool validatePositiveInput(int value, const string& fieldName) const {
        if (value < 0) {
            cout << "Error: " << fieldName << " cannot be negative." << endl;
            return false;
        }
        return true;
    }

public:
    // Add item to cart
    void addItem() {
        printHeader("ADD NEW PRODUCT");

        int id = getNextProductId();
        string name;
        double price;
        int quantity;

        cout << "Auto-generated Product ID: " << id << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (true) {
            cout << "Enter Product Name: ";
            getline(cin, name);
            if (!name.empty()) {
                break;
            }
            cout << "Error: Product name cannot be empty." << endl;
        }

        while (true) {
            cout << "Enter Product Price: $";
            if (cin >> price && validatePositiveInput(price, "Price")) {
                break;
            }
            cout << "Error: Please enter a valid positive price." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        while (true) {
            cout << "Enter Product Quantity: ";
            if (cin >> quantity && validatePositiveInput(quantity, "Quantity")) {
                break;
            }
            cout << "Error: Please enter a valid positive quantity." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Product newProduct(id, name, price, quantity);
        if (newProduct.isValid()) {
            products.push_back(newProduct);
            cout << "Product added successfully!" << endl;
        } else {
            cout << "Error: Invalid product data. Product not added." << endl;
        }
    }

    // Delete item from cart
    void deleteItem() {
        printHeader("DELETE PRODUCT");

        if (products.empty()) {
            cout << "Cart is empty. No products to delete." << endl;
            return;
        }

        displayAllItems();

        int id;
        cout << "Enter Product ID to delete: ";
        if (!(cin >> id)) {
            cout << "Error: Invalid input. Please enter a numeric ID." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int index = findProductIndex(id);
        if (index == -1) {
            cout << "Error: Product with ID " << id << " not found." << endl;
            return;
        }

        string productName = products[index].getName();
        products.erase(products.begin() + index);
        cout << "Product '" << productName << "' deleted successfully." << endl;
    }

    // Search item in cart
    void searchItem() const {
        printHeader("SEARCH PRODUCT");

        if (products.empty()) {
            cout << "Cart is empty. No products to search." << endl;
            return;
        }

        int choice;
        cout << "1. Search by ID" << endl;
        cout << "2. Search by Name" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Error: Invalid input." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (choice == 1) {
            int id;
            cout << "Enter Product ID: ";
            if (!(cin >> id)) {
                cout << "Error: Invalid ID input." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            int index = findProductIndex(id);
            if (index == -1) {
                cout << "Product with ID " << id << " not found." << endl;
                return;
            }

            printSection("PRODUCT FOUND");
            products[index].displayProduct();
        }
        else if (choice == 2) {
            string name;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter Product Name: ";
            getline(cin, name);

            bool found = false;
            printSection("SEARCH RESULTS");

            for (const auto& product : products) {
                string productNameLower = product.getName();
                transform(productNameLower.begin(), productNameLower.end(), productNameLower.begin(), ::tolower);
                string searchNameLower = name;
                transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);

                if (productNameLower.find(searchNameLower) != string::npos) {
                    product.displayProduct();
                    found = true;
                }
            }

            if (!found) {
                cout << "No products found with name containing '" << name << "'" << endl;
            }
        }
        else {
            cout << "Error: Invalid choice." << endl;
        }
    }

    // Calculate total bill
    void calculateBill() const {
        if (products.empty()) {
            cout << "Cart is empty. No items to calculate bill." << endl;
            return;
        }

        double total = 0.0;
        int totalItems = 0;

        printHeader("SHOPPING CART BILL");

        cout << setw(4) << "ID"
             << setw(25) << left << "Name"
             << setw(12) << right << "Price"
             << setw(8) << "Qty"
             << setw(12) << "Subtotal" << endl;
        cout << string(65, '-') << endl;

        for (const auto& product : products) {
            double subtotal = product.getSubtotal();
            total += subtotal;
            totalItems += product.getQuantity();

            cout << setw(4) << product.getId()
                 << setw(25) << left << (product.getName().length() > 24 ?
                     product.getName().substr(0, 22) + ".." : product.getName())
                 << setw(12) << right << fixed << setprecision(2) << product.getPrice()
                 << setw(8) << product.getQuantity()
                 << setw(12) << subtotal << endl;
        }

        cout << string(65, '=') << endl;
        cout << setw(49) << left << "Total Items:" << setw(16) << right << totalItems << endl;
        cout << setw(49) << left << "Grand Total:" << "$" << setw(15) << right << fixed << setprecision(2) << total << endl;
        cout << string(65, '=') << endl;
    }

    // Display all items
    void displayAllItems() const {
        if (products.empty()) {
            cout << "Cart is empty. No products to display." << endl;
            return;
        }

        printHeader("ALL PRODUCTS IN CART");

        for (const auto& product : products) {
            product.displayProduct();
        }

        cout << "\nTotal Products: " << products.size() << endl;
    }

    // Update product quantity
    void updateQuantity() {
        printHeader("UPDATE PRODUCT QUANTITY");

        if (products.empty()) {
            cout << "Cart is empty. No products to update." << endl;
            return;
        }

        displayAllItems();

        int id, newQuantity;
        cout << "Enter Product ID: ";
        if (!(cin >> id)) {
            cout << "Error: Invalid input." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int index = findProductIndex(id);
        if (index == -1) {
            cout << "Error: Product with ID " << id << " not found." << endl;
            return;
        }

        cout << "Current Quantity: " << products[index].getQuantity() << endl;

        while (true) {
            cout << "Enter New Quantity: ";
            if (cin >> newQuantity && validatePositiveInput(newQuantity, "Quantity")) {
                break;
            }
            cout << "Error: Please enter a valid positive quantity." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (products[index].setQuantity(newQuantity)) {
            cout << "Quantity updated successfully." << endl;
        } else {
            cout << "Error: Failed to update quantity." << endl;
        }
    }

    // File Handling - Save to file
    void saveToFile() const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file '" << filename << "' for writing." << endl;
            return;
        }

        for (const auto& product : products) {
            file << product.toFileString() << endl;
        }

        file.close();
        cout << "All products saved to '" << filename << "' successfully." << endl;
    }

    // File Handling - Load from file
    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "No existing data file found. Starting with empty cart." << endl;
            return;
        }

        vector<Product> loadedProducts;
        string line;
        int loadedCount = 0;
        int errorCount = 0;

        while (getline(file, line)) {
            if (!line.empty()) {
                try {
                    Product product = Product::fromFileString(line);
                    if (product.isValid()) {
                        loadedProducts.push_back(product);
                        loadedCount++;
                    } else {
                        errorCount++;
                    }
                }
                catch (const exception& e) {
                    errorCount++;
                }
            }
        }

        file.close();

        if (!loadedProducts.empty()) {
            products = loadedProducts;
        }

        cout << "Loaded " << loadedCount << " products from '" << filename << "'." << endl;
        if (errorCount > 0) {
            cout << "Warning: " << errorCount << " invalid records were skipped." << endl;
        }
    }

    // Clear all items
    void clearCart() {
        if (products.empty()) {
            cout << "Cart is already empty." << endl;
            return;
        }

        printHeader("CLEAR CART");

        char confirm;
        cout << "Are you sure you want to clear all items from cart? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            products.clear();
            cout << "Cart cleared successfully." << endl;
        }
        else {
            cout << "Clear operation cancelled." << endl;
        }
    }

    // Get cart statistics
    void displayStatistics() const {
        printHeader("CART STATISTICS");

        if (products.empty()) {
            cout << "Cart is empty." << endl;
            return;
        }

        double totalValue = 0.0;
        int totalItems = 0;
        double highestPrice = 0.0;
        string highestPriceProduct;
        int lowestQuantity = numeric_limits<int>::max();
        string lowestQuantityProduct;

        for (const auto& product : products) {
            double subtotal = product.getSubtotal();
            totalValue += subtotal;
            totalItems += product.getQuantity();

            if (product.getPrice() > highestPrice) {
                highestPrice = product.getPrice();
                highestPriceProduct = product.getName();
            }

            if (product.getQuantity() < lowestQuantity) {
                lowestQuantity = product.getQuantity();
                lowestQuantityProduct = product.getName();
            }
        }

        cout << "Total Products: " << products.size() << endl;
        cout << "Total Items: " << totalItems << endl;
        cout << "Total Cart Value: $" << fixed << setprecision(2) << totalValue << endl;
        cout << "Highest Priced Item: " << highestPriceProduct << " ($" << highestPrice << ")" << endl;
        cout << "Lowest Stock Item: " << lowestQuantityProduct << " (" << lowestQuantity << " units)" << endl;
    }
};

class Menu {
private:
    ShoppingCart cart;

    void displayHeader() const {
        cout << "\n" << string(60, '*') << endl;
        cout << "           SHOPPING CART MANAGEMENT SYSTEM" << endl;
        cout << string(60, '*') << endl;
                cout << "\n" << string(60, '*') << endl;
        cout << "           Made By: Syed Azan Mehdi" << endl;
        cout << string(60, '*') << endl;
    }

    void displayOptions() const {
        cout << "1.  Add Item to Cart" << endl;
        cout << "2.  Delete Item from Cart" << endl;
        cout << "3.  Search Item" << endl;
        cout << "4.  Calculate Total Bill" << endl;
        cout << "5.  Display All Items" << endl;
        cout << "6.  Update Product Quantity" << endl;
        cout << "7.  Save Cart to File" << endl;
        cout << "8.  Load Cart from File" << endl;
        cout << "9.  Clear Entire Cart" << endl;
        cout << "10. Display Cart Statistics" << endl;
        cout << "0.  Exit" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice (0-10): ";
    }

    void handleChoice(int choice) {
        switch (choice) {
            case 1:
                cart.addItem();
                break;
            case 2:
                cart.deleteItem();
                break;
            case 3:
                cart.searchItem();
                break;
            case 4:
                cart.calculateBill();
                break;
            case 5:
                cart.displayAllItems();
                break;
            case 6:
                cart.updateQuantity();
                break;
            case 7:
                cart.saveToFile();
                break;
            case 8:
                cart.loadFromFile();
                break;
            case 9:
                cart.clearCart();
                break;
            case 10:
                cart.displayStatistics();
                break;
            case 0:
                cart.saveToFile();
                cout << "Thank you for using Shopping Cart System." << endl;
                cout << "Data saved successfully. Goodbye." << endl;
                break;
            default:
                cout << "Error: Invalid choice. Please enter a number between 0-10." << endl;
        }
    }

public:
    void run() {
        cout << "Welcome to Shopping Cart Management System." << endl;
        cart.loadFromFile();

        int choice;

        do {
            displayHeader();
            displayOptions();

            if (!(cin >> choice)) {
                cout << "Error: Invalid input. Please enter a number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            handleChoice(choice);

            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }

        } while (choice != 0);
    }
};

int main() {
    Menu systemMenu;
    systemMenu.run();
    return 0;
}
