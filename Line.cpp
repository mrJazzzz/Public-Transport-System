#include "Line.h"
#include <sstream>
using namespace std;

//row: name num1 num2 num3 num4...
Line::Line(const string& row){
	string tmp;
	stringstream ss(row);
	getline(ss, tmp, ' ');
	this->name_ = tmp;
	while (getline(ss, tmp, ' '))
		this->stations_.push_back(stoi(tmp));
}

string Line::getName() const{
	return this->name_;
}
std::list<int>& Line::getStations(){
	return this->stations_;
}