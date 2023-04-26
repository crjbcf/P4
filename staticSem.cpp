//
// Created by chris on 4/26/2023.
//

#include "staticSem.h"

vector<string> symbolTable;

void staticSem(node_t* node)
{
    if (node!= nullptr)
    {
        if (!node->tokens.empty())
        {
            for (size_t i = 0; i < node->tokens.size(); i++)
            {
                /*
                 * Below I am saying if node token id is tk2 and this tk instance is the ^ also
                 * if the next token directly after this is a t1 token and the current token is not the last token
                 * then it looks to be an identifier declaration so insert the token instance of the t1 token
                 */
                if ((node->tokens[i].tokenID == T2_token && node->tokens[i].tokenInstance == "^") &&
                    node->tokens[i + 1].tokenID == T1_tk &&  (i != (node->tokens.size() - 1))){
                    insert(node->tokens[i + 1].tokenInstance); //attempt to insert
                }
                else if (node->tokens[i].tokenID == T1_tk) //if we come across an IDentifier tk verify it
                {
                    if(!verify(node->tokens[i].tokenInstance)) //if verify returns false output error message and exit.
                    {
                        cout << "Error Identifier was not defined: " <<  node->tokens[i].tokenInstance;
                        exit(0);
                    }
                }
            }
        }
    }
    else
        return;

    staticSem(node->child1);
    staticSem(node->child2);
    staticSem(node->child3);

}


void insert(string tkInstance)
{
    if (verify(tkInstance))
    {
        cout << "Error Identifier " << tkInstance << " already declared";
        exit(0);
    }

    symbolTable.push_back(tkInstance);
}

bool verify(string tkInstance)
{
    for (size_t i = 0; i < symbolTable.size(); i++)
    {
        if (tkInstance == symbolTable[i])
        {
            return true;
        }
    }

    return false;
}

void printTable()
{
    int i = 1;
    cout << "\n\tSymbol Table\n---------------------------- \n";
    cout << "Number\t\tSymbol\n";
    for(string symbol : symbolTable)
    {
        cout << setw(16) << left <<  i <<  symbol << endl;
        i++;
    }
}