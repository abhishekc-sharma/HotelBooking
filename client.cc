#include "receptionist.h"
#include "guest.h"
#include "booking.h"

#include <iostream>
#include <string>
using namespace std;

#define DATE 22-11-2017

int main () {

	string date {"DATE"};
	char cont = 'y';
	receptionist *r = receptionist::get_instance();

	do {
		int choice;
		cout << "1.Booking\n2.Cancel\n";
		cin >> choice;
		if(choice == 1) {
			string name;
			string date_start, date_end; 
			char room_choice; 
			receptionist::room_type room;

			cout << "Please enter your name: ";
			cin >> name;
			cout << "Start date: ";
			cin >> offset_start;
			cout << "End date: ";
			cin >> offset_end;
			cout << "Enter type of room : s - standard, d - deluxe, $ - suite: ";
			cin >> room_choice;
			switch(room_choice) {
				case 's': room = receptionist::room_type::R_STANDARD;
					break;
				case 'd': room = receptionist::room_type::R_DELUXE;
					break;
				case '$': room = receptionist::room_type::R_SUITE;
					break;
				default: cout << "Wtf ???";
						 continue;	
			}

			guest *g = new guest(name, "9611481691");
			pair <int,int> dates(offset_start ,offset_end);

			booking *b = r->make_booking(g, dates, room);
			cout << &b;

		} else if(choice == 2) {
			cout << "no cancelling, sucks to be you";
		} else {

		}

		cout << "\nContinue ? y/n : ";
		cin >> cont;

	} while(cont == 'y');

	return 0;
}