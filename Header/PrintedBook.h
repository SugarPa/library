#pragma once
#include "Book.h"
#include <string>

class PrintedBook : public Book {
private:
    int page_count;

public:
    PrintedBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, int pages);

    int get_page_count() const;
    void print_info() const override;
};