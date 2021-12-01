#include "parse.h"

map<string, bool> defVar;
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

static int error_count = 0;

int ErrCount(){
    return error_count;
}

void ParseError(int line, string msg){
    ++error_count;
    cout << line << ": " << msg << endl;
}

//Prog ::= PROGRAM IDENT StmtList END PROGRAM
bool Prog(istream& in, int& line){
    bool sl = false; //StmtList check
    LexItem tok = Parser::GetNextToken(in, line);

    if (tok.GetToken() == PROGRAM){
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == IDENT){
            sl = StmtList(in, line);
            if (!sl){
                sl = false;
            }

            tok = Parser::GetNextToken(in, line);

            if (tok.GetToken() == END){
                tok = Parser::GetNextToken(in, line);
                if (tok.GetToken() != PROGRAM){
                    ParseError(line, "Missing PROGRAM at the End");
                    if(error_count >0){
                        ParseError(line, "Incorrect Syntax in the program");
                    }
                    return  false;
                }
                sl = true;
            }else{
                ParseError(line, "Missing END at end of program.");
                if(error_count >0){
                    ParseError(line, "Incorrect Syntax in the program");
                }
                return false;
            }
        }
    }
    else if (tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        sl = false;
    }
    if(error_count >0){
        ParseError(line, "Incorrect Syntax in the program");
        return false;
    }

    return sl;
}

//StmtList ::= Stmt; {Stmt;}
bool StmtList(istream& in, int& line){
    bool status = Stmt(in, line);

    if (!status){
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);

    if (tok.GetToken() == END || tok.GetToken() == PROGRAM || tok.GetToken() == DONE){
        Parser::PushBackToken(tok);
        return true;
    }
    if (tok.GetToken() == SEMICOL){
        status = StmtList(in, line);
    }
    else{
        ParseError(line, "Missing a semicolon.");
        return false;
    }
    return status;
}

//Stmt ::= DeclStmt | ControlStmt
bool Stmt(istream& in, int& line){
    bool status=true;
    //cout << "in Stmt" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    switch(t.GetToken()) {
        case INT: case FLOAT:
            Parser::PushBackToken(t);
            status = DeclStmt(in, line);
            if(!status){
                ParseError(line, "Incorrect Declaration Statement");
                return status;
            }
            break;
        case IF: case WRITE: case IDENT:
            Parser::PushBackToken(t);
            status = ControlStmt(in, line);
            if(!status){
                ParseError(line, "Incorrect Control Statement");
                return status;
            }
            break;
        default:
            Parser::PushBackToken(t);
    }
    return status;
}

//ControlStmt ::= AssignStmt | IfStmt | WriteStmt
bool ControlStmt(istream& in, int& line){
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    switch (t.GetToken()){
        case IDENT:
            Parser::PushBackToken(t);
            status = AssignStmt(in, line);
            if (!status){
                return false;
            }
            break;

        case IF:
            status = IfStmt(in, line);
            if (!status){
                return false;
            }
            break;
        case WRITE:
            status = WriteStmt(in, line);
            if (!status){
                return false;
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

    if(!ex) {
        ParseError(line, "Missing expression after Write");
        return false;
    }

    return ex;
}

bool IfStmt(istream& in, int& line){
    LexItem t;
    bool ex = false;

    if ((t= Parser::GetNextToken(in, line)) != LPAREN){
        ParseError(line, "Missing Left Parenthesis");
        return false;
    }

    ex = LogicExpr(in, line);
    if (!ex){
        ParseError(line, "Missing If Statement Expression");
        return false;
    }

    if ((t= Parser::GetNextToken(in, line)) != RPAREN){
        ParseError(line, "Missing Right Parenthesis");
        return false;
    }

    bool status = Stmt(in, line);
    if (!status){
        ParseError(line, "Missing Control Statement");
        return false;
    }

    return true;
}

//IdentList ::= IDENT {, IDENT}
bool IdList(istream&in, int& line, LexItem& tok){
    bool status;
    LexItem t = Parser::GetNextToken(in, line);


    if (t.GetToken() == IDENT){
        if (defVar.find(t.GetLexeme())->second){
            ParseError(line, "Variable Redefinition");
            return false;
        }else{
            defVar.insert(pair<string, bool> (t.GetLexeme(), true));
            SymTable.insert(pair<string, Token> (t.GetLexeme(), tok.GetToken()));
        }

        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == COMMA){
            status = IdList(in, line, tok);
            if (!status){
                ParseError(line, "Missing IDENT after COMA");
                return false;
            }
        }else{
            Parser::PushBackToken(t);
            return true;
        }
    }else{
        ParseError(line, "Missing Variable");
        return false;
    }
    return true;
}

//Decl = Type IdentList
//Type = INT | FLOAT
bool DeclStmt(istream& in, int& line) {
    bool status = false;
    LexItem tok;
    //cout << "in Decl" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    if(t == INT || t == FLOAT ){
        status = IdList(in, line, t);
        //cout<< "returning from IdentList" << " " << (status? 1: 0) << endl;
        if (!status){
            ParseError(line, "Incorrect variable in Declaration Statement");
            return status;
        }
    }else{
        Parser::PushBackToken(t);
        ParseError(line, "Incorrect Type");
        return false;
    }
    return true;
}

