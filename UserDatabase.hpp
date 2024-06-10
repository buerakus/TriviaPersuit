#ifndef USERDATABASE_HPP
#define USERDATABASE_HPP

#include <string>
#include <stdexcept>
#include <iostream>

// Max users and vertices for the graph
const int maxUsers = 100;
const int maxVertices = 100;

struct User {
    int id = 0;
    std::string name;
    std::string password;
    int score = 0;
};

class Graph {
private:
    struct AdjListNode {
        int dest;
        AdjListNode* next;
    };

    struct AdjList {
        AdjListNode* head;
    };

    AdjList* array;
    std::string vertexNames[maxVertices];

public:
    Graph();
    ~Graph();
    void addVertex(int vertex, const std::string& name);
    void addEdge(int src, int dest);
    void displayGraph();
};

class UserDatabase {
private:
    User users[maxUsers];
    int lastUserId = 0;
    const std::string filename = "credentials.txt";

    UserDatabase();
    void loadUsers();
    void saveUsers();
    void bubbleSort();
    void insertionSort(int n);

    class BSTNode {
    public:
        User user;
        BSTNode* left;
        BSTNode* right;
        BSTNode(const User& user) : user(user), left(nullptr), right(nullptr) {}
    };

    class BST {
    private:
        BSTNode* root;

        void insert(BSTNode*& node, const User& user);
        void inOrder(BSTNode* node, int n, int& count);
        void inOrderAll(BSTNode* node);
        void viewUsersAboveThreshold(int threshold);

    public:
        BST();
        ~BST();
        void insert(const User& user);
        void inOrder(int n);
        void inOrderAll();
        void clear(BSTNode* node);
    };

    BST userBST;

public:
    static UserDatabase& getInstance();
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void updateUserScore(const std::string& username, int score);
    void viewScoreboard();
    void viewTopByPoints(int n);
    void displayTop30BarGraph();
    int getUserRank(const std::string& username);
    int getLastUserId() const;
    User getUserById(int id) const;
};

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
    Stack();
    ~Stack();
    void push(T data);
    void pop();
    T peek() const;
    bool isEmpty() const;
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
    Queue();
    ~Queue();
    void enqueue(T data);
    void dequeue();
    T peek() const;
    bool isEmpty() const;
};

struct Card {
    std::string question;
    std::string answers[4];
    int correctAnswer = 0;
};

class Deck {
private:
    Stack<Card> cards;

public:
    void loadDeck(const std::string& filename);
    Card drawCard();
    void discardCard(const Card& card);
    void displayDiscardedQuestions();
    Card getDiscardedCard(int index);
    void removeCard(int index);
    bool isEmpty() const;
};

struct Player {
    std::string name;
    int score;
    int rounds;
    Player(const std::string& name);
    void answerQuestion(const Card& card, int answer);
    void discardQuestion();
};

void gameLoop(UserDatabase& database, const std::string& username);
void adminLoop(UserDatabase& database);

#endif // USERDATABASE_HPP
