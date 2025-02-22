#include "AddContactDialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddContactDialog::AddContactDialog(QWidget* parent, const Contact& contact)
    : AddContactDialog(parent) { // Вызываем основной конструктор, чтобы не дублировать код

    // Заполняем текстовые поля
    firstNameEdit->setText(QString::fromStdString(contact.GetFirstName()));
    middleNameEdit->setText(QString::fromStdString(contact.GetMiddleName()));
    lastNameEdit->setText(QString::fromStdString(contact.GetLastName()));
    birthDateEdit->setText(QString::fromStdString(contact.GetBirthDate()));
    emailEdit->setText(QString::fromStdString(contact.GetEmail()));

    // Добавляем телефоны
    for (const auto& phone : contact.GetPhoneNumbers()) {
        AddPhoneField(); // Создаём новое поле с QComboBox и QLineEdit

        // Берём последнюю добавленную строку
        QWidget* phoneWidget = phoneNumbersLayout->itemAt(phoneNumbersLayout->count() - 1)->widget();
        if (!phoneWidget) continue;

        QComboBox* phoneTypeCombo = phoneWidget->findChild<QComboBox*>();
        QLineEdit* phoneNumberEdit = phoneWidget->findChild<QLineEdit*>();

        if (phoneTypeCombo && phoneNumberEdit) {
            phoneTypeCombo->setCurrentText(QString::fromStdString(phone.first)); // Устанавливаем тип
            phoneNumberEdit->setText(QString::fromStdString(phone.second)); // Устанавливаем номер
        }
    }
}


AddContactDialog::AddContactDialog(QWidget* parent) : QDialog(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    firstNameEdit = new QLineEdit(this);
    middleNameEdit = new QLineEdit(this);
    lastNameEdit = new QLineEdit(this);
    birthDateEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);

    firstNameEdit->setPlaceholderText("Имя");
    middleNameEdit->setPlaceholderText("Отчество");
    lastNameEdit->setPlaceholderText("Фамилия");
    birthDateEdit->setPlaceholderText("Дата рождения (ГГГГ-ММ-ДД)");
    emailEdit->setPlaceholderText("Email");

    layout->addWidget(firstNameEdit);
    layout->addWidget(middleNameEdit);
    layout->addWidget(lastNameEdit);
    layout->addWidget(birthDateEdit);
    layout->addWidget(emailEdit);

    phoneNumbersLayout = new QVBoxLayout();
    layout->addLayout(phoneNumbersLayout);

    addPhoneButton = new QPushButton("Добавить номер", this);
    connect(addPhoneButton, &QPushButton::clicked, this, &AddContactDialog::AddPhoneField);
    layout->addWidget(addPhoneButton);

    QPushButton* addButton = new QPushButton("Добавить контакт", this);
    connect(addButton, &QPushButton::clicked, this, &AddContactDialog::AcceptContact);
    layout->addWidget(addButton);
}

// Добавляет строку для нового номера
void AddContactDialog::AddPhoneField() {
    QWidget* phoneWidget = new QWidget(this);
    QHBoxLayout* phoneLayout = new QHBoxLayout(phoneWidget);

    QComboBox* phoneTypeCombo = new QComboBox(phoneWidget);
    phoneTypeCombo->addItem("Мобильный");
    phoneTypeCombo->addItem("Рабочий");
    phoneTypeCombo->addItem("Домашний");

    QLineEdit* phoneNumberEdit = new QLineEdit(phoneWidget);
    phoneNumberEdit->setPlaceholderText("Введите номер");

    QPushButton* removeButton = new QPushButton("Удалить", phoneWidget);
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        RemovePhoneField(phoneWidget);
    });

    phoneLayout->addWidget(phoneTypeCombo);
    phoneLayout->addWidget(phoneNumberEdit);
    phoneLayout->addWidget(removeButton);
    phoneWidget->setLayout(phoneLayout);
    phoneNumbersLayout->addWidget(phoneWidget);

    phoneFields.push_back({phoneNumberEdit, phoneTypeCombo});
}

// Удаляет строку с номером
void AddContactDialog::RemovePhoneField(QWidget* widget) {
    auto it = std::remove_if(phoneFields.begin(), phoneFields.end(), [widget](const std::pair<QLineEdit*, QComboBox*>& pair){
        return pair.first == widget->findChild<QLineEdit*>();
    });

    phoneFields.erase(it, phoneFields.end());
    phoneNumbersLayout->removeWidget(widget);
    delete widget;
}

void AddContactDialog::AcceptContact() {
    try {
        Contact newContact;

        newContact.SetFirstName(firstNameEdit->text().trimmed().toStdString());
        newContact.SetMiddleName(middleNameEdit->text().trimmed().toStdString());
        newContact.SetLastName(lastNameEdit->text().trimmed().toStdString());
        newContact.SetBirthDate(birthDateEdit->text().trimmed().toStdString());
        newContact.SetEmail(emailEdit->text().trimmed().toStdString());

        // Извлекаем номера телефонов
        std::vector<std::pair<std::string, std::string>> phoneNumbers = ExtractPhoneNumbers();

        if (phoneNumbers.empty()) {
            throw std::invalid_argument("Необходимо ввести хотя бы один номер телефона!");
        }
        newContact.SetPhoneNumbers(phoneNumbers);  // Используем сеттер для валидации

        accept();

    } catch (const std::invalid_argument& e) {
        QMessageBox::critical(this, "Ошибка ввода", e.what());
    }
}

Contact AddContactDialog::GetContact() const {
    Contact contact;

    contact.SetFirstName(firstNameEdit->text().trimmed().toStdString());
    contact.SetMiddleName(middleNameEdit->text().trimmed().toStdString());
    contact.SetLastName(lastNameEdit->text().trimmed().toStdString());
    contact.SetBirthDate(birthDateEdit->text().trimmed().toStdString());
    contact.SetEmail(emailEdit->text().trimmed().toStdString());

    // Извлекаем номера телефонов и передаем через сеттер
    contact.SetPhoneNumbers(ExtractPhoneNumbers());

    return contact;
}

// Вспомогательный метод для извлечения номеров телефонов
std::vector<std::pair<std::string, std::string>> AddContactDialog::ExtractPhoneNumbers() const {
    std::vector<std::pair<std::string, std::string>> phoneNumbers;

    for (int i = 0; i < phoneNumbersLayout->count(); ++i) {
        QWidget* phoneWidget = phoneNumbersLayout->itemAt(i)->widget();
        if (!phoneWidget) continue;

        QComboBox* phoneTypeCombo = phoneWidget->findChild<QComboBox*>();
        QLineEdit* phoneNumberEdit = phoneWidget->findChild<QLineEdit*>();

        if (!phoneTypeCombo || !phoneNumberEdit) continue;

        QString phoneType = phoneTypeCombo->currentText().trimmed();
        QString phoneNumber = phoneNumberEdit->text().trimmed();

        if (!phoneNumber.isEmpty()) {
            phoneNumbers.emplace_back(phoneType.toStdString(), phoneNumber.toStdString());
        }
    }
    return phoneNumbers;
}

