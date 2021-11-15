/* Incomplete Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Fall 2021
*/

#include "parse.h"


//A map container that keeps a record of the defined variables in the parsed program
map<string, bool> defVar;
//A map container that keeps a record for each declared variable in the parsed program and its corresponding type
map<string, Token> SymTable;

namespace Parser {
    bool pushed_back = false;
    LexItem	pushed_token;

    static LexItem GetNextToken(istream& in, int& line) {
        if( pushed_back ) {
            pushed_back = false;
            return pushed_token;
        }
        return GetNextToken(in, line);
    }

    static void PushBackToken(LexItem & t) {
        if( pushed_back ) {
            abort();
        }

        pushed_back = true;
        pushed_token = t;
    }

}
/* Global Variables  */
static int error_count = 0;
/*End of Global Variables*/

/*Functions*/
int ErrCount(){
    return error_count;
}

void ParseError(int line, string msg){
    ++error_count;
    cout << line << ": " << msg << endl;
}

//Decl = Type IdentList
//Type = INTEGER | REAL | CHAR
bool DeclStmt(istream& in, int& line) {
    bool status = false;
    LexItem tok;
    //cout << "in Decl" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    if(t == INT || t == FLOAT ) {
        status = IdentList(in, line, t);
        //cout<< "returning from IdentList" << " " << (status? 1: 0) << endl;
        if (!status){
            ParseError(line, "Incorrect variable in Declaration Statement.");
            return status;
        }
    }else{
        Parser::PushBackToken(t);
        ParseError(line, "Incorrect Type.");
        return false;
    }
    return true;
}

bool Stmt(istream& in, int& line){
    bool status=true;
    //cout << "in Stmt" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    switch( t.GetToken() ) {
        case INT: case FLOAT:
            Parser::PushBackToken(t);
            status = DeclStmt(in, line);
            if(!status){
                ParseError(line, "Incorrect Declaration Statement.");
                return status;
            }
            break;
        case IF: case WRITE: case IDENT:
            Parser::PushBackToken(t);
            status = ControlStmt(in, line);
            if(!status){
                ParseError(line, "Incorrect control Statement.");
                return status;
            }
            break;
        default:
            Parser::PushBackToken(t);
    }
    return status;
}

//WriteStmt:= wi, ExpreList
bool WriteStmt(istream& in, int& line) {
    LexItem t;
    //cout << "in WriteStmt" << endl;

    bool ex = ExprList(in, line);

    if( !ex ) {
        ParseError(line, "Missing expression after Write");

    }

    //Evaluate: print out the list of expressions values

    return ex;
}

bool Prog(istream& in, int& line){
    bool parse = false;

    LexItem t = Parser::GetNextToken(in,line);
    if(t.GetToken() == PROGRAM){
        t = Parser::GetNextToken(in,line);
        if(t.GetToken() == IDENT){
            parse = StmtList(in, line);
            if(!parse){
                ParseError(line,"Incorrect Syntax in the Program ");
                return parse;
            }
            LexItem done = Parser::GetNextToken(in,line);
            if(done.GetToken()== END){
                done = Parser::GetNextToken(in,line);
                if(done.GetToken()== PROGRAM){
                    return parse;
                }else{
                    parse = false;
                    ParseError(line, "Missing PROGRAM at the End");
                }
            }else{
                parse = false;
                ParseError(line, "Missing END at end of program.");
            }
        }else{
            ParseError(line, "Missing Program Name.");
        }
    }else{
        ParseError(line, "Missing PROGRAM.");
    }

    return parse;
}

bool StmtList(istream& in, int& line){
    bool stmt = Stmt(in,line);
    LexItem t = Parser::GetNextToken(in, line);

    if(t.GetToken() != SEMICOL){
        ParseError(line, "Missing a semicolon.");//FIGURE OUT: how you can get here in prog2 error case
        return stmt;
    }

    if(!stmt){
        return stmt;
    }

    //CHECK THIS CODE BELOW

    LexItem temp = Parser::GetNextToken(in,line);
    if(temp.GetToken() == DONE){
        Parser::PushBackToken(temp);
        return stmt;
    } else{
        Parser::PushBackToken(temp);
        stmt = StmtList(in,line); // no idea if this logic works AT ALL
    }

    return stmt;
}

bool ControlStmt(istream& in, int& line){
    bool status = true;

    LexItem t = Parser::GetNextToken(in,line);

    switch(t.GetToken()){
        case IF:
            status = IfStmt(in,line);

            if(!status){
                ParseError(line,"Incorrect control Statement.");//double check this error msg
                return status;
            }
            break;

        case WRITE:
            status = WriteStmt(in,line);
            if(!status){
                ParseError(line,"Incorrect control Statement.");//double check this error msg
                return status;
            }
            break;

        case IDENT:
            status = AssignStmt(in, line);
            if(!status){
                ParseError(line,"Incorrect control Statement.");//double check this error msg
                return status;
            }
            break;

        default:
            Parser::PushBackToken(t);//idk if i call an error here
            ParseError(line, "Incorrect control Statement.");//double check this error msg
    }

    return status;
}
/*
 * IfStmt function is not finished
 */
