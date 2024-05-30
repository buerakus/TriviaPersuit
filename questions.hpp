#ifndef QUESTIONS_HPP
#define QUESTIONS_HPP

#include <string>

class Question {
private:
    std::string questionText;
    int score;

public:
    Question(const std::string& text, int s) : questionText(text), score(s) {}

    std::string getQuestionText() const { return questionText; }
    void setQuestionText(const std::string& text) { questionText = text; }

    int getScore() const { return score; }
    void setScore(int s) { score = s; }
};

#endif // QUESTIONS_HPP
