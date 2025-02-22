#pragma once
#include <vector>
#include "Contact.h"
class ContactManager
{
private:
	std::vector <Contact> Contacts;
public:
	void AddContact(const Contact& contact);
	void RemoveContact(size_t index);
	void DisplayAllContacts() const;
	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);
	Contact GetContact(int index) const;
	std::vector<Contact> GetContacts() const;
	void sortByField(const std::string& field);
};

