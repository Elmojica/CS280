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
    bool parse = true;

    LexItem t = Parser::GetNexToken(in,line);
    if(t.GetToken() == PROGRAM)
        t = Parser::GetNexToken(in,line);
        if(t.GetToken() == IDENT){


        }else{

        }
    }else{

    }



}

bool StmtList(istream& in, int& line){

}

bool ControlStmt(istream& in, int& line){

}

bool IfStmt(istream& in, int& line){

}

bool IdentList(istream& in, int& line, LexItem tok){
    bool x = true;
    if(defVar.find(tok.GetLexeme())!= m.end()){
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

}

bool Term(istream& in, int& line){

}

bool SFactor(istream& in, int& line){

}

bool Factor(istream& in, int& line, int sign){

}



