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
            //not sure if something else goes here
            //check if there is "END PROGRAM"
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
        ParseError(line, "Missing PROGRAM");
    }

    return parse;
}

bool StmtList(istream& in, int& line){
/*
 ParseTree *s = Stmt(in, line);
	if( s == 0 )
		return 0;

     if ( (Parser::GetNextToken(in, line)) != SC)
    {
        ParseError(line, "Missing Semi Colon");
         return 0;
    }

	return new StmtList(s, Slist(in,line));*/
}

bool ControlStmt(istream& in, int& line){

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
    if(defVar.find(tok.GetLexeme())!= defVar.end()){
       x = false;
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



