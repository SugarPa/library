#include "LibraryManager.h"
#include "PrintedBook.h"
#include "EBook.h"
#include "AudioBook.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <format>

LibraryManager::LibraryManager() : next_book_id(1) {}

void LibraryManager::log_operation(const std::string& entry) {
    operation_log.add(entry);
}

void LibraryManager::add_book(std::unique_ptr<Book> book) {
    if (!book) return;
    if (book->get_id() == 0) {
        book->set_id(next_book_id++);
    }
    else if (book->get_id() >= next_book_id) {
        next_book_id = book->get_id() + 1;
    }
    std::string title = book->get_title();
    int id = book->get_id();
    books.add(std::move(book));
    log_operation(std::format("Добавлена книга [ID={}] \"{}\"", id, title));
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
    std::tm local_time{};
#ifdef _WIN32
    localtime_s(&local_time, &t);
#else
    localtime_r(&t, &local_time);
#endif
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
    readers.add(reader);
    log_operation(std::format("Зарегистрирован читатель: {} ({})", reader.get_name(), reader.get_library_card()));
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

Book* LibraryManager::find_book_by_id(int id) {
    const auto* const found = books.get_by_key(id, [](const std::unique_ptr<Book>& b) {
        return b->get_id();
        });
    return found ? found->get() : nullptr;
}

const Book* LibraryManager::find_book_by_id(int id) const {
    const auto* const found = books.get_by_key(id, [](const std::unique_ptr<Book>& b) {
        return b->get_id();
        });
    return found ? found->get() : nullptr;
}

Book* LibraryManager::find_book_by_title(std::string_view title) {
    const auto* const found = books.find_if([title](const std::unique_ptr<Book>& b) {
        return b->get_title() == title;
        });
    return found ? found->get() : nullptr;
}

std::vector<const Book*> LibraryManager::find_books_from_year(int min_year) const {
    auto matches = find_all_by_criteria(books, [min_year](const std::unique_ptr<Book>& b) {
        return b && b->get_year() >= min_year;
        });

    std::vector<const Book*> result;
    result.reserve(matches.size());
    for (const auto* ptr : matches) {
        result.push_back(ptr->get());
    }
    return result;
}

std::size_t LibraryManager::restock_available_books(unsigned short add_copies) {
    std::size_t updated = process_by_criteria(
        books,
        [](const std::unique_ptr<Book>& b) { return b && b->is_available(); },
        [add_copies](const std::unique_ptr<Book>& b) {
            b->set_copies(static_cast<unsigned short>(b->get_copies() + add_copies));
        }
    );
    if (updated > 0) {
        log_operation(std::format("Пополнено книг в наличии: {} (+{} экз.)", updated, add_copies));
    }
    return updated;
}

void LibraryManager::add_reader_from_console() {
    Reader r;
    std::cin >> r;
    std::string info = std::format("{} ({})", r.get_name(), r.get_library_card());
    readers.add(std::move(r));
    log_operation(std::format("Добавлен читатель: {}", info));
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
        log_operation(std::format("Изменено имя читателя ({}) на \"{}\"", library_card, name));
        return true;
    }
    else if (choice == 2) {
        std::string card;
        std::cout << "Новый билет: ";
        std::getline(std::cin >> std::ws, card);
        reader->set_library_card(card);
        log_operation(std::format("Изменён номер билета читателя на \"{}\"", card));
        return true;
    }
    return false;
}

void LibraryManager::show_all_readers() const {
    readers.print(std::cout, "Зарегистрированные читатели");
}

std::vector<const Reader*> LibraryManager::find_readers_with_books(bool with_book) const {
    return find_all_by_criteria(readers, [with_book](const Reader& r) {
        return r.has_book_now() == with_book;
        });
}

std::vector<const Reader*> LibraryManager::find_readers_by_name(std::string_view name) const {
    return find_by_field(readers, [](const Reader& r) { return r.get_name(); }, std::string(name));
}

