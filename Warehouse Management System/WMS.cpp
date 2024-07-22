#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Product
{
private:
    int ID;
    string name;
    string description;
    int quantity;
    double unitPrice;

public:
    Product() : ID(0), name(""), description(""), quantity(0), unitPrice(0.0)
    {
    }
    ~Product()
    {
    }
    friend ostream &operator<<(ostream &out, const Product &obj)
    {
        out << obj.ID << "\t" << obj.name << "\t\t  " << obj.quantity << "\t\t\t  " << obj.unitPrice << "\t\t   \t" << obj.description << endl;
        return out;
    }
    Product(int ID, string name, string des, int quantity, double unit)
    {
        this->ID = ID;
        this->name = name;
        this->description = des;
        this->quantity = quantity;
        this->unitPrice = unit;
    }
    int getID() const
    {
        return this->ID;
    }
    string getName() const
    {
        return this->name;
    }
    int &getQuan()
    {
        return this->quantity;
    }
    double getPrice() const
    {
        return this->unitPrice;
    }
    int operator[](int i)
    {
        return getQuan();
    }
    void saveToFile(ofstream& outFile) const 
	{
        outFile << ID << '\n' << name << '\n' << description << '\n' << quantity << '\n' << unitPrice << '\n';
    }

    void loadFromFile(ifstream& inFile) {
        inFile >> ID;
        inFile.ignore();
        getline(inFile, name);
        getline(inFile, description);
        inFile >> quantity >> unitPrice;
    }
};

class Inventory
{
private:
    Product *Products;
    int numP;

public:
    int getTotal()
    {
        return numP;
    }
    Inventory()
    {
        Products = nullptr;
        numP = 0;
    }
    ~Inventory()
    {
        delete[] Products;
    }
    friend ostream &operator<<(ostream &out, const Inventory &obj)
    {
        out << "ID\tName\t\tQuantity\t\tUnit Price\t\tDescription\n\n\n";
        for (int i = 0; i < obj.numP; i++)
        {
            out << obj.Products[i];
        }
        return out;
    }
    void AddProduct(const Product &obj)
    {
        Product *temp = new Product[numP + 1];
        for (int i = 0; i < numP; i++)
        {
            temp[i] = Products[i];
        }
        temp[numP] = obj;
        delete[] Products;
        Products = temp;
        numP++;
    }
    void RemoveProduct(int id)
    {
        if (numP == 0)
        {
            return;
        }
        int index = -1;
        for (int i = 0; i < numP; i++)
        {
            if (Products[i].getID() == id)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            return;
        }
        Product *temp = new Product[numP - 1];
        for (int i = 0, j = 0; i < numP; i++)
        {
            if (i != index)
            {
                temp[j++] = Products[i];
            }
        }
        delete[] Products;
        Products = temp;
        numP--;
    }
    Product *GetProductByID(int id)
    {
        for (int i = 0; i < numP; i++)
        {
            if (Products[i].getID() == id)
            {
                return &Products[i];
            }
        }
        return nullptr;
    }
    int operator[](int i)
    {
        return Products[i].getQuan();
    }
    void saveToFile(ofstream& outFile) const {
        outFile << numP << '\n';
        for (int i = 0; i < numP; i++) {
            Products[i].saveToFile(outFile);
        }
    }

    void loadFromFile(ifstream& inFile) {
        inFile >> numP;
        inFile.ignore();
        Products = new Product[numP];
        for (int i = 0; i < numP; i++) {
            Products[i].loadFromFile(inFile);
        }
    
	}
};

