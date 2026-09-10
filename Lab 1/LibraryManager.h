#pragma once
#include <vector>
#include <string_view>
#include "Book.h"
#include "Reader.h"

class LibraryManager {
private:
	std::vector<Book> books;
	std::vector<Reader> readers;
	int next_book_id;
	int next_reader_id;

	Book input_book_from_console();
	Reader input_reader_from_console() const;

public:
	LibraryManager();
	//работа с книгами
	void add_book(const std::string& title, const std::string& publisher, int year, unsigned short copies);
	void add_book_from_console();
	void show_all_books() const;
	int get_books_count() const;
	bool remove_book(int book_id);
	bool borrow_book(const std::string& library_card, int book_id);
	bool return_book(const std::string& library_card);
	Book* find_book_by_id(int id);
	Book* find_book_by_title(std::string_view title);
	//работа с читателями
	void add_reader(const std::string& name, const std::string& library_card);
	void add_reader_from_console();
	bool edit_reader(const std::string& library_card);
	int get_readers_count() const;
	void show_all_readers() const;
	Reader* find_reader_by_card(std::string_view library_card);
	const Reader* find_reader_by_card(std::string_view library_card) const;
	const Reader* find_reader_by_name(std::string_view name) const;
	//доп метод по очистке
	void clear_all_data();
};