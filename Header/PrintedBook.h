#pragma once
#include "Book.h"
#include <string>

class PrintedBook : public Book {
private:
    int page_count;

public:
    PrintedBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, int pages);

    int get_page_count() const;
    std::string get_type_name() const override;
    void print_info() const override;
    double get_specific_value() const override;
    double calculate_weight_grams() const;
};