#include "ContactManager.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>




// Редактирование контакта
void ContactManager::UpdateContact(size_t index, const Contact& updatedContact) {
    if (index >= Contacts.size()) {
        throw std::out_of_range("Неверный индекс контакта.");
    }
    Contacts[index] = updatedContact;
}

// Получение контакта по индексу
Contact ContactManager::GetContact(int index) const {
    if (index >= 0 && index < Contacts.size()) {
        return Contacts.at(index);
    }
    throw std::out_of_range("Индекс выходит за границы списка контактов");
}

// Получение всех контактов
std::vector<Contact> ContactManager::GetContacts() const {
    return Contacts;
}


void ContactManager::AddContact(const Contact& contact) {
    Contacts.push_back(contact);
}


// Удаление контакта
void ContactManager::RemoveContact(size_t index) {
    if (index < Contacts.size()) {
        Contacts.erase(Contacts.begin() + index);
    } else {
        throw std::out_of_range("Индекс выходит за границы списка контактов");
    }
}
// Сохранение контактов в файл
void ContactManager::SaveToFile(const QString& filename) {
    if (filename.isEmpty()) return; // Проверка на пустое имя файла

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }

    QTextStream out(&file);
    for (const auto& contact : Contacts) {
        out << QString::fromStdString(contact.GetFirstName()) << "|"
            << QString::fromStdString(contact.GetMiddleName()) << "|"
            << QString::fromStdString(contact.GetLastName()) << "|"
            << QString::fromStdString(contact.GetBirthDate()) << "|"
            << QString::fromStdString(contact.GetEmail()) << "|";

        // Сохранение номеров (тип:номер,тип:номер...)
        QStringList phoneList;
        for (const auto& phone : contact.GetPhoneNumbers()) {
            phoneList.append(QString::fromStdString(phone.first) + ":" + QString::fromStdString(phone.second));
        }

        out << phoneList.join(",") << "\n"; // Записываем номера, разделенные запятыми
    }

    file.close();
}

// Загрузка контактов из файла
void ContactManager::LoadFromFile(const QString& filename) {
    if (filename.isEmpty()) return; // Проверка на пустое имя файла

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл для чтения.");
        return;
    }

    QTextStream in(&file);
    Contacts.clear(); // Очистка списка перед загрузкой

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() < 6) continue; // Проверка целостности данных

        QString firstName  = parts[0].trimmed();
        QString middleName = parts[1].trimmed();
        QString lastName   = parts[2].trimmed();
        QString birthDate  = parts[3].trimmed();
        QString email      = parts[4].trimmed();
        QString phonesStr  = parts[5].trimmed();

        // Разбираем номера телефонов
        std::vector<std::pair<std::string, std::string>> phoneNumbers;
        QStringList phoneList = phonesStr.split(",");

        for (const QString& phonePair : phoneList) {
            QStringList phoneParts = phonePair.split(":");
            if (phoneParts.size() == 2) {
                phoneNumbers.emplace_back(phoneParts[0].toStdString(), phoneParts[1].toStdString());
            }
        }

        // Добавляем контакт в список
        Contacts.emplace_back(
            firstName.toStdString(),
            middleName.toStdString(),
            lastName.toStdString(),
            birthDate.toStdString(),
            email.toStdString(),
            phoneNumbers
            );
    }

    file.close();
}


// Сортировка контактов по определённому полю
void ContactManager::SortByField(const std::string& field) {
    if (field == "firstName") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            return a.GetFirstName() < b.GetFirstName();
        });
    } else if (field == "lastName") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            return a.GetLastName() < b.GetLastName();
        });
    } else if (field == "birthDate") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            std::tm tmA = {}, tmB = {};
            std::istringstream ssA(a.GetBirthDate());
            std::istringstream ssB(b.GetBirthDate());

            ssA >> std::get_time(&tmA, "%d.%m.%Y");
            ssB >> std::get_time(&tmB, "%d.%m.%Y");

            return std::mktime(&tmA) < std::mktime(&tmB);
        });
    } else if (field == "email") {
        std::sort(Contacts.begin(), Contacts.end(), [](const Contact& a, const Contact& b) {
            return a.GetEmail() < b.GetEmail();
        });
    }
}

std::vector<Contact> ContactManager::SearchContacts(const std::string& searchTerm) const {
    std::vector<Contact> foundContacts;
    std::string termLower = searchTerm;
    std::transform(termLower.begin(), termLower.end(), termLower.begin(), ::tolower);

    for (const auto& contact : Contacts) {
        std::string firstName = contact.GetFirstName();
        std::string middleName = contact.GetMiddleName();
        std::string lastName = contact.GetLastName();
        std::string birthDate = contact.GetBirthDate();
        std::string email = contact.GetEmail();

        // Преобразуем в нижний регистр для поиска без учета регистра
        std::transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);
        std::transform(middleName.begin(), middleName.end(), middleName.begin(), ::tolower);
        std::transform(lastName.begin(), lastName.end(), lastName.begin(), ::tolower);
        std::transform(birthDate.begin(), birthDate.end(), birthDate.begin(), ::tolower);
        std::transform(email.begin(), email.end(), email.begin(), ::tolower);

        // Поиск совпадения в любом поле
        if (firstName.find(termLower) != std::string::npos ||
            middleName.find(termLower) != std::string::npos ||
            lastName.find(termLower) != std::string::npos ||
            birthDate.find(termLower) != std::string::npos ||
            email.find(termLower) != std::string::npos) {
            foundContacts.push_back(contact);
            continue;
        }

        // Поиск в номерах телефонов
        for (const auto& phone : contact.GetPhoneNumbers()) {
            std::string phoneType = phone.first;
            std::string phoneNumber = phone.second;
            std::transform(phoneType.begin(), phoneType.end(), phoneType.begin(), ::tolower);
            std::transform(phoneNumber.begin(), phoneNumber.end(), phoneNumber.begin(), ::tolower);

            if (phoneType.find(termLower) != std::string::npos || phoneNumber.find(termLower) != std::string::npos) {
                foundContacts.push_back(contact);
                break;
            }
        }
    }

    return foundContacts;
}
