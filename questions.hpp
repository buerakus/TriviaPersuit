#ifndef QUESTIONS_HPP
#define QUESTIONS_HPP

#include <iostream>
#include <string>
#include <stack>

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

class QuestionStack {
private:
    std::stack<Question> stack;

public:
    void push(const Question& question) {
        stack.push(question);
    }

    Question pop() {
        Question top = stack.top();
        stack.pop();
        return top;
    }

    bool isEmpty() const {
        return stack.empty();
    }
};

#endif // QUESTIONS_HPP
