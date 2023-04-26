//
// Created by chris on 4/26/2023.
//

#include "parser.h"

//set global token
static Token tk;


int lineNum = 1;


node_t* getNode(string label)
{
    node_t* node = new node_t;
    node->label = label;

    return node;
}

void error() {
    cout << "There was an error.";
    cout << "\ntoken: " << tk.tokenInstance << " line number: " << lineNum;
    exit(0); //terminate program
}

//<K> -> t1 || t3
//I think its okay
node_t* K(fstream& file)
{
    node_t* p = getNode("K");

    if (tk.tokenID == 0) //t1 == 0
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);
        return p;
    }
    else if (tk.tokenID == 2) //t3 == 2
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);
        return p;
    }
    else
    {
        cout << "Expected token 1 or token 3\n";
        error(); // expected T1 or T3
    }
    return nullptr;
}




//<H> -> { || }
node_t* H(fstream& file)
{
    node_t* p = getNode("H");
    if (tk.tokenID == 1 && tk.tokenInstance == "{") //t2 == 1
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);

        return p; //end of rule
    }
    else if (tk.tokenID == 1 && tk.tokenInstance == "}")
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);

        return p; //end of rule
    }
    else
    {
        cout << "Expected token 2\n";
        error(); //T2 expected
    }
    return nullptr;
}



//<M> -> <K><H> || empty
node_t* M(fstream& file)
{
    node_t* p = getNode("M");
    if (tk.tokenID == 0 || tk.tokenID == 2) //since Ks first set is t1 or t3
    {
        p->child1 = K(file); // predict K

        p->child2 = H(file); //predict H

        return p; //end of production
    }
    else
    {
        return p;//empty
    }
}

//<G> -> <K><M>
node_t* G(fstream& file)
{
    node_t* p = getNode("G");

    p->child1 = K(file); // predict K

    p->child2 = M(file); // predict M

    return p; //end of production rule
}


//<F> -> |<G>~
//updated
node_t* F(fstream& file)
{
    node_t* p = getNode("F");

    if (tk.tokenID == 1 && tk.tokenInstance == "|")
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);

        p->child1 = G(file); //predict G

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            // p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            return p; // end of production rule
        }
        else
        {
            cout << "Expected token 2 '~'";
            error(); //expected T2
        }
    }
    else
    {
        cout << "Expected token 2 '|'";
        error(); //expected T2
    }

    return nullptr;
}

//<E> -> ]<K>~
//updated
node_t* E(fstream& file)
{
    node_t* p = getNode("E");

    if (tk.tokenID == 1 && tk.tokenInstance == "]")
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);

        p->child1 = K(file); //predict K

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            return p; // end of production rule
        }
        else//keep this error maybe
        {
            cout << "Expected token 2 '~'";
            error(); //expected T2
        }
    }
    else //keep this error
    {
        cout << "Expected token 2 ']'";
        error(); //expected T2
    }

    return nullptr;
}

//<D> -> [t1
//updated
node_t* D(fstream& file)
{
    node_t* p = getNode("D");

    if (tk.tokenID == 1 && tk.tokenInstance == "[") //predict T2
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);

        if (tk.tokenID == 0) //predict T1
        {
            p->tokens.push_back(tk);
            tk = scanner(file, lineNum);
            return p;
        }
        else
        {
            cout << "Expected token 1\n";
            error(); //expected T1
        }
    }
    else
    {
        cout << "Expected token 2 '['\n";
        error();//expected T2
    }

    return nullptr;
}

//<C> -> *<K>
//fixed?
node_t* C(fstream& file)
{
    node_t* p = getNode("C");

    if (tk.tokenID == 1 && tk.tokenInstance == "*")//* is a T2 token
    {
        p->tokens.push_back(tk);
        tk = scanner(file, lineNum);
        p->child1 = K(file); //predicting K

        return p; // end of production rule
    }//maybe error here
    else
    {
        cout << "Expected token 2 '*'";
        error();
    }
    return nullptr;
}

