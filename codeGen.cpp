//
// Created by chris on 4/28/2023.
//

#include "codeGen.h"

vector<string> labels;
vector<string> variables;
static int labelCount = 0;

string makeLabel()
{
    labelCount++;
    string lb = "LABEL";
    string num = to_string(labelCount);

    lb += num;
    labels.push_back(lb);
    return lb;
}



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
            //genCode(node->child3);

            outFile << "STOP\n";
            // output vars
            if (!variables.empty())
            {
                for (int i = 0; i < variables.size(); i++)
                {
                    outFile << variables[i] << " 0\n";
                }
            }
        }
        else if(nodeLabel == "A") //node A returns empty or initializes a variable t1
        {
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
                {
                    genCode(node->child1, outFile); //visit child 1 first (C)
                    genCode(node->child2, outFile); //visit child 2 (J)
                    string label = makeLabel();
                    outFile << "BR " << label << endl;
                    outFile << labels[0] << ": ";
                    labels.erase(labels.begin());
                    genCode(node->child3, outFile);
                    outFile << labels[0] << ": NOOP\n";
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
                else if (node->tokens[0].tokenID == T1_tk && node->child1->label == "F")
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
                return;

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
                else if (node->tokens[0].tokenInstance == "{") //PLUS
                {
                    outFile << "ADD ";
                }
            }
        }
        else if (nodeLabel == "F")
        {
            genCode(node->child1, outFile);

            outFile << "STORE ";
        }
        else if (nodeLabel == "G")
        {
            if (node->child1 != nullptr && node->child2 != nullptr)
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
            string label;
            outFile << "LOAD ";
            genCode(node->child1, outFile);
            label = makeLabel();
            outFile << "BRZNEG " << label << endl;
        }
    }
}

void initVar(string var, fstream& outFile)
{
    outFile << "LOAD 0" << endl;
    outFile << "STORE " << var << endl;
    variables.push_back(var);
}

