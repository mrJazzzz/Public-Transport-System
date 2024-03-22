#include "Station.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
//row: id name [!]
Station::Station(const std::string& row){
	this->x_ = NOT_IMPORTANT;

	string tmp;
	stringstream ss(row);
	getline(ss, tmp, ' ');
	this->id_ = stoi(tmp);
	getline(ss, tmp, '[');
	if (tmp[tmp.length() - 1] == ' ') {
		tmp.pop_back();
		this->x_ = IMPORTANT;
	}
	this->name_ = tmp;
}

int Station::getId() const{
	return this->id_;
}
Prior Station::getPrior() const{
	return this->x_;
}
string Station::getName() const{
	return this->name_;
}
set<string>& Station::getLines(){
	return this->lines_;
}
set<Neighbour>& Station::getNeighbours() {
	return this->neighbours_;
}

//ove su meni za pomoc da vidim sta se desava
void Station::printNeighbours(){
	set<Neighbour>::iterator it;
	for (it = this->neighbours_.begin(); it != this->neighbours_.end(); it++)
		cout << "(" << it->first<<","<<it->second<<")" << "  ";
	cout << endl;
}
int Station::getSize(){
	return this->neighbours_.size();//warning
}