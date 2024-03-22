#pragma once
#include "PathMaker.h"
#include "SystemGSP.h"
#include <map>
#include <string>




class Interface{
public:
	Interface();
	virtual ~Interface() { delete this->gsp_; };
	void openTerminal();

	void printOptions(const std::map<int, std::string>& options);
private:
	int readingFiles();//vraca 1 za uspesno ucitane fajlove inace ne
	void stationInfo();
	void lineInfo();
	void findPath();
	void endOfWork();

	std::map<int, std::string> options1_;
	std::map<int, std::string> options2_;
	std::map<int, std::string> options3_;
	SystemGSP* gsp_;
};