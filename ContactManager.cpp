#include "ContactManager.h"
#include <iostream>
#include <fstream>
#include <sstream>  
#include <stdexcept>
#include <algorithm>
#include <iomanip>
void ContactManager::AddContact(const Contact& contact) {
    Contacts.push_back(contact);
}

void ContactManager::RemoveContact(size_t index) {
    if (index < Contacts.size()) {
        Contacts.erase(Contacts.begin() + index);
    }
    else {
        throw std::out_of_range("The index is out of range");
    }
}
void ContactManager::DisplayAllContacts() const {
    if (Contacts.empty()) {
        std::cout << "Contact list is empty" << std::endl;
        return;
    }

    for (size_t i = 0; i < Contacts.size(); ++i) {
        std::cout << std::endl << "Contact #" << i + 1 << ":" << std::endl;
        Contacts[i].PrintContact();
        std::cout << "-----------------------------------" << std::endl;
    }
}

void ContactManager::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Couldn't write in file");
    }

    for (const auto& contact : Contacts) {
        file << contact.GetFirstName() << "|"
            << contact.GetMiddleName() << "|"
            << contact.GetLastName() << "|"
            << contact.GetBirthDate() << "|"
            << contact.GetEmail() << "|"
            << contact.GetPhoneNumber() << std::endl;
    }

    file.close();
    std::cout << "Contacts have been successfully saved to a file " << filename << "!" << std::endl;
}

void ContactManager::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Couldn't write in file");
    }
    Contacts.clear();
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string firstName, middleName, lastName, birthDate, email, phoneNumber;

        std::getline(ss, firstName, '|');
        std::getline(ss, middleName, '|');
        std::getline(ss, lastName, '|');
        std::getline(ss, birthDate, '|');
        std::getline(ss, email, '|');
        std::getline(ss, phoneNumber, '|');

        Contacts.emplace_back(firstName, middleName, lastName, birthDate, email, phoneNumber);
    }

    file.close();
}

Contact ContactManager::GetContact(int index) const {
    if (index >= 0 && index < Contacts.size()) {
        return Contacts.at(index);
    }
    return Contact();
}
std::vector<Contact> ContactManager::GetContacts() const {
    return Contacts;
}
void ContactManager::sortByField(const std::string& field) {
    if (field == "firstName") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            return a.GetFirstName() < b.GetFirstName();
            });
    }
    else if (field == "lastName") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            return a.GetLastName() < b.GetLastName();
            });
    }
    else if (field == "birthDate") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            std::tm tmA = {}, tmB = {};
            std::istringstream ssA(a.GetBirthDate());
            std::istringstream ssB(b.GetBirthDate());

            ssA >> std::get_time(&tmA, "%d.%m.%Y");
            ssB >> std::get_time(&tmB, "%d.%m.%Y");

            return std::mktime(&tmA) < std::mktime(&tmB); // Сортировка от младшего к старшему
            });
    }
}
