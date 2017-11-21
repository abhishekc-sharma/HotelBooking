#include "room.h"
#include "room_state.h"


room::room(float cpn) {
	cost_per_night = cpn;
}

bool room::check_available(room_state *st, std::pair<int, int> dates) {
	bool is_available = true;
	if(dates.first < 0 || dates.second < 0 || dates.first > 27 || dates.second > 27) {
		return false;
	}

	for(int i = dates.first; is_available && i <= dates.second; i++) {
		is_available = is_available && st->availability.test(i);
	}

	return is_available;
}

void room::mark_unavailable(room_state *st, std::pair<int, int> dates) {
	for(int i = dates.first; i <= dates.second; i++) {
		st->availability.set(i);
	}
}

void room::mark_available(room_state *st, std::pair<int, int> dates) {
	for(int i = dates.first; i <= dates.second; i++) {
		st->availability.reset(i);
	}
}

