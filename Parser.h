#ifndef _PARSER_H
#define _PARSER_H
#include <string>
#include <map>
#include "node.h"

class Parser {
private:
    std::string filename;
public:
    std::map<int, Node> node_map;
    Parser(std::string filename) {
        this->filename = filename;
        this->node_map = std::map<int, Node>();
    }

    void process();
    bool findPath(int start_node, int end_node, int start_time, int end_time);
};
#endif