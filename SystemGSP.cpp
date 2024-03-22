#include "Exceptions.h"
#include "SystemGSP.h"
#include <iostream>
#include <fstream>
#include <utility>
using namespace std;

SystemGSP::SystemGSP(const std::string& filename_stat, const std::string& filename_lines) {
	try {
		ifstream sFile(filename_stat);//red formata: id name [!]
		if (!sFile.is_open()) throw FileNotOpened("Fajl sa stanicama nije otvoren.");
		ifstream lFile(filename_lines);//red formata: name num1 num2 ...
		if (!lFile.is_open()) throw FileNotOpened("Fajl sa linijama nije otvoren.");
		string tmp;
		while (getline(sFile, tmp)) {
			Station* newS = new Station(tmp);
			int id = newS->getId();
			pair<int, Station*> elem = make_pair(id, newS);
			this->n_stations_.insert(elem);
		}
		while (getline(lFile, tmp)) {
			Line* newL = new Line(tmp);
			string name = newL->getName();
			pair<string, Line*> elem = make_pair(name, newL);
			this->e_lines_.insert(elem);
		}
		map<string, Line*>::iterator itL = this->e_lines_.begin(); //kroz linije, kroz e_lines
		list<int>::iterator itS;//kroz listu stanica iz objekata line klase
		//za sve linije iz gsp sistema, prolazi kroz njihove stanice, i spaja svake dve susedne
		for (itL; itL != this->e_lines_.end(); itL++) {
			itS = (*itL).second->getStations().begin();
			for (itS; itS != prev((*itL).second->getStations().end()); itS++) {
				Neighbour tmp;
				int id_next = *next(itS);
				int id_curr = *itS;
				string line_name = itL->second->getName();

				tmp = make_pair(id_next, line_name); //broj stanice i ime linije
				this->n_stations_[*itS]->getNeighbours().insert(tmp);
				//u komsije stanice pod sifrom trenutne ubaci tmp
				tmp = make_pair(id_curr, line_name);
				this->n_stations_[*next(itS)]->getNeighbours().insert(tmp);
				//u komsije stanice pod sifrom sledece od trenutne ubaci tmp;

				this->n_stations_[*itS]->getLines().insert(itL->first);
				//ubacujem ;iniju u spisak linija koje prolaze kroz stanicu
				if (next(itS) == prev((*itL).second->getStations().end()))
					this->n_stations_[*(next(itS))]->getLines().insert(itL->first);
				//i ako sam na pretposlednjoj ubacujem liniju i u spisak naredne stanice
				//zato sto petlju ne izvrsavam za poslednju stanicu zbog radnje sa next(...)
			}
		}
		sFile.close();
		lFile.close();
	}
	catch (FileNotOpened& e) {
		cout << endl<< e.what() << endl;// neko finally? ili u catch neki n_stations.clear()
	}
}
SystemGSP::~SystemGSP(){
	for (auto& it : this->e_lines_)
		delete it.second;
	for (auto& it : this->n_stations_)
		delete it.second;
}

void SystemGSP::connectGSP(map<int, Station*>& stations, map<string, Line*>& lines){
	this->n_stations_ = stations;
	this->e_lines_ = lines;
	map<string, Line*>::iterator itL = this->e_lines_.begin(); //kroz linije, kroz e_lines
	list<int>::iterator itS;//kroz listu stanica iz objekata line klase
	//za sve linije iz gsp sistema, prolazi kroz njihove stanice, i spaja svake dve susedne
	for (itL; itL != this->e_lines_.end(); itL++) {
		itS = (*itL).second->getStations().begin();
		for (itS; itS != prev((*itL).second->getStations().end()); itS++) {
			Neighbour tmp;
			int id_next = *next(itS);
			int id_curr = *itS;
			string line_name = itL->second->getName();

			tmp = make_pair(id_next, line_name); //broj stanice i ime linije
			this->n_stations_[*itS]->getNeighbours().insert(tmp);
			//u komsije stanice pod sifrom trenutne ubaci tmp
			tmp = make_pair(id_curr, line_name);
			this->n_stations_[*next(itS)]->getNeighbours().insert(tmp);
			//u komsije stanice pod sifrom sledece od trenutne ubaci tmp;

			this->n_stations_[*itS]->getLines().insert(itL->first);
			//ubacujem ;iniju u spisak linija koje prolaze kroz stanicu
			if (next(itS) == prev((*itL).second->getStations().end()))
				this->n_stations_[*(next(itS))]->getLines().insert(itL->first);
			//i ako sam na pretposlednjoj ubacujem liniju i u spisak naredne stanice
			//zato sto petlju ne izvrsavam za poslednju stanicu zbog radnje sa next(...)
		}
	}
}

