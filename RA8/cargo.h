#include <iostream>
#include <string>  

using namespace std;


class Cargo: public Ship
{
	private:
		int cargoCapacity;
		
	public:
		Cargo(string name, int year, int capacity);
		
		int getCapacity();
		
		void setCapacity(int capacity);
		
		virtual void print() const;
};

