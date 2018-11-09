#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>

using namespace std;

// Person class
class User{
    string name;
    int score;
    
public:
    User(string n, int s){
        name = n;
        score = s;
    }
    void setName(string s){
        name = s;
    }
    void addScore(int s){
        score += s;
    }
    int getScore(){
        return score;
    }
    string getName(){
        return name;
    }
};

// Book class
class Book{
    string name;
    string writer;
    string year;
    string owner;
    string holder;
    bool goodCondition;
    time_t returnDeadline;
    vector <string> history; // stores history of past users of a book
    
public:
    Book(string n, string a, string o, string y){
        name = n;
        writer = a;
        owner = o;
        year = y;
        goodCondition = true;
        
    }
    void setReturnDeadline(){
        returnDeadline = time(0) + 2592000; // 30 days after
    }
    time_t getReturnDeadline(){
        return returnDeadline;
    }
    void setHolder(string h){
        holder = h;
    }
    void setCondition(bool c){
        goodCondition = c;
    }
    void addToHistory(string p){
        history.push_back(p);
    }
    string getName(){
        return name;
    }
    string getWriter(){
        return writer;
    }
    string getOwner(){
        return owner;
    }
    string getHolder(){
        return holder;
    }
    string getYear(){
        return year;
    }
    vector <string> getHistory(){
        return history;
    }
    bool isGood(){
        return goodCondition;
    }
    bool isAvalaible(){
        return (holder == "");
    }
};


// To check if a person is a registered user
int searchPerson(string name, vector<User> users){
    for (int i = 0; i < users.size(); i++){
        if (name == users[i].getName()) return i;
    }
    return -1;
}

// To check if a book was registered
int searchBook(string name, string author, string year, vector<Book> books){
    for (int i = 0; i < books.size(); i++){
        if (name == books[i].getName() && author == books[i].getWriter() && year == books[i].getYear())
            return i;
    }
    return -1;
}

// To register a book
void registerBook(string registrant, string bookName, string author, string year, vector <User>& users, vector <Book>& books){
    Book tempBook(bookName, author, registrant, year);
    books.push_back(tempBook);
    
    int index = searchPerson(registrant, users);
    
    if (index != -1){// if person is already a registered user
        users[index].addScore(1);
    }
    else{
        User temp(registrant, 1);
        users.push_back(temp);
    }
}

// request to borrow a book
void requestBook(string borrower, string bookName, string author, string year, vector<User>& users, vector <Book>& books){
    int i = searchBook(bookName, author, year, books);
    if (i == -1)
        cout << bookName << " by " << author << " is not a registered book\n" << endl;
    else if (!books[i].isAvalaible()){
        cout << bookName << " by " << author << " published in " << year << " currently with " << books[i].getHolder() << endl;
        time_t deadline = books[i].getReturnDeadline();
        tm *timePtr = localtime(&deadline);
        cout << "Come back and check after the return deadline date which is " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday) <<
        "/" << (timePtr->tm_year)+1900 << "\n" << endl;
    }
    else{
        books[i].setHolder(borrower);
        string lender = books[i].getOwner();
        int j = searchPerson(lender,users);
        users[j].addScore(5);
        books[i].setReturnDeadline();
        time_t deadline = books[i].getReturnDeadline();
        tm *timePtr = localtime(&deadline);
        cout << "Return book by " << (timePtr->tm_mon)+1 << "/" << (timePtr->tm_mday) << "/" << (timePtr->tm_year)+1900 << endl;
        
        // register borrower if not registered
        int k = searchPerson(borrower,users);
        if (k == -1){
            User temp(borrower, 0);
            users.push_back(temp);
        }
    }
}

