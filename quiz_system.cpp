#include <iostream>
#include <string>
#include <mysql_connection.h>
#include <mysql/mysql.h>
#include <cstring>
#include <limits>
#include<stdlib.h>
using namespace std;
class Games
{
protected:
    MYSQL *conn;

public:
    Games()
    {

        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "cppuser", "Nishant@123", "quiz_system", 0, NULL, 0);

        // to be removed in final debug

        if (!conn)
        {
            cout << " connection is not established with the Database" << endl;
        }
        else
        {
            cout << "Connection Established" << endl;
        }
    }
    ~Games()
    {
        if (conn)
        {
            mysql_close(conn);
            cout << "Connection Closed" << endl;
        }
    }

public:
    void show_leaderBoard()
    {
        MYSQL_RES *res;
        MYSQL_ROW row;
        string query = "SELECT uid , u_name , score FROM user_data ORDER BY score DESC";
        if (mysql_query(conn, query.c_str()))
        {
            cout << "Failed To Fetch The user data :- " << endl;
            return;
        }
        else
        {
            cout << "User Data Fetched succesfully" << endl;
        }
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            cout << "USER ID :- " << row[0] << " USER NAME :- " << row[1] << " SCORE = " << row[2] << endl;
        }
        mysql_free_result(res);
    }
    void display_question(int id)
    {
        MYSQL_RES *res;
        MYSQL_ROW row;
        string querry = "SELECT id , question_name, options_text FROM questions WHERE id = " + to_string(id);
        if (mysql_query(conn, querry.c_str()))
        {
            cout << "Failed To Fetch The Quetions :- " << endl;
            return;
        }
        // else
        // {
        //     cout << "Questions Fetched Succesfully" << endl;
        // }
        res = mysql_store_result(conn);
        if(!res){
            cout << "Error caused at line 78" << mysql_error(conn) << endl;
        }
        // else {
        //     cout << "row fetched" << endl;
        // }
        while ((row = mysql_fetch_row(res)))
        {
            cout << "ID: " << row[0] << " | q_Name: " << row[1] << " | options: " << row[2] << endl;
        }
        mysql_free_result(res);
    }
    bool check_answer(char u_inp, int id)
    {
        string userAns(1, u_inp);
        string querry = "SELECT answer FROM questions WHERE id =" + to_string(id);
        if (mysql_query(conn, querry.c_str()))
        {
            return false;
        }
        MYSQL_RES *res = mysql_store_result(conn);
        if (!res)
            return false;
        MYSQL_ROW row = mysql_fetch_row(res);
        if (!row)
        {
            mysql_free_result(res);
            return false;
        }
        string correctAns = row[0];
        mysql_free_result(res);

        return (userAns == correctAns);
    }
    void displayRule()
    {
        cout << "You have Given 1 Questions And there Are 4 Output corrospond to them Select one Option If You win You will Gain 20 points" << endl;
        cout << "If You gives a Wrong answer 5 Points Will be Decrased" << endl;
        return;
    }
    int save_user_data(string name, string email, int score)
    {
        int uid = -1;
        string query = "INSERT INTO user_data (u_name, u_email, score) VALUES ('" + name + "', '" + email + "'," + to_string(score) + ")";
        // Can Be ReMoved Later
        if (mysql_query(conn, query.c_str()))
        {
            cout << "Data Not inserted Due To some Error" << mysql_error(conn) << endl;
            return uid;
        }
        else
        {
            cout << "Data Inserted SuccesFully " << endl;
        }
        uid = mysql_insert_id(conn);
        return uid;
    }
    void update_data(int uid, int score)
    {
        string query = "UPDATE user_data SET score = " + to_string(score) + " WHERE uid = " + to_string(uid);
        if (mysql_query(conn, query.c_str()))
        {
            // can be removed after some time
            cout << "Data not Updated" << endl;
            return;
        }
        else
        {
            cout << "Data Updated succesfully" << endl;
        }
    }
    int save_user_information()
    {
        string u_name;
        string u_email;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Your Name :- ";
        getline(cin, u_name);
        cout << "Ente Your Email :- ";
        getline(cin, u_email);
        int uid = save_user_data(u_name, u_email, 0); // uid of user is returned
        cout << "Your User id is :- " << uid << " Remember this For Future use" << endl;
        return uid;
    }

    void playGames()
    {
        cout << "Are You Sure You want to Play The Game (Enter 1) Once You Start Playing the Game You Can't Quit If YOU QUIT The score Will Be saved AND you wont be able to change it" << endl;
        int u_inp_c;
        cin >> u_inp_c;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (u_inp_c != 1)
        {
            return;
        }
        else
        {
            // fetch all question_id from database
            string query = "SELECT id FROM questions";
            if (mysql_query(conn, query.c_str()))
            {
                cout << "question id Not fetched succesfully :- " << mysql_error(conn) << endl;
            }
            // else
            // {
            //     cout << "Question id Fetched succesfully :- " << endl;
            // }
            MYSQL_RES *res = mysql_store_result(conn);
            vector<int> ids;
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)))
            {
                ids.push_back(atoi(row[0]));
            }
            mysql_free_result(res);

            int uid = save_user_information();
            if(uid == -1){
                cout << "Duplicate Entry's Are Not Allowed" << endl;
                return;
            }
            int score = 0;
            for (int id : ids)
            {
                display_question(id);
                cout << "Enter Your Prefered Output (a,b,c,d) :- ";
                char u_choice;
                cin >> u_choice;
                if (check_answer(u_choice, id) == true)
                {
                    score += 20;
                }
                else
                {
                    score -= 5;
                    if (score < 0)
                    {
                        score = 0;
                    }
                }
                update_data(uid, score);
                system("clear");
                cout << "YOUR SCORE :- " <<  score << endl;
            }
        }
    }
};
class Admin : public Games
{
    public :

