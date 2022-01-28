#include <iostream>
#include <fstream>

using namespace std;

class Student {
  public:
    string username;
    string password;
    string first_name;
    string last_name;
    double avg = 0;
};

class Admin {
public:
    string username;
    string password;
};

class Lesson {
public:
    string name;
    int unit;
    double grade;
};

class FileManager {
  public:
    void write(const string& content, const string &fileName)
    {
        fstream fwrite;
        fwrite.open(fileName, ios::out | ios::app);
        // if (!fwrite.is_open()) {
        //     cout << "cannot open file!\n";
        // }
        fwrite << content << '\n';
        fwrite.close();
    }

    int get_number_of_lines(const string &fileName = "student_info.txt")
    {
        string line;
        int numLines = 0;
        fstream fpread;
        fpread.open(fileName, ios::in);
        // if (!fpread.is_open()) {
        //     cout << "cannot open file!\n";
        // }
        while (getline(fpread, line)) {
            numLines++;
        }
        fpread.close();
        return numLines;
    }

    string* read_lines(int &size, const string &fileName = "student_info.txt")
    {
        size = get_number_of_lines(fileName);
        string* lines = new string[size];
        fstream fread;
        fread.open(fileName, ios::in);
        // if (!fread.is_open()) {
        //     cout << "cannot open file!\n";
        // }
        for (int i = 0; i < size; i++) {
            getline(fread, lines[i]);
        }
        fread.close();
        return lines;
    }
};

string repeater(char c, int n = 30);
void menu();
void admin_login();
void admin_signup();
void login();
void signup();
bool is_user_authenticate(const string &, const string &);
bool is_user_exist(const string &);
string get_student_data(const string &);
int get_student_index(const string &);
string format(const Student &);
string format(const Lesson &);
string format(const Admin &);
string* split(const string &, char);
int count_char(const string &, char);
void admin_dashboard();
void student_dashboard(const string &);
void show_grades(const string &);
void list_students();
void add_lesson(const string &);
double average(const string &);

int main()
{
    ifstream admin_info;
    admin_info.open("admin_info.txt");
    if (!admin_info.is_open())
    {
        Admin admin;
        FileManager file;
        cout << "This is the first time you run this application\nPlease enter admin username: ";
        cin >> admin.username;
        cout << "Please enter admin password: ";
        cin >> admin.password;
        file.write(format(admin), "admin_info.txt");
        cout << repeater('\n', 60);
    }
    admin_info.close();
    menu();
    return 0;
}

// Repeat a single character for n times (default n = 30) 
string repeater(char c, int n)
{
    string result;
    for (int i = 0; i < n; i++) {
        result += c;
    }
    return result; 
}

void menu()
{
    string command;
    cout << "(!" << repeater('-') << "MENU" << repeater('-') << "!)\n";
    cout << "(1)  Log In (for admins)\n";
    cout << "(2)  Log In (for students)\n";
    cout << "(3)  Sign Up (for students)\n";
    cout << "(4)  exit the program\n";
    cout << "Please enter a number from above: ";
    cin >> command;
    cout << repeater('\n', 60);
    if (command == "1") {
        admin_login();
    }
    if (command == "2") {
        login();
    }
    if (command == "3") {
        signup();
    }
    if (command == "4") {
        // exit
        exit(0);
    }
    menu();
}

void admin_login()
{
    Admin admin_object;
    cout << "Please enter your username: ";
    cin >> admin_object.username;
    cout << "Please enter your password: ";
    cin >> admin_object.password;
    cout << repeater('\n', 60);
    FileManager fileManager_object;
    int number_of_lines;
    string* all_admin_data = fileManager_object.read_lines(number_of_lines, "admin_info.txt");
    for (int i = 0; i < number_of_lines; i++)
    {
        string* admin_data = split(all_admin_data[i], ':');
        if (admin_object.username == admin_data[0] && admin_object.password == admin_data[1]) {
            delete[] admin_data;
            delete[] all_admin_data;
            cout << "Successfully logged in!\n";
            admin_dashboard();
        }
        else {
            delete[] admin_data;
        }
    }
    cout << "Failed! Username or Password is wrong\n";
    delete[] all_admin_data;
}

void admin_signup()
{
    Admin admin_object;
    cout << "Please enter your username: ";
    cin >> admin_object.username;
    cout << "Please enter your password: ";
    cin >> admin_object.password;
    cout << repeater('\n', 60);
    FileManager fileManager_object;
    int number_of_lines;
    string* all_admin_data = fileManager_object.read_lines(number_of_lines, "admin_info.txt");
    for (int i = 0; i < number_of_lines; i++)
    {
        string* admin_data = split(all_admin_data[i], ':');
        if (admin_object.username == admin_data[0]) {
            delete[] admin_data;
            delete[] all_admin_data;
            cout << "Failed! Username already exist\n";
            admin_dashboard();
        }
        else {
            delete[] admin_data;
        }
    }
    FileManager file;
    file.write(format(admin_object), "admin_info.txt");
    cout << "Successfully Registered!\n";
    delete[] all_admin_data;
    admin_dashboard();
}

