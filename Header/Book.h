#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <limits>
#include <chrono>
#include <ctime>
#include <compare>

class Book {
private:
    std::string title;
    std::string publisher;
    int id;
    int year;
    unsigned short copies;

public:
    Book() : id(0), year(0), copies(0) {}

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

    bool operator==(const Book& other) const { return id == other.id; }
    auto operator<=>(const Book& other) const {
        if (auto cmp = year <=> other.year; cmp != 0) {
            return cmp;
        }
        return id <=> other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Book& book) {
        os << "ID: " << book.id
            << ", Название: \"" << book.title << "\""
            << ", Год: " << book.year
            << ", Издательство: " << book.publisher
            << ", Доступно: " << book.copies << " экз.";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Book& book) {
        if (is.peek() == '\n') {
            is.ignore();
        }

        std::cout << "\nВвод данных книги" << std::endl;

        std::cout << "Введите название: ";
        std::getline(is >> std::ws, book.title);
        while (book.title.empty()) {
            std::cout << "Название не может быть пустым! Повторите ввод: ";
            std::getline(is, book.title);
        }

        std::cout << "Введите издательство: ";
        std::getline(is, book.publisher);
        if (book.publisher.empty()) {
            book.publisher = "Неизвестно";
        }

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm local_time;
        localtime_s(&local_time, &t);
        int current_year = local_time.tm_year + 1900;

        std::cout << "Введите год издания (0-" << current_year << "): ";
        while (!(is >> book.year) || book.year < 0 || book.year > current_year) {
            std::cout << "Ошибка! Введите корректный год (0-" << current_year << "): ";
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Введите количество экземпляров: ";
        while (!(is >> book.copies)) {
            std::cout << "Ошибка! Введите корректное число: ";
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return is;
    }
};