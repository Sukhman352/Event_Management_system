#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
using namespace std;

int bookingIDCounter = 1000; // Start from 1001

// Global lists for venues and services
vector<string> venues = {"Grand Ballroom", "Beach Resort", "Conference Center", "Garden", "Rooftop Terrace"};
vector<string> weddingServices = {"Decoration", "Photography", "Bridal Makeup", "Music Band"};
vector<string> conferenceServices = {"Projector", "Sound System", "WiFi", "Recording"};
vector<string> partyServices = {"DJ", "Lighting", "Cocktail Bar", "Photo Booth"};

// Global data structure for venue availability: venue -> set of booked dates
map<string, set<string>> venueBookings;

// Get current date and time as string for booking time
string getCurrentDateTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    dt.erase(dt.find('\n'), 1); // remove newline
    return dt;
}

// Validate date format (dd-mm-yyyy)
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    
    try {
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        
        if (year < 2024 || year > 2030) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        // Additional checks for specific months
        if (month == 2 && day > 29) return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}

// Check venue availability
bool isVenueAvailable(const string& venue, const string& date) {
    if (venueBookings.find(venue) == venueBookings.end()) return true;
    return venueBookings[venue].find(date) == venueBookings[venue].end();
}

// Load existing bookings from file to initialize venueBookings
void loadVenueBookings() {
    ifstream inFile("bookings.txt");
    if (!inFile) return;

    string line;
    string currentVenue, currentDate;
    
    while (getline(inFile, line)) {
        if (line.find("Venue: ") != string::npos) {
            currentVenue = line.substr(7);
        } 
        else if (line.find("Event Date: ") != string::npos) {
            currentDate = line.substr(12);
        } 
        else if (line.find("---------------------------------------------") != string::npos) {
            if (!currentVenue.empty() && !currentDate.empty()) {
                venueBookings[currentVenue].insert(currentDate);
            }
            currentVenue = "";
            currentDate = "";
        }
    }
    inFile.close();
}

class Event {
protected:
    string venue;
    int guests;
    float foodCostPerPerson;
    int bookingID;
    string bookingDateTime; // When booking was made
    string eventDate;       // When event will happen (dd-mm-yyyy)
    vector<string> selectedServices;
    string customerName;
    string customerContact;
    float discount;

    // Common invoice header
    void printInvoiceHeader() {
        cout << "\n-----------------------------------------------\n";
        cout << "               EVENT INVOICE\n";
        cout << "-----------------------------------------------\n";
        cout << "Booking ID: " << bookingID << endl;
        cout << "Booking Date: " << bookingDateTime << endl;
        cout << "Event Date: " << eventDate << endl;
        cout << "Customer: " << customerName << endl;
        cout << "Contact: " << customerContact << endl;
        cout << "Event Type: ";
    }

    // Get event date from user
    void getEventDate() {
        do {
            cout << "Enter event date (dd-mm-yyyy): ";
            cin >> eventDate;
            if (!isValidDate(eventDate)) {
                cout << "Invalid date format or date! Please use dd-mm-yyyy format.\n";
                continue;
            }
            
            if (!isVenueAvailable(venue, eventDate)) {
                cout << "Venue '" << venue << "' is already booked on " << eventDate << "!\n";
                cout << "Please choose a different date or venue.\n";
            } else {
                break;
            }
        } while (true);
    }
    
public:
    virtual void getDetails() = 0;
    virtual void generateInvoice() = 0;
    virtual void showSummary() = 0;
    virtual ~Event() {}
    
    // Common method to select venue
    void selectVenue() {
        cout << "\nAvailable Venues:\n";
        for (int i = 0; i < venues.size(); i++) {
            cout << i+1 << ". " << venues[i] << endl;
        }
        
        int choice;
        do {
            cout << "Select venue (1-" << venues.size() << "): ";
            cin >> choice;
        } while(choice < 1 || choice > venues.size());
        
        venue = venues[choice-1];
    }
    
    // Common method to get customer details
    void getCustomerDetails() {
        cin.ignore(); // Clear buffer
        cout << "Enter customer name: ";
        getline(cin, customerName);
        cout << "Enter contact number: ";
        getline(cin, customerContact);
    }
    
