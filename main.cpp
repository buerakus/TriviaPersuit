#include "UserDatabase.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

Graph::Graph() {
    array = new AdjList[maxVertices];
    for (int i = 0; i < maxVertices; ++i) {
        array[i].head = nullptr;
    }
}

Graph::~Graph() {
    for (int i = 0; i < maxVertices; ++i) {
        AdjListNode* node = array[i].head;
        while (node != nullptr) {
            AdjListNode* temp = node;
            node = node->next;
            delete temp;
        }
    }
    delete[] array;
}

void Graph::addVertex(int vertex, const std::string& name) {
    if (vertex < maxVertices) {
        array[vertex].head = nullptr;
        vertexNames[vertex] = name;
    }
}

void Graph::addEdge(int src, int dest) {
    if (src < maxVertices && dest < maxVertices) {
        AdjListNode* newNode = new AdjListNode{ dest, array[src].head };
        array[src].head = newNode;
        newNode = new AdjListNode{ src, array[dest].head };
        array[dest].head = newNode;
    }
}

void Graph::displayGraph() {
    for (int i = 0; i < maxVertices; ++i) {
        if (array[i].head != nullptr) {
            std::cout << vertexNames[i] << " -> ";
            AdjListNode* node = array[i].head;
            while (node != nullptr) {
                std::cout << vertexNames[node->dest] << " ";
                node = node->next;
            }
            std::cout << std::endl;
        }
    }
}

UserDatabase::UserDatabase() {
    loadUsers();
}

UserDatabase& UserDatabase::getInstance() {
    static UserDatabase instance;
    return instance;
}

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

void UserDatabase::viewScoreboard() {
    bubbleSort();
    for (int i = 0; i < lastUserId; ++i) {
        std::cout << "Rank " << i + 1 << ": " << users[i].name << " - " << users[i].score << " points" << std::endl;
    }
}

void UserDatabase::viewTopByPoints(int n) {
    insertionSort(n);
    for (int i = 0; i < n && i < lastUserId; ++i) {
        std::cout << "Rank " << i + 1 << ": " << users[i].name << " - " << users[i].score << " points" << std::endl;
    }
}

int UserDatabase::getUserRank(const std::string& username) {
    bubbleSort();
    for (int i = 0; i < lastUserId; ++i) {
        if (users[i].name == username) {
            return i + 1;
        }
    }
    return -1;
}

int UserDatabase::getLastUserId() const {
    return lastUserId;
}

User UserDatabase::getUserById(int id) const {
    for (int i = 0; i < lastUserId; ++i) {
        if (users[i].id == id) {
            return users[i];
        }
    }
    return User{};
}

