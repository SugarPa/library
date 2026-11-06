#include "EBook.h"
#include <iostream>

EBook::EBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, double size, const std::string& fmt)
    : Book(id, title, publisher, year, copies), file_size_mb(size), format(fmt) {
}

double EBook::get_file_size_mb() const {
    return file_size_mb;
}

std::string EBook::get_format() const {
    return format;
}

std::string EBook::get_type_name() const {
    return "Электронная книга";
}

void EBook::print_info() const {
    Book::print_info();
    std::cout << ", Тип: " << get_type_name() << ", Формат: " << format << ", Размер: " << file_size_mb << " MB";
}

double EBook::get_specific_value() const {
    return file_size_mb;
}

double EBook::calculate_download_time(double speed_mbps) const {
    if (speed_mbps <= 0) return 0.0;
    return (file_size_mb * 8.0) / speed_mbps;
}