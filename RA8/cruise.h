#include <iostream>
#include <string>  

using namespace std;
# include "ship.h"

class Cruise: public Ship
{
	private:
		int maxPassengers;
		
	public:
		Cruise(string name, int year, int max);
				
		int getmaxPassengers();
			
		void setmaxPassengers(int passengers);
				
		virtual void print() const;
};

