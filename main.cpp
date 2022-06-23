#include "head.h"
#include "ClassInfor.h"
#include "Student.h"

//变量定义与声明
//公共必修、学科必修、学科选修、专业必修 、专业选修、实践必修、实践选修
std::map<unsigned int, std::string> course_nature_list = {{1, "公共必修"},{2, "学科必修"},{3, "学科选修"},{4, "专业必修"},{5, "专业选修"},{6, "实践必修"},{7, "实践选修"}};
int COURSE_LIST_NUMBER=10001;
//0是英语，1是数学，2是计算机专业
std::vector<std::string> major_list = {"major"};
std::map<long long, ClassInformation *> total_class_list;
std::map<long long, Student *> total_student_list;
std::map<unsigned long long, int> in;

//函数声明
//课程
bool
new_class(const std::string &Course_name, unsigned int Course_nature, unsigned int Total_hours, unsigned int Credits,
          unsigned int Semester);

bool remove_class(const int &course_id);

void class_list_show();

void target_class_list_show(const int &course_id);

void who_choose_this_class(const unsigned int &course_id);

long long how_many_student_choose_this_class(const unsigned int &course_id);

long long how_many_class();

//学生
bool new_student(const long long &student_id, const std::string &student_name, bool gender, unsigned int ages,
                 unsigned int major, unsigned int classroom_id, long long phone_number);

bool remove_student(const long long &student_id);

void student_list_show();

void target_student_show(const long long &student_id);

bool student_choose_class(const long long &student_id, const int &course_id);

bool student_remove_class(const long long &student_id, const int &course_id);

void student_change_information(const long long &student_id);

long long how_many_student();

//专业操作
unsigned int new_major(const std::string &major_name);

void major_list_show();

//交互功能
void welcome();
void dropInput();

//函数操作
//课程

bool remove_class(const int &course_id) {
    auto cour = total_class_list.find(course_id);
    if (cour != total_class_list.end()) {
        auto course = cour->second;
        for (const auto &student_id_name: course->choose_student_list) {
            auto student = total_student_list.find(student_id_name.first);
            student->second->remove_class(course_id);
        }
        total_class_list.erase(course_id);
        printf("remove %d success\n", course_id);
        return true;
    } else {
        printf("remove %d fail\n", course_id);
        return false;
    }
}

bool
new_class(const std::string &Course_name, unsigned int Course_nature, unsigned int Total_hours, unsigned int Credits,
          unsigned int Semester) {
    auto *tmp_class = new ClassInformation(Course_name, Course_nature, Total_hours, Credits, Semester);
    if (tmp_class->course_id == -1) {
        printf("new class error,can't not add total_class_list\n");
        delete (tmp_class);
        return false;
    } else {
        total_class_list.insert({tmp_class->course_id, tmp_class});
        return true;
    }
}

void class_list_show() {
    printf("\tclass id\tclass name\tclass nature\ttotal hour\tcredit\tsemester\tchoose number\n");
    for (const auto &course: total_class_list) {
        const auto &course_data = course.second;
        course_data->class_show();
    }
    printf("the end\n");
}

void target_class_list_show(const int &course_id) {
    const auto &course = total_class_list.find(course_id);
    if (course != total_class_list.end()) {
        printf("\tclass id\tclass name\tclass nature\ttotal hour\tcredit\tsemester\tchoose number\n");
        course->second->class_show();
        who_choose_this_class(course_id);
        printf("the end\n");
        return;
    }
    printf("error");
}

void who_choose_this_class(const unsigned int &course_id) {
    auto course = total_class_list.find(course_id);
    printf("choose %s 's student\t\tstudent id\tstudent name\n", course->second->course_name.c_str());
    course->second->choose_student_show();
    printf("the end\n");
}

long long how_many_student_choose_this_class(const long long &course_id) {
    long long number = total_class_list.size();
    return number;
}

long long how_many_class() {
    long long number = total_class_list.size();
//    printf("there are %lld class", number);
    return number;
}

//学生
bool new_student(const long long &student_id, const std::string &student_name, bool gender, unsigned int ages,
                 unsigned int major, unsigned int classroom_id, long long phone_number) {
    if (total_student_list.count(student_id)) {
        printf("student id have existed\n");
        return false;
    }
    auto *tmp_student = new Student(student_id, student_name, gender, ages, major, classroom_id, phone_number);
    total_student_list.insert({student_id, tmp_student});
    return true;
}

