#include "LibraryManager.h"
#include <iostream>
#include <limits>
#include <algorithm>

LibraryManager::LibraryManager() : next_book_id(1), next_reader_id(1) {}
LibraryManager& LibraryManager::operator+=(const Book& book) {
    Book copy = book;
    if (copy.get_id() == 0) {
        copy.set_id(next_book_id++);
    }
    else if (copy.get_id() >= next_book_id) {
        next_book_id = copy.get_id() + 1;
    }
    books.push_back(copy);
    return *this;
}
LibraryManager& LibraryManager::operator+=(const Reader& reader) {
    readers.push_back(reader);
    return *this;
}
LibraryManager& LibraryManager::operator-=(int book_id) {
    auto it = std::find_if(books.begin(), books.end(), [book_id](const Book& b) {
        return b.get_id() == book_id;
        });

    if (it == books.end()) {
        std::cout << "Ошибка: книга с ID " << book_id << " не найдена!" << std::endl;
        return *this;
    }

    for (const auto& reader : readers) {
        if (reader.get_borrowed_book_id() == book_id && reader.get_has_book()) {
            std::cout << "Ошибка: книгу \"" << it->get_title()
                << "\" нельзя удалить — она на руках у читателя \""
                << reader.get_name() << "\"!" << std::endl;
            return *this;
        }
    }

    std::cout << "Книга \"" << it->get_title() << "\" (ID: " << book_id << ") удалена из библиотеки." << std::endl;
    books.erase(it);
    return *this;
}

void LibraryManager::add_book_from_console() {
    Book new_book;          
    std::cin >> new_book;   
    *this += new_book;     

    std::cout << "\nКнига \"" << new_book.get_title()
        << "\" успешно добавлена с ID: " << new_book.get_id() << "!\n";
}

void LibraryManager::add_reader_from_console() {
    Reader new_reader;
    std::cin >> new_reader;
    *this += new_reader;
    std::cout << "Читатель зарегистрирован!\n";
}

void LibraryManager::show_all_books() const {
    if (books.empty()) {
        std::cout << "В библиотеке нет книг." << std::endl;
        return;
    }
    std::cout << "Всего книг: " << books.size() << std::endl;
    for (const auto& book : books) {
        std::cout << book << std::endl;
    }
}

void LibraryManager::show_all_readers() const {
    if (readers.empty()) {
        std::cout << "Нет зарегистрированных читателей." << std::endl;
        return;
    }
    std::cout << "Всего читателей: " << readers.size() << std::endl;
    for (const auto& reader : readers) {
        std::cout << reader << std::endl;
    }
}

Book* LibraryManager::find_book_by_id(int id) {
    for (auto& book : books) {
        if (book.get_id() == id) return &book;
    }
    return nullptr;
}

Book* LibraryManager::find_book_by_title(std::string_view title) {
    for (auto& book : books) {
        if (book.get_title() == title) return &book;
    }
    return nullptr;
}

Reader* LibraryManager::find_reader_by_card(std::string_view library_card) {
    for (auto& reader : readers) {
        if (reader.get_library_card() == library_card) return &reader;
    }
    return nullptr;
}

const Reader* LibraryManager::find_reader_by_card(std::string_view library_card) const {
    for (const auto& reader : readers) {
        if (reader.get_library_card() == library_card) return &reader;
    }
    return nullptr;
}

const Reader* LibraryManager::find_reader_by_name(std::string_view name) const {
    for (const auto& reader : readers) {
        if (reader.get_name() == name) return &reader;
    }
    return nullptr;
}

bool LibraryManager::borrow_book(const std::string& library_card, int book_id) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader) {
        std::cout << "Ошибка: читатель с билетом \"" << library_card << "\" не найден!" << std::endl;
        return false;
    }
    if (reader->has_book_now()) {
        std::cout << "Ошибка: читатель \"" << reader->get_name() << "\" уже взял книгу!" << std::endl;
        return false;
    }

    Book* book = find_book_by_id(book_id);
    if (!book) {
        std::cout << "Ошибка: книга с ID " << book_id << " не найдена!" << std::endl;
        return false;
    }
    if (!book->is_available()) {
        std::cout << "Ошибка: книга \"" << book->get_title() << "\" недоступна!" << std::endl;
        return false;
    }

    if (book->borrow()) {
        reader->borrow_book(book_id);
        std::cout << "Книга \"" << book->get_title() << "\" выдана читателю \"" << reader->get_name() << "\"" << std::endl;
        return true;
    }
    return false;
}

