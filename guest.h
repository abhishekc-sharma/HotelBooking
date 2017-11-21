#ifndef GUEST_H
#define GUEST_H

#include <string>

struct guest {
public:
	guest(std::string name, std::string contact_num) : name(name), contact_num(contact_num) {}
	std::string name;
	std::string contact_num;
};

#endif
