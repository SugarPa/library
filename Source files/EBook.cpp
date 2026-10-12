#include "EBook.h"
#include <iostream>
#include <utility>

EBook::EBook(int id, std::string title, std::string publisher, int year, unsigned short copies, double size, std::string fmt)
    : Book(id, std::move(title), std::move(publisher), year, copies), file_size_mb(size), format(std::move(fmt)) {
}

double EBook::get_file_size_mb() const {
    return file_size_mb;
}

std::string EBook::get_format() const {
    return format;
}

void EBook::print_info() const {
    Book::print_info();
    std::cout << ", Тип: Электронная книга, Формат: " << format << ", Размер: " << file_size_mb << " MB";
}