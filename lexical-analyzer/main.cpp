#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
#include <list>
#include "lex.h" 
using namespace std;

int main(int argc, char *argv[]){
  ifstream strfile;
  //istream *in = &cin;
  int linenum=0, tokcount =0;
  bool v = false, ids = false, ints = false, strs = false, fcon = false;
  LexItem token;
  LexItem kwtok;
  list <string> idents;
  list <string> sconsts;
  list <int>    inconsts;
  list <float>  fconsts;
  
  if(argc < 2){
    cout << "NO SPECIFIED INPUT FILE NAME." << endl;
    return 0;
  }
    
    
    
  strfile.open(argv[1]);
  if(strfile.is_open()){
    for(int i = 2; i < argc; i ++){
        string var(argv[i]);
        
        
        if(var[0] == '-'){
            
            if(var == "-v"){
                v = true;
            }else if(var == "-fconst"){
                fcon = true;
            }else if(var == "-strconst"){
                strs = true;
            }else if(var == "-ident"){
                ids = true;
            }else if(var == "-iconst"){
                ints = true;
            }else {
                cout << "UNRECOGNIZED FLAG "<< var << endl;
                return 0;
            }
        } else if(isalnum(var[0]) && var[0] != '-') {
            cout << "ONLY ONE FILE NAME ALLOWED" << endl;
            return 0;
        }
        
    }
      
    //checks flags: END 


  //printing out process 

    while((token = getNextToken(strfile, linenum)) != Token:: ERR && token != Token::DONE){
       //v = true;
        kwtok = token;
         
            if(v){
                //do all of the flags
                
                //check if error here 

                cout << kwtok << endl; //change this to be pushed into a list
            }
            if (kwtok.GetToken()== IDENT){
                idents.push_back(kwtok.GetLexeme());
            }
            if (kwtok.GetToken()== SCONST){
                sconsts.push_back(kwtok.GetLexeme());
            }
            if (kwtok.GetToken()== RCONST){
                float fltemp = stof(kwtok.GetLexeme());
                fconsts.push_back(fltemp);
            }
            if (kwtok.GetToken()== ICONST){
                int intemp = stoi(kwtok.GetLexeme());
                inconsts.push_back(intemp);
            }
        
        tokcount++;
    }
      
      LexItem test = getNextToken(strfile, linenum);
      
      
      
      /*int check = test.GetLinenum();
      if(check >-1){
         cout << token <<endl;
         return 0;
      }*/
      if(token==ERR){
        cout << token <<endl;
         return 0;
      }
      
      
    
        cout << "Lines: " << linenum << endl;
        if(linenum >1){
            cout << "Tokens: " << tokcount << endl;
        }

        if(strs){
            //print out strings
            cout << "STRINGS:" << endl;
            if(!(sconsts.empty())){
                sconsts.sort();
                cout << "\""<< sconsts.front() << "\""<< endl;
                sconsts.pop_front();
                for(auto i: sconsts )
                    cout << "\"" << i << "\"" << endl;
            }
        }
        
        
        
        if(ints){
            //print out all int constants
            cout << "INTEGERS:" << endl;
            if(!(inconsts.empty())){
                inconsts.sort();
                inconsts.unique();
                cout << inconsts.front()<< endl;
                inconsts.pop_front();
                for(auto i: inconsts )
                    cout << i<< endl;
            }
        }
        
        
        if(fcon){
            //print out reals
            cout <<  "REALS:" << endl;
            if(!(fconsts.empty())){
                fconsts.sort();
                fconsts.unique();
                cout << fconsts.front()<<endl;
                fconsts.pop_front();
                for(auto i: fconsts )
                    cout << i<<endl;
            }
        } 
        if(ids){
            //print out idents
            cout << "IDENTIFIERS:" << endl;
            if(!(idents.empty())){
                idents.sort();
                idents.unique();
                cout<< idents.front();
                idents.pop_front();
                for(auto i: idents )
                    cout << ", " << i ;
            }
            cout<< endl;

        }
    
  } else {
    cout << "CANNOT OPEN the File "<< argv[1] << endl;
  }
  return 0;  
}