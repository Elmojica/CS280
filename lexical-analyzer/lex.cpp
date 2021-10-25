#include "lex.h" 
#include <map>
#include <iterator>
#include <sstream>
#include <string>

using namespace std;


string toUpper(string &word){
  for(int i = 0; i <word.length(); i++){
    word[i] = toupper(word[i]);
  }
  return word;
}

map <Token ,string> tokenMap = {
  { PROGRAM,  "PROGRAM"},
  { WRITE,    "WRITE"},
  { INT,      "INT"},
  { END,      "END"},
  { IF,       "IF"},
  { FLOAT,    "FLOAT"},
  { STRING,   "STRING"},
  { REPEAT,   "REPEAT"},
  { BEGIN,    "BEGIN"},
  { IDENT,    "IDENT"},
  { ICONST,   "ICONST"},
  { RCONST,   "RCONST"},
  { SCONST,   "SCONST"},
  { PLUS,     "PLUS"},
  { MINUS,    "MINUS"},
  { MULT,     "MULT"},
  { DIV,      "DIV"},
  { REM,      "REM"},
  { ASSOP,    "ASSOP"},
  { LPAREN,   "LPAREN"},
  { RPAREN,   "RPAREN"},
  { COMMA,    "COMMA"},
  { EQUAL,    "EQUAL"},
  { GTHAN,    "GTHAN"},
  { SEMICOL,  "SEMICOL"},
  { ERR,      "ERR"}, 
  { DONE,     "DONE"}
};

map<string, Token> reserved= {
  { "PROGRAM",  PROGRAM},
  { "WRITE",    WRITE},
  { "INT",      INT},
  { "END",      END},
  { "IF",       IF},
  { "FLOAT",    FLOAT},
  { "STRING",   STRING},
  { "REPEAT",   REPEAT},
  { "BEGIN",    BEGIN},

};


ostream& operator<<(ostream& out, const LexItem& tok){
  Token n = tok.GetToken();
  
  if(n==ERR){
    out << "Error in line " <<tok.GetLinenum() +1 <<" ("<<tok.GetLexeme()<<")";
      
  } else if(n == IDENT || n == SCONST || n == ICONST || n== RCONST){
      out<<tokenMap[n];
      out <<"(" << tok.GetLexeme() << ")";
  } else {
      out<<tokenMap[n];
  }
  
  return out;
}

LexItem id_or_kw(const string& lexeme, int linenum){
    Token n = IDENT;

    auto kIt = reserved.find(lexeme);
    if(kIt != reserved.end()){
        n = kIt->second;
    } 

    return LexItem(n, lexeme, linenum);
}

LexItem getNextToken(istream& inf , int& linenumber){
  enum TokState{
  START,
  INID, 
  INSTRING,
  ININT, 
  INREAL,
  INCOMMENT} lexstate = START;

  string lexeme = "";
  char ch; 
    
  while(inf.get(ch)){  
     /*if(ch == EOF){
         return LexItem(DONE, "", 0);
     }*/
    switch (lexstate){
      case START:
        if(ch =='\n')
          linenumber++;
        if(isspace(ch))
          continue;//restarts the while loop
        lexeme = ch;

        if(isalpha(ch)){
        
          lexstate = INID;
        } else if(ch=='"'){
          lexstate = INSTRING;
          lexeme.clear();
            
        } else if(isdigit(ch)){
          lexstate = ININT; 
            
        } else if (ch == '.'){
            lexstate = INREAL;
           
        } else if(ch == '+'){
           return LexItem(PLUS, lexeme, linenumber);
            

        } else if(ch == '-'){
          return LexItem(MINUS, lexeme, linenumber);
            

        }else if(ch == '#'){
          lexstate = INCOMMENT;
          continue;
        }else if(ch == '('){
          return LexItem(LPAREN, lexeme, linenumber);
            
        }else if(ch == ')'){
          return LexItem(RPAREN, lexeme, linenumber);
            
        }else if(ch == '*'){
          return LexItem(MULT, lexeme, linenumber);
            
        }else if(ch == '/'){
          return LexItem(DIV, lexeme, linenumber);
            
        }else if(ch == '='){  
          if(inf.peek() == '='){
              inf.get(ch);
              lexeme += ch;
              return LexItem(EQUAL, lexeme, linenumber);    
          }
            return LexItem(ASSOP, lexeme, linenumber);
          continue;
                
        }else if(ch == '%'){
          return LexItem(REM, lexeme, linenumber);
           
        }else if(ch == '>'){
          return LexItem(GTHAN, lexeme, linenumber);
        }else if(ch == ','){
          return LexItem(COMMA, lexeme, linenumber);   
            
        } else if(ch == '_' || ch=='<'||ch=='?'){
          return LexItem(ERR, lexeme, linenumber);
        } else if(ch == ';'){
          return LexItem(SEMICOL, lexeme, linenumber);

        }
 
        break;
            
      case INID:
        

        if(isalpha(ch) || isdigit(ch)||ch =='_'){
            lexeme += ch;
            continue;
          
        }else if(ch == '.' ){
          //lexstate = START;
            
            char temp = inf.peek();
            
            if(isalpha(temp))
               {
                    lexeme = "";
                    lexeme+=ch;
                    lexeme+=temp;
                   return LexItem(ERR, lexeme, linenumber);
               }else
               return LexItem(ERR, lexeme, linenumber);
        } else{
          inf.putback(ch);
          string tmp = toUpper(lexeme);
          lexstate = START;
          return id_or_kw(tmp, linenumber); 
        }
        break; 


      case INSTRING:
       
          if(ch == '"'){
          lexstate = START; 
          return LexItem(SCONST, lexeme, linenumber);
          }else {
            
              if(inf.peek() == EOF || inf.peek() == '\n')
                  return LexItem(ERR, lexeme, linenumber);
           lexeme += ch;
              
            continue;
              
          
        }
                                              
        break;
      case ININT:
            if(isdigit(ch)){
                lexeme +=ch;
                continue;
            
            }else if(ch == '.'){
                lexeme += ch;
                lexstate = INREAL;
                 
                        if(inf.peek()==' '){
                            return LexItem(ERR, lexeme, linenumber);
                        }
           
                continue;
            } else {
                lexstate = START;
                inf.putback(ch);
                return LexItem(ICONST, lexeme, linenumber);
            }
             
            
        break; 
      case INREAL: 
        if(isdigit(ch)){
           lexeme += ch;
            char temp = inf.peek();
                   
            if(!isdigit(temp)){
                lexstate= START;
                return LexItem(RCONST, lexeme, linenumber);
            }
            
            
        }else{
            lexeme +=ch;
            
            return LexItem(ERR, lexeme, linenumber);
        }
        
        break;
      case INCOMMENT:
        
        if(ch == '\n'){
          lexstate = START;
          linenumber++;
        } else {
          continue;
        }
        break;
    }
      
  }
  if(inf.eof()){
      return LexItem(DONE, "", linenumber);
    }
  return LexItem();
}