    Admin()
    {
        cout << "Welcome To Admin's Panel :- " << endl;
    }
    // to insert question

    void insert_question(string q_name, string ans, string options)
    {
        string query = "INSERT INTO questions (question_name, answer, options_text) VALUES ('" + q_name + "', '" + ans + "', '" + options + "')";

        if (mysql_query(conn, query.c_str()))
        {
            cout << "Failed to insert question: " << mysql_error(conn) << endl;
        }
        else
        {
            cout << "Question inserted successfully!" << endl;
        }
    }

    // remove question with id_number 
    void remove_question(int q_id) {
        string query = "DELETE FROM questions WHERE id = " +to_string(q_id);
        if(mysql_query(conn , query.c_str())){
            cout << "Delete Unsuccesfull due to :- " << mysql_error(conn) << endl;
        }
        else {
            cout << "Question Deleted Succesfully" << endl;
        }
    }

    // remove user from database
     void remove_user(int uid) {
        string query = "DELETE FROM user_data WHERE uid = " + to_string(uid);
        if(mysql_query(conn , query.c_str())){
            cout << "Delete Unsuccesfull due to :- " << mysql_error(conn) << endl;
        }
        else {
            cout << "user  Deleted Succesfully" << endl;
        }
    }
     
    // show all questions 
    void show_questions(){
        string query = "SELECT question_name , options_text , answer FROM questions";
        if(mysql_query(conn , query.c_str())){
            cout << "question Not Fetched" << mysql_error(conn) << endl;
        }
        // else {
        //     cout << "question fetchde succesfull" << endl;
        // }
        MYSQL_RES *res = mysql_store_result(conn);
        if(!res) {
            cout << "Error caused " << mysql_error(conn) << endl;
            return;
        }
        MYSQL_ROW row;
        while((row =  mysql_fetch_row(res))) {
            cout << row[0] << endl << row[1] << endl << "ANS = " << row[2] << endl;
        }
        mysql_free_result(res);
    }
    // reset score of a user
    void reset_user(int uid) {
        string query = "UPDATE user_data SET score = 0 where uid = " +to_string(uid);
        if(mysql_query(conn,query.c_str())){
            cout << "Some error Caused " << mysql_error(conn) <<endl;
        }
        else {
            cout << "Update Succes" << mysql_affected_rows(conn) << endl;
        }
    }
    
};
// class user : public Games {

// };
int main() {
    Games game;       // Object for general game functionality
    Admin adminPanel; // Object for admin functionality
    const string password = "Nishant@123";

    while (true) {
        cout << "\n======= Welcome to Quiz Game =======" << endl;
        cout << "1. Play Game" << endl;
        cout << "2. Show Leaderboard" << endl;
        cout << "3. Admin Panel" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        system("clear");
        switch (choice) {
            case 1:
                int user_inp_1;
                cout << "1. Display Rule " << endl;
                cout << "2. Play Games " << endl;
                cout << "3. Exit.  " << endl;
                cin >> user_inp_1;
                system("claer");
                switch(user_inp_1) {
                    case 1 : 
                    game.displayRule();
                    int user_choice_to_play;
                    cout << "Enter 1 To Play The Game :- " ;
                    cin >> user_choice_to_play;
                    if(user_choice_to_play == 1){
                        game.playGames();
                    }
                    else {
                        cout << "You have Opted Out" << endl;
                        break;
                    }
                    case 2 :
                    game.playGames();
                }

                break;
            case 2:
                game.show_leaderBoard();
                break;
            case 3: {
                int adminChoice;
                cout << "Enter Password to acces the admins panel" << endl;
                string pass_inp;
                cin >> pass_inp;
                if(pass_inp == password){
                cout << "\n--- Admin Panel ---" << endl;
                cout << "1. Insert Question" << endl;
                cout << "2. Remove Question" << endl;
                cout << "3. Remove User" << endl;
                cout << "4. Show All Questions" << endl;
                cout << "5. Reset User Score" << endl;
                cout << "6. Back to Main Menu" << endl;
                cout << "Enter your choice: ";
                cin >> adminChoice;
                system("clear");
                }
                else {
                    cout << "Wrong password enterd :- " << endl;
                    break;
                }


                switch (adminChoice) {
                    case 1: {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        string q_name, ans, options;
                        cout << "Enter question text: ";
                        getline(cin, q_name);
                        cout << "Enter correct answer (a/b/c/d): ";
                        getline(cin, ans);
                        cout << "Enter options text: ";
                        getline(cin, options);
                        adminPanel.insert_question(q_name, ans, options);
                        break;
                    }
                    case 2: {
                        int q_id;
                        cout << "Enter Question ID to remove: ";
                        cin >> q_id;
                        adminPanel.remove_question(q_id);
                        break;
                    }
                    case 3: {
                        int uid;
                        cout << "Enter User ID to remove: ";
                        cin >> uid;
                        adminPanel.remove_user(uid);
                        break;
                    }
                    case 4:
                        adminPanel.show_questions();
                        break;
                    case 5: {
                        int uid;
                        cout << "Enter User ID to reset score: ";
                        cin >> uid;
                        adminPanel.reset_user(uid);
                        break;
                    }
                    case 6:
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
                break;
            }
            case 4:
                cout << "Thank you for playing! Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    }

    return 0;
}