bool LibraryManager::return_book(const std::string& library_card) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader) {
        std::cout << "Ошибка: читатель с билетом \"" << library_card << "\" не найден!" << std::endl;
        return false;
    }
    if (!reader->has_book_now()) {
        std::cout << "Ошибка: у читателя \"" << reader->get_name() << "\" нет книг на руках!" << std::endl;
        return false;
    }

    int book_id = reader->get_borrowed_book_id();
    Book* book = find_book_by_id(book_id);
    if (!book) {
        std::cout << "Ошибка: книга с ID " << book_id << " не найдена!" << std::endl;
        return false;
    }

    book->return_copy();
    reader->return_book();
    std::cout << "Книга \"" << book->get_title() << "\" возвращена читателем \"" << reader->get_name() << "\"" << std::endl;
    return true;
}

int LibraryManager::get_books_count() const { return static_cast<int>(books.size()); }
int LibraryManager::get_readers_count() const { return static_cast<int>(readers.size()); }

void LibraryManager::clear_all_data() {
    books.clear();
    readers.clear();
    next_book_id = 1;
    next_reader_id = 1;
    std::cout << "Все данные очищены!" << std::endl;
}

bool LibraryManager::edit_reader(const std::string& library_card) {
    Reader* reader = find_reader_by_card(library_card);
    if (!reader) {
        std::cout << "Ошибка: читатель с билетом \"" << library_card << "\" не найден!" << std::endl;
        return false;
    }

    std::cout << "\nДанные читателя:\n" << *reader << std::endl;
    std::cout << "\nЧто хотите изменить?\n1. Имя читателя\n2. Номер читательского билета\n0. Отмена\nВаш выбор: ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    switch (choice) {
    case 1: {
        std::string new_name;
        std::cout << "Введите новое имя: ";
        std::getline(std::cin >> std::ws, new_name);
        if (new_name.empty()) return false;
        reader->set_name(new_name);
        return true;
    }
    case 2: {
        std::string new_card;
        std::cout << "Введите новый номер билета: ";
        std::getline(std::cin >> std::ws, new_card);
        if (new_card.empty() || find_reader_by_card(new_card) != nullptr) return false;
        reader->set_library_card(new_card);
        return true;
    }
    case 0:
    default:
        return false;
    }
}

void LibraryManager::sort_books_by_year() {
    std::sort(books.begin(), books.end());
    std::cout << "Книги успешно отсортированы по году издания!\n";
}

bool LibraryManager::contains_book(const Book& book) const {
    return std::ranges::any_of(books, [&book](const Book& b) {
        return b == book;
        });
}

const Book* LibraryManager::get_oldest_book() const {
    if (books.empty()) return nullptr;

    auto it = std::min_element(books.begin(), books.end());
    return std::to_address(it);
}

void print_library_stats(const LibraryManager& manager) {
    int total_copies = 0;
    int readers_with_books = 0;

    for (const auto& book : manager.books) {
        total_copies += book.get_copies();
    }
    for (const auto& reader : manager.readers) {
        if (reader.has_book_now()) readers_with_books++;
    }

    std::cout << "\n Статистика библиотекм " << std::endl;
    std::cout << "Количество наименований книг: " << manager.books.size() << std::endl;
    std::cout << "Суммарно экземпляров на полках: " << total_copies << std::endl;
    std::cout << "Зарегистрировано читателей: " << manager.readers.size() << std::endl;
    std::cout << "Читателей с книгами на руках: " << readers_with_books << std::endl;
    std::cout << "Следующий ID для книги: " << manager.next_book_id << std::endl;
}