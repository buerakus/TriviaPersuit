#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

const int maxUsers = 100;

struct User {
    int id;
    std::string name;
    std::string password;
    int score;
};

// Singleton class for User Database
class UserDatabase {
private:
    User users[maxUsers];
    int lastUserId = 0;
    const std::string filename = "credentials.txt";

    UserDatabase() {
        loadUsers();
    }

public:
    static UserDatabase& getInstance() {
        static UserDatabase instance;
        return instance;
    }

    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void updateUserScore(const std::string& username, int score);

private:
    void loadUsers();
    void saveUsers();
};

bool UserDatabase::registerUser(const std::string& username, const std::string& password) {
    if (lastUserId >= maxUsers) {
        std::cout << "The amount of registered users has reached the limit (" << maxUsers << ")!" << std::endl;
        return false;
    }

    for (int i = 0; i < lastUserId; ++i) {
        if (users[i].name == username) {
            std::cout << "User with this username already exists!" << std::endl;
            return false;
        }
    }

    User newUser;
    lastUserId++;
    newUser.id = lastUserId;
    newUser.name = username;
    newUser.password = password;
    newUser.score = 0;

    users[lastUserId - 1] = newUser;
    saveUsers();

    std::cout << "Successfully registered as student!" << std::endl;
    return true;
}

bool UserDatabase::loginUser(const std::string& username, const std::string& password) {
    for (int i = 0; i < lastUserId; ++i) {
        if (users[i].name == username && users[i].password == password) {
            std::cout << "Welcome " << username << std::endl;
            return true;
        }
    }
    std::cout << "Invalid username or password!" << std::endl;
    return false;
}

void UserDatabase::updateUserScore(const std::string& username, int score) {
    for (int i = 0; i < lastUserId; ++i) {
        if (users[i].name == username) {
            users[i].score = score;
            saveUsers();
            break;
        }
    }
}

void UserDatabase::loadUsers() {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            User user;
            std::string idStr, name, password, scoreStr;
            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, password, ',');
            std::getline(ss, scoreStr, ',');
            user.id = std::stoi(idStr);
            user.score = std::stoi(scoreStr);
            user.name = name;
            user.password = password;

            if (user.id > lastUserId) {
                lastUserId = user.id;
            }

            if (user.id <= maxUsers) {
                users[user.id - 1] = user;
            }
        }
        file.close();
    }
}

void UserDatabase::saveUsers() {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < lastUserId; ++i) {
            file << users[i].id << "," << users[i].name << "," << users[i].password << "," << users[i].score << "\n";
        }
        file.close();
    }
    else {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
    }
}

void adminLogin(const std::string& username, const std::string& password) {
    if (username == "admin" && password == "admin") {
        std::cout << "Welcome admin!" << std::endl;
    }
    else {
        std::cout << "Invalid admin credentials!" << std::endl;
    }
}

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
    };
    Node* top;

