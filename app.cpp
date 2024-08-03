#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Book
{
    string title;
    string author;
    string isbn;
    int publicationYear;
    string genre;
    bool borrowed;
};

struct User
{
    string username;
    string password;
};

class Library
{
private:
    vector<Book> books;
    unordered_map<string, User> users;
    string currentUser;

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
        Book newBook{title, author, isbn, publicationYear, genre, false};
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
                 << ", Genre: " << book.genre
                 << ", Status: " << (book.borrowed ? "Borrowed" : "Available") << endl;
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
                     << ", Genre: " << book.genre
                     << ", Status: " << (book.borrowed ? "Borrowed" : "Available") << endl;
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

    void borrowBook(const string &title)
    {
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (!books[index].borrowed)
            {
                books[index].borrowed = true;
                cout << "You have borrowed: " << title << endl;
            }
            else
            {
                cout << "Book is already borrowed." << endl;
            }
        }
        else
        {
            cout << "No book found with the title: " << title << endl;
        }
    }

    void returnBook(const string &title)
    {
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (books[index].borrowed)
            {
                books[index].borrowed = false;
                cout << "You have returned: " << title << endl;
            }
            else
            {
                cout << "Book was not borrowed." << endl;
            }
        }
        else
        {
            cout << "No book found with the title: " << title << endl;
        }
    }

    void registerUser(const string &username, const string &password)
    {
        if (users.find(username) != users.end())
        {
            cout << "User already exists." << endl;
        }
        else
        {
            users[username] = User{username, password};
            cout << "User registered successfully." << endl;
        }
    }

    bool loginUser(const string &username, const string &password)
    {
        auto it = users.find(username);
        if (it != users.end() && it->second.password == password)
        {
            currentUser = username;
            return true;
        }
        return false;
    }

    void logoutUser()
    {
        currentUser.clear();
        cout << "Logged out successfully." << endl;
    }

    bool isLoggedIn() const
    {
        return !currentUser.empty();
    }

    string getCurrentUser() const
    {
        return currentUser;
    }
};

void showMenu()
{
    cout << "Library Management System" << endl;
    cout << "1. Register User" << endl;
    cout << "2. Login" << endl;
    cout << "3. Add Book" << endl;
    cout << "4. List Books" << endl;
    cout << "5. Search Book" << endl;
    cout << "6. Borrow Book" << endl;
    cout << "7. Return Book" << endl;
    cout << "8. Delete Book" << endl;
    cout << "9. Logout" << endl;
    cout << "10. Exit" << endl;
}

int main()
{
    Library library;
    int choice;
    string title, author, isbn, genre, username, password;
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
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            library.registerUser(username, password);
            break;
        case 2:
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            if (library.loginUser(username, password))
            {
                cout << "Logged in successfully." << endl;
            }
            else
            {
                cout << "Invalid username or password." << endl;
            }
            break;
        case 3:
            if (library.isLoggedIn())
            {
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
            }
            else
            {
                cout << "Please log in first." << endl;
            }
            break;
        case 4:
            library.listBooks();
            break;
        case 5:
            cout << "Enter search query (title or author): ";
            getline(cin, title);
            library.searchBook(title);
            break;
        case 6:
            if (library.isLoggedIn())
            {
                cout << "Enter title of the book to borrow: ";
                getline(cin, title);
                library.borrowBook(title);
            }
            else
            {
                cout << "Please log in first." << endl;
            }
            break;
        case 7:
            if (library.isLoggedIn())
            {
                cout << "Enter title of the book to return: ";
                getline(cin, title);
                library.returnBook(title);
            }
            else
            {
                cout << "Please log in first." << endl;
            }
            break;
        case 8:
            if (library.isLoggedIn())
            {
                cout << "Enter title of the book to delete: ";
                getline(cin, title);
                library.deleteBook(title);
            }
            else
            {
                cout << "Please log in first." << endl;
            }
            break;
        case 9:
            library.logoutUser();
            break;
        case 10:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}
