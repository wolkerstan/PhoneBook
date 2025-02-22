#include <iostream>
#include <limits>
#include "ContactManager.h"
#include <regex>
#include <codecvt>
#include <stdexcept>

// Функции для каждого пункта меню
void addContact(ContactManager& contactManager);
void editContact(ContactManager& contactManager);
void deleteContact(ContactManager& contactManager);
void saveContactsToFile(const ContactManager& contactManager);
void loadContactsFromFile(ContactManager& contactManager);
void sortContacts(ContactManager& contactManager);
void searchContacts(const ContactManager& contactManager);

// Функция для удаления пробелов в начале и конце строки
std::string trim(const std::string& str) {
    std::string trimmed = str;
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), trimmed.end());
    return trimmed;
}

int main() {
    setlocale(LC_ALL, "Russian"); 

    ContactManager contactManager; 
    int choice;

    while (true) {

        std::cout << std::endl << "Available actions:" << std::endl
            << "1. Add Contact" << std::endl
            << "2. Edit Contact" << std::endl
            << "3. Remove Contact" << std::endl
            << "4. Show All Contacts" << std::endl
            << "5. Sort Contacts by Field" << std::endl
            << "6. Search Contact" << std::endl
            << "7. Save to File" << std::endl
            << "8. Load from File" << std::endl
            << "0. Exit" << std::endl
            << "Your Choice: ";

        std::cin >> choice;

        if (std::cin.fail()) { // для защиты от вводы белиберды
            std::cin.clear();  // Сбрасываем ошибку
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input, please enter a number." << std::endl;
            continue;
        }

        switch (choice) {

        case 1:
            addContact(contactManager);
            break;
        case 2:
            editContact(contactManager);
            break;
        case 3:
            deleteContact(contactManager);
            break;
        case 4:
            contactManager.DisplayAllContacts();
            break;
        case 5:
            sortContacts(contactManager);
            break;
        case 6:
            searchContacts(contactManager);
            break;
        case 7:
            saveContactsToFile(contactManager);
            break;
        case 8:
            loadContactsFromFile(contactManager);
            break;
        case 0:
            std::cout << "Exiting..." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice, try again." << std::endl;
        }
    }
}

void addContact(ContactManager& contactManager) {
    std::string firstName, middleName, lastName, birthDate, email, phoneNumber;
    std::cin.ignore();

    std::cout << "Enter First Name: ";
    std::getline(std::cin, firstName);
    firstName = trim(firstName);

    std::cout << "Enter Middle Name: ";
    std::getline(std::cin, middleName);
    middleName = trim(middleName);

    std::cout << "Enter Last Name: ";
    std::getline(std::cin, lastName);
    lastName = trim(lastName);

    std::cout << "Enter Birth Date (dd.mm.yyyy): ";
    std::getline(std::cin, birthDate);
    birthDate = trim(birthDate);

    std::cout << "Enter Email: ";
    std::getline(std::cin, email);
    email = trim(email);

    std::cout << "Enter Phone Number: ";
    std::getline(std::cin, phoneNumber);
    phoneNumber = trim(phoneNumber);

    try {
        Contact newContact(firstName, middleName, lastName, birthDate, email, phoneNumber);
        contactManager.AddContact(newContact);
        std::cout << "Contact added successfully!" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "" << std::endl;
    }
}

void editContact(ContactManager& contactManager) {
    int index;
    contactManager.DisplayAllContacts();
    std::cout << "Enter index of contact to edit: ";
    std::cin >> index;

    if (index < 0 || index >= static_cast<int>(contactManager.GetContacts().size())) {
        std::cout << "Invalid index" << std::endl;
        return;
    }

    int fieldChoice;
    std::string input;
    std::cin.ignore();

    while(true) {
        std::cout << std::endl << "Choose field to edit:" << std::endl
            << "1. First Name" << std::endl
            << "2. Middle Name" << std::endl
            << "3. Last Name" << std::endl
            << "4. Birth Date" << std::endl
            << "5. Email" << std::endl
            << "6. Phone Number" << std::endl
            << "7. Save and Exit" << std::endl
            << "Enter your choice: ";
        std::cin >> fieldChoice;
        std::cin.ignore();

        try {
            switch (fieldChoice) {
            case 1:
                std::cout << "Enter new First Name: ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetFirstName(input);
                break;
            case 2:
                std::cout << "Enter new Middle Name: ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetMiddleName(input);
                break;
            case 3:
                std::cout << "Enter new Last Name: ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetLastName(input);
                break;
            case 4:
                std::cout << "Enter new Birth Date (dd.mm.yyyy): ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetBirthDate(input);
                break;
            case 5:
                std::cout << "Enter new Email: ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetEmail(input);
                break;
            case 6:
                std::cout << "Enter new Phone Number: ";
                std::getline(std::cin, input);
                contactManager.GetContact(index).SetPhoneNumber(input);
                break;
            case 7:
                std::cout << "Contact updated successfully!" << std::endl;
                return;
            default:
                std::cout << "Invalid choice, try again." << std::endl;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    contactManager.DisplayAllContacts();
}


void deleteContact(ContactManager& contactManager) {
    size_t index;
    contactManager.DisplayAllContacts();

    std::cout << "Enter the index of the contact to delete: ";
    std::cin >> index;

    if (index == 0 || index > contactManager.GetContacts().size()) {
        std::cout << "Invalid index!" << std::endl;
        return;
    }

    contactManager.RemoveContact(index - 1);
    std::cout << "Contact deleted successfully!" << std::endl;
}




void sortContacts(ContactManager& contactManager) {
    std::string field;

    while (true) {
        std::cout << "Enter field to sort by (firstName, lastName, birthDate): ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
        std::getline(std::cin, field);


        if (field == "firstName" || field == "lastName" || field == "birthDate") {
            contactManager.sortByField(field);
            contactManager.DisplayAllContacts();
            break; 
        }
        else {
            std::cout << "Error: Invalid field for sorting! Please try again."<<std::endl;
        }
    }
}
void searchContacts(const ContactManager& contactManager) {

    std::string query;
    std::cout << "Enter name, email, or phone to search: ";
    std::cin.ignore();
    std::getline(std::cin, query);

    bool found = false;
    for (const auto& contact : contactManager.GetContacts()) {
        if (contact.GetFirstName().find(query) != std::string::npos ||
            contact.GetMiddleName().find(query) != std::string::npos ||
            contact.GetLastName().find(query) != std::string::npos ||
            contact.GetEmail().find(query) != std::string::npos ||
            contact.GetPhoneNumber().find(query) != std::string::npos) {
            contact.PrintContact();
            found = true;
        }
    }

    if (!found) {
        std::cout << "No contacts found!"<<std::endl;
    }
}

void saveContactsToFile(const ContactManager& contactManager) {
    std::string filename;
    std::cout << "Enter filename to save contacts: ";
    std::cin >> filename;

    try {
        contactManager.SaveToFile(filename);
        std::cout << "Contacts saved to file successfully!" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
   
}

void loadContactsFromFile(ContactManager& contactManager) {
    std::string filename;
    std::cout << "Enter filename to load contacts: ";
    std::cin >> filename;

    try {
        contactManager.LoadFromFile(filename);
        std::cout << "Contacts loaded successfully!" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}