std::vector<const Reader*> LibraryManager::find_readers_by_borrowed_book_id(int book_id) const {
    return find_by_field(readers, [](const Reader& r) { return r.get_borrowed_book_id(); }, book_id);
}

Reader* LibraryManager::find_reader_by_card(std::string_view library_card) {
    return readers.get_by_key(library_card, [](const Reader& r) {
        return r.get_library_card();
        });
}

const Reader* LibraryManager::find_reader_by_card(std::string_view library_card) const {
    return readers.get_by_key(library_card, [](const Reader& r) {
        return r.get_library_card();
        });
}

void LibraryManager::clear_all_data() {
    books.clear();
    readers.clear();
    operation_log.clear();
    next_book_id = 1;
    std::cout << "Все данные очищены!" << std::endl;
}

void LibraryManager::show_operation_log() const {
    operation_log.print(std::cout, "Журнал операций библиотеки");
}

bool LibraryManager::is_book_borrowed(int book_id) const {
    return readers.contains_if([book_id](const Reader& reader) {
        return reader.get_borrowed_book_id() == book_id && reader.get_has_book();
        });
}

void LibraryManager::sort_books_by_year() {
    std::sort(books.begin(), books.end(), [](const auto& a, const auto& b) {
        return a->get_year() < b->get_year();
        });
    log_operation("Выполнена сортировка книг по году издания");
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
    log_operation(std::format("Выдана книга ID={} читателю {}", book_id, library_card));
    std::cout << "Книга успешно выдана!" << std::endl;
    return true;
}

void LibraryManager::remove_book(const Book& book) {
    int id = book.get_id();
    std::string title = book.get_title();
    books.remove_if([id](const auto& ptr) {
        return ptr->get_id() == id;
        });
    log_operation(std::format("Удалена книга [ID={}] \"{}\"", id, title));
    std::cout << "Книга удалена из библиотеки." << std::endl;
}

LibraryManager& LibraryManager::operator-=(const Book& book) {
    remove_book(book);
    return *this;
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
    log_operation(std::format("Возвращена книга ID={} от читателя {}", book_id, library_card));
    std::cout << "Книга успешно возвращена в библиотеку!" << std::endl;
    return true;
}

void print_library_stats(const LibraryManager& manager) {
    int total_copies = 0;
    for (const auto& book_ptr : manager.books) {
        total_copies += book_ptr->get_copies();
    }

    auto readers_with_books = find_all_by_criteria(manager.readers, [](const Reader& r) {
        return r.has_book_now();
        }).size();

    auto available_books = find_all_by_criteria(manager.books, [](const std::unique_ptr<Book>& b) {
        return b && b->is_available();
        }).size();

    std::cout << "\n--- Статистика библиотеки ---" << std::endl;
    std::cout << "Количество наименований книг: " << manager.books.size() << std::endl;
    std::cout << "Наименований в наличии: " << available_books << std::endl;
    std::cout << "Суммарно экземпляров на полках: " << total_copies << std::endl;
    std::cout << "Зарегистрировано читателей: " << manager.readers.size() << std::endl;
    std::cout << "Читателей с книгами на руках: " << readers_with_books << std::endl;
    std::cout << "Записей в журнале операций: " << manager.operation_log.size() << std::endl;
    std::cout << "Следующий ID для книги: " << manager.next_book_id << std::endl;
}

void LibraryManager::show_detailed_inf() const {
    if (books.empty()) {
        std::cout << "Библиотека пустая" << std::endl;
        return;
    }

    for (const auto& book_ptr : books) {
        const Book& book_ref = *book_ptr;

        std::cout << "\tИмя типа: " << book_ref.get_type_name() << std::endl;
        std::cout << "Информация:    ";
        book_ref.print_info();
        std::cout << std::endl;
        std::cout << "Специфическое значение: " << book_ref.get_specific_value()
            << " (для бумажной — вес в г., для эл. — размер в МБ, для аудио — мин.)" << std::endl;
    }
}