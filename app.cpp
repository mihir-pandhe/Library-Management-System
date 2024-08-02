#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Book
{
    string title;
    string author;
    string isbn;
    int publicationYear;
    string genre;
};

class Library
{
private:
    vector<Book> books;

    int findBookIndexByTitle(const string &title) const
    {
        for (size_t i = 0; i < books.size(); ++i)
        {
            if (books[i].title == title)
            {
                return i;
            }
        }
        return -1;
    }

public:
    void addBook(const string &title, const string &author, const string &isbn, int publicationYear, const string &genre)
    {
        Book newBook{title, author, isbn, publicationYear, genre};
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
            cout << "Title: " << book.title << ", Author: " << book.author
                 << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                 << ", Genre: " << book.genre << endl;
        }
    }

    void searchBook(const string &query) const
    {
        bool found = false;
        for (const auto &book : books)
        {
            if (book.title.find(query) != string::npos || book.author.find(query) != string::npos)
            {
                cout << "Title: " << book.title << ", Author: " << book.author
                     << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                     << ", Genre: " << book.genre << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No books found matching query." << endl;
        }
    }

    void deleteBook(const string &title)
    {
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            books.erase(books.begin() + index);
            cout << "Book deleted successfully." << endl;
        }
        else
        {
            cout << "No book found with the title: " << title << endl;
        }
    }
};

void showMenu()
{
    cout << "Library Management System" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. List Books" << endl;
    cout << "3. Search Book" << endl;
    cout << "4. Delete Book" << endl;
    cout << "5. Exit" << endl;
}

int main()
{
    Library library;
    int choice;
    string title, author, isbn, genre;
    int publicationYear;

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
            cout << "Enter book ISBN: ";
            getline(cin, isbn);
            cout << "Enter publication year: ";
            cin >> publicationYear;
            cin.ignore();
            cout << "Enter book genre: ";
            getline(cin, genre);
            library.addBook(title, author, isbn, publicationYear, genre);
            break;
        case 2:
            library.listBooks();
            break;
        case 3:
            cout << "Enter search query (title or author): ";
            getline(cin, title);
            library.searchBook(title);
            break;
        case 4:
            cout << "Enter title of the book to delete: ";
            getline(cin, title);
            library.deleteBook(title);
            break;
        case 5:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}
