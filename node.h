#ifndef _NODE_H
#define _NODE_H
#include <vector>

class Node{
public:
    class Connection{
    private:
        int time;
        Node* n1;
        Node* n2;
    public:
        Connection(){
            time = -1;
            n1 = NULL;
            n2 = NULL;
        }
        Connection(int time, Node *n1, Node *n2) : time(time), n1(n1), n2(n2) {
        }

        Connection(const Connection& other) {
            this->time = other.time;
            this->n1 = other.n1;
            this->n2 = other.n2;
        }

        int getTime() const {
            return time;
        }

        Node *getN1() const {
            return n1;
        }

        Node *getN2() const {
            return n2;
        }
    };
private:
    int id;
    bool traversed;
    std::vector<Connection> connections;

public:
    // bare constructor
    Node(){
        traversed=true;
        id = -1;
        connections = std::vector<Connection>();
    }
    // basic constructor
    Node(int id) : id(id) {
        traversed = false;
        connections = std::vector<Connection>();
    }
    // copy constructor
    Node(const Node& other) {
        this->id = other.id;
        traversed = false;
        connections = std::vector<Connection>();
    }
    int getId() const {
        return id;
    }

    bool isTraversed() const {
        return traversed;
    }

    void setTraversed(bool traversed) {
        Node::traversed = traversed;
    }

    void addConnection(Node* other, int time) {
        connections.push_back(Connection(time, this, other));
    }

    bool hasConnections(){
        return !connections.empty();
    }

    Connection getConnection() {
        Connection ret_val = connections.back();
        connections.pop_back();
        return ret_val;
    }

};
#endif