//<L> -> <D>~ || <E>~ || <F>~ || empty
node_t* L(fstream& file)
{
    node_t* p = getNode("L");
    //first check for T2 token (first sets for non terms are all t2 tokens
    if (tk.tokenID == 1 && tk.tokenInstance == "[")
    {
        p->child1 = D(file); // predict D

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk); //dnt save
            tk = scanner(file, lineNum); //consume

            return p; //end of rule
        }
        else
        {
            cout << "Expected token 2 '~'";
            error(); //expected ~
        }
    }
    else if (tk.tokenID == 1 && tk.tokenInstance == "]") //E path
    {
        p->child1 = E(file); //predict E

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            return p; //end of rule
        }
        else
        {
            cout << "Expected token 2 '~' \n";
            error(); //expected ~
        }
    }
    else if (tk.tokenID == 1 && tk.tokenInstance == "|") //F path
    {
        p->child1 = F(file); //predict f

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            return p; //end of rule
        }
        else
        {
            cout << "Expected token 2 '~' \n";
            error(); //expected ~
        }
    }
    else
    {
        return p; //empty token
    }

    return nullptr;
}


node_t* J(fstream &file)
{
    node_t* p = getNode("J");
    //first check for t1 or t2 in current token if not one of these we are in empty case
    if (tk.tokenID == 1 && tk.tokenInstance == "*") //its one of the first few rules
    {
        p->child1 = C(file); //predict C

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            p->child2 = J(file); //predict J

            if (tk.tokenID == 1 && tk.tokenInstance == "~")
            {

                //p->tokens.push_back(tk);
                tk = scanner(file, lineNum);

                p->child3 = L(file); //predict L

                return p; //end of production rule 1
            }//maybe else error here
            else
            {
                cout << "Expected token 2 '~'\n";
                error();//expected ~
            }
        }//maybe else error here
        else
        {
            cout << "Expected token 2 '~' \n";
            error(); //expected ~
        }
    }
    else if (tk.tokenID == 1 && tk.tokenInstance == "[") //process <D> rule
    {
        p->child1 = D(file); //predict D

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            p->child2 = J(file); //predict J

            return p ; //end of production 2
        }
        else
        {
            cout << "Expected token 2 '~'\n";
            error();
        }
    }
    else if (tk.tokenID == 1 && tk.tokenInstance == "]") //process <E> rule
    {
        p->child1 = E(file); //predict E

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {
            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            p->child2 = J(file); //predict J

            return p; //end of rule 3
        }
        else
        {
            cout << "Expected token 2 '~' \n";
            error();
        }
    }
    else if (tk.tokenID == 0) //its the 4th rule
    {
        p->tokens.push_back(tk); //need to consume
        tk = scanner(file, lineNum);

        p->child1 = F(file); //predict F

        if (tk.tokenID == 1 && tk.tokenInstance == "~")
        {

            //p->tokens.push_back(tk);
            tk = scanner(file, lineNum); //consume

            p->child2 = J(file); //predict J

            return p; //end of rule 4
        }
        else
        {
            cout << "Expected token 2 '~' \n";
            error();
        }
    }
    else
    {
        return p; //empty token
    }

    return nullptr;
}

//A -> ^t1<A> || empty
node_t* A(fstream &file)
{
    node_t* p = getNode("A");

    if (tk.tokenID == 1 && tk.tokenInstance == "^") //^ is a t2 token == 1
    {
        p->tokens.push_back(tk);

        tk = scanner(file, lineNum);

        if (tk.tokenID == 0) //t1 == 0
        {
            p->tokens.push_back(tk);
            tk = scanner(file, lineNum);

            p->child1 = A(file); //predicting A

            return p;
        }
    }
    else
    {

        return p; //empty token
    }


    return nullptr;
}


// <S> -> <S> -> <A><J>
node_t* S(fstream &file)
{
    node_t* p = getNode("S");

    p->child1 = A(file); //predicting A

    p->child2 = J(file); //predicting J"

    return p; //end of production
}


node_t* parser(fstream &file)
{
    node_t* root;
    tk = scanner(file, lineNum);

    root = S(file); //start with starting nonTerm S

    if (tk.tokenID != EOF_token)
        error();

    return root;
}
