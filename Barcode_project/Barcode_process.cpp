#include <iostream>
#include <sqlite3.h>
#include "cpr/cpr.h"
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include <nlohmann/json.hpp>

using json=nlohmann::json;
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
 //init_books();
 for (int i =0;i<=4;i++)
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
 Book webScraping(string ISBN)
 {
    cpr::Response response =cpr::Get(cpr::Url{"https://www.googleapis.com/books/v1/volumes?q=isbn:" + ISBN});
    Book currentBook;

    try
    { 
    cout<<response.text<<endl;
    json data=json::parse(response.text);

    if(!data.contains("items") || data["items"].empty())
    {
        cout<<"No book found"<<endl;
    }
    string title=data["items"][0]["volumeInfo"]["title"];

    std::string author = "Unknown";
    auto info = data["items"][0]["volumeInfo"];
    if (info.contains("authors") && info["authors"].is_array() && !info["authors"].empty() && !info["authors"][0].is_null())
    {
    author = info["authors"][0].get<std::string>();
    }
    
    std::string genre = "Unknown";
    if (info.contains("categories") && info["categories"].is_array() &&!info["categories"].empty())
    {
    genre = info["categories"][0].get<std::string>();
    }
    
    cout<<title<<endl;
    cout<<genre<<endl;
    cout<<author<<endl;

    currentBook.ISBN=ISBN;
    currentBook.Title=title;
    currentBook.Author_firstname=author; //this is just their whole name without separation
    currentBook.Genre=genre;
    
    cout<<currentBook.ISBN<<endl;
    cout<<currentBook.Title<<endl;
    cout<<currentBook.Author_firstname<<endl;
    cout<<currentBook.Genre<<endl;

    return currentBook;
    }
     catch(const std::exception& e)
    {
         cerr<<"JSON error"<<e.what()<<endl;
    }
  
 }
 void migrateToDB(sqlite3* db,string ISBN, string title, string author,string genre)
    {
        char* errMsg;
        int exit=0;
        Book db_book;
        string author_lastmix;
        string author_firstmix;
        enum nameState
        {
            FIRST,
            LAST
        };
        string temp;
        nameState current=LAST;
        sqlite3_stmt* stmt;

        exit=sqlite3_open("/Users/talentyolandandlovu/Cpp_projects/Cpp/Barcode_project/books.db", &db);
        db_book.ISBN=ISBN;
        db_book.Title=title;
        db_book.Genre=genre;
        size_t pos=author.find_last_of(' ');
        if(pos!=string::npos)
        {
            db_book.Author_firstname=author.substr(0,pos);
            db_book.Author_lastname=author.substr(pos+1);
        }
        else
        {
            db_book.Author_firstname=author;
            db_book.Author_lastname="";
        }
      
        cout<<db_book.Author_firstname<<endl;
        cout<<db_book.Author_lastname<<endl;

    string add_book="INSERT OR IGNORE INTO BOOKS VALUES(?,?,?,?,?);";
    sqlite3_prepare_v2(db,add_book.c_str(),-1,&stmt,NULL);
    sqlite3_bind_text(stmt,1,db_book.ISBN.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,2,db_book.Title.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,db_book.Author_firstname.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,4,db_book.Author_lastname.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,5,db_book.Genre.c_str(),-1,SQLITE_STATIC);

     if (sqlite3_exec(db, add_book.c_str(), 0, 0, &errMsg) != SQLITE_OK)
    {
        cerr << "Error adding book " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else
    {
        cout<<"Book added successfully!!"<<endl;
    }

    if(sqlite3_step(stmt)!=SQLITE_DONE)
    {
        cerr<<"Error inserting data"<<endl;
    }
    else
    {
        cout<<"Book added successfully"<<endl;
    }
    sqlite3_finalize(stmt);

   
    sqlite3_close(db);
}
    
};


// bool compareItems()
// {

// }



int main()
{
    sqlite3* db;
    int exit=0;
    char* messageError;
    Y_library currentbooks;
    string currentISBN="978-1-529-11339-6";
    //currentbooks.init_books();
    

    string sql="CREATE TABLE IF NOT EXISTS BOOKS("
    "ISBN TEXT PRIMARY KEY NOT NULL,"
    "TITLE TEXT NOT NULL,"
    "AUTHOR_FIRST_NAME TEXT NOT NULL,"
    "AUTHOR_LAST_NAME TEXT NOT NULL,"
    "GENRE TEXT NOT NULL);";

    exit=sqlite3_open("/Users/talentyolandandlovu/Cpp_projects/Cpp/Barcode_project/books.db", &db);
    exit=sqlite3_exec(db,sql.c_str(),NULL,0,&messageError);
    if(exit!=SQLITE_OK)
    {
        cerr<<"Error creating table"<<endl;//cerr allows the printing of a message to the console unbuffered
        sqlite3_free(messageError);
    }
    else{
        cout<<"Table created Successfully"<<endl;
    }

    // for (int i=0; i<=4;i++)
    // {
    // string books_enter="INSERT OR IGNORE INTO BOOKS VALUES('"
    // + currentbooks.booklist[i].ISBN + "','"
    // +currentbooks.booklist[i].Title +"','"
    // +currentbooks.booklist[i].Author_firstname+"','"
    // +currentbooks.booklist[i].Author_lastname+ "','"
    // +currentbooks.booklist[i].Genre+ "');";

    // char* errMsg;
    // if (sqlite3_exec(db, books_enter.c_str(), 0, 0, &errMsg) != SQLITE_OK)
    // {
    //     cerr << "Error adding book " << errMsg << endl;
    //     sqlite3_free(errMsg);
    // }
    // else{
    // cout<< currentbooks.booklist[i].ISBN<<endl;
    // cout<<currentbooks.booklist[i].Title<<endl;
    // cout<<currentbooks.booklist[i].Author_firstname<<endl;
    // cout<<currentbooks.booklist[i].Author_lastname<<endl;
    
    //     cout<<"Book added successfully"<<endl;
    // }

    //}
    Y_library::Book testing=currentbooks.webScraping(currentISBN);
    currentbooks.migrateToDB(db,testing.ISBN,testing.Title,testing.Author_firstname,testing.Genre);
     sqlite3_close(db);

   

    //Opening the database
    

}