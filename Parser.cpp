#include "Parser.h"
#include "split.h"
#include <fstream>

using namespace std;
void Parser::process(){
    ifstream my_ifstream = ifstream();
    my_ifstream.open(this->filename);
    string line;
    if (my_ifstream.is_open()){
        getline(my_ifstream, line);
        vector<string> first_line = split(line, ' ');
        int num_traces = stoi(first_line[1]);
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
            Node::Connection conn = Node::Connection(t, &node_map[n1], &node_map[n2]);
            node_map[n1].addConnection(conn);
            node_map[n2].addConnection(conn);
        }
        getline(my_ifstream, line);
        my_ifstream.close();
        vector<string> split_line = split(line, ' ');
        int start_node = stoi(split_line[0]);
        int end_node = stoi(split_line[1]);
        start_time = stoi(split_line[2]);
        int end_time = stoi(split_line[3]);
        for (auto& kv : node_map){
            kv.second.sortConnections();
        }

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
    node_map[start_node].setTraversed(true);
    int current_time = start_time;
    while (!path.empty() && path.back()->getId() != end_node){
        if (path.back()->hasValidConnection(current_time) && current_time <= end_time){
            Node::Connection conn = path.back()->getValidConnection(current_time);
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
    return connections;
}

int Parser::traverse(vector<Node*>&path, vector<Node::Connection>& connections, Node* node, Node::Connection* connection){
    path.push_back(node);
    node->setTraversed(true);
    connections.push_back(*connection);
    return connection->getTime();
}

int Parser::backtrack(vector<Node*>& path, vector<Node::Connection>& connections){
    path.back()->setTraversed(false);
    path.pop_back();
    if (!connections.empty()) {
        // if we're removing the last node, it doesn't have a connection
        connections.pop_back();
        if (!connections.empty())
            return connections.back().getTime();
        else {
            return start_time;
        }
    } else {
        return start_time;
    }
}

void Parser::output(vector<Node::Connection> path) {
    cout << path.size() << endl;
    for (Node::Connection conn: path){
        cout << conn;
    }

}
