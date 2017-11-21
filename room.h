#ifndef ROOM_H
#define ROOM_H

#include "room_state.h"

class room {
public:
	room(float cpn);
	float get_cost();
	bool check_available(room_state *st, std::pair<int, int> dates);
	void mark_unavailable(room_state *st, std::pair<int, int> dates);
	void mark_available(room_state *st, std::pair<int, int> dates);
private:
	float cost_per_night;
};

#endif
