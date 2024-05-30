#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>

class Student {
private:
    std::string studentID;
    std::string name;
    int score;

public:
    Student(const std::string& id, const std::string& n) : studentID(id), name(n), score(0) {}

    std::string getStudentID() const { return studentID; }

    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    int getScore() const { return score; }
    void setScore(int s) { score = s; }
};

#endif 
