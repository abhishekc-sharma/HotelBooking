#ifndef BOOKING_H
#define BOOKING_H

#include "guest.h"
#include "receptionist.h"
#include "room_state.h"


class booking {
public:
	receptionist::room_type type;
	room_state *p_room;
	guest *p_guest;
	std::pair<int, int> dates;
	booking(receptionist::room_type type, room_state *p_room, guest *p_guest, std::pair<int, int> dates) :
			type(type), p_room(p_room), p_guest(p_guest), dates(dates) {}
	float get_cost();
};

#endif
