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
string get_student_line(const string &);
int get_student_index(const string &);
string format(const Student &);
string format(const Lesson &);
string format(const Admin &);
int count_char(const string &, char);
void admin_dashboard();
void student_dashboard(const string &);
void show_grades(const string &);
void list_students();
void add_lesson(const string &);
double average(const string &);
Student get_student_info(const string &);
Lesson* get_student_lessons(const string &);
Admin get_admin_info(const string &);

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
        exit(0);
    }
    menu();
}

void admin_login()
{
    Admin admin_object_check;
    cout << "Please enter your username: ";
    cin >> admin_object_check.username;
    cout << "Please enter your password: ";
    cin >> admin_object_check.password;
    cout << repeater('\n', 60);
    FileManager fileManager_object;
    int number_of_lines;
    string* all_admin_data = fileManager_object.read_lines(number_of_lines, "admin_info.txt");
    for (int i = 0; i < number_of_lines; i++) {
        Admin admin_object = get_admin_info(all_admin_data[i]);
        if (admin_object_check.username == admin_object.username && admin_object_check.password == admin_object.password) {
            delete[] all_admin_data;
            cout << "Successfully logged in!\n";
            admin_dashboard();
        }
    }
    cout << "Failed! Username or Password is wrong\n";
    delete[] all_admin_data;
}

void admin_signup()
{
    Admin admin_object_check;
    cout << "Please enter your username: ";
    cin >> admin_object_check.username;
    cout << "Please enter your password: ";
    cin >> admin_object_check.password;
    cout << repeater('\n', 60);
    FileManager fileManager_object;
    int number_of_lines;
    string* all_admin_data = fileManager_object.read_lines(number_of_lines, "admin_info.txt");
    for (int i = 0; i < number_of_lines; i++) {
        Admin admin_object = get_admin_info(all_admin_data[i]);
        if (admin_object_check.username == admin_object.username) {
            delete[] all_admin_data;
            cout << "Failed! Username already exist\n";
            admin_dashboard();
        }
    }
    FileManager file;
    file.write(format(admin_object_check), "admin_info.txt");
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
        student_dashboard(get_student_line(student_object.username));
    }
    cout << "Failed! Username or Password is wrong\n";
}

void signup()
{
    Student student_object;
    cout << "Please enter a username: ";
    cin >> student_object.username;
    cout << "Please enter a password: ";
    cin >> student_object.password;
    cout << "Please enter your first name: ";
    cin >> student_object.first_name;
    cout << "Please enter your last name: ";
    cin >> student_object.last_name;
    cout << repeater('\n', 60);
    if (get_student_index(student_object.username) == -1) {
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
    for (int i = 0; i < number_of_lines; i++) {
        Student student_object = get_student_info(all_student_data[i]);
        if (username == student_object.username && pass == student_object.password) {
            delete[] all_student_data;
            return true;
        }
    }
    delete[] all_student_data;
    return false;
}

string get_student_line(const string &username)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++) {
        Student student_object = get_student_info(all_student_data[i]);
        if (username == student_object.username) {
            string data = all_student_data[i];
            delete[] all_student_data;
            return data;
        }
    }
    delete[] all_student_data;
    return "Nothing Found";
}

int get_student_index(const string &username)
{
    FileManager fileManager_object;
    int number_of_lines;
    string* all_student_data = fileManager_object.read_lines(number_of_lines);
    for (int i = 0; i < number_of_lines; i++) {
        Student student_object = get_student_info(all_student_data[i]);
        if (username == student_object.username) {
            delete[] all_student_data;
            return i;
        }
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

Student get_student_info(const string &line)
{
    Student student_object;
    int size = count_char(line, '&');
    string student_info_str;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] != '&') {
            student_info_str += line[i];
        }
        else {
            break;
        }
    }
    int length = student_info_str.length();
    string student_info_array[5];
    int count = 0;
    for (int j = 0; j < length; j++) {
        if (student_info_str[j] == ':') {
            count++;
        }
        else {
            student_info_array[count] += student_info_str[j];
        }
    }
    cout << student_info_array[0] << '\n';
    cout << student_info_array[4] << '\n';
    student_object.username = student_info_array[0];
    student_object.password = student_info_array[1];
    student_object.first_name = student_info_array[2];
    student_object.last_name = student_info_array[3];
    student_object.avg = stod(student_info_array[4]);
    return student_object;
}

