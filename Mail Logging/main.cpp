#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> // For stringstream to parse lines

using namespace std;

// Declare the struct and function prototypes first
struct Resident {
    string name;
    string email;
};

struct Mail {
    string recipient;
    string sender;
    string staffReceiving;
    string arrivalDate;
    bool pickedUp;
};

// Function declarations
bool isRecipientValid(const string& recipientName, const vector<Resident>& residents);
void sendEmailNotification(const Resident& recipient);
void saveMailLogToFile(const vector<Mail>& mailLog);
void loadRecipientsFromFile(const string& filename, vector<Resident>& residents);

int main() {
    vector<Resident> residents;

    // Load recipients from a file (e.g., "recipients.txt")
    loadRecipientsFromFile("recipients.txt", residents);

    if (residents.empty()) {
        cout << "No recipients found! Exiting program." << endl;
        return 1; // Exit if no recipients loaded
    }

    // Vector to store logged mail
    vector<Mail> mailLog;

    cout << "Mail Logging System\n";

    // Loop for logging new mail
    while (true) {
        string recipientName;
        cout << "\nEnter recipient name (or 'exit' to quit): ";
        getline(cin, recipientName);

        if (recipientName == "exit") {
            break;
        }

        // Validate recipient
        if (!isRecipientValid(recipientName, residents)) {
            cout << "Invalid recipient! Please try again.\n";
            continue;
        }

        string senderName, staffName, arrivalDate;
        cout << "Enter sender's name: ";
        getline(cin, senderName);
        cout << "Enter staff member's name receiving the mail: ";
        getline(cin, staffName);

        // Prompt user for the arrival date
        cout << "Enter the arrival date (YYYY-MM-DD): ";
        getline(cin, arrivalDate);

        // Log the mail details
        Mail newMail = { recipientName, senderName, staffName, arrivalDate, false };
        mailLog.push_back(newMail);

        // Send email notification
        for (const auto& resident : residents) {
            if (resident.name == recipientName) {
                sendEmailNotification(resident);
                break;
            }
        }

        cout << "Mail logged successfully!\n";
    }

    // Save the mail log to a file
    saveMailLogToFile(mailLog);

    // Display logged mails (for review)
    cout << "\nLogged Mails:\n";
    for (const auto& mail : mailLog) {
        cout << "Recipient: " << mail.recipient << ", Sender: " << mail.sender << ", Staff: " << mail.staffReceiving
            << ", Arrival Date: " << mail.arrivalDate << ", Picked Up: " << (mail.pickedUp ? "Yes" : "No") << endl;
    }

    return 0;
}

// Function implementations

bool isRecipientValid(const string& recipientName, const vector<Resident>& residents) {
    for (const auto& resident : residents) {
        if (resident.name == recipientName) {
            return true;
        }
    }
    return false;
}

void sendEmailNotification(const Resident& recipient) {
    cout << "\nSending email notification to: " << recipient.email << endl;
    cout << "Subject: Mail Arrival Notification\n";
    cout << "Dear " << recipient.name << ",\n";
    cout << "Your mail has arrived and is ready for pickup.\n";
    cout << "Best regards,\nYour Mail System\n";
}

// Load recipients from a file (e.g., recipients.txt)
void loadRecipientsFromFile(const string& filename, vector<Resident>& residents) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, email;

        // Assuming the name and email are separated by a comma
        if (getline(ss, name, ',') && getline(ss, email)) {
            Resident newResident = { name, email };
            residents.push_back(newResident);
        }
    }

    file.close();
}

// Save the mail log to a file (append mode)
void saveMailLogToFile(const vector<Mail>& mailLog) {
    ofstream file("mail_log.txt", ios::app); // Open file in append mode
    if (!file.is_open()) {
        cerr << "Failed to open mail log file!" << endl;
        return;
    }

    // Write the mail log to the file (appending to the existing content)
    for (const auto& mail : mailLog) {
        file << mail.recipient << "," << mail.sender << "," << mail.staffReceiving << ","
            << mail.arrivalDate << "," << (mail.pickedUp ? "Yes" : "No") << endl;
    }

    file.close();
    cout << "\nMail log has been saved to mail_log.txt!" << endl;
}
