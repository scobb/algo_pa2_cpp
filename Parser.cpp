#include "Parser.h"
#include "split.h"
#include <fstream>

using namespace std;

void Parser::parse(ifstream* my_ifstream){
    if (my_ifstream->is_open()) {
        // local var to hold lines of the file
        string line;

        // get number of traces
        getline(*my_ifstream, line);
        vector<string> first_line = split(line, ' ');
        int num_traces = stoi(first_line[1]);

        // create node, connection objects
        for (int i = 0; i < num_traces; i++) {
            getline(*my_ifstream, line);
            vector<string> split_line = split(line, ' ');
            int n1 = stoi(split_line[0]);
            int n2 = stoi(split_line[1]);
            int t = stoi(split_line[2]);
            if (!node_map.count(n1)) {
                node_map[n1] = Node(n1);
            }
            if (!node_map.count(n2)) {
                node_map[n2] = Node(n2);
            }
            Node::Connection conn = Node::Connection(t, &node_map[n1], &node_map[n2]);
            node_map[n1].addConnection(conn);
            node_map[n2].addConnection(conn);
        }
        getline(*my_ifstream, line);
        my_ifstream->close();
        vector<string> split_line = split(line, ' ');

        // update the target members
        start_node = stoi(split_line[0]);
        end_node = stoi(split_line[1]);
        start_time = stoi(split_line[2]);
        end_time = stoi(split_line[3]);

        // sort connections by time to more closely match output
        for (auto &kv : node_map) {
            kv.second.sortConnections();
        }
    }
}

void Parser::process(){

    // parse input file to create objects
    ifstream my_ifstream = ifstream();
    my_ifstream.open(this->filename);
    if (my_ifstream.is_open()){
        // populate objects
        parse(&my_ifstream);

        // find the target path
        vector<Node::Connection> path = findPath();

        // output the target path
        output(path);

    } else {
        cout << "Unable to open " << this->filename << endl;
    }
}

vector<Node::Connection> Parser::findPath() {
    // vectors to hold nodes, connections (edges)
    vector<Node*> path = vector<Node*>();
    vector<Node::Connection> connections = vector<Node::Connection>();

    // add initial node to start
    path.push_back(&node_map[start_node]);
    node_map[start_node].setTraversed(true);
    int current_time = start_time;

    // have we arrived at the end_node or failed?
    while (!path.empty() && path.back()->getId() != end_node){
        // do we have a valid connection for when and where we are?
        if (path.back()->hasValidConnection(current_time, end_time)){

            // get that connection
            Node::Connection conn = path.back()->getValidConnection(current_time, end_time);

            // determine which part of the connection is the new node
            if (conn.getN1()->getId() == path.back()->getId() && !conn.getN2()->isTraversed()) {
                current_time = traverse(path, connections, conn.getN2(), &conn);
            } else if (conn.getN2()->getId() == path.back()->getId() && !conn.getN1()->isTraversed()){
                current_time = traverse(path, connections, conn.getN1(), &conn);
            }
        } else {
            // if no valid connections, we need to backtrack
            current_time = backtrack(path, connections);
        }
    }
    // this will either be a valid connection path or empty
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
