#include "PrintedBook.h"
#include <iostream>

PrintedBook::PrintedBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, int pages)
    : Book(id, title, publisher, year, copies), page_count(pages) {
}

int PrintedBook::get_page_count() const {
    return page_count;
}

std::string PrintedBook::get_type_name() const {
    return "Печатная книга";
}

void PrintedBook::print_info() const {
    Book::print_info();
    std::cout << ", Тип: " << get_type_name() << ", Страниц: " << page_count;
}

double PrintedBook::get_specific_value() const {
    return calculate_weight_grams();
}

double PrintedBook::calculate_weight_grams() const {
    return page_count * 5.5;
}