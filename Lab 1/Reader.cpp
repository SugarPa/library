#include "Reader.h"

Reader::Reader(const std::string& name, const std::string& library_card,int borrowed_book_id, bool has_book) : name(name), library_card(library_card), borrowed_book_id(borrowed_book_id), has_book(has_book) {}

std::string Reader::get_name() const {
	return name;
}

std::string Reader::get_library_card() const {
	return library_card;
}

int Reader::get_borrowed_book_id() const {
	return borrowed_book_id;
}

bool Reader::get_has_book() const {
	return has_book;
}

void Reader::set_name(const std::string& new_name) {
	if (!new_name.empty()) {
		name = new_name;
	}
}

void Reader::set_library_card(const std::string& new_library_card) {
	if (!new_library_card.empty()) {
		library_card = new_library_card;
	}
}

void Reader::set_borrowed_book_id(int new_id) {
	if (new_id>0) {
		borrowed_book_id=new_id;
		has_book = true;
	}
}

void Reader::set_has_book(bool status) {
	has_book = status;
	if (!status) {
		borrowed_book_id = 0;
	}
}

void Reader::borrow_book(int book_id) {
	if (!has_book && book_id > 0) {
		borrowed_book_id = book_id;
		has_book = true;
	}
}

bool Reader::return_book() {
	if (has_book) {
		has_book = false;
		borrowed_book_id = 0;
		return true;
	}
	return false;
}

bool Reader::has_book_now() const{
	return has_book;
}

void Reader::print_info() const {
	std::cout << "Имя " << name
		<< ",Билет: " << library_card;
	if (has_book) {
		std::cout << ",Книга на руках: " << borrowed_book_id<<std::endl;
	}

}