    // Common method to select services
    void selectServices(const vector<string>& services) {
        cout << "\nAvailable Services:\n";
        for (int i = 0; i < services.size(); i++) {
            cout << i+1 << ". " << services[i] << endl;
        }
        
        cout << "Enter service numbers to add (0 to finish): ";
        int choice;
        while (cin >> choice && choice != 0) {
            if (choice > 0 && choice <= services.size()) {
                selectedServices.push_back(services[choice-1]);
                cout << "Added: " << services[choice-1] << endl;
            } else {
                cout << "Invalid selection!\n";
            }
        }
    }
    
    // Common method to apply discount
    void applyDiscount() {
        discount = 0;
        if (guests > 100) {
            discount = 0.1; // 10% discount for large events
            cout << "10% discount applied for large event!\n";
        }
    }
    
    // Common booking steps
    void getCommonDetails() {
        bookingID = ++bookingIDCounter;
        bookingDateTime = getCurrentDateTime();
        
        getCustomerDetails();
        selectVenue();
        getEventDate();
    }
};

class Wedding : public Event {
    const float DECORATION_COST = 50000;
    
public:
    void getDetails() override {
        cout << "\n--- Wedding Booking ---\n";
        getCommonDetails();
        
        cout << "Enter number of guests: ";
        cin >> guests;
        cout << "Enter food cost per person: ";
        cin >> foodCostPerPerson;
        selectServices(weddingServices);
        applyDiscount();
    }

    void generateInvoice() override {
        float baseCost = guests * foodCostPerPerson;
        float serviceCost = selectedServices.size() * 15000; // 15k per service
        float totalCost = baseCost + DECORATION_COST + serviceCost;
        totalCost -= totalCost * discount; // Apply discount
        
        printInvoiceHeader();
        cout << "Wedding\n";
        cout << "Venue: " << venue << endl;
        cout << "Guests: " << guests << endl;
        cout << "Food Cost: Rs." << fixed << setprecision(2) << baseCost << endl;
        cout << "Decoration Cost: Rs." << DECORATION_COST << endl;
        
        if (!selectedServices.empty()) {
            cout << "\nAdditional Services:\n";
            for (const auto& service : selectedServices) {
                cout << " - " << service << " (Rs.15000)\n";
            }
            cout << "Total Service Cost: Rs." << serviceCost << endl;
        }
        
        if (discount > 0) {
            cout << "Discount: " << discount*100 << "% (-Rs." << totalCost * discount << ")\n";
        }
        
        cout << "\nTOTAL COST: Rs." << totalCost << endl;
        cout << "-----------------------------------------------\n";
        cout << "Wedding has been successfully booked!\n";
        cout << "-----------------------------------------------\n";

        // Save to file
        ofstream outFile("bookings.txt", ios::app);
        outFile << "Booking ID: " << bookingID << "\n";
        outFile << "Booking Date: " << bookingDateTime << "\n";
        outFile << "Event Date: " << eventDate << "\n";
        outFile << "Type: Wedding\n";
        outFile << "Customer: " << customerName << "\n";
        outFile << "Contact: " << customerContact << "\n";
        outFile << "Venue: " << venue << "\n";
        outFile << "Guests: " << guests << "\n";
        outFile << "Food Cost: " << foodCostPerPerson << " per person\n";
        outFile << "Total Cost: " << totalCost << "\n";
        outFile << "---------------------------------------------\n";
        outFile.close();
        
        // Update venue availability
        venueBookings[venue].insert(eventDate);
    }

    void showSummary() override {
        cout << "[" << bookingID << "] Wedding at " << venue 
             << " on " << eventDate << " for " << customerName << " (" << guests << " guests)\n";
    }
};

class Conference : public Event {
    const float EQUIPMENT_COST = 20000;
    
public:
    void getDetails() override {
        cout << "\n--- Conference Booking ---\n";
        getCommonDetails();
        
        cout << "Enter number of attendees: ";
        cin >> guests;
        cout << "Enter food cost per person: ";
        cin >> foodCostPerPerson;
        selectServices(conferenceServices);
        applyDiscount();
    }