bool IfStmt(istream& in, int& line)
{
    bool status = true;
    LexItem t = Parser::GetNextToken(in, line);

    if( t.GetToken() == LPAREN )
    {
        status = LogicExpr(in,line);
        if(!status)
        {
            if (t.GetToken() == RPAREN)
            {
                status = ControlStmt(in, line);
                if(!status)
                    return status;
            }
            else if((t.GetToken() != RPAREN))
            {
                ParseError(line, "missing parentheses");
                return false;
            }

        }
    }
    else if(t.GetToken() != LPAREN )
    {
        ParseError(line, "missing parentheses");
        return false;
    }
    return status;
}

bool IdentList(istream& in, int& line, LexItem tok){
    bool x = true;
    LexItem temp = Parser::GetNextToken(in, line);
    //if the ident isnt found
    if(defVar.find(tok.GetLexeme()) == defVar.end()){
        defVar.insert({tok.GetLexeme(), true});//then add it to the maps
        SymTable.insert({tok.GetLexeme(), tok.GetToken()});
    } else {//if the ident IS FOUND
        defVar[tok.GetLexeme()] = false; // due to the logic
        Parser::PushBackToken(tok);//pushes back the error var
        ParseError(line, "Variable Redefinition");//variable defined for the 2nd time
        return false;//so you can return le error
    }

    if(temp.GetToken() == SEMICOL){
        Parser::PushBackToken(temp);
        return x;
    } else if (temp.GetToken() == COMMA){
        temp = Parser::GetNextToken(in, line);
        x = IdentList(in, line, temp);
    } else if(temp.GetToken() == IDENT){//idk being extra and double checking it
        x = IdentList(in, line, temp);
    }

    return x;
}

bool Var(istream& in, int& line){
    bool status = true;

    LexItem t = Parser::GetNextToken(in,line);

    if(t.GetToken()== IDENT){
        status = IdentList(in,line,t);
    } else{
        ParseError(line, "missing ident token");
        Parser::PushBackToken(t);
        return false;
    }

    return status;
}

bool AssignStmt(istream& in, int& line){
    bool var = Var(in, line);
    bool expr = false;
    LexItem t = Parser::GetNextToken(in,line);

    if(t.GetToken() == ASSOP){
        expr= Expr(in, line);
    }

    if(var && expr){
        return true;
    }
    return false;
}

/*
 * ExprList function is not finished
 */

bool ExprList(istream& in, int& line) {


    bool status = Expr(in, line);

    if (!status) {
        return status;
    }
    LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken() == COMMA){
        status = ExprList(in, line);
    } else{
        Parser::PushBackToken(t);
    }
    return status;
}

/*
 * LogicExpr function is not finished
 */

bool LogicExpr(istream& in, int& line){
    bool status = Expr(in, line);
    if(!status)
        return status;

    LexItem t = Parser::GetNextToken(in, line);

    switch(t.GetToken()){
        case EQUAL: case GTHAN:
            status = Expr(in, line);
            if(!status)
                return status;
            break;
        default:
            ParseError(line, "wrong conditional operator");
            status = false;
    }
    return status;
}
/*
 * Expr function is not finished
 */
bool Expr(istream& in, int& line){

    //check if the term is a string somehow
    bool status = Term(in, line);

    if (!status)
        return status;

    LexItem t = Parser::GetNextToken(in, line);

    if (t.GetToken()== PLUS || t.GetToken()== PLUS) {
        status = Expr(in, line);

        if (!status)
            return status;
    } else {
        Parser::PushBackToken(t);
    }
    return status;
}

bool Term(istream& in, int& line){
    bool status = SFactor(in,line);
    if(!status)
        return status;

    LexItem temp = Parser::GetNextToken(in,line);
    if(temp.GetToken() == MULT||temp.GetToken() == DIV||temp.GetToken() == REM) {
        status = SFactor(in, line);
    } else {
        Parser::PushBackToken(temp);
    }

    return status;
}

bool SFactor(istream& in, int& line){
    bool status = true;
    LexItem temp = Parser::GetNextToken(in,line);

    if(temp.GetToken()==PLUS || temp.GetToken()==MINUS){
        status = Factor(in,line, 1);
    } else {
        status = Factor(in,line, 1);
    }

    return status;

}

/*
 * Factor function is not finished
 */

bool Factor(istream& in, int& line, int sign){
    bool status = false;

    LexItem t = Parser::GetNextToken(in,line);

    switch (t.GetToken()){
        case IDENT: case ICONST: case RCONST: case SCONST:
            status =true;
            break;
        case LPAREN:
            status = Expr(in, line);
            if(!status)
                return status;
            if(Parser::GetNextToken(in,line) != RPAREN){
                ParseError(line, "missing paren");
                return false;
            }
            status =true;
            break;
        default:
            ParseError(line, "some error in factor function");
            break;
    }
    return status;
}