//Var ::= IDENT
bool Var(istream& in, int& line){
    string ident;
    LexItem tok = Parser::GetNextToken(in, line);

    if (tok == IDENT){
        ident = tok.GetLexeme();
        if (!(defVar.find(ident)->second)){
            ParseError(line, "Undeclared Variable");
            return false;
        }
        return true;
    }
    else if (tok == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
}

//AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){
    bool varstatus = false, status = false;
    LexItem t;

    varstatus = Var( in, line);

    if (varstatus){
        if ((t=Parser::GetNextToken(in, line)) == ASSOP){
            status = Expr(in, line);
            if(!status)
            {
                ParseError(line, "Missing Expression in Assignment Statment");
                return false;
            }
        }else if(t.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }else{
            ParseError(line, "Missing Assignment Operator =");
            return false;
        }
    }
    else{
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    return true;
}

//ExprList ::= Expr {, Expr}
bool ExprList(istream& in, int& line){
    bool status = Expr(in, line);
    if (!status){
        ParseError(line, "Missing Expression");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    if (t.GetToken() == COMMA){
        status = ExprList(in, line);
    }
    else if (t.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t.GetLexeme() << ")" << endl;
        return false;
    }
    else{
        Parser::PushBackToken(t);
        return true;
    }
    return status;
}

//LogicExpr ::= Expr (== | >) Expr
bool LogicExpr(istream& in, int& line){
    bool status = Expr(in, line);

    if (!status){
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t.GetLexeme() << ")" << endl;
        return false;
    }

    if (t.GetToken() == EQUAL || t.GetToken() == GTHAN){
        status = Expr(in, line);
        if (!status){
            ParseError(line, "Missing expression after relational operator");
            return false;
        }
    }

    return true;
}

//Expr ::= Term {(+|-) Term}
bool Expr(istream& in, int& line){
    bool status = Term(in, line);

    if (!status){
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    if (t.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t.GetLexeme() << ")" << endl;
        return false;
    }

    while (t.GetToken() == PLUS || t.GetToken() == MINUS){
        status = Term(in, line);
        if (!status){
            ParseError(line, "Missing operand after operator");
            return false;
        }

        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == ERR) {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
    }

    Parser::PushBackToken(t);
    return true;
}

//Term ::= SFactor {( *| / | % ) SFactor}
bool Term(istream& in, int& line){
    bool status = SFactor(in, line);

    if (!status){
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    if (t.GetToken() == ERR) {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t.GetLexeme() << ")" << endl;
        return false;
    }

    while (t.GetToken() == MULT || t.GetToken() == DIV){
        status = SFactor(in, line);
        if (!status){
            ParseError(line, "Missing operand after operator");
            return false;
        }

        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == ERR) {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
    }

    Parser::PushBackToken(t);
    return true;
}

//SFactor ::= (+ | -) Factor | Factor
bool SFactor(istream& in, int& line){
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    int sign = 1;
    if (t.GetToken() == MINUS){
        sign = -1;
    }else if (t.GetToken() == PLUS){
        sign = 1;
    }else{
        Parser::PushBackToken(t);
    }
    status = Factor(in, line, sign);

    return status;
}

//Factor = IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign){
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == IDENT){
        if (!(defVar.find(t.GetLexeme())->second)){
            ParseError(line, "Undefined variable");
            return false;
        }
        return true;
    }else if (t.GetToken() == ICONST){
        return true;
    }else if (t.GetToken() == RCONST){
        return true;
    }else if (t.GetToken() == SCONST){
        return true;
    }else if (t.GetToken() == LPAREN){
        bool ex = Expr(in, line);
        if (!ex){
            ParseError(line, "Missing Expression after (");
            return false;
        }
        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == RPAREN){
            return ex;
        }
        ParseError(line, "Missing ) after Expression");
        return false;
    }else if(t.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << t.GetLexeme() << ")" << endl;
        return false;
    }

    ParseError(line, "Unrecognized input");
    return true;
}


