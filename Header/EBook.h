#pragma once
#include "Book.h"
#include <string>

class EBook : public Book {
private:
    double file_size_mb;
    std::string format;

public:
    EBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, double size, const std::string& fmt);

    double get_file_size_mb() const;
    std::string get_format() const;
    std::string get_type_name() const override;
    void print_info() const override;
    double get_specific_value() const override;
    double calculate_download_time(double speed_mbps) const;
};