#include "AudioBook.h"
#include <iostream>

AudioBook::AudioBook(int id, const std::string& title, const std::string& publisher, int year, unsigned short copies, int duration, const std::string& narrator)
    : Book(id, title, publisher, year, copies), duration_minutes(duration), narrator(narrator) {
}

int AudioBook::get_duration_minutes() const {
    return duration_minutes;
}

std::string AudioBook::get_narrator() const {
    return narrator;
}

void AudioBook::print_info() const {
    Book::print_info();
    std::cout << ", Тип: Аудиокнига, Длительность: " << duration_minutes
        << " мин., Чтец: " << narrator;
}