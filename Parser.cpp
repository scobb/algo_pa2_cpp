#include "Parser.h"
#include "split.h"
#include <fstream>
#include <iostream>

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
            cout << "Adding " << n1 << " " << n2 << " " << t << endl;
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

//        cout << "start_node: " << start_node << endl;
//        cout << "end_node: " << end_node << endl;
//        cout << "start_time: " << start_time << endl;
//        cout << "end_time: " << end_time << endl;

        vector<Node::Connection> path = findPath(start_node, end_node, start_time, end_time);
        output(path);

    } else {
        cout << "Unable to open " << this->filename << endl;
    }
}

vector<Node::Connection> Parser::findPath(int start_node, int end_node, int start_time, int end_time) {
    vector<Node*> path = vector<Node*>();
    vector<Node::Connection> connections = vector<Node::Connection>();
    path.push_back(&node_map[start_node]);
    int current_time = start_time;
    while (!path.empty() && path.back()->getId() != end_node){
//        cout << "current_time: " << current_time << endl;
        if (path.back()->hasConnections() && current_time <= end_time){
//            cout << "path.back() is " << path.back()->getId() << endl;
            Node::Connection conn = path.back()->getConnection();
//            cout << "conn: " << conn << endl;
//            cout << "conn.getN2()->isTraversed(): " << conn.getN2()->isTraversed() << endl;
//            cout << "conn.getTime(): " << conn.getTime() << ", end_time: " << end_time << endl;
            if (conn.getN1()->getId() == path.back()->getId() && !conn.getN2()->isTraversed()
                    && conn.getTime() <= end_time && conn.getTime() >= current_time) {
                current_time = traverse(path, connections, conn.getN2(), &conn);
            } else if (conn.getN2()->getId() == path.back()->getId() && !conn.getN1()->isTraversed()
                    && conn.getTime() <= end_time && conn.getTime() >= current_time){
                current_time = traverse(path, connections, conn.getN1(), &conn);
            }
        } else {
            current_time = backtrack(path, connections);
        }
    }
//    cout << "connections.size(): " << (int)connections.size() << endl;
//    for (Node::Connection connection: connections){
//        cout << connection;
//    }
    return connections;
}

int Parser::traverse(vector<Node*>&path, vector<Node::Connection>& connections, Node* node, Node::Connection* connection){
//    cout << "Traversing node: " << path.back()->getId() << endl;
    path.push_back(node);
    node->setTraversed(true);
    connections.push_back(*connection);
    return connection->getTime();
}

int Parser::backtrack(vector<Node*>& path, vector<Node::Connection>& connections){
//    cout << "Backtracking on node: " << path.back()->getId() << endl;
//    cout << "connections.size(): " << connections.size() << endl;
//    cout << "path.size(): " << path.size() << endl;
    while (!path.empty() && !path.back()->hasConnections()) {
        path.back()->setTraversed(false);
        path.pop_back();
        if (!connections.empty()) {
            // if we're removing the last node, it doesn't have a connection
            connections.pop_back();
        }
    }
    if (path.empty()) {
        // we're done unsuccessfully, but still need to return an integer
        return -1;
    } else {
        // return the current time after using this connection
        return path.back()->getConnection().getTime();
    }
}

void Parser::output(vector<Node::Connection> path) {
    cout << path.size() << endl;
    for (Node::Connection conn: path){
        cout << conn;
    }

}
