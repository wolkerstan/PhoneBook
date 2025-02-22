#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ContactManager.h"

class ContactManagerGUI : public QWidget {
    Q_OBJECT

private:
    ContactManager contactManager;
    QTableWidget* tableWidget;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* saveButton;
    QPushButton* loadButton;
    QPushButton* sortButton;
    QPushButton* editButton;
    QPushButton* searchButton;
    QPushButton* resetSearchButton;
    void SetupUI();
    void LoadContactsToTable();

public:
    explicit ContactManagerGUI(QWidget* parent = nullptr);

private slots:
    void AddContact();
    void RemoveContact();
    void SaveContacts();
    void LoadContacts();
    void SortContacts();
    void EditContact();
    void SearchContacts();
    void ResetSearch();
};
