#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <algorithm>

using namespace std;

struct Book
{
    string title;
    string author;
    string isbn;
    int publicationYear;
    string genre;
    string category;
    bool borrowed;
    bool reserved;
    string reservedBy;
    time_t dueDate;
};

struct User
{
    string username;
    string password;
    string fullName;
    string email;
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

    vector<Book> findBooksByCategory(const string &category) const
    {
        vector<Book> result;
        for (const auto &book : books)
        {
            if (book.category == category)
            {
                result.push_back(book);
            }
        }
        return result;
    }

    vector<Book> findBooksByGenre(const string &genre) const
    {
        vector<Book> result;
        for (const auto &book : books)
        {
            if (book.genre == genre)
            {
                result.push_back(book);
            }
        }
        return result;
    }

    bool isValidYear(int year) const
    {
        time_t now = time(nullptr);
        tm *ltm = localtime(&now);
        int currentYear = 1900 + ltm->tm_year;
        return year > 0 && year <= currentYear;
    }

public:
    void addBook(const string &title, const string &author, const string &isbn, int publicationYear, const string &genre, const string &category)
    {
        if (title.empty() || author.empty() || isbn.empty() || genre.empty() || category.empty() || !isValidYear(publicationYear))
        {
            cout << "Invalid input. Please provide valid book details." << endl;
            return;
        }
        Book newBook{title, author, isbn, publicationYear, genre, category, false, false, "", 0};
        books.push_back(newBook);
        cout << "Book added successfully." << endl;
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
                 << ", Genre: " << book.genre << ", Category: " << book.category
                 << ", Status: " << (book.borrowed ? "Borrowed" : (book.reserved ? "Reserved" : "Available"))
                 << (book.dueDate ? ", Due Date: " + to_string(book.dueDate) : "") << endl;
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
                     << ", Genre: " << book.genre << ", Category: " << book.category
                     << ", Status: " << (book.borrowed ? "Borrowed" : (book.reserved ? "Reserved" : "Available"))
                     << (book.dueDate ? ", Due Date: " + to_string(book.dueDate) : "") << endl;
                found = true;
            }
        }
        if (!found)
        {
            cout << "No books found matching query." << endl;
        }
    }

    void searchBookByCategory(const string &category) const
    {
        vector<Book> booksInCategory = findBooksByCategory(category);
        if (booksInCategory.empty())
        {
            cout << "No books found in category: " << category << endl;
            return;
        }
        for (const auto &book : booksInCategory)
        {
            cout << "Title: " << book.title << ", Author: " << book.author
                 << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                 << ", Genre: " << book.genre << ", Status: " << (book.borrowed ? "Borrowed" : (book.reserved ? "Reserved" : "Available"))
                 << (book.dueDate ? ", Due Date: " + to_string(book.dueDate) : "") << endl;
        }
    }

    void searchBookByGenre(const string &genre) const
    {
        vector<Book> booksInGenre = findBooksByGenre(genre);
        if (booksInGenre.empty())
        {
            cout << "No books found in genre: " << genre << endl;
            return;
        }
        for (const auto &book : booksInGenre)
        {
            cout << "Title: " << book.title << ", Author: " << book.author
                 << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                 << ", Genre: " << book.genre << ", Status: " << (book.borrowed ? "Borrowed" : (book.reserved ? "Reserved" : "Available"))
                 << (book.dueDate ? ", Due Date: " + to_string(book.dueDate) : "") << endl;
        }
    }

    void reserveBook(const string &title)
    {
        if (!isLoggedIn())
        {
            cout << "Please login first." << endl;
            return;
        }
        if (title.empty())
        {
            cout << "Book title cannot be empty." << endl;
            return;
        }
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (!books[index].reserved && !books[index].borrowed)
            {
                books[index].reserved = true;
                books[index].reservedBy = currentUser;
                cout << "Book reserved successfully." << endl;
            }
            else
            {
                cout << "Book is already reserved or borrowed." << endl;
            }
        }
        else
        {
            cout << "No book found with the title: " << title << endl;
        }
    }

    void cancelReservation(const string &title)
    {
        if (!isLoggedIn())
        {
            cout << "Please login first." << endl;
            return;
        }
        if (title.empty())
        {
            cout << "Book title cannot be empty." << endl;
            return;
        }
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (books[index].reserved && books[index].reservedBy == currentUser)
            {
                books[index].reserved = false;
                books[index].reservedBy = "";
                cout << "Reservation canceled successfully." << endl;
            }
            else
            {
                cout << "Book is not reserved by you or not reserved at all." << endl;
            }
        }
        else
        {
            cout << "No book found with the title: " << title << endl;
        }
    }

    void borrowBook(const string &title)
    {
        if (!isLoggedIn())
        {
            cout << "Please login first." << endl;
            return;
        }
        if (title.empty())
        {
            cout << "Book title cannot be empty." << endl;
            return;
        }
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (!books[index].borrowed && !books[index].reserved)
            {
                books[index].borrowed = true;
                books[index].dueDate = time(nullptr) + 30 * 24 * 60 * 60;
                cout << "You have borrowed: " << title << endl;
            }
            else if (books[index].reserved)
            {
                cout << "Book is reserved by another user." << endl;
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
        if (!isLoggedIn())
        {
            cout << "Please login first." << endl;
            return;
        }
        if (title.empty())
        {
            cout << "Book title cannot be empty." << endl;
            return;
        }
        int index = findBookIndexByTitle(title);
        if (index != -1)
        {
            if (books[index].borrowed)
            {
                books[index].borrowed = false;
                books[index].dueDate = 0;
                cout << "You have returned: " << title << endl;
                if (books[index].reserved && books[index].reservedBy == currentUser)
                {
                    books[index].reserved = false;
                    books[index].reservedBy = "";
                    cout << "Reservation for this book has been canceled." << endl;
                }
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

    void generateOverdueReport() const
    {
        time_t now = time(nullptr);
        bool found = false;
        for (const auto &book : books)
        {
            if (book.borrowed && book.dueDate < now)
            {
                cout << "Overdue Book: " << book.title << ", Borrower: " << book.reservedBy << ", Due Date: " << ctime(&book.dueDate);
                found = true;
            }
        }
        if (!found)
        {
            cout << "No overdue books found." << endl;
        }
    }

    bool isLoggedIn() const
    {
        return !currentUser.empty();
    }

    void addUser(const string &username, const string &password, const string &fullName, const string &email)
    {
        if (username.empty() || password.empty() || fullName.empty() || email.empty())
        {
            cout << "Invalid input. Please provide valid user details." << endl;
            return;
        }
        if (users.find(username) != users.end())
        {
            cout << "Username already exists. Please choose a different username." << endl;
            return;
        }
        User newUser{username, password, fullName, email};
        users[username] = newUser;
        cout << "User added successfully." << endl;
    }

    bool loginUser(const string &username, const string &password)
    {
        if (username.empty() || password.empty())
        {
            cout << "Invalid input. Please provide valid credentials." << endl;
            return false;
        }
        auto it = users.find(username);
        if (it != users.end() && it->second.password == password)
        {
            currentUser = username;
            cout << "Login successful. Welcome " << it->second.fullName << "!" << endl;
            return true;
        }
        else
        {
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    void logoutUser()
    {
        if (!isLoggedIn())
        {
            cout << "No user is currently logged in." << endl;
            return;
        }
        currentUser.clear();
        cout << "Logout successful." << endl;
    }
};

int main()
{
    Library library;

    library.addUser("admin", "password", "Admin User", "admin@example.com");

    string command;
    while (true)
    {
        cout << "Enter command (addBook, listBooks, searchBook, searchBookByCategory, searchBookByGenre, reserveBook, cancelReservation, borrowBook, returnBook, overdueReport, addUser, login, logout, exit): ";
        cin >> command;

        if (command == "addBook")
        {
            string title, author, isbn, genre, category;
            int year;
            cout << "Enter title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            cout << "Enter publication year: ";
            cin >> year;
            cin.ignore();
            cout << "Enter genre: ";
            getline(cin, genre);
            cout << "Enter category: ";
            getline(cin, category);
            library.addBook(title, author, isbn, year, genre, category);
        }
        else if (command == "listBooks")
        {
            library.listBooks();
        }
        else if (command == "searchBook")
        {
            string query;
            cout << "Enter search query: ";
            cin.ignore();
            getline(cin, query);
            library.searchBook(query);
        }
        else if (command == "searchBookByCategory")
        {
            string category;
            cout << "Enter category: ";
            cin.ignore();
            getline(cin, category);
            library.searchBookByCategory(category);
        }
        else if (command == "searchBookByGenre")
        {
            string genre;
            cout << "Enter genre: ";
            cin.ignore();
            getline(cin, genre);
            library.searchBookByGenre(genre);
        }
        else if (command == "reserveBook")
        {
            string title;
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            library.reserveBook(title);
        }
        else if (command == "cancelReservation")
        {
            string title;
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            library.cancelReservation(title);
        }
        else if (command == "borrowBook")
        {
            string title;
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            library.borrowBook(title);
        }
        else if (command == "returnBook")
        {
            string title;
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            library.returnBook(title);
        }
        else if (command == "overdueReport")
        {
            library.generateOverdueReport();
        }
        else if (command == "addUser")
        {
            string username, password, fullName, email;
            cout << "Enter username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter full name: ";
            getline(cin, fullName);
            cout << "Enter email: ";
            getline(cin, email);
            library.addUser(username, password, fullName, email);
        }
        else if (command == "login")
        {
            string username, password;
            cout << "Enter username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            library.loginUser(username, password);
        }
        else if (command == "logout")
        {
            library.logoutUser();
        }
        else if (command == "exit")
        {
            break;
        }
        else
        {
            cout << "Invalid command. Please try again." << endl;
        }
    }

    return 0;
}