void login()
{
    Student student_object;
    cout << "Please enter your username: ";
    cin >> student_object.username;
    cout << "Please enter your password: ";
    cin >> student_object.password;
    cout << repeater('\n', 60);
    if (is_user_authenticate(student_object.username, student_object.password)) {
        cout << "Successfully logged in!\n";
        student_dashboard(get_student_data(student_object.username));
    }
    cout << "Failed! Username or Password is wrong\n";
}

void signup()
{
    Student student_object;
    // get user data
    cout << "Please enter a username: ";
    cin >> student_object.username;
    cout << "Please enter a password: ";
    cin >> student_object.password;
    cout << "Please enter your first name: ";
    cin >> student_object.first_name;
    cout << "Please enter your last name: ";
    cin >> student_object.last_name;
    cout << repeater('\n', 60);
    // Check if user already exist if yes it will not be created
    if (!is_user_exist(student_object.username)) {
        FileManager fileManager_object;
        fileManager_object.write(format(student_object), "student_info.txt");
        cout << "Successfully registered!\n";
    }
    else {
        cout << "Failed! Username already exist\n";
    }
}

bool is_user_authenticate(const string &username, const string &pass)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++)
    {
        string* student_data = split(all_student_data[i], '&');
        string* credentials = split(student_data[0], ':');
        if (username == credentials[0] && pass == credentials[1]) {
            delete[] student_data;
            delete[] credentials;
            delete[] all_student_data;
            return true;
        }
        delete[] student_data;
        delete[] credentials;
    }
    delete[] all_student_data;
    return false;
}

bool is_user_exist(const string &username)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++)
    {
        string* student_data = split(all_student_data[i], '&');
        string* credentials = split(student_data[0], ':');
        if (username == credentials[0]) {
            delete[] student_data;
            delete[] credentials;
            delete[] all_student_data;
            return true;
        }
        delete[] student_data;
        delete[] credentials;
    }
    delete[] all_student_data;
    return false;
}

string get_student_data(const string &username)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++)
    {
        string* student_data = split(all_student_data[i], '&');
        string* credentials = split(student_data[0], ':');
        if (username == credentials[0]) {
            string data = all_student_data[i];
            delete[] student_data;
            delete[] credentials;
            delete[] all_student_data;
            return data;
        }
        delete[] student_data;
        delete[] credentials;
    }
    delete[] all_student_data;
    return "Nothing Found";
}

int get_student_index(const string &username)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++)
    {
        string* student_data = split(all_student_data[i], '&');
        string* credentials = split(student_data[0], ':');
        if (username == credentials[0]) {
            delete[] student_data;
            delete[] credentials;
            delete[] all_student_data;
            return i;
        }
        delete[] student_data;
        delete[] credentials;
    }
    delete[] all_student_data;
    return -1;
}

string format(const Student &student_object)
{
    string result;
    result = student_object.username + ':' + student_object.password + ':' 
            + student_object.first_name + ':' + student_object.last_name + ':'
            + to_string(student_object.avg) + '&';
    return result;
}

string format(const Lesson &lesson_object)
{
    string result;
    result = lesson_object.name + ':' + to_string(lesson_object.unit) + ':' 
            + to_string(lesson_object.grade) + '&';
    return result;
}

string format(const Admin &admin_object)
{
    string result;
    result = admin_object.username + ':' + admin_object.password + ':';
    return result;
}

string* split(const string &line, char seperator)
{
    int counter = 0;
    int size = count_char(line, seperator);
    int length = line.length();
    string* result = new string[size + 1];
    for (int i = 0; i < length; i++) {
        if (line[i] != seperator) {
            result[counter] += line[i];
        }
        else {
            counter++;
        }
    }
    return result;   
}

int count_char(const string &line, char seperator)
{
    int counter = 0;
    int length = line.length();
    for (int i = 0; i < length; i++) {
        if (line[i] == seperator) {
            counter++;
        }
    }
    return counter;   
}

void admin_dashboard()
{
    string command;
    cout << "(!" << repeater('-') << "MENU" << repeater('-') << "!)\n";
    cout << "(1)  Show all Students\n";
    cout << "(2)  Add a new Admin\n";
    cout << "(3)  Add a new Student\n";
    cout << "(4)  Show student info\n";
    cout << "(5)  Add a lesson for a student\n";
    cout << "(6)  Log out\n";
    cout << "(7)  Exit\n";
    cout << "Please enter a number from above: ";
    cin >> command;
    cout << repeater('\n', 60);
    if (command == "1") {
        list_students();
    }
    if (command == "2") {
        admin_signup();
    }
    if (command == "3") {
        signup();
    }
    if (command == "4") {
        string username;
        cout << "Enter Student username: ";
        cin >> username;
        if (is_user_exist(username)) {
            show_grades(get_student_data(username));
        }
        else {
            cout << "Username Does not exist!\n";
        }
    }
    if (command == "5") {
        string username;
        cout << "Enter Student username: ";
        cin >> username;
        if (is_user_exist(username)) {
            add_lesson(username);
        }
        else {
            cout << "Username Does not exist!";
        }
    }
    if (command == "6") {
        menu();
    } 
    if (command == "7") {
        exit(0);
    } 
    admin_dashboard();
}