// return a book
void returnBook(string returner, string bookName, string author, string year, bool goodCondition, vector<User>& users, vector <Book>& books){
    int i = searchBook(bookName, author, year, books);
    books[i].setHolder("");
    
    books[i].addToHistory(returner);
    
    int j = searchPerson(returner, users);
    
    // condition of book
    if(goodCondition){
        users[j].addScore(2);
    }
    else{
        users[j].addScore(-10);
    }
    
    
    // deduction for missing deadline
    time_t now = time(0)/60/60/24;
    time_t deadline = books[i].getReturnDeadline()/60/60/24;
    if(now > deadline){
        users[j].addScore(-2 * (int)(now - deadline));
    }
}

// print previous users of a book
void printHistory(string bookName, string author, string year, vector<Book> books){
    int i = searchBook(bookName, author, year, books);
    if (i == -1)
        cout << bookName << " by " << author << " published in " << year << " is not a registered book. " << endl;
    else{
        vector <string> history = books[i].getHistory();
        if (history.empty())
            cout << bookName << " by " << author << " published in " << year << " has no previous user" << endl;
        else {
            for (int i = 0; i < history.size(); i++){
                cout << history[i] << endl;
            }
        }
    }
}

// print scores of registered users
void printScores(vector <User> users){
    for (int i = 0; i < users.size(); i++){
        cout << users[i].getName() << " ==> " << users[i].getScore() << endl;
    }
}

// search by title
vector <Book> searchBookByName(string title, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getName() == title){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}

// search by writer
vector <Book> searchBookByWriter(string writer, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getWriter() == writer){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}

// search by year
vector <Book> searchBookByYear(string year, vector<Book> listofBooks){
    vector<Book> list;
    for (int i = 0; i < listofBooks.size(); i++){
        if (listofBooks[i].isAvalaible() && listofBooks[i].getYear() == year){
            list.push_back(listofBooks[i]);
        }
    }
    return list;
}

int main(){
    
    vector<User> users;
    vector<Book> books;
    
    do{
        cout << "*******************************************************************************" << endl;
        cout << "\t\t\tPeer-to-Peer Book Lending Services\t\t\t" << endl;
        cout << "\n";
        cout << "1. Register a Book: " << endl;
        cout << "2. Request a Book: " << endl;
        cout << "3. Return a Book: " << endl;
        cout << "4. Check Scores: " << endl;
        cout << "5. Search: " << endl;
       
       
        cout << "Enter a number: " << endl;
        int num;
        cin >> num;
        cin.ignore();
       
       
        switch(num){
            case 1:{
                cout << "What is your Name: " << endl;
                string name;
                getline(cin, name);
                cout << "Enter the name of the Book: " << endl;
                string bookName;
                getline(cin, bookName);
                cout << "Enter the Author's name: " << endl;
                string author;
                getline(cin, author);
                cout <<  "Enter the year it was published: " << endl;
                string year;
                getline(cin, year);
                
                registerBook(name, bookName, author, year, users, books);
                
                break;
            }
            case 2:{
                string borrower;
                string bookName;
                string author;
                string year;
                
                cout<<"Who are you: " << endl;
                getline(cin,borrower);
                cout<<"What book are you requesting? " << endl;
                getline(cin,bookName);
                cout<<"By? " << endl;
                getline(cin,author);
                cout<<"What year was this book published? " << endl;
                getline(cin,year);
                
                requestBook(borrower, bookName, author, year, users, books);
                break;
            }
            case 3:{
                string returner;
                string bookName;
                string author;
                string year;
                char ans;
                bool goodCondition;
                
                cout<<"Who are you? \n";
                getline(cin,returner);
                cout<<"What book are you returning? \n";
                getline(cin, bookName);
                cout<<"By? \n";
                getline(cin, author);
                cout<<"What year was the book published? \n";
                getline(cin, year);
                cout<<"Is the book in good condition: \n\ty or n\n";
                cin >> ans;
                if (ans == 'y' || ans == 'Y')
                    goodCondition = 1;
                else
                    goodCondition = 0;
                
                returnBook(returner, bookName, author, year, goodCondition, users, books);
                
                break;
            }
            case 4:{
                printScores(users);
                break;
            }
        }
    }
    while (true);
    
}
