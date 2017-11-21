#include <algorithm>
#include "booking.h"
#include "guest.h"
#include <map>
#include "receptionist.h"
#include "room_state.h"
#include "room.h"
#include "standard_room.h"
#include <stdio.h>
#include "deluxe_room.h"
#include "suite_room.h"
#include <vector>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;


receptionist *receptionist::get_instance() {
	static receptionist ptr;
	return &ptr;
}

receptionist::receptionist() {
	sqlite3_open("hb.db", &p_db);
	string query;
	int result_data_count;
	int result;

	sqlite3_stmt *create_roomstable_stmt;
	query = "CREATE TABLE IF NOT EXISTS room_states (room_no INTEGER, room_type INTEGER, availability INTEGER, PRIMARY KEY(room_no, room_type))";
	sqlite3_prepare(p_db, query.c_str(), query.size(), &create_roomstable_stmt, NULL);
	sqlite3_step(create_roomstable_stmt);
	sqlite3_finalize(create_roomstable_stmt);

	sqlite3_stmt *create_bookingstable_stmt;
	query = "CREATE TABLE IF NOT EXISTS bookings (booking_no INTEGER PRIMARY KEY AUTOINCREMENT, room_type INTEGER, room_no INTEGER, from_date INTEGER, to_date INTEGER, name VARCHAR(20), contact_no VARCHAR(12))";
	sqlite3_prepare(p_db, query.c_str(), query.size(), &create_bookingstable_stmt, NULL);
	sqlite3_step(create_bookingstable_stmt);
	sqlite3_finalize(create_bookingstable_stmt);

	// room flyweight intrinsic states
	this->p_standard_room = new standard_room();
	this->p_deluxe_room = new deluxe_room();
	this->p_suite_room = new suite_room();

	sqlite3_stmt *select_roomstable_stmt;
	query = "SELECT * FROM room_states";
	sqlite3_prepare(p_db, query.c_str(), query.size(), &select_roomstable_stmt, NULL);
	result = sqlite3_step(select_roomstable_stmt);
	result_data_count = sqlite3_data_count(select_roomstable_stmt);
	if(result_data_count == 0) {
		// first run
		char insert_query[400];
		for(int i = 0; i < 4; i++) {
			room_state *st = new room_state(i);
			this->standard_room_states.push_back(st);
			sqlite3_stmt *insert_roomstable_stmt;
			sprintf(insert_query, "INSERT INTO room_states VALUES (%d, %d, %lu)", st->room_no, 0, st->availability.to_ulong());
			query = insert_query;
			sqlite3_prepare(p_db, query.c_str(), query.size(), &insert_roomstable_stmt, NULL);
			sqlite3_step(insert_roomstable_stmt);
			sqlite3_finalize(insert_roomstable_stmt);
		}

		for(int i = 0; i < 4; i++) {
			room_state *st = new room_state(i);
			this->deluxe_room_states.push_back(st);
			sqlite3_stmt *insert_roomstable_stmt;
			sprintf(insert_query, "INSERT INTO room_states VALUES (%d, %d, %lu)", st->room_no, 1, st->availability.to_ulong());
			query = insert_query;
			sqlite3_prepare(p_db, query.c_str(), query.size(), &insert_roomstable_stmt, NULL);
			sqlite3_step(insert_roomstable_stmt);
			sqlite3_finalize(insert_roomstable_stmt);
		}

		for(int i = 0; i < 4; i++) {
			room_state *st = new room_state(i);
			this->suite_room_states.push_back(st);
			sqlite3_stmt *insert_roomstable_stmt;
			sprintf(insert_query, "INSERT INTO room_states VALUES (%d, %d, %lu)", st->room_no, 2, st->availability.to_ulong());
			query = insert_query;
			sqlite3_prepare(p_db, query.c_str(), query.size(), &insert_roomstable_stmt, NULL);
			sqlite3_step(insert_roomstable_stmt);
			sqlite3_finalize(insert_roomstable_stmt);
		}
	} else {
		// subsequent runs
		int i = 0;
		while(result == SQLITE_ROW) {
			int room_no, room_type, availability;
			room_no = sqlite3_column_int(select_roomstable_stmt, 0);
			room_type = sqlite3_column_int(select_roomstable_stmt, 1);
			availability = sqlite3_column_int(select_roomstable_stmt, 2);
			if(room_type == 0) {
				room_state *st = new room_state(room_no, availability);
				this->standard_room_states.push_back(st);
			} else if(room_type == 1) {
				room_state *st = new room_state(room_no, availability);
				this->deluxe_room_states.push_back(st);
			} else {
				room_state *st = new room_state(room_no, availability);
				this->suite_room_states.push_back(st);
			}
			result = sqlite3_step(select_roomstable_stmt);
		}
	}
	sqlite3_finalize(select_roomstable_stmt);

	sqlite3_stmt *select_bookingstable_stmt;
	query = "SELECT * FROM bookings";
	sqlite3_prepare(p_db, query.c_str(), query.size(), &select_bookingstable_stmt, NULL);
	result = sqlite3_step(select_bookingstable_stmt);
	result_data_count = sqlite3_data_count(select_bookingstable_stmt);

	if(result_data_count != 0) {
		while(result == SQLITE_ROW) {
			int booking_no, room_type, room_no, from, to;
			string name, contact_no;
			booking_no = sqlite3_column_int(select_bookingstable_stmt, 0);
			room_type = sqlite3_column_int(select_bookingstable_stmt, 1);
			room_no = sqlite3_column_int(select_bookingstable_stmt, 2);
			from = sqlite3_column_int(select_bookingstable_stmt, 3);
			to = sqlite3_column_int(select_bookingstable_stmt, 4);
			const char *t1 = (const char *)(sqlite3_column_text(select_bookingstable_stmt, 5));
			const char *t2 = (const char *)(sqlite3_column_text(select_bookingstable_stmt, 6));
			char c_name[20], c_contact_no[20];
			for(int i = 0; i < 20; i++) {
				c_name[i] = t1[i];
				c_contact_no[i] = t2[i];
			}
			name = c_name;
			contact_no = c_contact_no;
			room_state *p_room;
			typename receptionist::room_type type;
			if(room_type == 0) {
				p_room = standard_room_states[room_no];
				type = receptionist::room_type::R_STANDARD;
			} else if(room_type == 1) {
				p_room = deluxe_room_states[room_no];
				type = receptionist::room_type::R_DELUXE;
			} else {
				p_room = suite_room_states[room_no];
				type = receptionist::room_type::R_SUITE;
			}
			guest *p_guest = new guest(name , contact_no);
			bookings[booking_no] = new booking(type, p_room, p_guest, pair<int, int>(from, to));
			result = sqlite3_step(select_bookingstable_stmt);
		}
	}

	sqlite3_finalize(select_bookingstable_stmt);
}

