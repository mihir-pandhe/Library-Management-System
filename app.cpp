#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

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
        return (year > 0 && year <= static_cast<int>(time(nullptr) / (60 * 60 * 24 * 365.25) + 1970));
    }

    bool isLoggedIn() const
    {
        return !currentUser.empty();
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
        if (query.empty())
        {
            cout << "Search query cannot be empty." << endl;
            return;
        }
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
        if (category.empty())
        {
            cout << "Category cannot be empty." << endl;
            return;
        }
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
        if (genre.empty())
        {
            cout << "Genre cannot be empty." << endl;
            return;
        }
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
                books[index].dueDate = time(nullptr) + 30 * 24 * 60 * 60; // 30 days from now
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
        cout << "Overdue Books Report:" << endl;
        bool overdueBooksFound = false;
        for (const auto &book : books)
        {
            if (book.borrowed && book.dueDate < time(nullptr))
            {
                cout << "Title: " << book.title << ", Author: " << book.author
                     << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                     << ", Genre: " << book.genre << ", Category: " << book.category
                     << ", Due Date: " << book.dueDate << endl;
                overdueBooksFound = true;
            }
        }
        if (!overdueBooksFound)
        {
            cout << "No overdue books found." << endl;
        }
    }

    void generateBooksByGenreReport() const
    {
        unordered_map<string, vector<Book>> genreMap;
        for (const auto &book : books)
        {
            genreMap[book.genre].push_back(book);
        }
        cout << "Books by Genre Report:" << endl;
        for (const auto &entry : genreMap)
        {
            cout << "Genre: " << entry.first << endl;
            for (const auto &book : entry.second)
            {
                cout << "Title: " << book.title << ", Author: " << book.author
                     << ", ISBN: " << book.isbn << ", Year: " << book.publicationYear
                     << ", Category: " << book.category
                     << ", Status: " << (book.borrowed ? "Borrowed" : (book.reserved ? "Reserved" : "Available"))
                     << (book.dueDate ? ", Due Date: " + to_string(book.dueDate) : "") << endl;
            }
        }
    }

    void loginUser(const string &username, const string &password)
    {
        if (users.find(username) != users.end() && users[username].password == password)
        {
            currentUser = username;
            cout << "Login successful. Welcome, " << users[username].fullName << "!" << endl;
        }
        else
        {
            cout << "Invalid username or password." << endl;
        }
    }

    void logoutUser()
    {
        if (!currentUser.empty())
        {
            currentUser.clear();
            cout << "Logged out successfully." << endl;
        }
        else
        {
            cout << "No user is logged in." << endl;
        }
    }

    void registerUser(const string &username, const string &password, const string &fullName, const string &email)
    {
        if (username.empty() || password.empty() || fullName.empty() || email.empty())
        {
            cout << "All fields are required." << endl;
            return;
        }
        if (users.find(username) == users.end())
        {
            User newUser{username, password, fullName, email};
            users[username] = newUser;
            cout << "User registered successfully." << endl;
        }
        else
        {
            cout << "Username already exists." << endl;
        }
    }

    void updateUserProfile(const string &fullName, const string &email)
    {
        if (!isLoggedIn())
        {
            cout << "Please login first." << endl;
            return;
        }
        if (fullName.empty() || email.empty())
        {
            cout << "Full name and email cannot be empty." << endl;
            return;
        }
        users[currentUser].fullName = fullName;
        users[currentUser].email = email;
        cout << "Profile updated successfully." << endl;
    }
};

int main()
{
    Library library;
    int choice;
    string title, author, isbn, genre, category, fullName, email, username, password;

    while (true)
    {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. List Books" << endl;
        cout << "3. Search Book" << endl;
        cout << "4. Search Book by Category" << endl;
        cout << "5. Search Book by Genre" << endl;
        cout << "6. Reserve Book" << endl;
        cout << "7. Cancel Reservation" << endl;
        cout << "8. Borrow Book" << endl;
        cout << "9. Return Book" << endl;
        cout << "10. Generate Overdue Report" << endl;
        cout << "11. Generate Books by Genre Report" << endl;
        cout << "12. Login" << endl;
        cout << "13. Logout" << endl;
        cout << "14. Update Profile" << endl;
        cout << "15. Register User" << endl;
        cout << "16. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            cout << "Enter publication year: ";
            int year;
            cin >> year;
            cin.ignore();
            cout << "Enter genre: ";
            getline(cin, genre);
            cout << "Enter category: ";
            getline(cin, category);
            library.addBook(title, author, isbn, year, genre, category);
            break;
        case 2:
            library.listBooks();
            break;
        case 3:
            cout << "Enter search query: ";
            getline(cin, title);
            library.searchBook(title);
            break;
        case 4:
            cout << "Enter category: ";
            getline(cin, category);
            library.searchBookByCategory(category);
            break;
        case 5:
            cout << "Enter genre: ";
            getline(cin, genre);
            library.searchBookByGenre(genre);
            break;
        case 6:
            cout << "Enter book title to reserve: ";
            getline(cin, title);
            library.reserveBook(title);
            break;
        case 7:
            cout << "Enter book title to cancel reservation: ";
            getline(cin, title);
            library.cancelReservation(title);
            break;
        case 8:
            cout << "Enter book title to borrow: ";
            getline(cin, title);
            library.borrowBook(title);
            break;
        case 9:
            cout << "Enter book title to return: ";
            getline(cin, title);
            library.returnBook(title);
            break;
        case 10:
            library.generateOverdueReport();
            break;
        case 11:
            library.generateBooksByGenreReport();
            break;
        case 12:
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            library.loginUser(username, password);
            break;
        case 13:
            library.logoutUser();
            break;
        case 14:
            cout << "Enter new full name: ";
            getline(cin, fullName);
            cout << "Enter new email: ";
            getline(cin, email);
            library.updateUserProfile(fullName, email);
            break;
        case 15:
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter full name: ";
            getline(cin, fullName);
            cout << "Enter email: ";
            getline(cin, email);
            library.registerUser(username, password, fullName, email);
            break;
        case 16:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
