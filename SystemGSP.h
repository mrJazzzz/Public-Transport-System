#pragma once
#include "Line.h"
#include "Station.h"
#include <map>
class SystemGSP
{
public:
	SystemGSP(const std::string& filename_stat, const std::string& filename_lines);
	~SystemGSP();
	void connectGSP(std::map<int, Station*>& stations, std::map<std::string, Line*>& lines);
	//void addStation(); void addStation1(const std::string& row);
	//void addLine(); void addLine1(const std::string& row);
	void deleteLine(const std::string& name);
	void deleteStation(const int& id);
	std::map<int, Station*>& getStations();
	std::map<std::string, Line*>& getLines();

	//u fajl
	void printLine(const std::string& name);
	void printStation(const int& id);

	void printGraph();
	int getLSize();
	//meni za pomoc
private:
	std::map<int, Station*> n_stations_;//stanice mapirane po sifri
	std::map<std::string, Line*> e_lines_;//linijem mapirane po imenu
};