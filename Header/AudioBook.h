#pragma once
#include "Book.h"
#include <string>

class AudioBook : public Book {
private:
    int duration_minutes;
    std::string narrator;

public:
    AudioBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, int duration, const std::string& narrator);

    int get_duration_minutes() const;
    std::string get_narrator() const;
    std::string duration_formatted() const;
    void print_info() const override;
};