class Warehouse
{
private:
    Inventory I;
    string name;
    int MaxCapacity;
    int CurrentCapacity;

public:
    Warehouse(string name, int maxCapacity)
    {
        this->name = name;
        MaxCapacity = maxCapacity;
        CurrentCapacity = 0;
    }
    void Addproduct(const Product obj)
    {
        I.AddProduct(obj);
    }
    void Removeproduct(int ID)
    {
        I.RemoveProduct(ID);
    }
    Product *FetchProduct(int ID)
    {
        return I.GetProductByID(ID);
    }
    int getAvail()
    {
        int Occupied = 0;
        for (int i = 0; i < I.getTotal(); i++)
        {
            Occupied += I[i];
        }
        if (Occupied == MaxCapacity)
        {
            return 0;
        }
        else
        {
            return MaxCapacity - Occupied;
        }
    }
    friend ostream &operator<<(ostream &out, Warehouse &obj)
    {
        out << "\n\n\t\t\t\t   " << obj.name << "\n\n\n      Capacity: " << obj.MaxCapacity << "\t\t\t\t\t\t Available Space " << obj.getAvail() << endl << endl ;
        out << "      ___________________________________________________________________________" << endl << endl << endl;
        out << obj.I;
        out << "\n\n      ___________________________________________________________________________" << endl << endl;
        return out;
    }
    void saveToFile(ofstream& outFile) const {
        outFile << name << '\n' << MaxCapacity << '\n' << CurrentCapacity << '\n';
        I.saveToFile(outFile);
    }

    void loadFromFile(ifstream& inFile) {
        getline(inFile, name);
        inFile >> MaxCapacity >> CurrentCapacity;
        inFile.ignore();
        I.loadFromFile(inFile);
    }
};

class Order
{
private:
    int ID;
    string Customer;
    string date;
    int status;
    Product *Products;
    int *quantities; // Array to store quantities of each product in the order
    int numProducts;

public:
    Order() : ID(0), Customer(""), date(""), status(0), Products(nullptr), quantities(nullptr), numProducts(0)
    {
    }
    Order(int ID, string Customer, string date, int status) : ID(ID), Customer(Customer), date(date), status(status), Products(nullptr), quantities(nullptr), numProducts(0)
    {
    }
    ~Order()
    {
        delete[] Products;
        delete[] quantities;
    }
    void AddProductFromWarehouse(Warehouse &warehouse, int productID, int quantity)
    {
        Product *product = warehouse.FetchProduct(productID);
        if (product != nullptr && product->getQuan() >= quantity)
        {
            // Reduce the quantity in the warehouse
            product->getQuan() -= quantity;

            // Add the product and quantity to the order
            Product *tempProducts = new Product[numProducts + 1];
            int *tempQuantities = new int[numProducts + 1];
            for (int i = 0; i < numProducts; i++)
            {
                tempProducts[i] = Products[i];
                tempQuantities[i] = quantities[i];
            }
            tempProducts[numProducts] = *product;
            tempQuantities[numProducts] = quantity;
            delete[] Products;
            delete[] quantities;
            Products = tempProducts;
            quantities = tempQuantities;
            numProducts++;
        }
        else
        {
            cout << "Product with ID " << productID << " not found in the warehouse or insufficient quantity.\n";
        }
    }
    void RemoveProduct(int id)
    {
        if (numProducts == 0)
        {
            return;
        }
        int index = -1;
        for (int i = 0; i < numProducts; i++)
        {
            if (Products[i].getID() == id)
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            return;
        }
        Product *tempProducts = new Product[numProducts - 1];
        int *tempQuantities = new int[numProducts - 1];
        for (int i = 0, j = 0; i < numProducts; i++)
        {
            if (i != index)
            {
                tempProducts[j] = Products[i];
                tempQuantities[j] = quantities[i];
                j++;
            }
        }
        delete[] Products;
        delete[] quantities;
        Products = tempProducts;
        quantities = tempQuantities;
        numProducts--;
    }
    void UpdateStatus(int newStatus)
    {
        status = newStatus;
    }
    void DisplayInvoice() const
    {
        double total = 0.0;
        cout << "Order ID: " << ID << "\nCustomer: " << Customer << "\nDate: " << date << "\nStatus: " << status << endl;
        cout << "Products:\nID\tName\t\tDescription\t\tQuantity\t\tUnit Price\t\tTotal Price\n";
        for (int i = 0; i < numProducts; i++)
        {
            double productTotal = quantities[i] * Products[i].getPrice();
            total += productTotal;
            cout << Products[i].getID() << "\t" << Products[i].getName() << "\t\t" << Products[i].getName() << "\t\t" << quantities[i] << "\t\t" << Products[i].getPrice() << "\t\t" << productTotal << endl;
        }
        cout << "Net Total Bill: $" << total << endl;
    }
    friend ostream &operator<<(ostream &out, const Order &obj)
    {
        out << "Order ID: " << obj.ID << "\nCustomer: " << obj.Customer << "\nDate: " << obj.date << "\nStatus: " << obj.status << endl;
        out << "Products:\nID\tName\t\tDescription\t\tQuantity\t\tUnit Price\n";
        for (int i = 0; i < obj.numProducts; i++)
        {
            out << obj.Products[i];
        }
        return out;
    }
    int getOrderID()
    {
    	return ID;
	}
	void saveToFile(ofstream& outFile) const {
        outFile << ID << '\n' << Customer << '\n' << date << '\n' << status << '\n' << numProducts << '\n';
        for (int i = 0; i < numProducts; i++) {
            Products[i].saveToFile(outFile);
            outFile << quantities[i] << '\n';
        }
    }

