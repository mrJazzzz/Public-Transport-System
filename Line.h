#pragma once
#include <string>
#include <list>
class Line
{
public:
	Line(const std::string& row);
	virtual ~Line() = default;

	std::string getName() const;
	std::list<int>& getStations();
	
private:
	std::string name_;
	std::list<int> stations_;//samo sifre
};