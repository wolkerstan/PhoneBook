    #include "Contact.h"
    #include <regex>
    #include <ctime>
    #include <stdexcept>
    #include <QMessageBox>
    #include <QRegularExpression>
    // Конструкторы

    Contact::Contact() : FirstName(""), MiddleName(""), LastName(""), BirthDate(""), Email(""), PhoneNumbers({}) {}

    Contact::Contact(const std::string& firstName, const std::string& middleName,
                     const std::string& lastName, const std::string& birthDate,
                     const std::string& email,
                     const std::vector<std::pair<std::string, std::string>>& phoneNumbers) {
        SetFirstName(firstName);
        SetMiddleName(middleName);
        SetLastName(lastName);
        SetBirthDate(birthDate);
        SetEmail(email);
        SetPhoneNumbers(phoneNumbers);
    }

    // Геттеры
        std::string Contact::GetFirstName() const {
        return FirstName;
    }

    std::string Contact::GetMiddleName() const {
        return MiddleName;
    }

    std::string Contact::GetLastName() const {
        return LastName;
    }

    std::string Contact::GetBirthDate() const {
        return BirthDate;
    }

    std::string Contact::GetEmail() const {
        return Email;
    }

    std::vector<std::pair<std::string, std::string>> Contact::GetPhoneNumbers() const {
        return PhoneNumbers;
    } // Возвращает (Тип, Номер)



    // Сеттеры
    void Contact::SetFirstName(const std::string& firstName) {
        if (IsValidName(firstName)) {
            FirstName = firstName;
        }
        else {
            throw std::invalid_argument("Invalid first name!");
        }
    }

    void Contact::SetMiddleName(const std::string& middleName) {
        if (IsValidName(middleName)) {
            MiddleName = middleName;
        }
        else {
            throw std::invalid_argument("Invalid middle name!");
        }
    }

    void Contact::SetLastName(const std::string& lastName) {
        if (IsValidName(lastName)) {
            LastName = lastName;
        }
        else {
            throw std::invalid_argument("Invalid last name!");
        }
    }

    void Contact::SetBirthDate(const std::string& birthDate) {
        if (IsValidBirthDate(birthDate)) {
            BirthDate = birthDate;
        }
        else {
            throw std::invalid_argument("Invalid birth date!");
        }
    }

    void Contact::SetEmail(const std::string& email) {
        if (IsValidEmail(email)) {
            Email = email;
        }
        else {
            throw std::invalid_argument("Invalid email address!");
        }
    }

    void Contact::SetPhoneNumbers(const std::vector<std::pair<std::string, std::string>>& phoneNumbers) {
        std::vector<std::pair<std::string, std::string>> validNumbers;

        for (const auto& [type, phone] : phoneNumbers) {
            if (!IsValidPhoneNumber(phone)) {
                throw std::invalid_argument("Invalid phone number: " + phone);
            }
            validNumbers.emplace_back(type, phone);
        }

        PhoneNumbers = validNumbers;
    }

    /*bool Contact::IsValidName(const std::string& Name) const {

        std::regex NameRegex(R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё\- ]*[A-Za-zА-Яа-яЁё]$)");
        return std::regex_match(Name, NameRegex);
    }*/
    bool Contact::IsValidName(const std::string& Name) const {
        QRegularExpression regex(R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё\- ]*[A-Za-zА-Яа-яЁё]$)");
        return regex.match(QString::fromStdString(Name)).hasMatch();
    }

    bool Contact::IsValidBirthDate(const std::string& date) const {

        std::regex dateRegex(R"(^(\d{2})\.(\d{2})\.(\d{4})$)");
        std::smatch match;

        if (std::regex_match(date, match, dateRegex)) {
            int day = std::stoi(match[1].str());
            int month = std::stoi(match[2].str());
            int year = std::stoi(match[3].str());

            // Проверка на корректность месяца и дня
            if (month < 1 || month > 12 || day < 1 || day > 31) return false;

            // Проверка на количество дней в месяце
            int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

            // Високосный год
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                daysInMonth[1] = 29;
            }

            if (day > daysInMonth[month - 1]) return false;

            // Проверка на то, что дата меньше текущей
            std::time_t t = std::time(nullptr);
            std::tm now;
            localtime_s(&now, &t);
            int currentYear = now.tm_year + 1900;
            int currentMonth = now.tm_mon + 1;
            int currentDay = now.tm_mday;
            if (year > currentYear || (year == currentYear && month > currentMonth) ||
                (year == currentYear && month == currentMonth && day >= currentDay)) {
                return false;
            }

            return true;
        }
        return false;


    }
    bool Contact::IsValidEmail(const std::string& Email) const {
        std::regex EmailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)", std::regex_constants::ECMAScript);
        return std::regex_match(Email, EmailRegex);
    }

    bool Contact::IsValidPhoneNumber(const std::string& PhoneNumber) const {
        std::regex PhoneRegex(R"(^(\+7|8)?\(?[489][0-9]{2}\)?[0-9]{3}-?[0-9]{2}-?[0-9]{2}$)");
        return std::regex_match(PhoneNumber, PhoneRegex);
    }

