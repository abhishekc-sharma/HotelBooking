#ifndef ROOM_STATE_H
#define ROOM_STATE_H

#include <bitset>

struct room_state {
	std::bitset<28> availability;
	int room_no;

	room_state(int rno) : room_no(rno) {}
};

#endif