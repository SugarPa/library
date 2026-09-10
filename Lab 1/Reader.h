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
    //геттеры
    std::string get_name() const;
    std::string get_library_card() const;
    int get_borrowed_book_id() const;
    //сеттеры
    void set_name(std::string_view new_name);
    void set_library_card(std::string_view new_library_card);
    void set_borrowed_book_id(int new_id);
    void set_has_book(bool status);
    void borrow_book(int book_id);
    //флаги
    bool get_has_book() const;
    bool return_book();
    bool has_book_now() const;
    //вывод информации
    void print_info() const;
};