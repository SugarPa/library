#include "LibraryManager.h"
#include <iostream>
#include <string>
#include <locale.h>
void run_menu();
int main() {
 
    setlocale(LC_ALL, ".65001");
    LibraryManager library;

    library.add_book("Peter Pen", "HarperCollins", 1911, 3);
    library.add_book("Winnie-the-Pooh", "Penguin Random House", 1866, 2);
    library.add_book("The Adventures of Sherlock Holmes", "Macmillan Publishers", 1926, 4);
    library.add_reader("Иван Петров", "B-001");
    library.add_reader("Мария Смирнова", "B-002");
    library.add_reader("Алексей Иванов", "B-003");

    run_menu();
    return 0;
}