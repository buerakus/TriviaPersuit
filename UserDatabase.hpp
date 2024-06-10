#ifndef USERDATABASE_HPP
#define USERDATABASE_HPP

#include <string>
//stdexcept is a part of error handling and does not help to implement any data structures in code, each structure is custom built
#include <stdexcept>
#include <iostream>

//constants for the graph function
const int maxUsers = 100;
const int maxVertices = 100;

//user/student structure attributes
struct User {
    int id = 0;
    std::string name;
    std::string password;
    int score = 0;
};

//using adjacency list for graph class
class Graph {
private:
    //node
    struct AdjListNode {
        int dest;
        AdjListNode* next;
    };
    //adjacency list
    struct AdjList {
        AdjListNode* head;
    };
    //adjacency list and array to store verticle names
    AdjList* array;
    std::string vertexNames[maxVertices];

//graph vertex add, edge add, display declarations
public:
    Graph();
    ~Graph();
    void addVertex(int vertex, const std::string& name);
    void addEdge(int src, int dest);
    void displayGraph();
};

// userdatabase class incorporates user objects: registration, login, score updating, also binary search tree to manage and query users
class UserDatabase {
private:
    User users[maxUsers];
    int lastUserId = 0;
    const std::string filename = "credentials.txt";
    //constructor 
    UserDatabase();

    //loads user info
    void loadUsers();

    //saves user info
    void saveUsers();

    //bubblesort for scoreboard
    void bubbleSort();

    //insertion sort for scoreboard top
    void insertionSort(int n);

    //binary search tree node class
    class BSTNode {
    public:
        User user;
        BSTNode* left;
        BSTNode* right;
        BSTNode(const User& user) : user(user), left(nullptr), right(nullptr) {}
    };

    //binary search tree used for managing users based on scores
    class BST {
    private:
        BSTNode* root;
        //insert user in tree
        void insert(BSTNode*& node, const User& user);

        //in-order traverse bst
        void inOrder(BSTNode* node, int n, int& count);

        //in-order to show all 
        void inOrderAll(BSTNode* node);

        //traverse to find users with score above n
        void viewUsersAboveThreshold(BSTNode* node, int threshold, int& count);

    public:
        // constructor to initialize the BST
        BST();
        ~BST();
        //insert user in tree
        void insert(const User& user);

        //in-order traverse
        void inOrder(int n);

        //in-order to show all
        void inOrderAll();

        //clear bst
        void clear(BSTNode* node);

        //traverse to find users with a score above n
        void viewUsersAboveThreshold(int threshold);
    };

    BST userBST;

public:
    //returns the singleton instance of UserDatabase
    static UserDatabase& getInstance();

    //registers a new user with username and password
    bool registerUser(const std::string& username, const std::string& password);

    //logs in for user
    bool loginUser(const std::string& username, const std::string& password);

    //updates the user score
    void updateUserScore(const std::string& username, int score);

    //displays the scoreboard of all users
    void viewScoreboard();

    //displays the top n users by points
    void viewTopByPoints(int n);

    //displays bar graph of the top30
    void displayTop30BarGraph();

    //displays users with scores above a threshold
    void viewUsersAboveThreshold(int threshold);

    //displays user rank
    int getUserRank(const std::string& username);

    //returns the last assigned user ID
    int getLastUserId() const;

    //gets the user identified by ID
    User getUserById(int id) const;
};

template <typename T>
class Stack {
private:
    // node structure for each element in the stack
    struct Node {
        T data;
        Node* next;
        // node constructor to initialize data and next pointer
        Node(T data) : data(data), next(nullptr) {}
    };
    Node* top;

public:
    // stack constructor to initialize the stack
    Stack();

    // stack destructor to clean up the stack
    ~Stack();

    // pushes a new element onto the stack
    void push(T data);

    // pops the top element from the stack
    void pop();

    // returns the top element of the stack without removing it
    T peek() const;

    // checks if the stack is empty
    bool isEmpty() const;
};

template <typename T>
class Queue {
private:
    // node structure for each element in the queue
    struct Node {
        T data;
        Node* next;

        // node constructor to initialize data and next pointer
        Node(T data) : data(data), next(nullptr) {}
    };
    Node* front, * rear;

public:
    // queue constructor to initialize the queue
    Queue();

    // queue destructor to clean up the queue
    ~Queue();

    // enqueues a new element to the rear of the queue
    void enqueue(T data);

    // dequeues the front element from the queue
    void dequeue();

    // returns the front element of the queue without removing it
    T peek() const;

    // checks if the queue is empty
    bool isEmpty() const;
};

// struct representing a card in the trivia game
struct Card {
    //card question
    std::string question;

    //answer to question
    std::string answers[4];

    //correct answer index
    int correctAnswer = 0;
};

class Deck {
private:

    //stack to hold the cards in the deck
    Stack<Card> cards;

public:
    //loads the deck from a file
    void loadDeck(const std::string& filename);

    //draws a card from the top of the deck
    Card drawCard();

    //discards a card to the deck
    void discardCard(const Card& card);

    //displays all discarded questions
    void displayDiscardedQuestions();

    //gets a specific discarded card by index
    Card getDiscardedCard(int index);

    //removes a card from the deck by index
    void removeCard(int index);

    //checks if the deck is empty
    bool isEmpty() const;
};

// struct representing a player in the trivia game
struct Player {
    //player attributes
    std::string name;
    int score;
    int rounds;

    //constructor to initialize player with a name
    Player(const std::string& name);

    //method for the player to answer a question
    void answerQuestion(const Card& card, int answer);

    //method for the player to discard a question
    void discardQuestion();
};

//game loop for a player
void gameLoop(UserDatabase& database, const std::string& username);

//admin loop for teacher 
void adminLoop(UserDatabase& database);

#endif // USERDATABASE_HPP
