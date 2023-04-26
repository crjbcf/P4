//
// Created by chris on 4/26/2023.
//

#ifndef P4_PARSER_H
#define P4_PARSER_H

#include "node.h"
#include "scanner.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
using namespace std;

extern int lineNum;
node_t* parser(fstream &file);
node_t* getNode(string label);

void error();


#endif //P4_PARSER_H