void SystemGSP::deleteLine(const std::string& name){
	for (auto& it : this->e_lines_[name]->getStations()) 
		this->n_stations_[it]->getLines().erase(name);
	for (auto& it : this->n_stations_) 
		it.second->getNeighbours().clear();
	delete this->e_lines_[name];
	this->e_lines_.erase(name);
	connectGSP(this->n_stations_, this->e_lines_);
}

void SystemGSP::deleteStation(const int& id){
	for (auto& it : this->n_stations_)
		it.second->getNeighbours().clear();
	delete this->n_stations_[id];
	this->n_stations_.erase(id);
	for (auto& it : this->e_lines_)
		it.second->getStations().remove(id);
	connectGSP(this->n_stations_, this->e_lines_);
}

map<int, Station*>& SystemGSP::getStations(){
	return this->n_stations_;
}
map<string, Line*>& SystemGSP::getLines(){
	return this->e_lines_;
}

void SystemGSP::printLine(const std::string& name) {
	try {
		if (this->e_lines_.find(name) == this->e_lines_.end())
			throw NonExistentLineError("Uneta linija ne postoji.");
		string filename = "linija_" + name + ".txt";
		ofstream outL(filename);
		Line* ln = this->e_lines_[name];
		int b = *(ln->getStations().begin());
		int e = *(prev(ln->getStations().end()));

		outL << name << " ";//ove linije za prvi red ispisa
		outL << this->n_stations_[b]->getName() << "->";
		outL << this->n_stations_[e]->getName() << endl;

		list<int>::iterator it;
		for (it = ln->getStations().begin(); it != ln->getStations().end(); it++) {
			outL << to_string(*it) << " " << this->n_stations_[*it]->getName();
			if (this->n_stations_[*it]->getPrior() == IMPORTANT)
				outL << " [!]";
			outL << endl;
		}
		outL.close();
		cout << endl << "Fajl sa informacijama o datoj liniji je uspesno napravljen!" << endl;
	}
	catch (NonExistentLineError& e) {
		cout << e.what() << endl;
	}
}
void SystemGSP::printStation(const int& id){
	try {
		if (this->n_stations_.find(id) == this->n_stations_.end())
			throw NonExistentStationError("Stanica sa unetom sifrom ne postoji.");
		string filename = "stajaliste_" + to_string(id) + ".txt";
		ofstream outS(filename);
		string row = "";
		Station* stat = this->n_stations_[id];

		row += to_string(id) + " " + stat->getName();//sifra i ime
		//linije
		row += " [";
		set<string>::iterator lines = stat->getLines().begin();
		for (lines; lines != stat->getLines().end(); lines++)
			row += *lines + " ";
		row.pop_back();
		row += "] {! ";
		//vazni susedi
		set<int> impNeighbours;
		set<Neighbour>::iterator it = stat->getNeighbours().begin();
		for (it; it != stat->getNeighbours().end(); it++) 
			if (this->n_stations_[it->first]->getPrior() == IMPORTANT)
				impNeighbours.insert(it->first);
		set<int>::iterator itn = impNeighbours.begin();
		for (itn; itn != impNeighbours.end(); itn++)
			row += to_string(*itn) + " ";
		row += "!}";
		//cout << row << endl;
		outS << row << endl;	//ovako da bih imao string sa tim, meni radi ispisa nekog eventualnog
		outS.close();
		cout << endl << "Fajl sa informacijama o datom stajalistu je uspesno napravljen!" << endl;
	}
	catch (NonExistentStationError& e) {
		cout << e.what() << endl;
	}
}

void SystemGSP::printGraph(){
	map<int, Station*>::iterator it;
	for (it = this->n_stations_.begin(); it != this->n_stations_.end(); it++) {
		cout << it->first << ": ";
		it->second->printNeighbours();
		//it->second->getSize();
	}
}
int SystemGSP::getLSize(){
	return this->e_lines_.size();
}
//warning, opet, meni za pomoc