bool remove_student(const long long &student_id) {
    auto stu = total_student_list.find(student_id);
    if (stu != total_student_list.end()) {
        auto student = stu->second;
        for (const auto &course_id: student->get_class_list()) {
            student->remove_class(course_id.first);
        }
        total_student_list.erase(student_id);
        printf("remove %lld success\n", student_id);
        return true;
    } else {
        printf("remove %lld fail,this student isn't exist\n", student_id);
        return false;
    }
}

void student_list_show() {
    printf("\tstudent id\tstudent name\tmajor\tclassroom\tphone number\tage\tgender\n");
    for (const auto &s: total_student_list) {
        s.second->detail_show();
    }
}

void target_student_show(const long long &student_id) {
    const auto &s = total_student_list.find(student_id);
    if (s != total_student_list.end()) {
        printf("\tstudent id\tstudent name\tmajor\tclassroom\tphone number\tage\tgender\n");
        s->second->detail_show();
        s->second->choose_class_list_show();
    } else {
        printf("error");
    }
}

bool student_choose_class(const long long &student_id, const int &course_id) {
    auto s = total_student_list.find(student_id);
    if (s == total_student_list.end()) {
        printf("student id is wrong\n");
        return false;
    }
    if (!total_class_list.count(course_id)) {
        printf("this class id is wrong\n");
        return false;
    }
    auto student = s->second;
    if (student->get_class_list().count(course_id)) {
        printf("have chosen\n");
        return true;
    }
    if (student->choose_class(course_id)) {
        return true;
    } else {
        return false;
    }
}

bool student_remove_class(const long long &student_id, const int &course_id) {
    auto s = total_student_list.find(student_id);
    if (s == total_student_list.end()) {
        printf("student id is wrong\n");
        return false;
    }
    if (!total_class_list.count(course_id)) {
        printf("this class id is wrong\n");
        return true;
    }
    auto student = s->second;
    if (!student->get_class_list().count(course_id)) {
        printf("haven't chosen\n");
        return true;
    }
    if (student->remove_class(course_id)) {
        return true;
    } else {
        return false;
    }
}

void student_change_information(const long long &student_id) {
    auto s = total_student_list.find(student_id);
    if (s == total_student_list.end()) {
        printf("student id is wrong\n");
        return;
    }
    s->second->change_info();
}

long long how_many_student() {
    long long number = total_student_list.size();
//    printf("there are %lld students", number);
    return number;
}

//保存与读取初始化
std::string get_dir() {
    char _tmp[256];
    _getcwd(_tmp, 256);
    for (auto &c: _tmp) {
        if (c == '\\')
            c = '/';
    }
    std::string tmp(_tmp);
    auto tmp_dir = tmp + "/data/";
    if (0 != _access(tmp_dir.c_str(), 0)) {
        _mkdir(tmp_dir.c_str());
    }
    return tmp_dir;
}

void save() {
    std::string dir = get_dir();
    std::ofstream out1(dir + "class_data.txt");
    if (out1.is_open()) {
        for (const auto &course: total_class_list) {
            auto course_data = course.second;
            out1 << course_data->course_id << " ";//课程代码
            out1 << course_data->course_name << " ";//课程名称
            out1 << course_data->course_nature << " ";//课程性质
            out1 << course_data->total_hours << " ";//总学时
            out1 << course_data->credits << " ";//学分
            out1 << course_data->semester << " ";//开课学期
            out1 << course_data->choose_number << "\n";//选修人数
            std::ofstream out2((dir + "class_choose_student_" + std::to_string(course_data->course_id) + ".txt"));
            for (const auto &i: course_data->choose_student_list) {
                out2 << i.first << " " << i.second << "\n";
            }
            out2.close();
        }
        out1.close();
    }
    std::ofstream out3(dir + "student_data.txt");
    if (out3.is_open()) {
        for (const auto &student: total_student_list) {
            auto student_data = student.second;
            out3 << student_data->student_id << " ";
            out3 << student_data->student_name << " ";
            out3 << student_data->gender << " ";
            out3 << student_data->age << " ";
            out3 << student_data->major << " ";
            out3 << student_data->classroom_id << " ";
            out3 << student_data->phone_number << "\n";
            std::ofstream out4((dir + "student_choose_class_" + std::to_string(student_data->student_id) + ".txt"));
            for (const auto &i: student_data->choose_class_list) {
                out4 << i.first << " " << i.second << "\n";
            }
            out4.close();
        }
        out3.close();
    }
    std::ofstream out5((dir + "major_list.txt"));
    if (out5.is_open()) {
        for (const auto &major: major_list) {
            out5 << major << "\n";
        }
        out5.close();
    }
    out1.close();
    out3.close();
    out5.close();
}

