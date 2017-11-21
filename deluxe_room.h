#ifndef DELUXE_ROOM_H
#define DELUXE_ROOM_H

#include "room.h"

class deluxe_room : public room {
public:
	deluxe_room();
private:
	int num_beds;
	bool air_cond;
	bool mini_fridge;
	bool prem_view;
};

#endif
