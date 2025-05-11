#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream> // For stringstream to parse lines
#include <cstdlib> // For system()

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
void loadMailLogFromFile(const string& filename, vector<Mail>& mailLog);
void markMailAsPickedUp(vector<Mail>& mailLog, const vector<Resident>& residents);

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
    loadMailLogFromFile("mail_log.txt", mailLog);

    // Loop for logging new mail
    while (true) {
        cout << "\nMail Logging System\n";
        cout << "1. Log new mail\n";
        cout << "2. Mark mail as picked up\n";
        cout << "3. Exit\n";
        cout << "Choose an option (1-3): ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            string recipientName;
            cout << "\nEnter recipient name: ";
            getline(cin, recipientName);

            // Validate recipient
            if (!isRecipientValid(recipientName, residents)) {
                cout << "Invalid recipient! Please try again.\n";
                continue;
            }

            // Prompt user for the mail information
            string senderName, staffName, arrivalDate;
            cout << "Enter sender's name: ";
            getline(cin, senderName);
            cout << "Enter staff member's name receiving the mail: ";
            getline(cin, staffName);
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

            // Save the mail log to a file
            saveMailLogToFile(mailLog); // Save after adding
        }
        else if (choice == "2") {
            markMailAsPickedUp(mailLog, residents);
        }
        else if (choice == "3") {
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    // Display logged mails (for review)
    cout << "\nFinal Logged Mails:\n";
    for (const auto& mail : mailLog) {
        cout << "Recipient: " << mail.recipient
            << ", Sender: " << mail.sender
            << ", Staff: " << mail.staffReceiving
            << ", Arrival Date: " << mail.arrivalDate
            << ", Picked Up: " << (mail.pickedUp ? "Yes" : "No") << endl;
    }

    return 0;
}

// Function implementations

// Function to validate if the recipient exists in the list
bool isRecipientValid(const string& recipientName, const vector<Resident>& residents) {
    for (const auto& resident : residents) {
        if (resident.name == recipientName) {
            return true;
        }
    }
    return false;
}

// Function to send an email notification to the recipient
void sendEmailNotification(const Resident& recipient) {
    string senderEmail = "tk.mailroomtest@gmail.com";  // Replace with your email
    string appPassword = "okik zyue pizt yrzd";    // Replace with your app password (Google or email service)

    // Email content
    string subject = "Mail Arrival Notification";
    string body = "Dear " + recipient.name + ",\n\nThe mailroom has received mail addressed to you. Please pick it up at your earliest convenience.\n\nBest regards,\nMailroom Staff";

    // Create the email content
    string emailContent = "To: " + recipient.email + "\n"
        "From: " + senderEmail + "\n"
        "Subject: " + subject + "\n\n"
        + body;

    // Write email content to a temporary file
    ofstream emailFile("email.txt");
    emailFile << emailContent;
    emailFile.close();

    // Create a temporary variable to hold the modified email (without leading/trailing spaces)
    string trimmedEmail = std::regex_replace(recipient.email, std::regex("^\\s+|\\s+$"), "");

    // Send the email using curl command with proper formatting
    string curlCommand = "curl -s --url \"smtp://smtp.gmail.com:587\" --ssl-reqd "
        "--mail-from \"" + senderEmail + "\" --mail-rcpt \"" + trimmedEmail + "\" "
        "--upload-file \"email.txt\" --user \"" + senderEmail + ":" + appPassword + "\"";


    // Run curl via system()
    system(curlCommand.c_str());

    // Clean up temporary email file
    remove("email.txt");

    cout << "Email sent to " << recipient.name << " at " << trimmedEmail << endl;
}

// Function to load recipients from a file (e.g., "recipients.txt")
void loadRecipientsFromFile(const string& filename, vector<Resident>& residents) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        string name, email;

        // Assuming the name and email are separated by a comma
        stringstream ss(line);
        if (getline(ss, name, ',') && getline(ss, email)) {
            residents.push_back({ name, email });
        }
    }

    file.close();
}

// Function to load the mail log from a file or create it not found
void loadMailLogFromFile(const string& filename, vector<Mail>& mailLog) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "No existing mail log found. Starting fresh." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string recipient, sender, staffReceiving, arrivalDate, pickedUpStr;

        if (getline(ss, recipient, ',') &&
            getline(ss, sender, ',') &&
            getline(ss, staffReceiving, ',') &&
            getline(ss, arrivalDate, ',') &&
            getline(ss, pickedUpStr)) {

            bool pickedUp = (pickedUpStr == "Yes");
            mailLog.push_back({ recipient, sender, staffReceiving, arrivalDate, pickedUp });
        }
    }

    file.close();
}

// Function to save the mail log to a file 
void saveMailLogToFile(const vector<Mail>& mailLog) {
    ofstream file("mail_log.txt");
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

// Function to mark the mail as picked up in the mail log
void markMailAsPickedUp(vector<Mail>& mailLog, const vector<Resident>& residents) {
    string recipientName;
    cout << "\nEnter recipient name to mark mail as picked up: ";
    getline(cin, recipientName);

    bool found = false;

    for (auto& mail : mailLog) {
        if (mail.recipient == recipientName && !mail.pickedUp) {
            cout << "\nMail found - Sender: " << mail.sender
                << ", Arrival Date: " << mail.arrivalDate << endl;
            cout << "Mark this mail as picked up? (y/n): ";
            string confirm;
            getline(cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                mail.pickedUp = true;
                cout << "Mail marked as picked up.\n";
                found = true;
            }
        }
    }

    if (found) {
        saveMailLogToFile(mailLog); // Save only if something changed
    }
    else {
        cout << "No uncollected mail found for " << recipientName << ".\n";
    }
}
