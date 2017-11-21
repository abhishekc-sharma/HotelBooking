#include "booking.h"
#include "receptionist.h"
#include "room_state.h"
#include "room.h"
#include "standard_room.h"
#include "deluxe_room.h"
#include "suite_room.h"

using namespace std;

float booking::get_cost() {
	room *required_room;
	if(this->type == receptionist::room_type::R_STANDARD) {
		required_room = receptionist::get_instance()->p_standard_room;
	} else if(this->type == receptionist::room_type::R_DELUXE) {
		required_room = receptionist::get_instance()->p_deluxe_room;
	} else if(this->type == receptionist::room_type::R_SUITE) {
		required_room = receptionist::get_instance()->p_suite_room;
	}

	int total_days = this->dates.second - this->dates.first;

	float total_cost = required_room->get_cost() * total_days;
	
	return total_cost;
}