#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

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

public:
    void addBook(const string &title, const string &author, const string &isbn, int publicationYear, const string &genre, const string &category)
    {
        Book newBook{title, author, isbn, publicationYear, genre, category, false};
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
                 << ", Genre: " << book.genre << ", Category: " << book.category
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
                     << ", Genre: " << book.genre << ", Category: " << book.category
                     << ", Status: " << (book.borrowed ? "Borrowed" : "Available") << endl;
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
                 << ", Genre: " << book.genre << ", Status: " << (book.borrowed ? "Borrowed" : "Available") << endl;
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

    void registerUser(const string &username, const string &password, const string &fullName, const string &email)
    {
        if (users.find(username) != users.end())
        {
            cout << "User already exists." << endl;
        }
        else
        {
            users[username] = User{username, password, fullName, email};
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

    void updateUserProfile(const string &fullName, const string &email)
    {
        if (users.find(currentUser) != users.end())
        {
            users[currentUser].fullName = fullName;
            users[currentUser].email = email;
            cout << "Profile updated successfully." << endl;
        }
        else
        {
            cout << "User not logged in." << endl;
        }
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
    cout << "6. Search Book by Category" << endl;
    cout << "7. Borrow Book" << endl;
    cout << "8. Return Book" << endl;
    cout << "9. Delete Book" << endl;
    cout << "10. Update Profile" << endl;
    cout << "11. Logout" << endl;
    cout << "12. Exit" << endl;
}

int main()
{
    Library library;
    int choice;
    string title, author, isbn, genre, category, username, password, fullName, email;
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
            cout << "Enter full name: ";
            getline(cin, fullName);
            cout << "Enter email: ";
            getline(cin, email);
            library.registerUser(username, password, fullName, email);
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
                cout << "Enter book category: ";
                getline(cin, category);
                library.addBook(title, author, isbn, publicationYear, genre, category);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 4:
            library.listBooks();
            break;
        case 5:
            cout << "Enter search query: ";
            getline(cin, title);
            library.searchBook(title);
            break;
        case 6:
            cout << "Enter category: ";
            getline(cin, category);
            library.searchBookByCategory(category);
            break;
        case 7:
            if (library.isLoggedIn())
            {
                cout << "Enter book title to borrow: ";
                getline(cin, title);
                library.borrowBook(title);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 8:
            if (library.isLoggedIn())
            {
                cout << "Enter book title to return: ";
                getline(cin, title);
                library.returnBook(title);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 9:
            if (library.isLoggedIn())
            {
                cout << "Enter book title to delete: ";
                getline(cin, title);
                library.deleteBook(title);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 10:
            if (library.isLoggedIn())
            {
                cout << "Enter new full name: ";
                getline(cin, fullName);
                cout << "Enter new email: ";
                getline(cin, email);
                library.updateUserProfile(fullName, email);
            }
            else
            {
                cout << "Please login first." << endl;
            }
            break;
        case 11:
            library.logoutUser();
            break;
        case 12:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
