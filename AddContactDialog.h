#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <vector>
#include "Contact.h"

class AddContactDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget* parent = nullptr);
    AddContactDialog(QWidget* parent, const Contact& contact);
    Contact GetContact() const;

private:

    QLineEdit* firstNameEdit;
    QLineEdit* middleNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* birthDateEdit;
    QLineEdit* emailEdit;
    QVBoxLayout* phoneNumbersLayout;
    QPushButton* addPhoneButton;

    std::vector<std::pair<QLineEdit*, QComboBox*>> phoneFields;
    std::vector<std::pair<std::string, std::string>> ExtractPhoneNumbers() const;
    void AddPhoneField();
    void RemovePhoneField(QWidget* widget);

private slots:
    void AcceptContact();
};
