#pragma once
#include "Book.h"
#include <string>

class EBook : public Book {
private:
    double file_size_mb;
    std::string format;

public:
    EBook(int id, std::string title, std::string publisher, int year, unsigned short copies, double size, std::string fmt);

    double get_file_size_mb() const;
    std::string get_format() const;

    void print_info() const override;
};
