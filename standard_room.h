#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "room.h"

class standard_room : public room {
public:
	standard_room();
private:
	int num_beds;
	bool air_cond;
	bool mini_fridge;
	bool prem_view;
};

#endif
