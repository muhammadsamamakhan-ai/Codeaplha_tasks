#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

// ─── Simple hash (djb2) – avoids storing plain-text passwords ───────────────
unsigned long hashPassword(const string& password) {
    unsigned long hash = 5381;
    for (char c : password)
        hash = ((hash << 5) + hash) + (unsigned long)c;
    return hash;
}

// ─── File helpers ────────────────────────────────────────────────────────────
const string DB_FILE = "users.txt";   // Format per line: username:hashedPassword

bool usernameExists(const string& username) {
    ifstream file(DB_FILE);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUser;
        getline(ss, storedUser, ':');
        if (storedUser == username) return true;
    }
    return false;
}

bool saveUser(const string& username, unsigned long hashedPwd) {
    ofstream file(DB_FILE, ios::app);
    if (!file.is_open()) return false;
    file << username << ":" << hashedPwd << "\n";
    return true;
}

bool verifyUser(const string& username, unsigned long hashedPwd) {
    ifstream file(DB_FILE);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUser, storedHash;
        getline(ss, storedUser, ':');
        getline(ss, storedHash);
        if (storedUser == username && storedHash == to_string(hashedPwd))
            return true;
    }
    return false;
}

// ─── Input validation ────────────────────────────────────────────────────────
bool isValidUsername(const string& username) {
    if (username.length() < 3 || username.length() > 20) return false;
    for (char c : username)
        if (!isalnum(c) && c != '_') return false;
    return true;
}

bool isValidPassword(const string& password) {
    if (password.length() < 6) return false;
    bool hasUpper = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasDigit;
}

// ─── Masked password input ───────────────────────────────────────────────────
// Note: Uses standard cin (cross-platform). For hidden input on Linux/macOS,
// termios can be used; on Windows, _getch(). Kept portable here.
string getPassword(const string& prompt) {
    cout << prompt;
    string pwd;
    cin >> pwd;
    return pwd;
}

// ─── Registration ────────────────────────────────────────────────────────────
void registerUser() {
    cout << "\n--- REGISTRATION ---\n";

    string username;
    cout << "Enter username (3-20 chars, letters/digits/underscore): ";
    cin >> username;

    if (!isValidUsername(username)) {
        cout << "[ERROR] Invalid username. Must be 3-20 characters, "
                "letters, digits, or underscores only.\n";
        return;
    }

    if (usernameExists(username)) {
        cout << "[ERROR] Username '" << username << "' is already taken. "
                "Please choose another.\n";
        return;
    }

    string password = getPassword("Enter password (min 6 chars, 1 uppercase, 1 digit): ");

    if (!isValidPassword(password)) {
        cout << "[ERROR] Weak password. Must be at least 6 characters "
                "with at least 1 uppercase letter and 1 digit.\n";
        return;
    }

    string confirmPwd = getPassword("Confirm password: ");
    if (password != confirmPwd) {
        cout << "[ERROR] Passwords do not match.\n";
        return;
    }

    unsigned long hashed = hashPassword(password);

    if (saveUser(username, hashed))
        cout << "[SUCCESS] Account created for '" << username << "'. You can now log in.\n";
    else
        cout << "[ERROR] Could not save user. Check file permissions.\n";
}

// ─── Login ───────────────────────────────────────────────────────────────────
void loginUser() {
    cout << "\n--- LOGIN ---\n";

    string username;
    cout << "Enter username: ";
    cin >> username;

    string password = getPassword("Enter password: ");

    unsigned long hashed = hashPassword(password);

    if (verifyUser(username, hashed))
        cout << "[SUCCESS] Welcome back, " << username << "! Login successful.\n";
    else
        cout << "[ERROR] Invalid username or password. Please try again.\n";
}

// ─── Main menu ───────────────────────────────────────────────────────────────
int main() {
    int choice;

    while (true) {
        cout << "\n==========================================\n";
        cout << "       LOGIN & REGISTRATION SYSTEM        \n";
        cout << "==========================================\n";
        cout << "  1. Register\n";
        cout << "  2. Login\n";
        cout << "  3. Exit\n";
        cout << "==========================================\n";
        cout << "Enter choice (1-3): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ERROR] Invalid input. Enter 1, 2, or 3.\n";
            continue;
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser();    break;
            case 3:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "[ERROR] Invalid choice. Enter 1, 2, or 3.\n";
        }
    }
}