void student_dashboard(const string &student_data_line)
{
    string* student_data = split(student_data_line, '&');
    string* credentials = split(student_data[2], ':');
    string command;
    cout << "(!" << repeater('-') << "Welcome " << credentials[0] << repeater('-') << "!)\n"
         << "(1)  Show my lessons\n"
         << "(2)  Log out\n"
         << "(3)  Exit the program\n"
         << "Please enter a number from above: ";
    delete[] student_data;
    delete[] credentials;
    cin >> command;
    cout << repeater('\n', 60);
    if (command == "1") {
        show_grades(student_data_line);
    }
    if (command == "2") { 
        cout << "Logged out!\n";
        menu();
    }
    if (command == "3") { 
        exit(0);
    }
    student_dashboard(student_data_line);
}

void show_grades(const string &student_data_line)
{
    string input;
    string* splitted_data = split(student_data_line, '&');
    string* credentials = split(splitted_data[0], ':');
    int number_of_blocks = count_char(student_data_line, '&');
    cout << "Username: " << credentials[0] << 
            "\tPassword: " << credentials[1] <<
            "\tName: " << credentials[2] << 
            "\tSurname: " << credentials[3] <<
            "\tAverage: " << credentials[4] << '\n';
    for (int i = 1; i < number_of_blocks; i++) {
        string* lesson_info = split(splitted_data[i], ':');
        cout << "Lesson: " << lesson_info[0] << "\tUnits: " << lesson_info[1] << "\tGrade: " << lesson_info[2] << '\n';
        delete[] lesson_info;
    }
    delete[] credentials;
    delete[] splitted_data;
    cout << "Enter anything to return: ";
    cin >> input;
}

void list_students()
{
    string input;
    int size;
    FileManager file;
    string* all_student_data = file.read_lines(size);
    for (int i = 0; i < size; i++) {
        string* student_data = split(all_student_data[i], '&');
        string* credentials = split(student_data[0], ':');
        cout << credentials[0] << ' ' << credentials[1] << ' ' << credentials[2] << ' ' << credentials[3] << ' ' << credentials[4] << '\n';
        delete[] credentials;
        delete[] student_data;
    }
    delete[] all_student_data;
    cout << "Enter anything to return: ";
    cin >> input;
}

void add_lesson(const string &username)
{
    Lesson lesson_object;
    cout << "Enter lesson name: ";
    cin >> lesson_object.name;
    cout << "Enter lesson units: ";
    cin >> lesson_object.unit;
    cout << "Enter lesson grade: ";
    cin >> lesson_object.grade;
    string lesson_string = format(lesson_object);
    FileManager file;
    int size;
    int index = get_student_index(username);
    string* all_student_data = file.read_lines(size);
    for (int i = 0; i < index; i++) {
        file.write(all_student_data[i], "temp.txt");
    }
    Student student_object;
    all_student_data[index] += lesson_string;
    double avg = average(all_student_data[index]);
    int number_of_lessons = count_char(all_student_data[index], '&') - 1;
    string* splitted_data = split(all_student_data[index], '&');
    string* credentials = split(splitted_data[0], ':');
    student_object.username = credentials[0];
    student_object.password = credentials[1];
    student_object.first_name = credentials[2];
    student_object.last_name = credentials[3];
    student_object.avg = avg;
    all_student_data[index] = format(student_object);
    for (int i = 1; i <= number_of_lessons; i++) {
        all_student_data[index] += splitted_data[i] + '&';
    }
    for (int j = index; j < size; j++) {
        file.write(all_student_data[j], "temp.txt");
    }
    remove("student_info.txt");
    rename("temp.txt", "student_info.txt");
    cout << "Successfully added!\nDo you want to add another lesson (y or n): ";
    string input;
    cin >> input;
    if (input == "y") {
        add_lesson(username);
    }
}

double average(const string &student_data)
{
    double sum = 0;
    int all_units = 0;
    string* splitted_data = split(student_data, '&');
    int number_of_lessons = count_char(student_data, '&') - 1;
    for (int i = 1; i <= number_of_lessons; i++) {
        string* lesson_info = split(splitted_data[i], ':');
        int unit = stoi(lesson_info[1]);
        int grade = stod(lesson_info[2]);
        all_units += unit;
        sum += unit * grade;
        delete[] lesson_info;
    }
    delete[] splitted_data;
    return sum / all_units;
}
