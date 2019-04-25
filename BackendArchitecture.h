#ifndef BackendArchitecture_h
#define BackendArchitecture_h

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <unordered_map>
using namespace std;

/*
 * User Class
 * Lenders, borrowers, and registerants are all considered as users
 */
class User{
    string name;
    int score;
    
public:
    User(): name(""), score(0) {} ;
    User(string n){ name = n; }
    void setName(string s){ name = s; }
    void addScore(int s){ score += s; }
    int getScore(){ return score; }
    string getName(){ return name; }
};

/*
 * BookIdentifier Struct
 * A combination of the name, author and year is used to uniquely identify a book
 * BookIdentifier is used as key for registeredBooks hashmap
 */
struct BookIdentifier {
    string name;
    string author;
    string year;
    BookIdentifier(string n, string a, string y): name(n), author(a), year(y) {};
    bool operator==(const BookIdentifier& b) const {
        return name == b.name && author == b.author && year == b.year;
    }
};

/*
 * Custom hash function to enable BookIdentifier to be used as key in hash map
 */
class MyHashFunction {
public:
    size_t operator()(const BookIdentifier& b) const {
        size_t h1 = hash<string>()(b.name);
        size_t h2 = hash<string>()(b.author);
        size_t h3 = hash<string>()(b.year);
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

/*
 * Book Class
 * Includes current holders, the condition and return deadline dates
 */
class Book {
    string name;
    string author;
    string year;
    User owner;
    User holder;
    bool inGoodCondition;
    time_t returnDeadline;
    vector <User> history; //  stores the history of past users
public:
    Book(string n, string a, string y)
    : name(n), author(a), year(y), inGoodCondition(true) {};
    
    void setReturnDeadline() { returnDeadline = time(0) + 2592000; } // after 30 days
    void setHolder(User h) { holder = h; }
    void setCondition(bool c) { inGoodCondition = c; }
    void setOwner(User o) {owner = o; }
    void addToHistory(User u) { history.push_back(u); }
    
    time_t getReturnDeadline() { return returnDeadline; }
    string getName(){ return name; }
    string getAuthor() { return author; }
    User getOwner() { return owner; }
    User getHolder() { return holder; }
    string getYear() { return year; }
    vector <User> getHistory() { return history; }
    
    bool isGood() { return inGoodCondition; }
    bool isAvalaible(){ return (holder.getName() == ""); }
};

// Global Variables
unordered_map<string, User> registeredUsers;
unordered_map<BookIdentifier, Book, MyHashFunction> registeredBooks;


/*                    Funtion Definitions                        */

// Stores User object in registeredUsers with object's name as key
void registerUser(string name) {
    User newUser(name);
    registeredUsers[name] = newUser;
}

// Stores Book in registeredBooks hash map with a combo of its name, author and year as key
// Adds registrant's score by 1
void registerBook(string registrant, string bookName, string bookAuthor, string bookYear) {
    if (registeredUsers.find(registrant) == registeredUsers.end()) {
        registerUser(registrant);
    }
    BookIdentifier bookID(bookName, bookAuthor, bookYear);
    Book newBook(bookName, bookAuthor, bookYear);
    registeredUsers[registrant].addScore(1);
    newBook.setOwner(registeredUsers[registrant]); // set owner to registrant
    registeredBooks.insert({bookID, newBook});
}

// If book is registered and is available to lend:
//      Add's lender's score by 5
//      Set's return deadline to 30 days from request time
void requestbook(string borrower, string bookName, string bookAuthor, string bookYear) {
    BookIdentifier bookID(bookName, bookAuthor, bookYear);
    // Not registered
    if (registeredBooks.find(bookID) == registeredBooks.end()){
        cout << "Sorry, " << bookName << " by " << bookAuthor << " published in " << bookYear
        << "is not a registered book. \n\n";
        return;
    }
    
    // Book Not available
    if (!registeredBooks.at(bookID).isAvalaible()) {
        cout << "Sorry, " << bookName << " by " << bookAuthor << " published in " << bookYear
        << "is currently with another user " << endl;
        time_t deadline = registeredBooks.at(bookID).getReturnDeadline();
        tm *timePtr = localtime(&deadline);
        cout << "Expected Return Date: " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday)
        << "/" << (timePtr->tm_year)+1900 << "\n\n";
        return;
    }
    
    // Available to lend
    if (registeredUsers.find(borrower) == registeredUsers.end()) {
        registerUser(borrower);
    }
    registeredBooks.at(bookID).setHolder(registeredUsers[borrower]);
    User owner = registeredBooks.at(bookID).getOwner();
    registeredUsers[owner.getName()].addScore(5);
    registeredBooks.at(bookID).setReturnDeadline();
    time_t deadline = registeredBooks.at(bookID).getReturnDeadline();
    tm *timePtr = localtime(&deadline);
    cout << "Thank you " << borrower << " for using our lending services. \n"
    << "Please return by " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday)
    << "/" << (timePtr->tm_year)+1900 << "\n\n";
    
    return;
}

// Returns a Book by making book object available
// Adds/deducts score to borrower(returner) depending on the return date and the book condition
void returnBook(string borrower, string bookName, string bookAuthor, string bookYear, bool inGoodCondition) {
    BookIdentifier bookID(bookName, bookAuthor, bookYear);
    registeredUsers[borrower];
    User u = User(); // Empty User object
    registeredBooks.at(bookID).setHolder(u);
    registeredBooks.at(bookID).addToHistory(registeredUsers[borrower]);
    
    // Book Condition Score Addition/Deductions
    if (inGoodCondition) {
        registeredUsers[borrower].addScore(2);
    }
    else {
        registeredUsers[borrower].addScore(-10);
    }
    
    // Deadline deductions
    time_t now = time(0)/60/60/24;
    time_t deadline = registeredBooks.at(bookID).getReturnDeadline()/60/60/24;
    if(now > deadline){
        registeredUsers[borrower].addScore(-2 * (int)(now - deadline));
    }
}

/********* Search Services **********/
// Filter By Name
vector<Book> filterAvailableBooksbyName(string name) {
    vector<Book> result;
    for (auto x: registeredBooks){
        if (x.second.isAvalaible() && x.second.getName() == name)
            result.push_back(x.second);
    }
    return result;
}

// Filter By Author
vector<Book> filterAvailableBooksbyAuthor(string author) {
    vector<Book> result;
    for (auto x: registeredBooks){
        if (x.second.isAvalaible() && x.second.getAuthor() == author)
            result.push_back(x.second);
    }
    return result;
}

// Filter By Year
vector<Book> filterAvailableBooksbyYear(string year) {
    vector<Book> result;
    for (auto x: registeredBooks){
        if (x.second.isAvalaible() && x.second.getYear() == year)
            result.push_back(x.second);
    }
    return result;
}

void printUserScores() {
    cout << "User Scores: \n";
    for (auto x: registeredUsers) {
        cout << "\t" << x.second.getName() << " => " << x.second.getScore() << endl;
    }
}

void printBookHistory() {
    cout << "History of each Book: \n";
    for (auto x: registeredBooks){
        vector<User> history = x.second.getHistory();
        cout << x.second.getOwner().getName() << " " << x.second.getHolder().getName() << endl;
        cout << "\t" << x.second.getName() << " by " << x.second.getAuthor() << " published in " << x.second.getYear()
        << " history of borrowers: \n";
        if (history.size() == 0) {
            cout << "\t\tNo History\n";
        }
        else {
            for (int i = 0; i < history.size(); i++) {
                cout << "\t\t" << history[i].getName() << "\n";
            }
        }
    }
}

#endif /* BackendArchitecture_h */
