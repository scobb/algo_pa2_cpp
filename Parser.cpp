#include "Parser.h"
#include "split.h"
#include <fstream>
#include <iostream>
#include "node.h"

using namespace std;
void Parser::process(){
    ifstream my_ifstream = ifstream();
    my_ifstream.open(this->filename);
    string line;
    if (my_ifstream.is_open()){
        getline(my_ifstream, line);
        vector<string> first_line = split(line, ' ');
        int num_devices = stoi(first_line[0]);
        int num_traces = stoi(first_line[1]);
        cout << "num_devices: " << num_devices << endl;
        cout << "num_traces: " << num_traces << endl;
        for (int i = 0; i < num_traces; i++) {
            getline(my_ifstream, line);
            vector<string> split_line = split(line, ' ');
            int n1 = stoi(split_line[0]);
            int n2 = stoi(split_line[1]);
            int t = stoi(split_line[2]);
            if (!node_map.count(n1)){
                node_map[n1] = Node(n1);
            }
            if (!node_map.count(n2)){
                node_map[n2] = Node(n2);
            }
            node_map[n1].addConnection(&node_map[n2], t);
            node_map[n2].addConnection(&node_map[n1], t);
        }
        getline(my_ifstream, line);
        my_ifstream.close();
        vector<string> split_line = split(line, ' ');
        int start_node = stoi(split_line[0]);
        int end_node = stoi(split_line[1]);
        int start_time = stoi(split_line[2]);
        int end_time = stoi(split_line[3]);

        cout << "start_node: " << start_node << endl;
        cout << "end_node: " << end_node << endl;
        cout << "start_time: " << start_time << endl;
        cout << "end_time: " << end_time << endl;



    } else {
        cout << "Unable to open " << this->filename << endl;
    }



}

bool Parser::findPath(int start_node, int end_node, int start_time, int end_time) {
    vector<Node*> path = vector<Node*>();
    Node* current_node = &node_map[start_node];
    path.push_back(current_node);
    int current_time = start_time;
    while (current_node->getId() != end_node){
        while (current_node->hasConnections()){
            Node::Connection conn = current_node->getConnection();
            if (conn.getN1()->getId() == start_node and !conn.getN2()->isTraversed() and conn.getTime() <= end_time) {
                current_time = conn.getTime();
                conn.getN2()->setTraversed(true);
                current_node = &node_map[conn.getN2()->getId()];
            } else if (conn.getN2()->getId() == start_node and !conn.getN1()->isTraversed() and conn.getTime() <= end_time){
                current_time = conn.getTime();
                conn.getN1()->setTraversed(true);
                current_node = &node_map[conn.getN1()->getId()];
            }
        }

    }
    return false;
}
