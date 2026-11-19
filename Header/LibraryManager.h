#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "Book.h"
#include "Reader.h"
#include "Collection.h"

class LibraryManager {
private:
    Collection<std::unique_ptr<Book>> books;
    Collection<Reader> readers;
    Collection<std::string> operation_log;
    int next_book_id;

    void log_operation(const std::string& entry);

public:
    LibraryManager();

    void add_book(std::unique_ptr<Book> book);
    void add_book_from_console();
    void show_all_books() const;
    void remove_book(const Book& book);

    bool borrow_book(const std::string& library_card, int book_id);
    bool return_book(const std::string& library_card);

    Book* find_book_by_id(int id);
    const Book* find_book_by_id(int id) const;
    Book* find_book_by_title(std::string_view title);

    std::vector<const Book*> find_books_from_year(int min_year) const;
    std::size_t restock_available_books(unsigned short add_copies);

    void add_reader_from_console();
    bool edit_reader(const std::string& library_card);
    void show_all_readers() const;
    std::vector<const Reader*> find_readers_with_books(bool with_book) const;
    std::vector<const Reader*> find_readers_by_name(std::string_view name) const;
    std::vector<const Reader*> find_readers_by_borrowed_book_id(int book_id) const;

    Reader* find_reader_by_card(std::string_view library_card);
    const Reader* find_reader_by_card(std::string_view library_card) const;

    void clear_all_data();
    void show_operation_log() const;

    LibraryManager& operator+=(std::unique_ptr<Book> book);
    LibraryManager& operator+=(const Reader& reader);
    LibraryManager& operator-=(const Book& book);

    bool is_book_borrowed(int book_id) const;
    void sort_books_by_year();
    const Book* get_oldest_book() const;
    void show_detailed_inf() const;

    friend void print_library_stats(const LibraryManager& manager);
};