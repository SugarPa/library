#pragma once
#include <vector>
#include "Book.h"
#include "Reader.h"

class LibraryManager {
private:
	std::vector<Book> books;
	std::vector<Reader> readers;
	int next_book_id;
	int next_reader_id;

	Book input_book_from_console();
	Reader input_reader_from_console();

public:
	LibraryManager();

	void add_book(const std::string& title, const std::string& publisher, int year, unsigned short copies);
	void add_book_from_console();
	void show_all_books() const;
	bool remove_book(int book_id);

	Book* find_book_by_id(int id) const;
	Book* find_book_by_title(const std::string& title) const;

	void add_reader(const std::string& name, const std::string& library_card);
	void add_reader_from_console();
	bool edit_reader(const std::string& library_card);
	void show_all_readers() const;

	Reader* find_reader_by_card(const std::string& library_card) const;
	Reader* find_reader_by_name(const std::string& name) const;

	bool borrow_book(const std::string& library_card, int book_id);
	bool return_book(const std::string& library_card);

	int get_books_count() const;
	int get_readers_count() const;
	void clear_all_data();
};