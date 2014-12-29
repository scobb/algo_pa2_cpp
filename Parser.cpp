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

        vector<Node*> path = findPath(start_node, end_node, start_time, end_time);


    } else {
        cout << "Unable to open " << this->filename << endl;
    }



}

vector<Node*> Parser::findPath(int start_node, int end_node, int start_time, int end_time) {
    vector<Node*> path = vector<Node*>();
    vector<Node::Connection> connections = vector<Node::Connection>();
    path.push_back(&node_map[start_node]);
    int current_time = start_time;
    while (!path.empty() && path.back()->getId() != end_node && current_time <= end_time){
        if (path.back()->hasConnections()){
            Node::Connection conn = path.back()->getConnection();
            if (conn.getN1()->getId() == start_node && !conn.getN2()->isTraversed() && conn.getTime() <= end_time) {
                current_time = traverse(path, connections, conn.getN2(), &conn);
//                current_time = conn.getTime();
//                conn.getN2()->setTraversed(true);
//                path.push_back(conn.getN2());
//                connections.push_back(conn);
            } else if (conn.getN2()->getId() == start_node && !conn.getN1()->isTraversed() && conn.getTime() <= end_time){
                current_time = traverse(path, connections, conn.getN1(), &conn);
//                conn.getN1()->setTraversed(true);
//                path.push_back(conn.getN1());
//                connections.push_back(conn);
            }
        } else {
            backtrack(path, connections);
        }
    }
    return path;
}

int Parser::traverse(vector<Node*>&path, vector<Node::Connection>& connections, Node* node, Node::Connection* connection){
    path.push_back(node);
    node->setTraversed(true);
    connections.push_back(*connection);
    return connection->getTime();
}

void Parser::backtrack(vector<Node*>& path, vector<Node::Connection>& connections){
    while (!path.empty() && !path.back()->hasConnections()) {
        path.back()->setTraversed(false);
        path.pop_back();
        connections.pop_back();
    }
}