    void loadFromFile(ifstream& inFile) {
        inFile >> ID;
        inFile.ignore();
        getline(inFile, Customer);
        getline(inFile, date);
        inFile >> status >> numProducts;
        inFile.ignore();
        Products = new Product[numProducts];
        quantities = new int[numProducts];
        for (int i = 0; i < numProducts; i++) {
            Products[i].loadFromFile(inFile);
            inFile >> quantities[i];
        }
    }
};
void saveWarehouseData(const Warehouse& warehouse, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        warehouse.saveToFile(outFile);
        outFile.close();
    } else {
        cerr << "Error: Could not open file for writing: " << filename << endl;
    }
}

void loadWarehouseData(Warehouse& warehouse, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        warehouse.loadFromFile(inFile);
        inFile.close();
    } else {
        cerr << "Error: Could not open file for reading: " << filename << endl;
    }
}

void saveOrderData(Order* orders[], int numOrders, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << numOrders << '\n';
        for (int i = 0; i < numOrders; i++) {
            orders[i]->saveToFile(outFile);
        }
        outFile.close();
    } else {
        cerr << "Error: Could not open file for writing: " << filename << endl;
    }
}

void loadOrderData(Order* orders[], int& numOrders, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        inFile >> numOrders;
        inFile.ignore();
        for (int i = 0; i < numOrders; i++) {
            orders[i] = new Order();
            orders[i]->loadFromFile(inFile);
        }
        inFile.close();
    } else {
        cerr << "Error: Could not open file for reading: " << filename << endl;
    }
}

