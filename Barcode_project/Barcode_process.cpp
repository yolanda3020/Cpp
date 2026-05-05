#include <iostream>
#include <sqlite3.h>

using namespace std;

class Y_library
{
    public:
        struct Book
        {
        string ISBN;
        string Title;
        string Author_firstname;
        string Author_lastname;
        string Genre;
        };

        Book booklist[5];

    
bool lookUp_ISBN(const string ISBN) 
{
 init_books();
 for (int i =0;i<=5;i++)
 {
    if (ISBN==booklist[i].ISBN)
    {
        return 1; //found
    }
 }
 return 0; //not found
}

 void init_books()
 {
 booklist[0]={"987654321","The Hunger Games", "Suzanne","Collins","Dystopian"}; //for testing purposes, want to test everything else before the web scraping
 booklist[1]={"12345678","The Maze Runner", "James","Dashner","Dystopian"};
 booklist[2]={"314123678","The Great Gatsby","F. Scott","Fitzgerald","Classic"};
 booklist[3]={"19258456","Six of Crows","Leigh","Bardugo","Fantasy"};
 booklist[4]={"657483900","Project Hail Mary", "Andy","Weir","Sci-Fi"};

 }
};
// string migrateToDB()
// {

// }

// bool compareItems()
// {

// }



int main()
{
    sqlite3* db;
    int exit=0;
    char* messageError;
    Y_library currentbooks;
    currentbooks.init_books();

    string sql="CREATE TABLE IF NOT EXISTS BOOKS("
    "ISBN TEXT PRIMARY KEY NOT NULL,"
    "TITLE TEXT NOT NULL,"
    "AUTHOR_FIRST_NAME TEXT NOT NULL,"
    "AUTHOR_LAST_NAME TEXT NOT NULL,"
    "GENRE TEXT NOT NULL);";

    exit=sqlite3_open("/Users/talentyolandandlovu/Cpp projects/Cpp/Barcode_project/books.db", &db);
    exit=sqlite3_exec(db,sql.c_str(),NULL,0,&messageError);
    if(exit!=SQLITE_OK)
    {
        cerr<<"Error creating table"<<endl;//cerr allows the printing of a message to the console unbuffered
        sqlite3_free(messageError);
    }
    else{
        cout<<"Table created Successfully"<<endl;
    }

    for (int i=0; i<=4;i++)
    {
    string books_enter="INSERT OR IGNORE INTO BOOKS VALUES('"
    + currentbooks.booklist[i].ISBN + "','"
    +currentbooks.booklist[i].Title +"','"
    +currentbooks.booklist[i].Author_firstname+"','"
    +currentbooks.booklist[i].Author_lastname+ "','"
    +currentbooks.booklist[i].Genre+ "');";

    char* errMsg;
    if (sqlite3_exec(db, books_enter.c_str(), 0, 0, &errMsg) != SQLITE_OK)
    {
        cerr << "Error adding book " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else{
        cout<< currentbooks.booklist[i].ISBN<<endl;
    cout<<currentbooks.booklist[i].Title<<endl;
    cout<<currentbooks.booklist[i].Author_firstname<<endl;
    cout<<currentbooks.booklist[i].Author_lastname<<endl;

        cout<<"Book added successfully"<<endl;
    }

    }
     sqlite3_close(db);

   

    //Opening the database
    

}