    void generateInvoice() override {
        float baseCost = guests * foodCostPerPerson;
        float serviceCost = selectedServices.size() * 10000; // 10k per service
        float totalCost = baseCost + EQUIPMENT_COST + serviceCost;
        totalCost -= totalCost * discount; // Apply discount
        
        printInvoiceHeader();
        cout << "Conference\n";
        cout << "Venue: " << venue << endl;
        cout << "Attendees: " << guests << endl;
        cout << "Food Cost: Rs." << fixed << setprecision(2) << baseCost << endl;
        cout << "Equipment Cost: Rs." << EQUIPMENT_COST << endl;
        
        if (!selectedServices.empty()) {
            cout << "\nAdditional Services:\n";
            for (const auto& service : selectedServices) {
                cout << " - " << service << " (Rs.10000)\n";
            }
            cout << "Total Service Cost: Rs." << serviceCost << endl;
        }
        
        if (discount > 0) {
            cout << "Discount: " << discount*100 << "% (-Rs." << totalCost * discount << ")\n";
        }
        
        cout << "\nTOTAL COST: Rs." << totalCost << endl;
        cout << "-----------------------------------------------\n";
        cout << "Conference has been successfully booked!\n";
        cout << "-----------------------------------------------\n";

        // Save to file
        ofstream outFile("bookings.txt", ios::app);
        outFile << "Booking ID: " << bookingID << "\n";
        outFile << "Booking Date: " << bookingDateTime << "\n";
        outFile << "Event Date: " << eventDate << "\n";
        outFile << "Type: Conference\n";
        outFile << "Customer: " << customerName << "\n";
        outFile << "Contact: " << customerContact << "\n";
        outFile << "Venue: " << venue << "\n";
        outFile << "Attendees: " << guests << "\n";
        outFile << "Food Cost: " << foodCostPerPerson << " per person\n";
        outFile << "Total Cost: " << totalCost << "\n";
        outFile << "---------------------------------------------\n";
        outFile.close();
        
        // Update venue availability
        venueBookings[venue].insert(eventDate);
    }

    void showSummary() override {
        cout << "[" << bookingID << "] Conference at " << venue 
             << " on " << eventDate << " for " << customerName << " (" << guests << " attendees)\n";
    }
};

class Party : public Event {
    const float ENTERTAINMENT_COST = 10000;
    
public:
    void getDetails() override {
        cout << "\n--- Party Booking ---\n";
        getCommonDetails();
        
        cout << "Enter number of guests: ";
        cin >> guests;
        cout << "Enter food cost per person: ";
        cin >> foodCostPerPerson;
        selectServices(partyServices);
        applyDiscount();
    }

    void generateInvoice() override {
        float baseCost = guests * foodCostPerPerson;
        float serviceCost = selectedServices.size() * 8000; // 8k per service
        float totalCost = baseCost + ENTERTAINMENT_COST + serviceCost;
        totalCost -= totalCost * discount; // Apply discount
        
        printInvoiceHeader();
        cout << "Party\n";
        cout << "Venue: " << venue << endl;
        cout << "Guests: " << guests << endl;
        cout << "Food Cost: Rs." << fixed << setprecision(2) << baseCost << endl;
        cout << "Entertainment Cost: Rs." << ENTERTAINMENT_COST << endl;
        
        if (!selectedServices.empty()) {
            cout << "\nAdditional Services:\n";
            for (const auto& service : selectedServices) {
                cout << " - " << service << " (Rs.8000)\n";
            }
            cout << "Total Service Cost: Rs." << serviceCost << endl;
        }
        
        if (discount > 0) {
            cout << "Discount: " << discount*100 << "% (-Rs." << totalCost * discount << ")\n";
        }
        
        cout << "\nTOTAL COST: Rs." << totalCost << endl;
        cout << "-----------------------------------------------\n";
        cout << "Party has been successfully booked!\n";
        cout << "-----------------------------------------------\n";

        // Save to file
        ofstream outFile("bookings.txt", ios::app);
        outFile << "Booking ID: " << bookingID << "\n";
        outFile << "Booking Date: " << bookingDateTime << "\n";
        outFile << "Event Date: " << eventDate << "\n";
        outFile << "Type: Party\n";
        outFile << "Customer: " << customerName << "\n";
        outFile << "Contact: " << customerContact << "\n";
        outFile << "Venue: " << venue << "\n";
        outFile << "Guests: " << guests << "\n";
        outFile << "Food Cost: " << foodCostPerPerson << " per person\n";
        outFile << "Total Cost: " << totalCost << "\n";
        outFile << "---------------------------------------------\n";
        outFile.close();
        
        // Update venue availability
        venueBookings[venue].insert(eventDate);
    }

