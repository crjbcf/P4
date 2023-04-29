//
// Created by chris on 4/28/2023.
//

#include "codeGen.h"





void genCode(node_t* node, fstream& outFile)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        string nodeLabel = node->label;
        //traverse child nodes
        if (nodeLabel == "S")
        {
            genCode(node->child1, outFile);
            genCode(node->child2, outFile);
            //cout <<" got here?";
            //genCode(node->child3);

            outFile << "STOP";
            // output vars and temp vars

            //j
        }
        else if(nodeLabel == "A") //node A returns empty or initializes a variable t1
        { //seems to be working as planned lets continue
            if (!node->tokens.empty())
            {
                string variableName = node->tokens[1].tokenInstance; //should be the variable name (2nd token of A nodes)
                initVar(variableName, outFile);
                genCode(node->child1, outFile);
            }
        }
        else if (nodeLabel == "J")
        {
            if (node->child1 != nullptr)
            {
                if (node->child1->label == "C") //C path
                { //i think i need to visit the second nad rd child first since this is an if and need to s
                    genCode(node->child2, outFile);
                    genCode(node->child3, outFile);
                    genCode(node->child1, outFile);
                }
                else if (node->child1->label == "D")
                {
                    genCode(node->child1, outFile);
                    genCode(node->child2, outFile);
                }
                else if (node->child1->label == "E")
                {
                    genCode(node->child1, outFile);
                    genCode(node->child2, outFile);

                }
                else if (node->tokens[0].tokenID == T1_tk && node->child1->label == "F") //t1 token path
                {
                    genCode(node->child1, outFile);
                    outFile << node->tokens[0].tokenInstance << endl;
                    genCode(node->child2, outFile);
                }
            }else
                return;


        }
        else if (nodeLabel == "E")
        {
            if (!node->tokens.empty())
            {
                outFile << "WRITE ";
            }

            genCode(node->child1, outFile);

        }
        else if (nodeLabel == "K")
        {
            if(!node->tokens.empty())
            {
                outFile << node->tokens[0].tokenInstance << endl;
            }
        }
        else if (nodeLabel == "M")
        {
            if (node->child1 != nullptr)
            {
                genCode(node->child2, outFile);
                genCode(node->child1, outFile);
            }
            else
                return; //empty?

        }
        else if (nodeLabel == "D")
        {
            if(!node->tokens.empty())
            {
                if ( node -> tokens[1].tokenID == T1_tk)
                {
                    outFile << "READ " << node->tokens[1].tokenInstance << endl; // should be the t1 token
                }

            }
        }
        else if (nodeLabel == "H")
        {
            if(!node->tokens.empty())
            {
                if (node -> tokens[0].tokenInstance == "}")  //minus
                {
                    outFile << "SUB ";
                }
                else if (node->tokens[0].tokenInstance == "{")
                {
                    outFile << "ADD ";
                }
            }
        }
        else if (nodeLabel == "F")
        {
            //may need to travel children out of order?
            genCode(node->child1, outFile);

            outFile << "STORE ";
        }
        else if (nodeLabel == "G")
        {
            if (node->child1 != nullptr && node->child2 != nullptr) //first path
            {
                outFile << "LOAD ";
                genCode(node->child1, outFile);
                genCode(node->child2, outFile);
            }
            else
            {
                if(node->child1 != nullptr)
                {
                    outFile << "LOAD ";
                    genCode(node->child1, outFile);
                }
            }
        }
        else if (nodeLabel == "L")
        {
            if (node->child1 != nullptr)
            {
                genCode(node->child1, outFile);
            }
            else
                return;

        }
        else if (nodeLabel == "C")
        {

        }
    }
}

void initVar(string var, fstream& outFile)
{
    outFile << "LOAD 0" << endl;
    outFile << "STORE " << var << endl;
}

