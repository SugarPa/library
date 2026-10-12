#include "LibraryManager.h"
#include "PrintedBook.h"
#include "EBook.h"
#include "AudioBook.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <ctime>

LibraryManager::LibraryManager() : next_book_id(1), next_reader_id(1) {}

void LibraryManager::add_book(std::unique_ptr<Book> book) {
    if (book->get_id() == 0) {
        book->set_id(next_book_id++);
    }
    else if (book->get_id() >= next_book_id) {
        next_book_id = book->get_id() + 1;
    }
    books.push_back(std::move(book));
}

void LibraryManager::add_book_from_console() {
    std::cout << "\nВыберите тип книги:\n1. Печатная книга\n2. Электронная книга\n3. Аудиокнига\nВаш выбор: ";
    int type_choice;
    if (!(std::cin >> type_choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::string title;
    std::string publisher;
    int year = 0;
    unsigned short copies = 0;

    if (std::cin.peek() == '\n') std::cin.ignore();

    std::cout << "Введите название: ";
    std::getline(std::cin >> std::ws, title);

    std::cout << "Введите издательство: ";
    std::getline(std::cin, publisher);
    if (publisher.empty()) publisher = "Неизвестно";

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time;
    localtime_s(&local_time, &t);
    int current_year = local_time.tm_year + 1900;

    std::cout << "Введите год издания (0-" << current_year << "): ";
    while (!(std::cin >> year) || year < 0 || year > current_year) {
        std::cout << "Ошибка! Введите корректный год: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Введите количество экземпляров: ";
    while (!(std::cin >> copies)) {
        std::cout << "Ошибка! Введите корректное число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int current_id = next_book_id++;

    if (type_choice == 1) {
        int pages;
        std::cout << "Введите количество страниц: ";
        std::cin >> pages;
        add_book(std::make_unique<PrintedBook>(current_id, title, publisher, year, copies, pages));
    }
    else if (type_choice == 2) {
        double size;
        std::string format;
        std::cout << "Введите размер файла (MB): ";
        std::cin >> size;
        std::cout << "Введите формат (например, PDF, EPUB): ";
        std::getline(std::cin >> std::ws, format);
        add_book(std::make_unique<EBook>(current_id, title, publisher, year, copies, size, format));
    }
    else if (type_choice == 3) {
        int duration;
        std::string narrator;
        std::cout << "Введите длительность (минут): ";
        std::cin >> duration;
        std::cout << "Введите имя чтеца: ";
        std::getline(std::cin >> std::ws, narrator);
        add_book(std::make_unique<AudioBook>(current_id, title, publisher, year, copies, duration, narrator));
    }
    else {
        std::cout << "Неверный тип книги!" << std::endl;
    }
}

LibraryManager& LibraryManager::operator+=(std::unique_ptr<Book> book) {
    add_book(std::move(book));
    return *this;
}

LibraryManager& LibraryManager::operator+=(const Reader& reader) {
    readers.push_back(reader);
    return *this;
}

LibraryManager& LibraryManager::operator-=(const Book& book) {
    std::erase_if(books, [&book](const auto& ptr) {
        return ptr->get_id() == book.get_id();
        });
    std::cout << "Книга удалена из библиотеки." << std::endl;
    return *this;
}

void LibraryManager::show_all_books() const {
    if (books.empty()) {
        std::cout << "Библиотека пуста." << std::endl;
        return;
    }
    std::cout << "\n--- Список всех книг в библиотеке ---" << std::endl;
    for (const auto& book_ptr : books) {
        book_ptr->print_info();
        std::cout << (book_ptr->is_available() ? " [Доступна]" : " [Нет в наличии]") << std::endl;
    }
}

int LibraryManager::get_books_count() const {
    return static_cast<int>(books.size());
}

Book* LibraryManager::find_book_by_id(int id) {
    for (const auto& book_ptr : books) {
        if (book_ptr->get_id() == id) {
            return book_ptr.get();
        }
    }
    return nullptr;
}

Book* LibraryManager::find_book_by_title(std::string_view title) {
    for (const auto& book_ptr : books) {
        if (book_ptr->get_title() == title) {
            return book_ptr.get();
        }
    }
    return nullptr;
}

void LibraryManager::add_reader_from_console() {
    Reader r;
    std::cin >> r;
    readers.push_back(r);
    std::cout << "Читатель успешно добавлен!" << std::endl;
}

bool LibraryManager::edit_reader(const std::string& library_card) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader) {
        std::cout << "Ошибка: читатель не найден!" << std::endl;
        return false;
    }
    std::cout << "\n1. Изменить имя\n2. Изменить номер билета\nВыбор: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::string name;
        std::cout << "Новое имя: ";
        std::getline(std::cin >> std::ws, name);
        reader->set_name(name);
        return true;
    }
    else if (choice == 2) {
        std::string card;
        std::cout << "Новый билет: ";
        std::getline(std::cin >> std::ws, card);
        reader->set_library_card(card);
        return true;
    }
    return false;
}

int LibraryManager::get_readers_count() const {
    return static_cast<int>(readers.size());
}

void LibraryManager::show_all_readers() const {
    for (const auto& reader : readers) {
        std::cout << reader << std::endl;
    }
}

Reader* LibraryManager::find_reader_by_card(std::string_view library_card) {
    for (auto& r : readers) {
        if (r.get_library_card() == library_card) return &r;
    }
    return nullptr;
}

const Reader* LibraryManager::find_reader_by_card(std::string_view library_card) const {
    for (const auto& r : readers) {
        if (r.get_library_card() == library_card) return &r;
    }
    return nullptr;
}

const Reader* LibraryManager::find_reader_by_name(std::string_view name) const {
    for (const auto& r : readers) {
        if (r.get_name() == name) return &r;
    }
    return nullptr;
}

void LibraryManager::clear_all_data() {
    books.clear();
    readers.clear();
    next_book_id = 1;
    next_reader_id = 1;
    std::cout << "Все данные очищены!" << std::endl;
}

bool LibraryManager::is_book_borrowed(int book_id) const {
    return std::ranges::any_of(readers, [book_id](const Reader& reader) {
        return reader.get_borrowed_book_id() == book_id && reader.get_has_book();
        });
}

void LibraryManager::sort_books_by_year() {
    std::sort(books.begin(), books.end(), [](const auto& a, const auto& b) {
        return a->get_year() < b->get_year();
        });
}

bool LibraryManager::contains_book(const Book& book) const {
    return std::ranges::any_of(books, [&book](const auto& b) {
        return *b == book;
        });
}

const Book* LibraryManager::get_oldest_book() const {
    if (books.empty()) return nullptr;
    auto oldest = books.begin();
    for (auto it = books.begin(); it != books.end(); ++it) {
        if ((*it)->get_year() < (*oldest)->get_year()) {
            oldest = it;
        }
    }
    return oldest->get();
}

bool LibraryManager::borrow_book(const std::string& library_card, int book_id) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader) {
        std::cout << "Читатель не найден!" << std::endl;
        return false;
    }
    if (reader->has_book_now()) {
        std::cout << "У читателя уже есть книга на руках!" << std::endl;
        return false;
    }
    Book* book = find_book_by_id(book_id);
    if (!book) {
        std::cout << "Книга не найдена!" << std::endl;
        return false;
    }
    if (!book->is_available()) {
        std::cout << "Нет доступных экземпляров книги!" << std::endl;
        return false;
    }
    book->borrow();
    reader->borrow_book(book_id);
    std::cout << "Книга успешно выдана!" << std::endl;
    return true;
}

bool LibraryManager::return_book(const std::string& library_card) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader || !reader->has_book_now()) {
        std::cout << "У этого читателя нет книг на руках." << std::endl;
        return false;
    }
    int book_id = reader->get_borrowed_book_id();
    if (Book* book = find_book_by_id(book_id); book) {
        book->return_copy();
    }
    reader->return_book();
    std::cout << "Книга успешно возвращена в библиотеку!" << std::endl;
    return true;
}

void print_library_stats(const LibraryManager& manager) {
    int total_copies = 0;
    int readers_with_books = 0;

    for (const auto& book_ptr : manager.books) {
        total_copies += book_ptr->get_copies();
    }
    for (const auto& reader : manager.readers) {
        if (reader.has_book_now()) readers_with_books++;
    }

    std::cout << "\n--- Статистика библиотеки ---" << std::endl;
    std::cout << "Количество наименований книг: " << manager.books.size() << std::endl;
    std::cout << "Суммарно экземпляров на полках: " << total_copies << std::endl;
    std::cout << "Зарегистрировано читателей: " << manager.readers.size() << std::endl;
    std::cout << "Читателей с книгами на руках: " << readers_with_books << std::endl;
    std::cout << "Следующий ID для книги: " << manager.next_book_id << std::endl;
}