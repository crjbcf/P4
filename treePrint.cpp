//
// Created by chris on 4/26/2023.
//
#include "treePrint.h"
#include "node.h"
#include <iostream>
using namespace std;

void traversePreorder(node_t* root, int level)
{

    if (root == nullptr)
    {
        return;
    }
    else
    {
        printNodeInfo(root, level);
        traversePreorder(root->child1, level+1);
        traversePreorder(root->child2, level+1);
        traversePreorder(root->child3, level+1);
    }

}


void printNodeInfo(node_t* node, int level)
{
    string tokenNames[] = {"token_1", "token_2", "token_3", "EOF", "Scanner_Error"};
    cout << setw(level * 4) << " " <<  node->label << ":";



    if (!node->tokens.empty())
    {
        for (std::size_t i = 0; i < node->tokens.size(); i++)
        {
            Token token = node->tokens[i];

            cout << endl << setw(level * 4) << " " << "  " << tokenNames[token.tokenID] << " " << token.tokenInstance;
        }
    }
    cout << endl;
}