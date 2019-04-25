#include "BackendArchitecture.h"

// Main Function to test Backend Architecture
int main(){
    registerBook("Timothy Itodo", "Oliver Twist", "Charles Dickson", "2004");
    registerBook("Marcel Itodo", "Americana", "Chiamanda", "2010");
    registerBook("Sean Udeke", "Game of Thrones", "Briene Tarth", "2002");
    
    printUserScores();
    cout << endl;
    printBookHistory();
    
    cout << "\n\n-------------------------------------------------------------------------\n";
    requestbook("Sean Udeke", "Americana", "Chiamanda", "2010");
    requestbook("Peter Griffin", "Oliver Twist", "Charles Dickson", "2004");
    requestbook("Ser Bronn", "The Great Gabsy", "F. Scott", "200");
    
    printUserScores();
    cout << endl;
    printBookHistory();
    
    cout << "\n\n-------------------------------------------------------------------------\n";
    returnBook("Sean Udeke", "Americana", "Chiamanda", "2010", 1); // return in good condition
    returnBook("Peter Griffin", "Oliver Twist", "Charles Dickson", "2004", 0); // return in bad condition
    
    printUserScores();
    cout << endl;
    printBookHistory();
    
    cout << "\n\n-------------------------------------------------------------------------\n";
    requestbook("Jon Snow", "Oliver Twist", "Charles Dickson", "2004");
    returnBook("Jon Snow", "Oliver Twist", "Charles Dickson", "2004", 1);
    requestbook("Timothy Itodo", "Game of Thrones", "Briene Tarth", "2002");
    returnBook("Timothy Itodo", "Game of Thrones", "Briene Tarth", "2002", 0);
    
    printUserScores();
    cout << endl;
    printBookHistory();
    
    return 0;
}

