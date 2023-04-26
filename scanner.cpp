//
// Created by chris on 4/26/2023.
//

#include "scanner.h"

const int SIZE = 9;

Token scanner(fstream& file, int &lineNum)
{
    int currentState = 0;
    int nextState, charType;
    char lookahead;
    string tokenString;
    Token token;
    int ** table;
    table = buildFSATable();

    //while the currentState is not Final and not End of File
    while (currentState >= 0 &&  currentState < 1000)
    {
        lookahead = file.get(); //set lookahead;

        //filter out comments
        if (lookahead == '&')
        {
            while(lookahead != '\n')
                lookahead = file.get();
            //keep processing chars until we get to end of comment

            lineNum++;//increase line number
            lineNum--;//decrease for extra line number that occurs on tokens before and after a comment
        }

        //here we are out of comment or no comment
        charType = characterLookup(lookahead); //find out mapped value of the character in the lookahead (which type)
        if (charType == -1) //if characterLookup returned -1, it means it was invalid char for our language
        {
            cout << "Invalid character detected terminating program";
            exit(0);
        }

        nextState = table[currentState][charType]; //find out what the next state should be using FSA table

        //check what to do depending on value of next State
        //if it is an error state lets output an Error Message
        if (nextState < 0) //since -1000 is my error state
        {
            cout << "Error: Line# " << lineNum << " invalid syntax character: " << static_cast<char>(lookahead) << endl;
            token.lineNumber = lineNum;
            token.tokenID = ERR_token;
            token.tokenInstance = tokenString;

            return token;
        }
        else if(nextState >= 1000) //state is a final state return the token
        {
            if(nextState == 1000) //it is an EOF token
            {
                file.unget();
                token.tokenID = EOF_token;
                token.tokenInstance = "EOF";
                return token;
            }
            else if(nextState != 1000) //its a final state find which final state and return
            {
                file.unget(); //
                token = checkTokens(nextState, tokenString); //function checks which final state token to return
                token.lineNumber = lineNum;

                return token;
            }

        }
        else //not a final state or error so keep reading in characters.
        {
            if(!isspace(lookahead))
            {
                currentState = nextState;
                tokenString += lookahead;
            }
        }

        if (lookahead == '\n')
        {
            //cout << "Current line Num: " << lineNum;
            lineNum++;
            //cout << "incremeneted: " << lineNum;
        }
    }

    return token;
}

struct tokenMap
{
    int key;
    TokenID idName;
};

tokenMap tokenTypeLookup[5] =
        {
                {1001, T1_tk},
                {1002, T2_token},
                {1003, T3_token},
                {1000, EOF_token},
                {-1000, ERR_token}
        };

Token checkTokens(int value, string input)
{
    Token token;

    for (int i = 0; i < 5; i++)
    {
        if (value == tokenTypeLookup[i].key)
        {
            token.tokenID = tokenTypeLookup[i].idName;
            token.tokenInstance = input;
        }
    }

    return token;
}

//this function will look at the lookahead character and return its mapped value
int characterLookup(int lookahead)
{
    if (isalpha(lookahead)) //if it is a letter
    {
        return 0;
    }
    else if(lookahead == 0)
    {
        return 1;
    }
    else if (isdigit(lookahead) && lookahead != 0)//non zero digit
    {
        return 2;
    }
    else if (lookahead == '_')
    {
        return 3;
    }
    else if (lookahead == '+' || lookahead == '-')
    {
        return 4;
    }
    else if (lookahead == '.')
    {
        return 5;
    }
    else if (lookahead == '^' || lookahead == '~' || lookahead == '*' || lookahead == '|'
             || lookahead == '[' || lookahead == ']' || lookahead == '{' || lookahead == '}')
    {
        return 6;
    }
    else if(isspace(lookahead))
    {
        return 7;
    }
    else if (lookahead == std::char_traits<char>::eof())
    {
        return 8;
    }
    else //invalid character
    {
        return -1;
    }
}

int** buildFSATable() //creates and returns my 2D array for the FSA table
{
    int**  table = new int*[SIZE];

    for(int i = 0; i < SIZE; i++)
    {
        table[i] = new int[SIZE];
    }
    //zero state
    table[0][0] = 1;
    table[0][1] = -1000;
    table[0][2] = -1000;
    table[0][3] = -1000;
    table[0][4] = 5;
    table[0][5] = -1000;
    table[0][6] = 4;
    table[0][7] = 0;
    table[0][8] = 1000;

    //1 state
    table[1][0] = -1000;
    table[1][1] = -1000;
    table[1][2] = -1000;
    table[1][3] = 2;
    table[1][4] = -1000;
    table[1][5] = -1000;
    table[1][6] = -1000;
    table[1][7] = -1000;
    table[1][8] = -1000;

    //2state
    table[2][0] = -1000;
    table[2][1] = -1000;
    table[2][2] = 3;
    table[2][3] = -1000;
    table[2][4] = -1000;
    table[2][5] = -1000;
    table[2][6] = -1000;
    table[2][7] = -1000;
    table[2][8] = -1000;

    table[3][0] = 1001;
    table[3][1] = 3;
    table[3][2] = 3;
    table[3][3] = 1001;
    table[3][4] = 1001;
    table[3][5] = 1001;
    table[3][6] = 1001;
    table[3][7] = 1001;
    table[3][8] = 1001;

    table[4][0] = 1002;
    table[4][1] = 1002;
    table[4][2] = 1002;
    table[4][3] = 1002;
    table[4][4] = 1002;
    table[4][5] = 1002;
    table[4][6] = 1002;
    table[4][7] = 1002;
    table[4][8] = 1002;

    table[4][0] = 1002;
    table[4][1] = 1002;
    table[4][2] = 1002;
    table[4][3] = 1002;
    table[4][4] = 1002;
    table[4][5] = 1002;
    table[4][6] = 1002;
    table[4][7] = 1002;
    table[4][8] = 1002;

    table[5][0] = -1000;
    table[5][1] = 6;
    table[5][2] = 6;
    table[5][3] = -1000;
    table[5][4] = -1000;
    table[5][5] = -1000;
    table[5][6] = -1000;
    table[5][7] = -1000;
    table[5][8] = -1000;

    table[6][0] = 1003;
    table[6][1] = 6;
    table[6][2] = 6;
    table[6][3] = 1003;
    table[6][4] = 1003;
    table[6][5] = 7;
    table[6][6] = 1003;
    table[6][7] = 1003;
    table[6][8] = 1003;

    table[7][0] = -1000;
    table[7][1] = 8;
    table[7][2] = 8;
    table[7][3] = -1000;
    table[7][4] = -1000;
    table[7][5] = -1000;
    table[7][6] = -1000;
    table[7][7] = -1000;
    table[7][8] = -1000;

    table[8][0] = 1003;
    table[8][1] = 8;
    table[8][2] = 8;
    table[8][3] = 1003;
    table[8][4] = 1003;
    table[8][5] = 1003;
    table[8][6] = 1003;
    table[8][7] = 1003;
    table[8][8] = 1003;

    return table;
}