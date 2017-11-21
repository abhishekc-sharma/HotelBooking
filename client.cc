#include "receptionist.h"
#include "guest.h"
#include "booking.h"
#include "date.h"

#include <iostream>
#include <string>
using namespace std;

#define DATE_Y 2017
#define DATE_M 11
#define DATE_D 22

int main () {

	Date dd;
	dd.y = DATE_Y;
	dd.m = DATE_M;
	dd.d = DATE_D;

 	char cont = 'y';
	receptionist *r = receptionist::get_instance();

	do {
		int choice, offset_start, offset_end;
		cout << "1.Booking\n2.Cancel\n3.Show all bookings\nYour option: ";
		cin >> choice;
		if(choice == 1) {
			string name, contact_num;
			Date d_start, d_end;
			char room_choice;
			receptionist::room_type room;

			cout << "Please enter your name: ";
			cin >> name;
			cout << "Please enter the contact number: ";
			cin>> contact_num;
			cout << "Start date:\n\tYear:";
			cin >> d_start.y;
			cout << "\tMonth: ";
			cin >> d_start.m;
			cout << "\tDay: ";
			cin >> d_start.d; 

			offset_start = getDifference(dd, d_start);
			if(offset_start < 0)
			{
				cout << "\nIncorrect date!\n";
				continue;
			}

			cout << "End date:\n\tYear:";
			cin >> d_end.y;
			cout << "\tMonth: ";
			cin >> d_end.m;
			cout << "\tDay: ";
			cin >> d_end.d; 

			if(getDifference(d_start, d_end) > 0) {
				offset_end = getDifference(dd, d_end);
			}
				
			if(offset_end < 0)
			{
				cout << "\nIncorrect date!\n";
				continue;
			}

			cout << "Enter type of room : s - standard, d - deluxe, $ - suite: ";
			cin >> room_choice;
			switch(room_choice) {
				case 's': room = receptionist::room_type::R_STANDARD;
					break;
				case 'd': room = receptionist::room_type::R_DELUXE;
					break;
				case '$': room = receptionist::room_type::R_SUITE;
					break;
				default: cout << "Please choose the correct option";
						 continue;
			}

			guest *g = new guest(name, contact_num);
			pair <int,int> dates(offset_start ,offset_end);

			int b = r->make_booking(g, dates, room); //booking id
			cout << b;

		} else if(choice == 2) {
			//cout << "no cancelling, sucks to be you";
			int b;
			cout << "Please enter the booking id: ";
			cin >> b;
			r->cancel_booking(b);

		} else if(choice == 3) {

			//Display all the bookings
			cout << "Displaying all current bookings:\n ";
			r->display();
		} else {

		}

		cout << "\nContinue ? y/n : ";
		cin >> cont;

	} while(cont == 'y');

	return 0;
}