void UserDatabase::bubbleSort() {
    for (int i = 0; i < lastUserId - 1; ++i) {
        for (int j = 0; j < lastUserId - i - 1; ++j) {
            if (users[j].score < users[j + 1].score) {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}

void UserDatabase::insertionSort(int n) {
    for (int i = 1; i < n && i < lastUserId; ++i) {
        User key = users[i];
        int j = i - 1;
        while (j >= 0 && users[j].score < key.score) {
            users[j + 1] = users[j];
            j--;
        }
        users[j + 1] = key;
    }
}

void UserDatabase::displayTop30BarGraph() {
    struct TopUser {
        std::string name;
        int score;
    };

    TopUser topUsers[30];
    int topUserCount = (lastUserId < 30) ? lastUserId : 30;

    bubbleSort();
    for (int i = 0; i < topUserCount; ++i) {
        topUsers[i].name = users[i].name;
        topUsers[i].score = users[i].score;
    }

    int maxScore = topUsers[0].score;
    std::cout << "\nTop 30 Players Bar Graph:\n";
    for (int i = 0; i < topUserCount; ++i) {
        std::cout << i + 1 << ". " << topUsers[i].name << " | ";
        int barLength = (topUsers[i].score * 50) / maxScore;
        for (int j = 0; j < barLength; ++j) {
            std::cout << "=";
        }
        std::cout << " " << topUsers[i].score << " points\n";
    }
}

template <typename T>
Stack<T>::Stack() : top(nullptr) {}

template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

template <typename T>
void Stack<T>::push(T data) {
    Node* newNode = new Node(data);
    newNode->next = top;
    top = newNode;
}

template <typename T>
void Stack<T>::pop() {
    if (!isEmpty()) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

template <typename T>
T Stack<T>::peek() const {
    if (!isEmpty()) {
        return top->data;
    }
    throw std::out_of_range("Stack is empty");
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return top == nullptr;
}

template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr) {}

template <typename T>
Queue<T>::~Queue() {
    while (!isEmpty()) {
        dequeue();
    }
}

template <typename T>
void Queue<T>::enqueue(T data) {
    Node* newNode = new Node(data);
    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
}

template <typename T>
void Queue<T>::dequeue() {
    if (!isEmpty()) {
        Node* temp = front;
        front = front->next;
        delete temp;
    }
}

template <typename T>
T Queue<T>::peek() const {
    if (!isEmpty()) {
        return front->data;
    }
    throw std::out_of_range("Queue is empty");
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return front == nullptr;
}

Player::Player(const std::string& name) : name(name), score(0), rounds(3) {}

void Player::answerQuestion(const Card& card, int answer) {
    if (answer == card.correctAnswer) {
        score += 100;
    }
}

void Player::discardQuestion() {
    score += 80;
}

void Deck::loadDeck(const std::string& filename) {
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

Card Deck::drawCard() {
    if (cards.isEmpty()) {
        throw std::out_of_range("No more cards in the deck.");
    }
    Card card = cards.peek();
    cards.pop();
    return card;
}

void Deck::discardCard(const Card& card) {
    cards.push(card);
}

void Deck::displayDiscardedQuestions() {
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

Card Deck::getDiscardedCard(int index) {
    Stack<Card> tempStack;
    Card selectedCard;
    int currentIndex = 1;

    while (!cards.isEmpty()) {
        Card card = cards.peek();
        if (currentIndex == index) {
            selectedCard = card;
        }
        else {
            tempStack.push(card);
        }
        cards.pop();
        currentIndex++;
    }

    while (!tempStack.isEmpty()) {
        cards.push(tempStack.peek());
        tempStack.pop();
    }

    if (currentIndex <= index) {
        throw std::out_of_range("Invalid card index.");
    }

    return selectedCard;
}

void Deck::removeCard(int index) {
    Stack<Card> tempStack;
    int currentIndex = 1;

    while (!cards.isEmpty()) {
        Card card = cards.peek();
        if (currentIndex != index) {
            tempStack.push(card);
        }
        cards.pop();
        currentIndex++;
    }

    while (!tempStack.isEmpty()) {
        cards.push(tempStack.peek());
        tempStack.pop();
    }
}

bool Deck::isEmpty() const {
    return cards.isEmpty();
}

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
                    discardedDeck.removeCard(questionChoice);
                }
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Invalid choice." << std::endl;
                --i;
            }
        }
        else {
            std::cout << "Invalid choice." << std::endl;
            --i;
        }
    }

    std::cout << "Game over, your final score: " << player.score << std::endl;
    database.updateUserScore(username, player.score);

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

void adminLoop(UserDatabase& database) {
    Graph userGraph;
    while (true) {
        std::cout << "Teacher Options:\n1. View Scoreboard\n2. View Top by Points\n3. View All Questions in Answered Deck\n4. Display Top 30 Players Bar Graph\n5. Display User Graph\n6. Logout\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            database.viewScoreboard();
            break;
        case 2:
            int n;
            std::cout << "Enter number of top students to view (e.g., 30 for top 30): ";
            std::cin >> n;
            database.viewTopByPoints(n);
            break;
        case 3: {
            Deck answeredDeck;
            answeredDeck.loadDeck("answered_deck.txt");
            while (!answeredDeck.isEmpty()) {
                Card card = answeredDeck.drawCard();
                std::cout << card.question << std::endl;
            }
            break;
        }
        case 4:
            database.displayTop30BarGraph();
            break;
        case 5:
            for (int i = 1; i <= database.getLastUserId(); ++i) {
                User user = database.getUserById(i);
                userGraph.addVertex(i, user.name);
                if (i > 1) {
                    userGraph.addEdge(i - 1, i);
                }
            }
            userGraph.displayGraph();
            break;
        case 6:
            return;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

int main() {
    int choice;
    std::string username, password;
    UserDatabase& database = UserDatabase::getInstance();

    while (true) {
        std::cout << "Welcome to the Card Game.\n";
        std::cout << "Choose option :\n1. Login as Student\n2. Login as Teacher\n3. Register as Student\n4. Leave\n";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            if (database.loginUser(username, password)) {
                gameLoop(database, username);
            }
            break;
        case 2:
            std::cout << "Enter admin username: ";
            std::cin >> username;
            std::cout << "Enter admin password: ";
            std::cin >> password;
            if (username == "admin" && password == "admin") {
                adminLoop(database);
            }
            else {
                std::cout << "Invalid admin credentials!" << std::endl;
            }
            break;
        case 3:
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin.ignore();
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
