#include <iostream>
#include <string>
#include <map>
#include "lex.h"

using std::map;
using namespace std;

map<string, Token> reserved_words= {
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

map <Token ,string> token_Map = {
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



LexItem id_or_kw(const string& lexeme, int linenum){
  Token n = IDENT;

  auto kIt = reserved_words.find(lexeme);
  if(kIt != reserved_words.end()){
    n = kIt->second;
  }

  return LexItem(n, lexeme, linenum);
}

ostream& operator<<(ostream& out, const LexItem& tok){
  Token n = tok.GetToken();
  if(n == IDENT||n == ICONST || n == RCONST || n == SCONST || n == ERR){
    out << token_Map.find(n)->second << " (" << tok.GetLexeme() << ") Line #: " << tok.GetLinenum() << endl;
  } else {
    out << token_Map.find(n)->second << " Line #: " << tok.GetLinenum() << endl;
  }
 
  return out;
}