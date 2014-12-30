#include <ostream>
#include "node.h"
using namespace std;
std::ostream& operator<<(std::ostream& os, const Node::Connection& conn){
    os << conn.getN1()->getId() << " " << conn.getN2()->getId() << " " << conn.getTime() << endl;
    return os;
}

bool operator<(const Node::Connection &left, const Node::Connection &right) {
    return left.getTime() < right.getTime();
}
