#ifndef _SPLIT_H
#define _SPLIT_H

#include <vector>
#include <string>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

#endif