#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Book
{
    string title;
    string author;
};

class Library
{
private:
    vector<Book> books;

public:
    void addBook(const string &title, const string &author)
    {
        Book newBook{title, author};
        books.push_back(newBook);
    }

    void listBooks() const
    {
        if (books.empty())
        {
            cout << "No books available." << endl;
            return;
        }
        for (const auto &book : books)
        {
            cout << "Title: " << book.title << ", Author: " << book.author << endl;
        }
    }
};

void showMenu()
{
    cout << "Library Management System" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. List Books" << endl;
    cout << "3. Exit" << endl;
}

int main()
{
    Library library;
    int choice;
    string title, author;

    while (true)
    {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter book author: ";
            getline(cin, author);
            library.addBook(title, author);
            break;
        case 2:
            library.listBooks();
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}
