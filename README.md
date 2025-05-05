# 📬 Mailroom Logging and Notification System

This project is a mailroom management application designed to log incoming mail, track delivery details, and automatically notify recipients via email when their mail arrives. Built in C++, it is ideal for residential facilities, front desks, or mailrooms needing basic automation and recordkeeping.

---

## ✨ Features

- ✅ Logs recipient, sender, staff member receiving the mail, and arrival date.
- 📧 Automatically sends an email notification to the recipient using SMTP via `curl`.
- 📂 Saves mail logs in a text file for tracking and auditing.
- 👤 Validates recipient names against a pre-loaded resident list.

---

## 📁 Project Structure

MailLogging/
├── main.cpp # Main source file
├── recipients.txt # List of residents (name,email)
├── mail_log.txt # Mail log file (auto-generated)

yaml
Copy
Edit

---

## 🔧 Setup Instructions

### 1. Requirements

- C++ compiler (e.g., `g++`, or Microsoft Visual Studio)
- [curl](https://curl.se/) installed (and accessible in system PATH)
- Gmail or email service that supports SMTP + app password authentication

### 2. Clone the Repository

```bash
git clone https://github.com/tim-keiper/mail-logging.git
cd mail-logging
3. Prepare the Recipients File
Create a file called recipients.txt in the project directory.

Each line should contain a resident's name and email, separated by a comma.

Example:
John Doe,john.doe@example.com
Jane Smith,jane.smith@example.com
4. Configure Email Credentials
In main.cpp, locate the sendEmailNotification function and replace the default sender credentials:

Copy
Edit
std::string senderEmail = "your-email@gmail.com";   // Replace with your email
std::string appPassword = "your-app-password";      // Replace with your app password
⚠️ If you're using Gmail, enable 2-step verification, then generate an App Password.

🛠️ Build and Run
Option 1: Using g++
Compile the code:

g++ -o mail_logger main.cpp
Run the program:

./mail_logger
Option 2: Using Visual Studio
Open main.cpp in Visual Studio.

Right-click the project and select Build.

Run the executable via Start Debugging (F5) or Start Without Debugging (Ctrl+F5).

💡 How It Works
Program loads valid recipients from recipients.txt.

Prompts the user to log new mail details.

Validates recipient against the list.

Sends an email notification using SMTP and curl.

Appends the mail data to mail_log.txt for records.

Example Output:

Mail Logging System

Enter recipient name (or 'exit' to quit): John Doe
Enter sender's name: UPS
Enter staff member's name receiving the mail: Sarah Lee
Enter the arrival date (YYYY-MM-DD): 2025-05-04
Mail logged successfully!

Email sent to John Doe at john.doe@example.com
🧱 File Output
After a successful mail log, the program appends this to mail_log.txt:

John Doe,UPS,Sarah Lee,2025-05-04,No
You can manually edit this file if needed or implement further logic to mark items as picked up.

🛣️ Future Improvements
Add ability to mark items as picked up

Add file attachments (e.g. scanned mail)

Integrate with calendar systems for pickup reminders

Use a proper database instead of flat files

📄 Scope of Work Summary
This project meets the outlined goals from the SOW:

✅ Logs incoming mail (sender, recipient, staff, timestamp)

✅ Stores residents and email addresses in a file

✅ Sends email notifications upon mail arrival

✅ Saves records for future reference

✅ Milestones Completed
 Mail logging and user input

 Resident file reference (recipients.txt)

 Email notification system using SMTP via curl

 Final testing and logging to mail_log.txt

🛡️ Disclaimer
This is a basic prototype and uses plaintext app passwords for demonstration purposes. Do not share this code with real credentials in public repositories.

Consider using a secure method to store credentials (e.g., environment variables or config files) in production environments.

