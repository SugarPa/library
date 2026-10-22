#include "Menu.h"
#include "LibraryManager.h"
#include "PrintedBook.h"
#include "EBook.h"
#include "AudioBook.h"
#include <iostream>
#include <limits>
#include <memory>

static void print_menu() {
    std::cout << "\nМеню:\n"
        << "1. Добавить книгу\n"
        << "2. Показать все книги\n"
        << "3. Найти книгу по ID\n"
        << "4. Найти книгу по названию\n"
        << "5. Зарегистрировать читателя\n"
        << "6. Показать всех читателей\n"
        << "7. Найти читателя по номеру билета\n"
        << "8. Выдать книгу читателю\n"
        << "9. Вернуть книгу\n"
        << "10. Статистика библиотеки\n"
        << "11. Очистить все данные\n"
        << "12. Удалить книгу\n"
        << "13. Изменить информацию о читателе\n"
        << "14. Отсортировать книги по году издания\n"
        << "15. Найти самую старую книгу\n"
        << "16. Рассчитать вес бумажной книги (PrintedBook)\n"
        << "17. Рассчитать время скачивания э-книги (EBook)\n"
        << "18. Показать отформатированную длительность аудиокниги (AudioBook)\n"
        << "0. Выход\n\nВыберите действие: ";
}

// Вспомогательная функция для специфичных действий с дочерними классами книг (пункты 16-18)
static void handle_specific_book_action(LibraryManager& library, int choice) {
    int id;
    std::cout << "Введите ID книги: ";
    std::cin >> id;

    // Исправлено S5350: используем const Book* чтобы удовлетворить анализатор
    const Book* book = library.find_book_by_id(id);
    if (!book) {
        std::cout << "Книга с ID " << id << " не найдена!" << std::endl;
        return;
    }

    if (choice == 16) {
        if (const auto* pBook = dynamic_cast<const PrintedBook*>(book); pBook) {
            std::cout << "Примерный вес книги \"" << pBook->get_title()
                << "\": " << pBook->calculate_weight_grams() << " г." << std::endl;
        }
        else {
            std::cout << "Ошибка: эта книга не является печатной!" << std::endl;
        }
    }
    else if (choice == 17) {
        if (const auto* eBook = dynamic_cast<const EBook*>(book); eBook) {
            double speed;
            std::cout << "Введите скорость скачивания (Мбит/с): ";
            std::cin >> speed;
            double time_sec = eBook->calculate_download_time(speed);
            std::cout << "Время скачивания книги \"" << eBook->get_title()
                << "\": " << time_sec << " сек." << std::endl;
        }
        else {
            std::cout << "Ошибка: эта книга не является электронной!" << std::endl;
        }
    }
    else if (choice == 18) {
        if (const auto* aBook = dynamic_cast<const AudioBook*>(book); aBook) {
            std::cout << "Длительность аудиокниги \"" << aBook->get_title()
                << "\": " << aBook->duration_formatted() << std::endl;
        }
        else {
            std::cout << "Ошибка: эта книга не является аудиокнигой!" << std::endl;
        }
    }
}

// Вспомогательная функция для работы с читателями (пункты 5-9, 13)
static void handle_reader_action(LibraryManager& library, int choice) {
    switch (choice) {
    case 5:
        library.add_reader_from_console();
        break;
    case 6:
        library.show_all_readers();
        break;
    case 7: {
        std::string card;
        std::cout << "Введите номер читательского билета: ";
        std::getline(std::cin >> std::ws, card);
        if (const Reader* reader = library.find_reader_by_card(card); reader) {
            std::cout << "\nНайден читатель:\n" << *reader << std::endl;
        }
        else {
            std::cout << "Читатель с билетом \"" << card << "\" не найден!" << std::endl;
        }
        break;
    }
    case 8: {
        std::string card;
        int book_id;
        std::cout << "Введите номер читательского билета: ";
        std::getline(std::cin >> std::ws, card);
        std::cout << "Введите ID книги: ";
        std::cin >> book_id;
        library.borrow_book(card, book_id);
        break;
    }
    case 9: {
        std::string card;
        std::cout << "Введите номер читательского билета: ";
        std::getline(std::cin >> std::ws, card);
        library.return_book(card);
        break;
    }
    case 13: {
        std::string card;
        std::cout << "Введите номер читательского билета: ";
        std::cin >> card;
        library.edit_reader(card);
        break;
    }
    default:
        break;
    }
}

void run_menu() {
    LibraryManager library;
    int choice;
    library += std::make_unique<PrintedBook>(1, "Peter Pan", "HarperCollins", 1911, 3, 200);
    library += std::make_unique<EBook>(2, "C++ Primer", "Addison-Wesley", 2013, 2, 5.4, "PDF");
    library += std::make_unique<AudioBook>(3, "The Hobbit", "HarperAudio", 1937, 4, 650, "Rob Inglis");

    library += Reader("Иван Петров", "B-001");
    library += Reader("Мария Смирнова", "B-002");
    library += Reader("Алексей Иванов", "B-003");

    do {
        print_menu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            library.add_book_from_console();
            break;
        case 2:
            library.show_all_books();
            break;
        case 3: {
            int id;
            std::cout << "Введите ID книги: ";
            std::cin >> id;
            if (const Book* book = library.find_book_by_id(id); book) {
                std::cout << "\nНайдена книга:\n";
                book->print_info();
                std::cout << std::endl;
            }
            else {
                std::cout << "Книга с ID " << id << " не найдена!" << std::endl;
            }
            break;
        }
        case 4: {
            std::string title;
            std::cout << "Введите название книги: ";
            std::getline(std::cin >> std::ws, title);
            if (const Book* book = library.find_book_by_title(title); book) {
                std::cout << "\nНайдена книга:\n";
                book->print_info();
                std::cout << std::endl;
            }
            else {
                std::cout << "Книга \"" << title << "\" не найдена!" << std::endl;
            }
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 13:
            handle_reader_action(library, choice);
            break;
        case 10:
            print_library_stats(library);
            break;
        case 11:
            library.clear_all_data();
            break;
        case 12: {
            int book_id;
            std::cout << "Введите ID книги для удаления: ";
            std::cin >> book_id;
            const Book* book_ptr = library.find_book_by_id(book_id);
            if (!book_ptr) {
                std::cout << "Ошибка: книга не найдена!" << std::endl;
                break;
            }
            if (library.is_book_borrowed(book_id)) {
                std::cout << "Книгу нельзя удалить, она на руках у читателя!" << std::endl;
                break;
            }
            library -= *book_ptr;
            break;
        }
        case 14:
            library.sort_books_by_year();
            std::cout << "Книги отсортированы по году издания." << std::endl;
            library.show_all_books();
            break;
        case 15: {
            if (const Book* oldest = library.get_oldest_book(); oldest != nullptr) {
                std::cout << "\nСамая старая книга:\n";
                oldest->print_info();
                std::cout << std::endl;
            }
            else {
                std::cout << "Библиотека пуста!" << std::endl;
            }
            break;
        }
        case 16:
        case 17:
        case 18:
            handle_specific_book_action(library, choice);
            break;
        case 0:
            std::cout << "Выход из программы." << std::endl;
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
            break;
        }
    } while (choice != 0);
}