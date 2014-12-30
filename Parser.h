#ifndef _PARSER_H
#define _PARSER_H
#include <string>
#include <map>
#include <vector>
#include "node.h"

class Parser {
private:
    // members
    std::string filename;
    int start_time, end_time, start_node, end_node;

    // private methods
    int backtrack(std::vector<Node *>& path, std::vector<Node::Connection>& connections);
    void output(std::vector<Node::Connection>);
    void parse(std::ifstream*);
    std::vector<Node::Connection> findPath();
    int traverse(std::vector<Node *> &path, std::vector<Node::Connection> &connections, Node *node, Node::Connection *connection);
public:
    // public members
    std::map<int, Node> node_map;

    // public methods
    Parser(std::string filename) {
        this->filename = filename;
        this->node_map = std::map<int, Node>();
    }

    void process();

};
#endif