    void showSummary() override {
        cout << "[" << bookingID << "] Party at " << venue 
             << " on " << eventDate << " for " << customerName << " (" << guests << " guests)\n";
    }
};

// Function to read and display bookings from file
void displayAllBookingsFromFile() {
    ifstream inFile("bookings.txt");

    if (!inFile) {
        cout << "\nNo bookings found.\n";
        return;
    }

    cout << "\n===== ALL BOOKINGS =====\n";
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

// Function to search bookings by customer name
void searchBookings() {
    ifstream inFile("bookings.txt");
    if (!inFile) {
        cout << "\nNo bookings found.\n";
        return;
    }

    string searchName;
    cin.ignore();
    cout << "Enter customer name to search: ";
    getline(cin, searchName);
    
    cout << "\n===== SEARCH RESULTS =====\n";
    string line;
    bool found = false;
    bool recordStarted = false;
    
    while (getline(inFile, line)) {
        if (line.find("Booking ID:") != string::npos) {
            recordStarted = true;
        }
        
        if (recordStarted) {
            if (line.find(searchName) != string::npos) {
                found = true;
                // Display the booking block
                cout << line << endl;
                for (int i = 0; i < 8; i++) { // Display next 8 lines
                    if (getline(inFile, line)) {
                        cout << line << endl;
                    }
                }
                cout << endl;
                recordStarted = false;
            }
        }
        
        if (line.find("---------------------------------------------") != string::npos) {
            recordStarted = false;
        }
    }
    
    if (!found) {
        cout << "No bookings found for \"" << searchName << "\"\n";
    }
    inFile.close();
}

// Function to check venue availability
void checkAvailability() {
    string date;
    cout << "\n===== VENUE AVAILABILITY CHECK =====\n";
    cout << "Enter date to check (dd-mm-yyyy): ";
    cin >> date;
    
    if (!isValidDate(date)) {
        cout << "Invalid date format! Please use dd-mm-yyyy.\n";
        return;
    }
    
    cout << "\nVenue Availability on " << date << ":\n";
    cout << "---------------------------------\n";
    
    bool anyAvailable = false;
    for (const auto& venue : venues) {
        bool available = isVenueAvailable(venue, date);
        cout << venue << ": " << (available ? "Available" : "Booked") << endl;
        anyAvailable = anyAvailable || available;
    }
    
    if (!anyAvailable) {
        cout << "\nAll venues are booked on " << date << "!\n";
    }
    cout << "---------------------------------\n";
}

int main() {
    loadVenueBookings(); // Load existing bookings at startup
    
    int choice;
    char again;

    do {
        cout << "\n===== EVENT MANAGEMENT SYSTEM =====\n";
        cout << "1. Book Wedding\n2. Book Conference\n3. Book Party\n";
        cout << "4. View All Bookings\n5. Search Bookings\n6. Check Venue Availability\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        Event* event = nullptr;

        switch(choice) {
            case 1:
                event = new Wedding();
                break;
            case 2:
                event = new Conference();
                break;
            case 3:
                event = new Party();
                break;
            case 4:
                displayAllBookingsFromFile();
                continue;
            case 5:
                searchBookings();
                continue;
            case 6:
                checkAvailability();
                continue;
            case 7:
                cout << "Thank you for using the Event Management System!\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
                continue;
        }

        event->getDetails();
        event->generateInvoice();
        delete event;

        cout << "\nBook another event? (y/n): ";
        cin >> again;

    } while(again == 'y' || again == 'Y');

    cout << "\nThank you for using the Event Management System!\n";
    return 0;
}