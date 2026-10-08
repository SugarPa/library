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

public:

	LibraryManager();
	void add_book_from_console();
	void show_all_books() const;
	int get_books_count() const;
	bool borrow_book(const std::string& library_card, int book_id);
	bool return_book(const std::string& library_card);
	Book* find_book_by_id(int id);
	Book* find_book_by_title(std::string_view title);
	void add_reader_from_console();
	bool edit_reader(const std::string& library_card);
	int get_readers_count() const;
	void show_all_readers() const;
	Reader* find_reader_by_card(std::string_view library_card);
	const Reader* find_reader_by_card(std::string_view library_card) const;
	const Reader* find_reader_by_name(std::string_view name) const;
	void clear_all_data();
	LibraryManager& operator+=(const Book& book);
	LibraryManager& operator+=(const Reader& reader);
	LibraryManager& operator-=(int book_id);
	void sort_books_by_year();
	bool contains_book(const Book& book) const;
	const Book* get_oldest_book() const;
	friend void print_library_stats(const LibraryManager& manager);
	std::vector<Book> find_books_by_same_publisher(const Book& sample) const;
};