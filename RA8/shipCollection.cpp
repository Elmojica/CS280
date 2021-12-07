//
// Created by unwan on 12/5/2021.
//

include "ship.h"
include "cruise.h"
include "cargo.h"

string Ship::getShName(){
    return shipName;
}
int Ship::getShYear(){
    return shipYear;
}
string Ship::getvesselType(){
    return vesselType;
}

void Ship::setVesselType(string vessel){
    vesselType = vessel;
}

void Ship::print(){
    cout << "Vessel: " << vesselType;
    cout << "Name: " << shipName;
    cout << "Year: " << shipYear;
    cout << "Capacity(in tons): " << cargoCapacity;
}

//cruise

int Cruise::getmaxPassengers(){
    return maxPassengers;
}

void Cruise::setmaxPassengers(int passengers){
    maxPassengers = pasengers;
}

void Cruise::print(){
    cout << "Vessel: " << vesselType;
    cout << "Name: " << shipName;
    cout << "Year: " << shipYear;
    cout << "Capacity(in tons): " << cargoCapacity;
}

//cargo

int Cargo::getCapacity(){
    return cargoCapacity;
}

void Cargo::setCapacity(int cap){
    cargoCapacity = cap;
}

void Cargo::print(){
    cout << "Vessel: " << vesselType;
    cout << "Name: " << shipName;
    cout << "Year: " << shipYear;
    cout << "Capacity(in tons): " << cargoCapacity;
}