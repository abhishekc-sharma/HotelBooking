#ifndef GUEST_H
#define GUEST_H

#include <string>

class guest {
public:
	guest(std::string name, std::string contact_num) : name(name), contact_num(contact_num) {}	
private:
	std::string name;
	std::string contact_num;
};

#endif
