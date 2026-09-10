#pragma once
#include <iostream>
#include <string>
class Reader {
	private:
	std::string name;
	std::string library_card;
	int borrowed_book_id;
	bool has_book;
public:
	Reader(const std::string& name, const std::string& library_card, int borrowed_book_id=0, bool has_book=false);
    std::string get_name() const;
    std::string get_library_card() const;
    int get_borrowed_book_id() const;
    bool get_has_book() const;
    void set_name(const std::string& new_name);
    void set_library_card(const std::string& new_library_card);
    void set_borrowed_book_id(int new_id);
    void set_has_book(bool status);
    void borrow_book(int book_id);
    bool return_book();
    bool has_book_now() const;
    void print_info() const;
};