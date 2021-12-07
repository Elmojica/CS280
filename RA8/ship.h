// shape.h
// Shape abstract base class definition.
#ifndef SHIP_H
#define SHIP_H

#include <string>  

using std::string;

class Ship {

   private: 
		string shipName;
		int shipYear;
		string vesselType;
		
	public:
		Ship(string name, int year): shipName{name}, shipYear{year}{
			vesselType = "Ship";
            shipYear = year;
            shipName = name;
		}
		
   string getShName() const;

   int getShYear() const;
	
	string getvesselType();
	
	void setvesselType(string type);
   // return ship name
   virtual void print() const;     // output ship info

}; // end class Ship


#endif


