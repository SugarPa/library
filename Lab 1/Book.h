#pragma once
#include <iostream>
#include <string>
#include <string_view>

class Book {
private:
	std::string title;
	std::string publisher;
	int id;
	int year;
	unsigned short copies;

public:
	Book(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies);

	std::string get_title() const;
	std::string get_publisher() const;
	int get_id() const;
	int get_year() const;
	unsigned short get_copies() const;

	void set_title(std::string_view new_title);      
	void set_publisher(std::string_view new_publisher);
	void set_id(int new_id);
	void set_year(int new_year);
	void set_copies(unsigned short new_copies);

	bool is_available() const;
	bool borrow();
	void return_copy();
	void print_info() const;
};