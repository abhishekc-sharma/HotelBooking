#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include <ctime>
#include "guest.h"
#include <map>
#include "room.h"
#include "room_state.h"
#include "sqlite3.h"
#include <string>
#include <vector>

class booking;

class receptionist {
public:
	enum class room_type {R_STANDARD, R_DELUXE, R_SUITE};
	room *p_standard_room;
	room *p_suite_room;
	room *p_deluxe_room;
	static receptionist *get_instance();
	int make_booking(guest *gst, std::pair<int, int> dates, room_type type);
	void cancel_booking(int bkng);
private:
	receptionist();
	static receptionist *ptr;
	std::map<int, booking *> bookings;
	int booking_no;
	std::vector<room_state *> standard_room_states;
	std::vector<room_state *> suite_room_states;
	std::vector<room_state *> deluxe_room_states;

};

#endif
