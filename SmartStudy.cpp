/*Basic flashcard app for Portuguese beginners 
Author :Talent Yolanda Ndlovu
Date: 13/09/2024*/
#include <iostream>
#include <fstream>
#include <stdlib.h>// used for memory allocation
#include <cctype> //used to check individual characters
#include<cstdlib>
using namespace std;


class Subjects{
    private:
        string question;
        string answer;
        int id;

    public: 
        string add(string q, string a){
            question=q;
            answer=a;
            id=rand()% 100;
            string line;
            line=question +"*"+answer+"|"+to_string(id);  //adding the identifier number of the flashcard, * is a separator between question and answer, | is separator between answer and id
            return line;
        }


    void revise(string readline)
    {
        string quest_view="";
        string correct_ans="";
        string user_answer="";
        bool same;
        int ans_start;
        enum Status 
        {
            QUESTION,
            ANSWER,
            ID
        };
        enum Status current=QUESTION;
        string flash_id="";

        cout<<"Question: ";
        for(int i=0;i<=readline.length()+1;i++)
        {
            while (current==QUESTION)
            {
                if (readline[i]!='*')
                {
                    quest_view=quest_view+readline[i];
                }
                else
                {
                    current=ANSWER;
                }
            }
            while (current==ANSWER)
            {
                if(readline[i]!='|')
                {
                    if (readline[i]!='*')
                    {
                        correct_ans=correct_ans+readline[i];
                    }
                }
                else
                {
                    current=ID;
                }
            }
            while (current==ID)
            {
                if(readline[i]!='\n')
                {
                    flash_id=flash_id+readline[i];
                }
            }
        }
        cout<<quest_view<<endl;

    //for compare() 0 is true and 1 is false
        same=user_answer.compare(correct_ans);

        if (same==0){
            cout<<"EUREKA, You've done it. Great job"<<endl;
        }
        else if(same==1){
        cout<<"Oh no, that is the wrong answer"<<endl;
            cout<<"The correct answer is : "<<correct_ans<<endl;
        }
    }
};


int main(){
    string q1,a1;
    string read;
    string file_to_open;

    int QuesNum;
    int id;
    
    enum Conversion
    {
        ENG_TO_POR,
        POR_TO_ENG
    };
    enum Conversion user_response1;
    
    char user_response;

    
    Subjects Science1;

    ofstream MyFile;
    ofstream MyFile2;
    ifstream MyReadFile;

    cout<<"Option 1: Questions are in Portuguese, enter the English translation"<<endl;
    cout<<"Option 2:Questions are in English, enter the Portuguese translation"<<endl;
    cout<<"Enter choice, 1 or 2"<<endl;

    cin>>user_response;
    try{
        if (user_response1==0){ // i just used the numbers instead of the actual names because i cba to write it so many times
            file_to_open="Port_to_Eng";
        
        }
        else if (user_response1==1){
            file_to_open="Eng_to_Port";

        }
        else
        {
            throw 505;
        }
    }
    catch(...)
    {
        cout<<"Enter valid response!!!"<<endl;
    }

    cout<<"Would you like to add any flashcards? Y or N"<<endl;
    cin>>user_response;

    
    if (user_response=='Y')
    {
         MyFile.open("/Users/talentyolandandlovu/Cpp projects/Cpp/"+file_to_open, ios::app);
        if (!MyFile.is_open())
        {
        cout<<"Error in creating file"<<endl;
        }
        else
        {
            cout<<"File created successfully"<<endl;
            cout<<"Time to add more flashcards!!\n"; 
            cout<<"How many flashcards would you like to add?"<<endl;
            cin>>QuesNum;
            system("clear");
            getline(cin,q1);
            for(int i=1; i<=QuesNum;i++)
            {
                cout<<"Input question\n"; 
                //in c++ as soon as a whitespace is encountered, our input is terminated therefore we must use getline
                getline(cin,q1);
                cout<<"Input answer"<<endl;
                getline(cin,a1);
                MyFile<<Science1.add(q1,a1)<<"\n";
                system("clear");
            }
        }
    }
    

    
    if(user_response=='Y')
    {
        cout<<"Time to add more flashcards!!\n"; 
        cout<<"How many flashcards would you like to add?"<<endl;
        cin>>QuesNum;
        system("clear");
        getline(cin,q1);
        for(int i=1; i<=QuesNum;i++)
        {
            cout<<"Input question\n"; 
            //in c++ as soon as a whitespace is encountered, our input is terminated therefore we must use getline
            getline(cin,q1);
            cout<<"Input answer"<<endl;
            getline(cin,a1);
            MyFile<<Science1.add(q1,a1)<<"\n";
            system("clear");
        }
    }
    MyFile.close();

    cout<<"Would you like to revise your flashcards? Y or N";
    cin>>user_response;

    MyReadFile.open("/Users/talentyolandandlovu/Cpp projects/Cpp/"+file_to_open);
    if (user_response=='Y')
    {
        if (!MyReadFile.is_open())
        {
            cout<<"Failed to open file"<<endl;
        }
        else
        {
            cout<<"File opened successfully"<<endl;
            cout<<"REVISION TIME!!"<<endl;
            system("clear");

            while(getline(MyReadFile,read))
            {
                Science1.revise(read);
            }
        }
    }
    else
    {
        cout<<"Until next time, byeeeee!";
    }
    MyReadFile.close();
}



