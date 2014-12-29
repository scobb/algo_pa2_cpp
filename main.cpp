#include <iostream>
#include "Parser.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " IN_FILE" << endl;
        return 1;
    }
    string filename = argv[1];
    Parser my_parser = Parser(filename);
    my_parser.process();
    return 0;
}