#ifndef _PARSER_H
#define _PARSER_H
#include <string>
#include <map>
#include <vector>
#include "node.h"

class Parser {
private:
    std::string filename;
    int backtrack(std::vector<Node *>& path, std::vector<Node::Connection>& connections);
    void output(std::vector<Node::Connection>);
    int traverse(std::vector<Node *> &path, std::vector<Node::Connection> &connections, Node *node, Node::Connection *connection);
public:
    std::map<int, Node> node_map;
    Parser(std::string filename) {
        this->filename = filename;
        this->node_map = std::map<int, Node>();
    }

    void process();
    std::vector<Node::Connection> findPath(int start_node, int end_node, int start_time, int end_time);

};
#endif