int main()
{
    Warehouse warehouse("Main Warehouse", 100);
    Order *orders[100]; // Assume a maximum of 100 orders for simplicity
    int numOrders = 0;
	loadWarehouseData(warehouse, "warehouse.txt");
    loadOrderData(orders, numOrders, "orders.txt");
    

    int choice;
    do
    {
    	system("cls");
        cout << "\n\t\tWarehouse Management System\t\t\n"<<endl
        <<"     ------------------------------------------------------------" << endl;
        cout << "\n          1. Add Product to Warehouse\n";
        cout << "\n          2. Remove Product from Warehouse\n";
        cout << "\n          3. Create Order\n";
        cout << "\n          4. Add Product to Order\n";
        cout << "\n          5. Remove Product from Order\n";
        cout << "\n          6. Update Order Status\n";
        cout << "\n          7. Display Warehouse\n";
        cout << "\n          8. Display Orders\n";
        cout << "\n          9. Display Invoice for an Order\n";
        cout << "\n          10. Exit\n\n";
        cout<<"     ------------------------------------------------------------" << endl << endl;
        cout << "     Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
        	system("cls");
            int id, quantity;
            string name, description;
            double price;
            cout << "Enter Product ID: ";
            cin >> id;
            system("cls");
            cout << "Enter Product Name: ";
            cin.ignore();
            getline(cin, name);
            system("cls");
            cout << "Enter Product Description: ";
            getline(cin, description);
            system("cls");
            cout << "Enter Product Quantity: ";
            cin >> quantity;
            system("cls");
            cout << "Enter Product Unit Price: ";
            cin >> price;
            system("cls");
            Product newProduct(id, name, description, quantity, price);
            warehouse.Addproduct(newProduct);
            break;
        }
        case 2:
        {
        
        	system("cls");
            int id;
            cout << "Enter Product ID to Remove: ";
            cin >> id;
            warehouse.Removeproduct(id);
            system("pause");
			break;
        }
        case 3:
        {
        	system("cls");
            int id, status;
            string customer, date;
            cout << "Enter Order ID: ";
            cin >> id;
            system("cls");
            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, customer);
            system("cls");
            cout << "Enter Order Date: ";
            getline(cin, date);
            system("cls");
            cout << "Enter Order Status (0 for Pending, 1 for Shipped, 2 for Delivered): ";
            cin >> status;
            system("cls");
            orders[numOrders] = new Order(id, customer, date, status);
            numOrders++;
            system("pause");
            break;
        }
        case 4:
        {
        	system("cls");
            int orderID, productID, quantity;
            cout << "Enter Order ID to Add Product to: ";
            cin >> orderID;
            system("cls");
            cout << "Enter Product ID to Add: ";
            cin >> productID;
            system("cls");
            cout << "Enter Quantity to Add: ";
            cin >> quantity;
            system("cls");
            for (int i = 0; i < numOrders; i++)
            {
                if (orders[i]->getOrderID() == orderID)
                {
                    orders[i]->AddProductFromWarehouse(warehouse, productID, quantity);
                    break;
                }
            }
            system("pause");
            break;
        }
        case 5:
        {
        	system("cls");
            int orderID, productID;
            cout << "Enter Order ID to Remove Product from: ";
            cin >> orderID;
            system("cls");
            cout << "Enter Product ID to Remove: ";
            cin >> productID;
            system("cls");
            for (int i = 0; i < numOrders; i++)
            {
                if (orders[i]->getOrderID() == orderID)
                {
                    orders[i]->RemoveProduct(productID);
                    break;
                }
            }
            system("pause");
            break;
        }
        case 6:
        {
        	system("cls");
            int orderID, status;
            cout << "Enter Order ID to Update Status: ";
            cin >> orderID;
            system("cls");
            cout << "Enter New Status (0 for Pending, 1 for Shipped, 2 for Delivered): ";
            cin >> status;
            system("cls");
            for (int i = 0; i < numOrders; i++)
            {
                if (orders[i]->getOrderID() == orderID)
                {
                    orders[i]->UpdateStatus(status);
                    break;
                }
            }
            system("pause");
            break;
        }
        case 7:
        {
        	system("cls");
            cout << warehouse;
            system("pause");
            break;
        }
        case 8:
        {
        	system("cls");
            for (int i = 0; i < numOrders; i++)
            {
                cout << *orders[i] << endl;
            }
            system("pause");
            break;
        }
        case 9:
        {
        	system("cls");
            int orderID;
            cout << "Enter Order ID to Display Invoice: ";
            cin >> orderID;
            for (int i = 0; i < numOrders; i++)
            {
                if (orders[i]->getOrderID() == orderID)
                {
                    orders[i]->DisplayInvoice();
                    break;
                }
            }
            system("pause");
            break;
        }
        case 10:
       	{
        		
        	system("cls");
            cout << "Exiting the program.\n";
            system("pause");
            break;
			}
        default:
        	system("cls");
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 10);

	saveWarehouseData(warehouse, "warehouse.txt");
    saveOrderData(orders, numOrders, "orders.txt");
    
 
    for (int i = 0; i < numOrders; i++)
    {
        delete orders[i];
    }
    

    return 0;
}
