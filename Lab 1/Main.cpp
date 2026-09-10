#include "LibraryManager.h"
#include <iostream>
#include <string>
#include <locale.h>

static void print_menu() {
    std::cout << "\nМеню:\n1. Добавить книгу\n2. Показать все книги\n3. Найти книгу по ID\n4. Найти книгу по названию\n5. Зарегистрировать читателя\n6. Показать всех читателей\n7. Найти читателя по номеру билета\n8. Выдать книгу читателю\n9. Вернуть книгу";
    std::cout << "\n10. Статистика библиотеки\n11. Очистить все данные\n12. Удалить книгу\n13 Изменить информацию о читателе\n0. Выход\n\nВыберите действие: ";
}

int main() {
 
    setlocale(LC_ALL, ".65001");
    LibraryManager library;
    int choice;
    library.add_book("Война и мир", "Русский вестник", 1869, 3);
    library.add_book("Преступление и наказание", "Русский вестник", 1866, 2);
    library.add_book("Мастер и Маргарита", "Художественная литература", 1967, 4);
    library.add_reader("Иван Петров", "B-001");
    library.add_reader("Мария Смирнова", "B-002");
    library.add_reader("Алексей Иванов", "B-003");

    do {
        print_menu();

        std::cin >> choice;

        switch (choice) {
        case 1: {
            library.add_book_from_console();
            break;
        }

        case 2: {
            library.show_all_books();
            break;
        }

        case 3: {
            int id;
            std::cout << "Введите ID книги: ";
            std::cin >> id;

            if (const Book* book = library.find_book_by_id(id); book) {
                std::cout << "\nНайдена книга:" << std::endl;
                book->print_info();
            }
            else {
                std::cout << "Книга с ID " << id << " не найдена!" << std::endl;
            }
            break;
        }

        case 4: {
            std::string title;
            std::cin.ignore();
            std::cout << "Введите название книги: ";
            std::getline(std::cin, title);

            if (const Book* book = library.find_book_by_title(title); book) {
                std::cout << "\nНайдена книга:" << std::endl;
                book->print_info();
            }
            else {
                std::cout << "Книга \"" << title << "\" не найдена!" << std::endl;
            }
            break;
        }

        case 5: {
            library.add_reader_from_console();
            break;
        }

        case 6: {
            library.show_all_readers();
            break;
        }

        case 7: {
            std::string card;
            std::cin.ignore();
            std::cout << "Введите номер читательского билета: ";
            std::getline(std::cin, card);

            if (const Reader* reader = library.find_reader_by_card(card); reader) {
                std::cout << "\nНайден читатель:" << std::endl;
                reader->print_info();
            }
            else {
                std::cout << "Читатель с билетом \"" << card << "\" не найден!" << std::endl;
            }
            break;
        }

        case 8: {
            std::string card;
            int book_id;
            std::cin.ignore();
            std::cout << "Введите номер читательского билета: ";
            std::getline(std::cin, card);
            std::cout << "Введите ID книги: ";
            std::cin >> book_id;

            library.borrow_book(card, book_id);
            break;
        }
        case 9: {
            std::string card;
            std::cin.ignore();
            std::cout << "Введите номер читательского билета: ";
            std::getline(std::cin, card);

            library.return_book(card);
            break;
        }

        case 10: {
            std::cout << "Всего книг: " << library.get_books_count() << std::endl;
            std::cout << "Всего читателей: " << library.get_readers_count() << std::endl;
            break;
        }

        case 11: {
                library.clear_all_data();
            break;
        }
        case 12: {
            int book_id;
            std::cout << "Введите id книги: ";
            std::cin >> book_id;
            library.remove_book(book_id);
            break;
        }
        case 13: {
            std::string card;
            std::cout << "Введите номер читательского билета: ";
            std::cin >> card;
            library.edit_reader(card);
            break;
        }
        case 0: {
            std::cout << "\nДо свидания! Спасибо за использование библиотеки!" << std::endl;
            break;
        }
        default: {
            std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
            break;
        }
        }
    } while (choice != 0);
    return 0;
}