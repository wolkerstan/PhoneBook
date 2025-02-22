#include "ContactManagerGUI.h"
#include "AddContactDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>


void ContactManagerGUI::EditContact() {
    int row = tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для редактирования.");
        return;
    }

    // Получаем данные выбранного контакта
    Contact contact = contactManager.GetContact(row);

    // Открываем диалог редактирования (используем тот же AddContactDialog)
    AddContactDialog dialog(this, contact);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Contact updatedContact = dialog.GetContact();
            contactManager.UpdateContact(row, updatedContact); // Добавь этот метод в ContactManager
            LoadContactsToTable(); // Перезагружаем таблицу
        } catch (const std::invalid_argument& e) {
            QMessageBox::critical(this, "Ошибка", e.what());
        }
    }
}


ContactManagerGUI::ContactManagerGUI(QWidget* parent) : QWidget(parent) {
    SetupUI();
    LoadContactsToTable(); // Загружаем контакты при запуске
}

// Настройка интерфейса
void ContactManagerGUI::SetupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({"Имя", "Отчество", "Фамилия", "Дата рождения", "Email", "Тип номера", "Телефоны"});
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSortingEnabled(true);
    layout->addWidget(tableWidget);


    addButton = new QPushButton("Добавить контакт", this);
    removeButton = new QPushButton("Удалить контакт", this);
    saveButton = new QPushButton("Сохранить контакты", this);
    loadButton = new QPushButton("Загрузить контакты", this);
    sortButton = new QPushButton("Сортировать", this);
    editButton = new QPushButton("Редактировать контакт", this);
    searchButton = new QPushButton("Поиск", this);
    resetSearchButton = new QPushButton("Сбросить поиск", this);

    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);
    layout->addWidget(sortButton);
    layout->addWidget(editButton);
    layout->addWidget(searchButton);
    layout->addWidget(resetSearchButton);



    connect(addButton, &QPushButton::clicked, this, &ContactManagerGUI::AddContact);
    connect(removeButton, &QPushButton::clicked, this, &ContactManagerGUI::RemoveContact);
    connect(saveButton, &QPushButton::clicked, this, &ContactManagerGUI::SaveContacts);
    connect(loadButton, &QPushButton::clicked, this, &ContactManagerGUI::LoadContacts);
    connect(sortButton, &QPushButton::clicked, this, &ContactManagerGUI::SortContacts);
    connect(editButton, &QPushButton::clicked, this, &ContactManagerGUI::EditContact);
    connect(searchButton, &QPushButton::clicked, this, &ContactManagerGUI::SearchContacts);
    connect(resetSearchButton, &QPushButton::clicked, this, &ContactManagerGUI::ResetSearch);
}

// Загрузка контактов в таблицу
void ContactManagerGUI::LoadContactsToTable() {
    tableWidget->clearContents(); // Очищает только содержимое, не удаляя заголовки
    tableWidget->setRowCount(0);
    std::vector<Contact> contacts = contactManager.GetContacts();
    for (size_t i = 0; i < contacts.size(); ++i) {
        tableWidget->insertRow(static_cast<int>(i));

        tableWidget->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(contacts[i].GetFirstName())));
        tableWidget->setItem(static_cast<int>(i), 1, new QTableWidgetItem(QString::fromStdString(contacts[i].GetMiddleName())));
        tableWidget->setItem(static_cast<int>(i), 2, new QTableWidgetItem(QString::fromStdString(contacts[i].GetLastName())));
        tableWidget->setItem(static_cast<int>(i), 3, new QTableWidgetItem(QString::fromStdString(contacts[i].GetBirthDate())));
        tableWidget->setItem(static_cast<int>(i), 4, new QTableWidgetItem(QString::fromStdString(contacts[i].GetEmail())));

        // Формируем строки для типов и номеров телефонов
        std::string phoneTypes;
        std::string phoneNumbers;

        for (const auto& phone : contacts[i].GetPhoneNumbers()) {
            if (!phoneTypes.empty()) {
                phoneTypes += ", ";
                phoneNumbers += ", ";
            }
            phoneTypes += phone.first;   // Тип номера (например: "Мобильный")
            phoneNumbers += phone.second; // Сам номер (например: "+79110549181")
        }

        tableWidget->setItem(static_cast<int>(i), 5, new QTableWidgetItem(QString::fromStdString(phoneTypes)));
        tableWidget->setItem(static_cast<int>(i), 6, new QTableWidgetItem(QString::fromStdString(phoneNumbers)));
    }
}