Lesson* get_student_lessons(const string &line)
{
    int size = count_char(line, '&');
    Lesson* lessons_array = new Lesson[size - 1];
    int counter = 0;
    int length = line.length();
    string result[size];
    for (int i = 0; i < length; i++) {
        if (line[i] != '&') {
            result[counter] += line[i];
        }
        else {
            counter++;
        }
    }
    
    for (int j = 1; j < size; j++) {
        int lesson_length = result[j].length();
        string lesson_info_array[3];
        int count = 0;
        for (int i = 0; i < lesson_length; i++) {
            if (result[j][i] == ':') {
                count++;
            }
            else {
                lesson_info_array[count] += result[j][i];
            }
        }
        lessons_array[j - 1].name = lesson_info_array[0];
        lessons_array[j - 1].unit = stoi(lesson_info_array[1]);
        lessons_array[j - 1].grade = stod(lesson_info_array[2]);
    }
    return lessons_array;
}

Admin get_admin_info(const string &line)
{
    Admin admin_object;
    int length = line.length();
    string admin_info_array[2];
    int count = 0;
    for (int j = 0; j < length; j++) {
        if (line[j] == ':') {
            count++;
        }
        else {
            admin_info_array[count] += line[j];
        }
    }
    admin_object.username = admin_info_array[0];
    admin_object.password = admin_info_array[1];
    return admin_object;
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
        if (get_student_index(username) != -1) {
            show_grades(get_student_line(username));
        }
        else {
            cout << "Username Does not exist!\n";
        }
    }
    if (command == "5") {
        string username;
        cout << "Enter Student username: ";
        cin >> username;
        if (get_student_index(username) != -1) {
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
    Student student_object = get_student_info(student_data_line);
    string command;
    cout << "(!" << repeater('-') << "Welcome " << student_object.first_name << repeater('-') << "!)\n"
         << "(1)  Show my lessons\n"
         << "(2)  Log out\n"
         << "(3)  Exit the program\n"
         << "Please enter a number from above: ";
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
    Student student_object = get_student_info(student_data_line);
    Lesson* lessons_array = get_student_lessons(student_data_line);
    int number_of_lessons = count_char(student_data_line, '&') - 1;
    cout << "Username: " << student_object.username << 
            "\tPassword: " << student_object.password <<
            "\tName: " << student_object.first_name <<
            "\tSurname: " << student_object.last_name <<
            "\tAverage: " << student_object.avg << '\n';
    for (int i = 0; i < number_of_lessons; i++) {
        cout << "Lesson: " << lessons_array[i].name << "\tUnits: " << lessons_array[i].unit << "\tGrade: " << lessons_array[i].grade << '\n';
    }
    delete[] lessons_array;
    cout << "Enter anything to return: ";
    cin >> input;
}

void list_students()
{
    string input;
    int size;
    FileManager file;
    string* all_student_data = file.read_lines(size);
    cout << "Username\tPassword\tName\tFamily\tAverage\n";
    for (int i = 0; i < size; i++) {
        Student student_object = get_student_info(all_student_data[i]);
        cout << student_object.username << '\t' <<
                student_object.password << '\t' <<
                student_object.first_name << '\t' <<
                student_object.last_name << '\t' <<
                student_object.avg << '\n';
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
    
    all_student_data[index] += lesson_string;
    double avg = average(all_student_data[index]);
    int number_of_lessons = count_char(all_student_data[index], '&') - 1;
    Lesson* lesson_array = get_student_lessons(all_student_data[index]);
    Student student_object = get_student_info(all_student_data[index]);
    student_object.avg = avg;
    all_student_data[index] = format(student_object);
    for (int i = 0; i < number_of_lessons; i++) {
        string lesson_string = format(lesson_array[i]);
        all_student_data[index] += lesson_string;
    }
    delete[] lesson_array;
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
    // string* splitted_data = split(student_data, '&');
    Lesson* lesson_array = get_student_lessons(student_data);
    int number_of_lessons = count_char(student_data, '&') - 1;
    for (int i = 0; i < number_of_lessons; i++) {
        // string* lesson_info = split(splitted_data[i], ':');
        int unit = lesson_array[i].unit;
        int grade = lesson_array[i].grade;
        all_units += unit;
        sum += unit * grade;
    }
    delete[] lesson_array;
    return sum / all_units;
}
