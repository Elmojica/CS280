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
		return getNextToken(in, line);
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
		return false;
	}
	
	//Evaluate: print out the list of expressions values

	return ex;
}

bool Prog(istream& in, int& line){
    bool parse = false;

    LexItem t = Parser::GetNexToken(in,line);
    if(t.GetToken() == PROGRAM){
        t = Parser::GetNexToken(in,line);
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






    //CHECK THIS CODE

    LexItem temp = Parser::GetNextToken(in,line)
    if(temp.GetToken() == DONE){
        Parser::PushBackToken(temp);
        return stmt;
    } else{
        Parser::PushBackToken(temp);
        stmt = new StmtList(in,line); // no idea if this logic works AT ALL
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

bool IfStmt(istream& in, int& line){
/*
ParseTree *exp = Expr(in, line);
	if( exp == 0 ) {
		ParseError(line, "Missing ID");
		return 0;
	}
    if(exp->IsIdent() && id.count(exp->GetId()) == 0)
    {
        cout << "UNDECLARED VARIABLE " << exp->GetId() << endl;
    }
    Lex b = Parser::GetNextToken(in, line);
    if (b != BEGIN)
    {
        ParseError(line, "Missing BEGIN");
        return 0;
    }
    ParseTree *stringList = Slist(in, line);
	if( stringList == 0 ) {
		ParseError(line, "Missing BEGIN");
		return 0;
	}
    Lex end = Parser::GetNextToken(in, line);
    if (end != END)
    {
        ParseError(line, "Missing END");
        return 0;
    }
    return new If(exp,b,stringList,end);
*/




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

}

bool AssignStmt(istream& in, int& line){

}

bool ExprList(istream& in, int& line){

}

bool LogicExpr(istream& in, int& line){

}

bool Expr(istream& in, int& line){
/*ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Lex t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}*/
}

bool Term(istream& in, int& line){

}

bool SFactor(istream& in, int& line){

}

bool Factor(istream& in, int& line, int sign){

}