void init() {
    std::string dir = get_dir();
    std::ifstream in1(dir + "class_data.txt");
    if (in1.is_open()) {
        int course_id;//课程代码
        std::string course_name;//课程名称
        std::string course_nature;//课程性质
        unsigned int total_hours;//总学时
        unsigned int credits;//学分
        unsigned int semester;//开课学期
        unsigned int choose_number;//选修人数
        while (!in1.eof()) {
            in1 >> course_id >> course_name >> course_nature >> total_hours >> credits >> semester >> choose_number;
            auto *temp_class = new ClassInformation(course_id, course_name, course_nature, total_hours, credits,
                                                    semester, choose_number);
            std::ifstream in2(dir + "class_choose_student_" + std::to_string(course_id) + ".txt");
            if (in2.is_open()) {
                std::map<long long, std::basic_string<char>> list;
                long long student_id;
                std::string student_name;
                while (in2 >> student_id >> student_name) {
                    list.insert({student_id, student_name});
                }
                temp_class->choose_student_list = list;
            }
            in2.close();
            total_class_list.insert({course_id, temp_class});
        }
        in1.close();
    }
    std::ifstream in3(dir + "student_data.txt");
    if (in3.is_open()) {
        long long student_id;
        std::string student_name;
        bool gender;
        unsigned int years;
        std::string major;
        unsigned int classroom_id;
        long long phone_number;
        while (!in3.eof()) {
            in3 >> student_id >> student_name >> gender >> years >> major >> classroom_id >> phone_number;
            auto *temp_student = new Student(student_id, student_name, gender, years, major, classroom_id,
                                             phone_number);
            std::ifstream in4(dir + "student_choose_class_" + std::to_string(student_id) + ".txt");
            if (in4.is_open()) {
                std::map<int, std::string> list;
                unsigned int class_id;
                std::string class_name;
                while (in4 >> class_id >> class_name) {
                    list.insert({class_id, class_name});
                }
                temp_student->choose_class_list = list;
            }
            in4.close();
            total_student_list.insert({student_id, temp_student});
        }
        in3.close();
    }
    std::ifstream in5(dir + "major_list.txt");
    if (in5.is_open()) {
        std::string tmp_major;
        while (in5 >> tmp_major) {
            for(const auto& tmp : major_list){
                if(tmp_major == tmp){
                    continue;
                }
            }
            major_list.push_back(tmp_major);
        }
        in5.close();
    }
    //初始化时更改COURSE_ID
    if (!total_class_list.empty()) {
        auto last = --total_class_list.end();
        COURSE_LIST_NUMBER = last->first + 1;
    } else{
        COURSE_LIST_NUMBER=10001;
    }
    //初始化in表
    {
        std::hash<std::string> str_hash;
        in.insert({str_hash("1"), 1});
        in.insert({str_hash("2"), 2});
        in.insert({str_hash("3"), 3});
        in.insert({str_hash("4"), 4});
        in.insert({str_hash("5"), 5});
        in.insert({str_hash("6"), 6});
        in.insert({str_hash("7"), 7});
        in.insert({str_hash("8"), 8});
        in.insert({str_hash("9"), 9});
        in.insert({str_hash("10"), 10});
        in.insert({str_hash("11"), 11});
        in.insert({str_hash("12"), 12});
        in.insert({str_hash("13"), 13});
        in.insert({str_hash("0"), 0});
    }
}

//专业操作
unsigned int new_major(const std::string &major_name) {
    major_list.push_back(major_name);
    if (major_list.empty()) {
        return 0;
    } else {
        return major_list.size() - 1;
    }
}
void major_list_show() {
    int i=0;
    for (const auto &major: major_list) {
        std::cout <<i<<":"<< major << " ";
        ++i;
    }
    printf("\n");
}
//课程性质操作
void course_nature_list_show() {
    for (const auto &i: course_nature_list) {
        printf("%d: %s  ", i.first, i.second.c_str());
    }
    printf("\n");
}


void welcome() {
    system("cls");
    std::cout << "1. show all class" << std::endl;
    std::cout << "2. show all student" << std::endl;
    std::cout << "3. new student" << std::endl;
    std::cout << "4. new class" << std::endl;
    std::cout << "5. show target class" << std::endl;
    std::cout << "6. show target student" << std::endl;
    std::cout << "7. remove class" << std::endl;
    std::cout << "8. remove student" << std::endl;
    std::cout << "9. change student information" << std::endl;
    std::cout << "10. statistical function" << std::endl;
    std::cout << "11. student choose course" << std::endl;
    std::cout << "12. student remove course" << std::endl;
    std::cout << "13. new major" << std::endl;
    std::cout << "0. save and exit" << std::endl;
}
unsigned long long input_solution(const std::string &a) {
    std::hash<std::string> str_hash;
    auto _hash = in.find(str_hash(a));
    if (_hash != in.end()) {
        return _hash->second;
    } else {
        return 100;
    }
}

