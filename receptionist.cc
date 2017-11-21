#include <algorithm>
#include "booking.h"
#include <ctime>
#include "receptionist.h"
#include "room_state.h"
#include "room.h"
#include "standard_room.h"
#include "deluxe_room.h"
#include "suite_room.h"
#include <vector>

using namespace std;


receptionist *receptionist::ptr = nullptr;

receptionist *receptionist::get_instance() {
	if(!ptr) {
		ptr = new receptionist;
	}

	return ptr;
}

receptionist::receptionist() {
	this->last_reference_time = time(NULL);

	// room flyweight intrinsic states
	this->p_standard_room = new standard_room();
	this->p_deluxe_room = new deluxe_room();
	this->p_suite_room = new suite_room();
	int room_no = 0;

	// room flyweight extrinsic states
	for(int i = 0; i < 4; i++) {
		room_state *st = new room_state(i);
		this->standard_room_states.push_back(st);
	}

	for(int i = 0; i < 4; i++) {
		room_state *st = new room_state(i);
		this->deluxe_room_states.push_back(st);
	}

	for(int i = 0; i < 4; i++) {
		room_state *st = new room_state(i);
		this->suite_room_states.push_back(st);
	}
}

booking *receptionist::make_booking(guest *gst, std::pair<int, int> dates, room_type type) {
	vector<room_state *> required_states;
	room *required_room;

	if(type == room_type::R_STANDARD) {
		required_states = this->standard_room_states;
		required_room = this->p_standard_room;
	} else if(type == room_type::R_DELUXE) {
		required_states = this->deluxe_room_states;
		required_room = this->p_deluxe_room;
	} else if(type == room_type::R_SUITE) {
		required_states = this->suite_room_states;
		required_room = this->p_suite_room;
	}

	for(room_state *st : required_states) {
		if(required_room->check_available(st, dates)) {
			required_room->mark_unavailable(st, dates);
			booking *bkng = new booking(type, st, gst, dates);
			this->bookings.push_back(bkng);
			return bkng;
		}
	}

	return nullptr;
}

void receptionist::cancel_booking(booking *bkng) {
	room *required_room;
	if(bkng->type == room_type::R_STANDARD) {
		required_room = this->p_standard_room;
	} else if(bkng->type == room_type::R_DELUXE) {
		required_room = this->p_deluxe_room;
	} else if(bkng->type == room_type::R_SUITE) {
		required_room = this->p_suite_room;
	}

	required_room->mark_available(bkng->p_room, bkng->dates);
	this->bookings.erase(std::find(this->bookings.begin(), this->bookings.end(), bkng));
	delete bkng;
}
