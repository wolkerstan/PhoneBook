#pragma once
#include <string>

class Contact {
private:
    std::string FirstName;
    std::string MiddleName;
    std::string LastName;
    std::string BirthDate;
    std::string Email;
    std::string PhoneNumber;

public:

    Contact();
    Contact(const std::string& firstName, const std::string& middleName, const std::string& lastName, 
        const std::string& birthDate, const std::string& email, const std::string& phoneNumber);

  
    std::string GetFirstName() const;
    std::string GetMiddleName() const;
    std::string GetLastName() const;
    std::string GetBirthDate() const;
    std::string GetEmail() const;
    std::string GetPhoneNumber() const;


    void SetFirstName(const std::string& firstName);
    void SetMiddleName(const std::string& middleName);
    void SetLastName(const std::string& lastName);
    void SetBirthDate(const std::string& birthDate);
    void SetEmail(const std::string& email);
    void SetPhoneNumber(const std::string& phoneNumber);


    bool IsValidName(const std::string& name) const;
    bool IsValidBirthDate(const std::string& date) const;
    bool IsValidEmail(const std::string& email) const;
    bool IsValidPhoneNumber(const std::string& phoneNumber) const;


    void PrintContact() const;
};
