#pragma once
#include "Station.h"
#include "Line.h"
#include "SystemGSP.h"
#include <map>
#include <string>
#include <utility>
#include <list>
#include <set>
typedef std::pair<int, std::string> step;
typedef std::pair <int, int> ind;

class PathMaker{
public:

	PathMaker(SystemGSP* gsp) :gsp_(gsp), stat_b_(-1), stat_e_(-1) {};
	void setStations();
	virtual ~PathMaker() { delete this->gsp_; };

	int setPath();
	void writePathFile();//iz staze ispisuije fajl
	virtual void setWritePath();

protected:
	SystemGSP* gsp_;
	int stat_b_, stat_e_;
	std::list<step> path_;
	
};

class MinTransPathMaker :public PathMaker {
public:
	MinTransPathMaker(SystemGSP* gsp) : PathMaker(gsp) {};
	~MinTransPathMaker() = default;

	void linePath(std::list<std::string>& lPath);
	//linePath da mi sastavi niz linija kroz koje se ide
	void setWritePath() override;
	void stsLL(const std::string& l1,const std::string& l2, std::list<step>& lineSteps);
	//station to station line line
};

class ImportantPathMaker :public PathMaker {
public:
	ImportantPathMaker(SystemGSP* gsp);
	~ImportantPathMaker() override = default;
	void setWritePath() override;
private:
	std::set<int> imp_stations_;
};