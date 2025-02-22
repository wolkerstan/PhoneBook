#pragma once
#include <vector>
#include "Contact.h"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class ContactManager {
private:
    std::vector<Contact> Contacts;
    QSqlDatabase db;
    void InitDatabase();

public:
    void UpdateContact(size_t index, const Contact& updatedContact);
    void AddContact(const Contact& contact);
    void RemoveContact(size_t index);
    void SaveToFile(const QString& filename);
    void LoadFromFile(const QString& filename);
    void SortByField(const std::string& field);

    Contact GetContact(int index) const;
    std::vector<Contact> GetContacts() const;
    std::vector<Contact> SearchContacts(const std::string& searchTerm) const;

    ContactManager();
    ~ContactManager();

    bool OpenDatabase(const QString& dbName);
    void CloseDatabase();
    void SaveToDatabase();
    void LoadFromDatabase();

};
