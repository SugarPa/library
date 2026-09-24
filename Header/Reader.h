#pragma once
#include <iostream>
#include <string>
#include <string_view>

class Reader {
private:
    std::string name;
    std::string library_card;
    int borrowed_book_id;
    bool has_book;

public:
    Reader() : borrowed_book_id(0), has_book(false) {}
    Reader(const std::string& name, const std::string& library_card, int borrowed_book_id = 0, bool has_book = false);

    std::string get_name() const;
    std::string get_library_card() const;
    int get_borrowed_book_id() const;
    bool get_has_book() const;

    void set_name(std::string_view new_name);
    void set_library_card(std::string_view new_library_card);
    void set_borrowed_book_id(int new_id);
    void set_has_book(bool status);

    void borrow_book(int book_id);
    bool return_book();
    bool has_book_now() const;

    bool operator==(const Reader& other) const { return library_card == other.library_card; }

    friend std::ostream& operator<<(std::ostream& os, const Reader& reader) {
        os << "Имя: " << reader.name << ", Билет: " << reader.library_card;
        if (reader.has_book) os << ", Книга на руках (ID): " << reader.borrowed_book_id;
        else os << ", Книг на руках нет";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Reader& reader) {
        if (is.peek() == '\n') {
            is.ignore();
        }
        std::cout << "Введите имя читателя: ";
        std::getline(is >> std::ws, reader.name);
        std::cout << "Введите номер читательского билета: ";
        std::getline(is, reader.library_card);

        return is;
    }
};