void dropInput() {
    while (std::cin && std::cin.get() != '\n')continue;
    std::cin.clear();
}

int main() {
    init();
    while (true) {
        welcome();
        std::string _str;
        std::cin >> _str;
        auto choice = input_solution(_str);
        switch (choice) {
            case 1:
                system("cls");
                class_list_show();
                system("pause");
                break;
            case 2:
                system("cls");
                student_list_show();
                system("pause");
                break;
            case 3: {
                system("cls");
                std::cout << "input id,name,gender ,age,major,classroom_id,phone number " << std::endl;
                std::cout << "gender 1 is man,0 is girl." << std::endl;
                major_list_show();
                long long student_id;
                std::string student_name;
                bool gender;
                unsigned int years;
                unsigned int major;
                unsigned int classroom_id;
                long long phone_number;
                std::cin >> student_id >> student_name >> gender >> years >> major >> classroom_id >> phone_number;
                if(!std::cin){
                    dropInput();
                    break;
                }
                if(major<0&&major>major_list.size()-1){
                    dropInput();
                    break;
                }
                dropInput();
                if (new_student(student_id, student_name, gender, years, major, classroom_id, phone_number)) {
                    printf("success");
                }
                save();
                system("pause");
                break;
            }
            case 4: {
                system("cls");
                std::cout << "input ,name,nature ,total hours ,credits,semester " << std::endl;
                std::cout << "course_nature is number" << std::endl;
                course_nature_list_show();
                std::string course_name;//课程名称
                int course_nature;//课程性质
                unsigned int total_hours;//总学时
                unsigned int credits;//学分
                unsigned int semester;//开课学期
                std::cin >> course_name >> course_nature >> total_hours >> credits >> semester;
                if(!std::cin){
                    dropInput();
                    break;
                }
                if(course_nature<0&&course_nature>course_nature_list.size()-1){
                    dropInput();
                    break;
                }
                dropInput();
                if (new_class(course_name, course_nature, total_hours, credits, semester)) {
                    printf("success");
                }
                save();
                system("pause");
                break;
            }
            case 5: {
                system("cls");
                std::cout << "target class_id" << std::endl;
                int class_id;
                std::cin >> class_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                target_class_list_show(class_id);
                system("pause");
                break;
            }
            case 6: {
                system("cls");
                std::cout << "target student_id" << std::endl;
                long long student_id;
                std::cin >> student_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                target_student_show(student_id);
                system("pause");
                break;
            }
            case 7: {
                system("cls");
                std::cout << "target class_id" << std::endl;
                int class_id;
                std::cin >> class_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                if (remove_class(class_id)) {
                    printf("success");
                }
                save();
                system("pause");
                break;
            }
            case 8: {
                system("cls");
                std::cout << "target student_id" << std::endl;
                long long student_id;
                std::cin >> student_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                if (remove_student(student_id)) {
                    printf("success");
                }
                save();
                system("pause");
                break;
            }
            case 9: {
                system("cls");
                std::cout << "target student_id" << std::endl;
                long long student_id;
                std::cin >> student_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                student_change_information(student_id);
                break;
            }
            case 10: {
                system("cls");
                std::cout << "there are" << how_many_class() << std::endl;
                std::cout << "there are" << how_many_student() << std::endl;
                system("pause");
                break;
            }
            case 11: {
                std::cout << "please input student_id and chosen_course_id" << std::endl;
                long long student_id, course_id;
                std::cin >> student_id >> course_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                printf("input success");
                student_choose_class(student_id, course_id);
                break;
            }
            case 12: {
                std::cout << "please input student_id and chosen_course_id" << std::endl;
                long long student_id, course_id;
                std::cin >> student_id >> course_id;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                student_remove_class(student_id, course_id);
                break;
            }
            case 13: {
                std::cout << "please input new major name" << std::endl;
                std::string tmp_major;
                std::cin >> tmp_major;
                if(!std::cin){
                    dropInput();
                    break;
                }
                dropInput();
                if (!tmp_major.empty()) {
                    new_major(tmp_major);
                    break;
                }
                break;
            }
            case 0: {
                save();
                return 0;
            }
            default: {
                printf("error input");
                dropInput();
                system("pause");
                continue;
            }
        }
    }
    return 0;
}