public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(T data) {
        Node* newNode = new Node(data);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (!isEmpty()) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    T peek() const {
        if (!isEmpty()) {
            return top->data;
        }
        throw std::out_of_range("Stack is empty");
    }

    bool isEmpty() const {
        return top == nullptr;
    }
};

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
    };
    Node* front, * rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T data) {
        Node* newNode = new Node(data);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue() {
        if (!isEmpty()) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    T peek() const {
        if (!isEmpty()) {
            return front->data;
        }
        throw std::out_of_range("Queue is empty");
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};

struct Card {
    std::string question;
    std::string answers[4];
    int correctAnswer;
};

class Deck {
private:
    Stack<Card> cards;

public:
    void loadDeck(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            Card card;
            std::stringstream ss(line);
            std::getline(ss, card.question, '|');
            for (int i = 0; i < 4; ++i) {
                std::getline(ss, card.answers[i], '|');
            }
            ss >> card.correctAnswer;
            cards.push(card);
        }
    }

    Card drawCard() {
        if (cards.isEmpty()) {
            throw std::out_of_range("Deck is empty");
        }
        Card topCard = cards.peek();
        cards.pop();
        return topCard;
    }

    void discardCard(const Card& card) {
        cards.push(card);
    }

    bool isEmpty() const {
        return cards.isEmpty();
    }

    void displayDiscardedQuestions() {
        if (cards.isEmpty()) {
            std::cout << "No discarded questions available." << std::endl;
            return;
        }

        Stack<Card> tempStack;
        int index = 1;
        while (!cards.isEmpty()) {
            Card card = cards.peek();
            std::cout << index << ". " << card.question << std::endl;
            tempStack.push(card);
            cards.pop();
            index++;
        }

        while (!tempStack.isEmpty()) {
            cards.push(tempStack.peek());
            tempStack.pop();
        }
    }

    Card getDiscardedCard(int choice) {
        if (cards.isEmpty()) {
            throw std::out_of_range("Deck is empty");
        }

        Stack<Card> tempStack;
        Card selectedCard;
        int index = 1;

        while (!cards.isEmpty()) {
            Card card = cards.peek();
            if (index == choice) {
                selectedCard = card;
            }
            tempStack.push(card);
            cards.pop();
            index++;
        }

        while (!tempStack.isEmpty()) {
            cards.push(tempStack.peek());
            tempStack.pop();
        }

        if (index <= choice) {
            throw std::out_of_range("Invalid choice");
        }

        return selectedCard;
    }

    void removeCard(int choice) {
        if (cards.isEmpty()) {
            throw std::out_of_range("Deck is empty");
        }

        Stack<Card> tempStack;
        int index = 1;

        while (!cards.isEmpty()) {
            Card card = cards.peek();
            if (index != choice) {
                tempStack.push(card);
            }
            cards.pop();
            index++;
        }

        while (!tempStack.isEmpty()) {
            cards.push(tempStack.peek());
            tempStack.pop();
        }

        if (index <= choice) {
            throw std::out_of_range("Invalid choice");
        }
    }
};

class Player {
public:
    std::string name;
    int score;
    int rounds;

    Player(const std::string& name) : name(name), score(0), rounds(3) {}

    void answerQuestion(const Card& card, int answer) {
        if (answer == card.correctAnswer) {
            score += 100;
        }
    }

    void discardQuestion() {
        score += 80;
    }
};

void gameLoop(UserDatabase& database, const std::string& username) {
    Deck unansweredDeck, answeredDeck, discardedDeck;
    unansweredDeck.loadDeck("unanswered_deck.txt");
    answeredDeck.loadDeck("answered_deck.txt");
    discardedDeck.loadDeck("discarded_deck.txt");

    Player player(username);

    for (int i = 0; i < player.rounds; ++i) {
        std::cout << "Round " << i + 1 << std::endl;
        std::cout << "1. Answer question from unanswered deck" << std::endl;
        std::cout << "2. Choose question from discarded deck" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            try {
                Card card = unansweredDeck.drawCard();
                std::cout << card.question << std::endl;
                for (int j = 0; j < 4; ++j) {
                    std::cout << j + 1 << ". " << card.answers[j] << std::endl;
                }
                int answer;
                std::cin >> answer;
                if (answer - 1 == card.correctAnswer) {
                    std::cout << "Correct!" << std::endl;
                    player.answerQuestion(card, answer - 1);
                    answeredDeck.discardCard(card);
                }
                else {
                    std::cout << "Wrong!" << std::endl;
                    discardedDeck.discardCard(card);
                }
            }
            catch (const std::out_of_range& e) {
                std::cerr << "No more cards in the deck." << std::endl;
            }
        }
        else if (choice == 2) {
            discardedDeck.displayDiscardedQuestions();
            std::cout << "Choose a question number: ";
            int questionChoice;
            std::cin >> questionChoice;

            try {
                Card card = discardedDeck.getDiscardedCard(questionChoice);
                std::cout << card.question << std::endl;
                for (int j = 0; j < 4; ++j) {
                    std::cout << j + 1 << ". " << card.answers[j] << std::endl;
                }
                int answer;
                std::cin >> answer;
                if (answer - 1 == card.correctAnswer) {
                    std::cout << "Correct!" << std::endl;
                    player.discardQuestion();
                    answeredDeck.discardCard(card);
                    discardedDeck.removeCard(questionChoice);
                }
                else {
                    std::cout << "Wrong!" << std::endl;
                    discardedDeck.discardCard(card);
                }
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Invalid choice." << std::endl;
                --i; // Invalid choice, don't count this round
            }
        }
        else {
            std::cout << "Invalid choice." << std::endl;
            --i; // Invalid choice, don't count this round
        }
    }

    std::cout << "Game over, your final score: " << player.score << std::endl;
    database.updateUserScore(username, player.score);

    // Save updated decks to files
    std::ofstream unansweredFile("unanswered_deck.txt");
    if (unansweredFile.is_open()) {
        while (!unansweredDeck.isEmpty()) {
            Card card = unansweredDeck.drawCard();
            unansweredFile << card.question << "|";
            for (int i = 0; i < 4; ++i) {
                unansweredFile << card.answers[i] << "|";
            }
            unansweredFile << card.correctAnswer << "\n";
        }
        unansweredFile.close();
    }

    std::ofstream answeredFile("answered_deck.txt");
    if (answeredFile.is_open()) {
        while (!answeredDeck.isEmpty()) {
            Card card = answeredDeck.drawCard();
            answeredFile << card.question << "|";
            for (int i = 0; i < 4; ++i) {
                answeredFile << card.answers[i] << "|";
            }
            answeredFile << card.correctAnswer << "\n";
        }
        answeredFile.close();
    }

    std::ofstream discardedFile("discarded_deck.txt");
    if (discardedFile.is_open()) {
        while (!discardedDeck.isEmpty()) {
            Card card = discardedDeck.drawCard();
            discardedFile << card.question << "|";
            for (int i = 0; i < 4; ++i) {
                discardedFile << card.answers[i] << "|";
            }
            discardedFile << card.correctAnswer << "\n";
        }
        discardedFile.close();
    }
}

int main() {
    int choice;
    std::string username, password;
    UserDatabase& database = UserDatabase::getInstance();

    while (true) {
        std::cout << "Welcome to the Trivia Pursuit DSTR Ed.\n";
        std::cout << "Choose option :\n1. Login as Student\n2. Login as Teacher\n3. Register as student\n4. Leave\n";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter login: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            if (database.loginUser(username, password)) {
                gameLoop(database, username);
            }
            break;
        case 2:
            std::cout << "Enter login: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            adminLogin(username, password);
            break;
        case 3:
            std::cout << "Enter login: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin.ignore(); // Clear the newline character left in the input buffer
            std::getline(std::cin, password);
            database.registerUser(username, password);
            break;
        case 4:
            std::cout << "Goodbye!" << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
