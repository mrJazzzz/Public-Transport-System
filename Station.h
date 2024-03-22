#pragma once
#include <string>
#include <set>
#include <utility>
enum Prior { NOT_IMPORTANT, IMPORTANT };
typedef std::pair<int, std::string> Neighbour;
//sifra suseda i linija kojom se dolazi do njega
class Station
{
public:
	Station(const std::string& row);
	Station(const int& id, const std::string& name, const Prior& x)
		: id_(id), name_(name), x_(x) {};
	virtual ~Station() = default;

	int getId() const;
	Prior getPrior() const;
	std::string getName() const;
	std::set<std::string>& getLines(); //const?
	std::set<Neighbour>& getNeighbours();//const?

	void printNeighbours();
	int getSize();
	//pomocne, samo da vidim ja sta radi program
private:
	int id_;
	std::string name_;
	Prior x_;
	std::set<Neighbour> neighbours_;
	std::set<std::string> lines_;
};