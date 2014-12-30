#ifndef _NODE_H
#define _NODE_H
#include <vector>
#include <iostream>

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
        friend std::ostream& operator<<(std::ostream& os, const Node::Connection& conn);
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

    void addConnection(Connection conn) {
        connections.push_back(conn);
    }

    void sortConnections(){
        std::sort(connections.begin(), connections.end());
    }

    bool hasValidConnection(int time) {
        if (connections.empty()){
            return false;
        } else {
            for (Connection conn: connections){
                if (conn.getTime() >= time){
                    return true;
                }
            }
        }
        return false;
    }

    Connection getValidConnection(int time) {
        for (std::vector<Connection>::iterator iter = connections.begin(); iter != connections.end(); iter++){
            if (iter->getTime() >= time){
                Connection tmp = *iter;
                connections.erase(iter);
                return tmp;
            }
        }
        return Connection();
    }
};
std::ostream& operator<<(std::ostream& os, const Node::Connection& conn);
bool operator<(const Node::Connection& left, const Node::Connection& right);
#endif