receptionist::~receptionist() {
	string query;
	char update_query[400];
	for(int i = 0; i < 4; i++) {
		room_state *st = standard_room_states[i];
		sqlite3_stmt *update_roomstable_stmt;
		sprintf(update_query, "UPDATE room_states SET availability = %lu WHERE room_no = %d AND room_type = %d", st->availability.to_ulong(), st->room_no, 0);
		query = update_query;
		sqlite3_prepare(p_db, query.c_str(), query.size(), &update_roomstable_stmt, NULL);
		sqlite3_step(update_roomstable_stmt);
		sqlite3_finalize(update_roomstable_stmt);
		delete st;
	}

	for(int i = 0; i < 4; i++) {
		room_state *st = deluxe_room_states[i];
		sqlite3_stmt *update_roomstable_stmt;
		sprintf(update_query, "UPDATE room_states SET availability = %lu WHERE room_no = %d AND room_type = %d", st->availability.to_ulong(), st->room_no, 1);
		query = update_query;
		sqlite3_prepare(p_db, query.c_str(), query.size(), &update_roomstable_stmt, NULL);
		sqlite3_step(update_roomstable_stmt);
		sqlite3_finalize(update_roomstable_stmt);
		delete st;
	}

	for(int i = 0; i < 4; i++) {
		room_state *st = suite_room_states[i];
		sqlite3_stmt *update_roomstable_stmt;
		sprintf(update_query, "UPDATE room_states SET availability = %lu WHERE room_no = %d AND room_type = %d", st->availability.to_ulong(), st->room_no, 2);
		query = update_query;
		sqlite3_prepare(p_db, query.c_str(), query.size(), &update_roomstable_stmt, NULL);
		sqlite3_step(update_roomstable_stmt);
		sqlite3_finalize(update_roomstable_stmt);
		delete st;
	}

	delete this->p_standard_room;
	delete this->p_deluxe_room;
	delete this->p_suite_room;

	sqlite3_close(p_db);
}

