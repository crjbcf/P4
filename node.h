//
// Created by chris on 4/26/2023.
//

#ifndef P4_NODE_H
#define P4_NODE_H

#include "scanner.h"
#include <vector>

struct node_t
{
    string label;
    vector<Token> tokens;

    node_t* child1  = nullptr;
    node_t* child2 = nullptr;
    node_t* child3 = nullptr;

};


#endif //P4_NODE_H
