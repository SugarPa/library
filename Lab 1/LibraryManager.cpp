#include "LibraryManager.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <string_view>

LibraryManager::LibraryManager() : next_book_id(1), next_reader_id(1) {}

Book LibraryManager::input_book_from_console() {
	std::string title;
	std::string publisher;
	int year;
	unsigned short copies;

	std::cout << "Введите данные книг" << std::endl;
	std::cout << "Название: ";
	std::getline(std::cin >> std::ws, title);
	while (title.empty()) {
		std::cout << "Название не может быть пустым!\nВведите название ещё раз: ";
		std::getline(std::cin, title);
	}

	std::cout << "Издательство: ";
	std::getline(std::cin, publisher);
	if (publisher.empty()) {
		publisher = "Неизвестно";
	}

	std::cout << "Год издания: ";
	while (!(std::cin >> year) || year < 0 || year > 2026) {
		std::cout << "Ошибка! Введите корректный год (0-2026): ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	std::cout << "Количество экземпляров: ";
	while (!(std::cin >> copies) || copies == 0) {
		std::cout << "Ошибка! Введите положительное число: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return Book(next_book_id++, title, publisher, year, copies);
}

Reader LibraryManager::input_reader_from_console() const {
	std::string name;
	std::string library_card;

	std::cout << "\nВведите данные читателя" << std::endl;
	std::cout << "Имя читателя: ";
	std::getline(std::cin >> std::ws, name);
	while (name.empty()) {
		std::cout << "Имя не может быть пустым! Введите заново: ";
		std::getline(std::cin, name);
	}

	std::cout << "Номер читательского билета: ";
	std::getline(std::cin, library_card);
	if (library_card.empty()) {
		library_card = "Не указан";
	}

	return Reader(name, library_card);
}

void LibraryManager::add_book(const std::string& title, const std::string& publisher, int year, unsigned short copies) {
	books.emplace_back(next_book_id++, title, publisher, year, copies);
}

void LibraryManager::add_book_from_console() {
	Book new_book = input_book_from_console();
	books.push_back(new_book);
	std::cout << "Книга \"" << new_book.get_title() << "\" добавлена! (ID: " << new_book.get_id() << ")" << std::endl;
}

void LibraryManager::show_all_books() const {
	if (books.empty()) {
		std::cout << "В библиотеке нет книг." << std::endl;
		return;
	}
	std::cout << "Всего книг: " << books.size() << std::endl;
	for (const auto& book : books) {
		book.print_info();
	}
}

Book* LibraryManager::find_book_by_id(int id) {
	for (auto& book : books) {
		if (book.get_id() == id) {
			return &book;
		}
	}
	return nullptr;
}

Book* LibraryManager::find_book_by_title(std::string_view title) {
	for (auto& book : books) {
		if (book.get_title() == title) {
			return &book;
		}
	}
	return nullptr;
}

void LibraryManager::add_reader(const std::string& name, const std::string& library_card) {
	readers.emplace_back(name, library_card);
}

void LibraryManager::add_reader_from_console() {
	Reader new_reader = input_reader_from_console();
	readers.push_back(new_reader);
	std::cout << "Читатель \"" << new_reader.get_name() << "\" зарегистрирован. (Билет: " << new_reader.get_library_card() << ")" << std::endl;
}

void LibraryManager::show_all_readers() const {
	if (readers.empty()) {
		std::cout << "Нет зарегистрированных читателей" << std::endl;
		return;
	}
	std::cout << "Всего читателей: " << readers.size() << std::endl;
	for (const auto& reader : readers) {
		reader.print_info();
		std::cout << std::endl;
	}
}

Reader* LibraryManager::find_reader_by_card(std::string_view library_card) {
	for (auto& reader : readers) {
		if (reader.get_library_card() == library_card) {
			return &reader;
		}
	}
	return nullptr;
}

const Reader* LibraryManager::find_reader_by_card(std::string_view library_card) const {
	for (const auto& reader : readers) {
		if (reader.get_library_card() == library_card) {
			return &reader;
		}
	}
	return nullptr;
}

const Reader* LibraryManager::find_reader_by_name(std::string_view name) const {
	for (const auto& reader : readers) {
		if (reader.get_name() == name) {
			return &reader;
		}
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
		std::cout << "Ошибка: читатель \"" << reader->get_name()
			<< "\" уже взял книгу! (ID: " << reader->get_borrowed_book_id() << ")" << std::endl;
		return false;
	}

	Book* book = find_book_by_id(book_id);
	if (!book) {
		std::cout << "Ошибка: книга с ID " << book_id << " не найдена!" << std::endl;
		return false;
	}
	if (!book->is_available()) {
		std::cout << "Ошибка: книга \"" << book->get_title()
			<< "\" недоступна!" << std::endl;
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
		std::cout << "Ошибка: у читателя \"" << reader->get_name()
			<< "\" нет книг на руках!" << std::endl;
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
	std::cout << "Книга \"" << book->get_title()
		<< "\" возвращена читателем \"" << reader->get_name() << "\"" << std::endl;
	return true;
}

int LibraryManager::get_books_count() const {
	return static_cast<int>(books.size());
}

int LibraryManager::get_readers_count() const {
	return static_cast<int>(readers.size());
}

void LibraryManager::clear_all_data() {
	books.clear();
	readers.clear();
	next_book_id = 1;
	next_reader_id = 1;
	std::cout << "Все данные очищены!" << std::endl;
}

bool LibraryManager::remove_book(int book_id) {
	auto it = std::find_if(books.begin(), books.end(),
		[book_id](const Book& book) {
			return book.get_id() == book_id;
		});

	if (it == books.end()) {
		std::cout << "Ошибка: книга с ID " << book_id << " не найдена!" << std::endl;
		return false;
	}
	for (const auto& reader : readers) {
		if (reader.get_borrowed_book_id() == book_id && reader.get_has_book()) {
			std::cout << "Ошибка: книгу \"" << it->get_title()
				<< "\" нельзя удалить — она на руках у читателя \""
				<< reader.get_name() << "\"!" << std::endl;
			return false;
		}
	}
	std::cout << "Книга \"" << it->get_title() << "\" (ID: " << book_id
		<< ") удалена из библиотеки." << std::endl;
	books.erase(it);
	return true;
}

bool LibraryManager::edit_reader(const std::string& library_card) {
	Reader* reader = find_reader_by_card(library_card);
	if (!reader) {
		std::cout << "Ошибка: читатель с билетом \"" << library_card << "\" не найден!" << std::endl;
		return false;
	}
	std::cout << "\nДанные читателя:" << std::endl;
	reader->print_info();
	std::cout << "\nЧто хотите изменить?\n1. Имя читателя\n2. Номер читательского билета\n0. Отмена" << std::endl;
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

		if (new_name.empty()) {
			std::cout << "Ошибка: имя не может быть пустым!" << std::endl;
			return false;
		}
		reader->set_name(new_name);
		return true;
	}

	case 2: {
		std::string new_card;
		std::cout << "Введите новый номер билета: ";
		std::getline(std::cin >> std::ws, new_card);
		if (new_card.empty()) {
			std::cout << "Ошибка: номер билета не может быть пустым!" << std::endl;
			return false;
		}
		if (find_reader_by_card(new_card) != nullptr) {
			std::cout << "Ошибка: билет уже занят другим читателем!" << std::endl;
			return false;
		}

		reader->set_library_card(new_card);
		return true;
	}
	case 0:
		return false;

	default:
		std::cout << "Неверный выбор!" << std::endl;
		return false;
	}
}