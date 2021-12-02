#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

//#include "ship.h"     // Ships class definitions
#include "ship.cpp"    // Implementation of class functions 
#include "cruise.cpp"
#include "cargo.cpp"

using namespace std;
     
int main(){
    vector< Ship * > shipVector( 6 );
    ofstream cargofile ("cargo.dat"), cruisefile("cruise.dat");
   if(!cargofile){
        cout<< "File cannot be openned." << endl;
        return 0;
    }
    
    if(!cruisefile)
    {
        cout<< "File cannot be openned." << endl;
        return 0;
    }
    //creat two ship objects
    Ship ship1( "Tiger", 1970), ship2("Houston", 1950);           
    //creat two Cruise objects       
    Cruise cruise1("Boston", 1985, 1200), cruise2("Los Angles", 1990, 2500);
    //create two cruise objects
    Cargo cargo1("Georgia", 2000, 400), cargo2("Alabama", 2010, 600);
    
    // shipVector[0] points at derived-class object
    shipVector[ 0 ] = &cruise2;                         
    
    // shipVector[1] points at derived-class  object
    shipVector[ 1 ] = & cargo1;                         
    
    // shipVector[2] points at derived-class  object
    shipVector[ 2 ] = &ship2;   
	   
	// shipVector[3] points at derived-class  object
	shipVector[ 3 ] = &ship1; 
	
	// shipVector[4] points at derived-class  object
	shipVector[ 4 ] = &cruise1;
	
	// shipVector[5] points at derived-class  object
	shipVector[ 5 ] = &cargo2;
	                     
    //output the ships information      
    for ( int i = 0; i < shipVector.size(); i++ ) 
    {
    	cout << "Vessel " << i+1 << " " << shipVector[i]->getvesselType() << ": "<<endl;            
        //string shipName = shipVector[i]->getName() ;                                              
       	shipVector[i]->print();
       	
    	if (shipVector[i]->getvesselType() == "Cruise")
		{
			cruisefile << shipVector[i]->getShName() << "  " << shipVector[i]->getShYear() << endl;
		}
		else
		{
			cargofile << shipVector[i]->getShName() << "  " << shipVector[i]->getShYear() << endl;
		}
	   	                              
                                                           
		cout << endl;                                
	}
	return 0;
}