// Добавление нового контакта
void ContactManagerGUI::AddContact() {
    AddContactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Contact newContact = dialog.GetContact();
            contactManager.AddContact(newContact);
            LoadContactsToTable();
        } catch (const std::invalid_argument& e) {
            QMessageBox::critical(this, "Ошибка", e.what());
        }
    }
}

// Удаление выбранного контакта
void ContactManagerGUI::RemoveContact() {
    int row = tableWidget->currentRow();
    if (row >= 0) {
        try {
            contactManager.RemoveContact(static_cast<size_t>(row));
            LoadContactsToTable();
        } catch (const std::out_of_range& e) {
            QMessageBox::warning(this, "Ошибка", "Невозможно удалить контакт: " + QString::fromStdString(e.what()));
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для удаления.");
    }
}


// Сохранение контактов в файл
void ContactManagerGUI::SaveContacts() {
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить контакты", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");

    if (filename.isEmpty()) return; // Пользователь отменил выбор

    try {
        contactManager.SaveToFile(filename); // Передаем filename
        QMessageBox::information(this, "Успех", "Контакты успешно сохранены.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }
}

// Загрузка контактов через GUI
void ContactManagerGUI::LoadContacts() {
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл контактов", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");

    if (filename.isEmpty()) return; // Пользователь отменил выбор

    try {
        contactManager.LoadFromFile(filename); // Передаем filename
        LoadContactsToTable(); // Обновление UI
        QMessageBox::information(this, "Успех", "Контакты загружены.");
    } catch (const std::invalid_argument& e) {
        QMessageBox::critical(this, "Ошибка в данных контактов", e.what());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка загрузки", e.what());
    }
}

void ContactManagerGUI::SortContacts() {

    QStringList fields = {"Имя", "Фамилия", "Дата рождения", "Email"};
    QMap<QString, std::string> fieldMap = {
        {"Имя", "firstName"},
        {"Фамилия", "lastName"},
        {"Дата рождения", "birthDate"},
        {"Email", "email"}
    };

    bool ok;
    QString selectedField = QInputDialog::getItem(this, "Сортировка", "Выберите поле для сортировки:", fields, 0, false, &ok);
    if (ok && !selectedField.isEmpty()) {
        contactManager.SortByField(fieldMap[selectedField]);
        LoadContactsToTable(); // Обновляем таблицу
    }
}

void ContactManagerGUI::SearchContacts() {
    bool ok;
    QString searchTerm = QInputDialog::getText(this, "Поиск", "Введите строку для поиска:", QLineEdit::Normal, "", &ok);

    if (ok && !searchTerm.trimmed().isEmpty()) {
        std::vector<Contact> foundContacts = contactManager.SearchContacts(searchTerm.toStdString());

        if (foundContacts.empty()) {
            QMessageBox::information(this, "Результат поиска", "Контакты не найдены.");
            LoadContactsToTable();
            return;  // Выходим из метода, не обновляя таблицу
        }

        tableWidget->setRowCount(0); // Очищаем таблицу

        for (size_t i = 0; i < foundContacts.size(); ++i) {
            tableWidget->insertRow(static_cast<int>(i));

            tableWidget->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(foundContacts[i].GetFirstName())));
            tableWidget->setItem(static_cast<int>(i), 1, new QTableWidgetItem(QString::fromStdString(foundContacts[i].GetMiddleName())));
            tableWidget->setItem(static_cast<int>(i), 2, new QTableWidgetItem(QString::fromStdString(foundContacts[i].GetLastName())));
            tableWidget->setItem(static_cast<int>(i), 3, new QTableWidgetItem(QString::fromStdString(foundContacts[i].GetBirthDate())));
            tableWidget->setItem(static_cast<int>(i), 4, new QTableWidgetItem(QString::fromStdString(foundContacts[i].GetEmail())));

            std::string phoneTypes, phoneNumbers;
            for (const auto& phone : foundContacts[i].GetPhoneNumbers()) {
                if (!phoneTypes.empty()) {
                    phoneTypes += ", ";
                    phoneNumbers += ", ";
                }
                phoneTypes += phone.first;
                phoneNumbers += phone.second;
            }

            tableWidget->setItem(static_cast<int>(i), 5, new QTableWidgetItem(QString::fromStdString(phoneTypes)));
            tableWidget->setItem(static_cast<int>(i), 6, new QTableWidgetItem(QString::fromStdString(phoneNumbers)));
        }
    }
}

void ContactManagerGUI::ResetSearch() {
    LoadContactsToTable();  // Перезагружаем всю таблицу
}
