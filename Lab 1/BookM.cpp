#include "Book.h"

Book::Book(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies) :  publisher(publisher), title(title), id(id), year(year), copies(copies) {}

std::string Book::get_title() const {
	return title;
}

std::string Book::get_publisher() const {
	return publisher;
}

int Book::get_id() const {
	return id;
}

int Book::get_year() const {
	return year;
}

unsigned short Book::get_copies() const {
	return copies;
}

void Book::set_title(const std::string& new_title) {
	if (!new_title.empty()) {
		title = new_title;
	}
}

void Book::set_publisher(const std::string& new_publisher) {
	if (!new_publisher.empty()) {
		publisher = new_publisher;
	}
}

void Book::set_id(int new_id) {
	if (new_id>=0) {
		id = new_id;
	}
}

void Book::set_year(int new_year) {
	if (new_year>= 0) {
		year = new_year;
	}
}

void Book::set_copies(unsigned short new_copies) {
	copies = new_copies;
}

bool Book::is_available() const {
	return copies > 0;
}

bool Book::borrow() {
	if (copies > 0) {
		copies--;
		return true;
	}
	return false;
}

void Book::return_copy() {
	copies++;
}

void Book::print_info() const {
	std::cout << "ID: " << id
		<< ", Название: \"" << title << "\""
		<< ", Год: " << year
		<< ", Издательство: " << publisher
		<< ", Доступно: " << copies << " экз." << std::endl;
}