int receptionist::make_booking(guest *gst, std::pair<int, int> dates, room_type type) {
	vector<room_state *> required_states;
	room *required_room;
	int itype;
	if(type == room_type::R_STANDARD) {
		required_states = this->standard_room_states;
		required_room = this->p_standard_room;
		itype = 0;
	} else if(type == room_type::R_DELUXE) {
		required_states = this->deluxe_room_states;
		required_room = this->p_deluxe_room;
		itype = 1;
	} else if(type == room_type::R_SUITE) {
		required_states = this->suite_room_states;
		required_room = this->p_suite_room;
		itype = 2;
	}

	for(room_state *st : required_states) {
		if(required_room->check_available(st, dates)) {
			required_room->mark_unavailable(st, dates);
			char insert_query[400];
			sqlite3_stmt *insert_bookingstable_stmt;
			sprintf(insert_query, "INSERT INTO bookings (room_type, room_no, name, contact_no, from_date, to_date) VALUES (%d, %d, \"%s\", \"%s\", %d, %d)", itype, st->room_no, gst->name.c_str(), gst->contact_num.c_str(), dates.first, dates.second);
			string query = insert_query;
			sqlite3_prepare(p_db, query.c_str(), query.size(), &insert_bookingstable_stmt, NULL);
			sqlite3_step(insert_bookingstable_stmt);
			sqlite3_finalize(insert_bookingstable_stmt);
			return sqlite3_last_insert_rowid(p_db);
		}
	}

	return -1;
}

void receptionist::cancel_booking(int bkng_no) {
	room *required_room;
	if(bookings.find(bkng_no) == bookings.end()) {
		return;
	}

	booking *bkng = bookings[bkng_no];

	if(bkng->type == room_type::R_STANDARD) {
		required_room = this->p_standard_room;
	} else if(bkng->type == room_type::R_DELUXE) {
		required_room = this->p_deluxe_room;
	} else if(bkng->type == room_type::R_SUITE) {
		required_room = this->p_suite_room;
	}

	required_room->mark_available(bkng->p_room, bkng->dates);
	this->bookings.erase(bkng_no);
	char delete_query[400];
	sqlite3_stmt *delete_bookingstable_stmt;
	sprintf(delete_query, "DELETE FROM bookings WHERE booking_no = %d", bkng_no);
	string query = delete_query;
	sqlite3_prepare(p_db, query.c_str(), query.size(), &delete_bookingstable_stmt, NULL);
	sqlite3_step(delete_bookingstable_stmt);
	sqlite3_finalize(delete_bookingstable_stmt);
	delete bkng;
}

void receptionist::display() {
	booking *bkng;

	for(map<int,booking *>::iterator i = bookings.begin(); i!=bookings.end(); i++)
	{
		*bkng = i->second[i->first];
		cout << "-------------------------------------------\n";
		cout << "\nGuest Name: " << bkng->p_guest->name;
		cout << "\nGuest contact number: " << bkng->p_guest->contact_num;
		cout << "\nRoom type: ";
		if(bkng->type == receptionist::room_type::R_STANDARD) {
			cout << "Standard Room";
		} else if(bkng->type == receptionist::room_type::R_DELUXE) {
			cout << "Deluxe Room";
		} else if(bkng->type == receptionist::room_type::R_SUITE) {
			cout << "Suite Room";
		}
		cout << "\nNumber of Days of Stay: " << (bkng->dates.second - bkng->dates.first);
		cout << "\nTotal Cost: " << bkng->get_cost();
	}
}
