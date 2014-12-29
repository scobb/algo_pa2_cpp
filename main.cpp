#include <iostream>
#include "Parser.h"
using namespace std;

int main() {
    string filename = "/Users/scobb/ClionProjects/Algo_PA2/input0.txt";
    Parser my_parser = Parser(filename);
    my_parser.process();
    return 0;
}