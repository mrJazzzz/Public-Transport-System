#include "Exceptions.h"
#include "PathMaker.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
using namespace std;

void PathMaker::setStations(){
	int statB, statE;
	cout << "Unesite sifru pocetne stanice: ";
	cin >> statB; this->stat_b_ = statB;
	cout << "Unesite sifru krajnje stanice: ";
	cin >> statE; this->stat_e_ = statE;
	cout << endl;
}
int PathMaker::setPath(){
	queue<int> Q; //za pretrazivanje stanica
	Q.push(this->stat_b_);
	map<int, bool> visited;//za queue i uslov za poslednju stanicu
	map<int, step> parent;//za cuvanje staze
	try {
		while (!visited[this->stat_e_] && !Q.empty()) {
			int curr = Q.front();
			set<Neighbour>& adj = this->gsp_->getStations()[curr]->getNeighbours();
			for (Neighbour it : adj)//it <int:sused, string:linija>
				if (!visited[it.first]) {
					Q.push(it.first);
					visited[it.first] = true;
					step x = make_pair(curr, it.second);//prethodnik i linija koja ih povezuje
					pair<int, step> par = make_pair(it.first, x);//taj koji ubacujem
					parent.insert(par); // ubacio sam ovo:   [novi]:(iz koga sam dosao, linija)
					if (it.first == this->stat_e_) break;
				}
			Q.pop();
		}
		if (!visited[this->stat_e_])
			throw UnconnectedSystem("Ne postoji put od pocetne do krajnje stanice.");
		step cur = parent[this->stat_e_];
		while (cur.first != this->stat_b_) {//1!!!!!!!!!!!!!!!!!!!!!!!!!! bilo while(1)
			this->path_.push_back(cur);
			cur = parent[cur.first];
		}
		this->path_.reverse();

		this->path_.push_front(cur); //da ubaci prvo korak daaaaa
		return 1;
	}
	catch (UnconnectedSystem& e) {
		cout << e.what() << endl;
		return 0;
	}
		//ovo za probu
		//for (step it : this->path_)
		//	cout << "( " << it.first << "," << it.second << " )" << endl;
}
void PathMaker::writePathFile() {
	//pravi fajl znajuci listu (stat1, l1) (stat2, l2) (stat3, l3) ... (statn, ln)
	//ulazim na stat1 i busom l1 se vozim do stat2, od stat2 do stat3 linijom l2,
	//  ... do this->stat_e_ iz statn linijom ln
	string filename = "putanja_" + to_string(this->stat_b_) + "_" + to_string(this->stat_e_)+".txt";
	ofstream outPath(filename);
	list<step>::iterator it = this->path_.begin(); 
	outPath << "->" << it->second << endl << to_string(it->first);
	it++;

	int numLine = 1;
	for (it; it != this->path_.end(); it++) {
		if (numLine)
			outPath << " ";
		outPath << to_string(it->first);
		numLine++;//ovo omogucava vise presedanja, da ne ostane numline 0, 
		//ovo broji stanice iduci po jednoj liniji i uvek ce biti barem 1 do narednog presedanja
		if (it->second != prev(it)->second) {//ako se menja linija
			outPath << endl<< prev(it)->second << "->" << it->second << endl <<it->first<<" ";
			numLine = 0;
		}	
	}
	if (numLine)
		outPath << " ";// ako se preseda od pretp do posl stanice onda ce sve odraditi lepo
	//samo ce uci u novi red, pa nema potebe za space karakterom, a inace treba da odvoji
	outPath << to_string(this->stat_e_);
	cout << "Fajl sa putanjom od pocetne do krajnje stanice je uspesno napravljen!" << endl;
}
void PathMaker::setWritePath(){
	try{
		this->setStations();
		if (this->stat_b_ == this->stat_e_)
			throw EqualStationsError("Pocetna i krajnja stanica su iste.");
		map<int, Station*> x = this->gsp_->getStations();
		if (x.find(stat_b_) == x.end())
			throw NonExistentStationError("Pocetna stanica sa datom sifrom ne postoji.");
		if (x.find(stat_e_) == x.end())
			throw NonExistentStationError("Krajnja stanica sa datom sifrom ne postoji.");
		int conn = this->setPath();
		if (!conn) return;
		this->writePathFile();
	}
	catch (EqualStationsError& e) {
		cout << e.what() << endl;
		return;
	}
	catch (NonExistentStationError& e) {
		cout << e.what() << endl;
		return;
	}
}

ImportantPathMaker::ImportantPathMaker(SystemGSP* gsp):PathMaker(gsp) {
	map<int, Station*>::iterator it = this->gsp_->getStations().begin();
	for (it; it != this->gsp_->getStations().end(); it++)
		if (it->second->getPrior() == IMPORTANT)
			this->imp_stations_.insert(it->first);
}
void ImportantPathMaker::setWritePath(){
	set<int> rem = this->imp_stations_;
	if (rem.find(this->stat_e_) != rem.end())
		rem.erase(this->stat_e_);//da mi ne juri nju dzabe kad ce je svakako na kraju juriti
	list<step> impPath;//ovo ce biti glavna lista koju cu na kraju prebaciti u this->path_
	try {
		this->setStations();
		map<int, Station*> x = this->gsp_->getStations();
		if (x.find(stat_b_) == x.end())
			throw NonExistentStationError("Pocetna stanica sa datom sifrom ne postoji.");
		if (x.find(stat_e_) == x.end())
			
			throw NonExistentStationError("Krajnja stanica sa datom sifrom ne postoji.");
		int end = this->stat_e_;
		int begin = this->stat_b_;//cuvam i zbog poslednje rute i zbog fajla koji se sluzi poljima klase
		this->stat_e_ = this->stat_b_;//da bi prva iteracija mogla u petlju, da napravi taj pomeraj, stat_e_ cuvam u end

		set<int>::iterator it = this->imp_stations_.begin();
		for (it; it != this->imp_stations_.end() && !rem.empty(); it++) {//pitanje hoce li biti izbrisano iz skupa
			if (rem.find(*it) == rem.end()) continue; //da mi preskace vec predjene vazne stanice*
			this->stat_b_ = this->stat_e_;
			this->stat_e_ = *it;//setPath koristi polja klase, pa ih menjam 
			//pocetna-prva vazna, prva vazna-druga vazna,...,poslednja vazna-kranja
			setPath();
			for (auto& itP : this->path_)//da znam koje sam vazne stanice presao
				if (rem.find(itP.first) != rem.end()) 
					rem.erase(itP.first);
				
			impPath.splice(impPath.end(), this->path_);
			this->path_.clear();
		}
		this->stat_b_ = this->stat_e_;
		this->stat_e_ = end;
		int conn = setPath();
		if (!conn) return;
		
		impPath.splice(impPath.end(), this->path_);
		this->path_ = impPath;
		this->stat_b_ = begin; //da bi lepo nazvao fajl jer se fajl naziva u skladu sa poljima klase
		this->writePathFile();
		//i onda za kraj da veze do odredista
	}
	catch (NonExistentStationError& e) {
		cout << e.what() << endl;
		return;
	}
}




void MinTransPathMaker::linePath(list<string>& lPath) {
	//da sastavi putanje po liniji od tacke do tacke
	queue<string> Q;
	map<string, string> parent;
	set<string> endLines; //mozda da imam skup stanica iz kraja
	int begin = this->stat_b_;
	int end = this->stat_e_;
	for (auto& it : this->gsp_->getStations()[this->stat_e_]->getLines())
		endLines.insert(it);

	for (auto& it : this->gsp_->getStations()[this->stat_b_]->getLines()) {
		if (endLines.find(it) != endLines.end()) {//ako postoji direktna linija, sto petlja ne proveri
			lPath.push_back(it);
			return;
		}	
		Q.push(it);
		parent.insert(make_pair(it," "));
	}
	int ind = 0;
	string last;
	
	while (!Q.empty() && !ind) {//uslov ako je pronadjena linija koja sadrzi krajnju stanicu
		for (auto& itS : this->gsp_->getLines()[Q.front()]->getStations()) {
			for (auto& itSL : this->gsp_->getStations()[itS]->getLines()) {
				parent.insert(make_pair(itSL, Q.front()));
				if (endLines.find(itSL) != endLines.end()) {
					ind++;
					last = itSL;
					break;
				}
			}
			if (ind) break; //da sam imao normalnu for petlju bilo bi uslov u njoj
		}
		if (ind) break; // da mi ne izbaci sve iz Q da mogu da vidim je li grad povezan
		Q.pop();
	}
	if (Q.empty()) {//ako ne moze da se dodje do krajnje stanice
		lPath.clear();
		return;
	}
	while (parent[last] != " ") {
		lPath.push_back(last);
		last = parent[last];
	}
	lPath.push_back(last);
	lPath.reverse();
	//meni za proveru
	//for (auto& it : lPath)
	//	cout << it << endl;
}
void MinTransPathMaker::stsLL(const string& l1, const string& l2, list<step>& lineSteps) {
	lineSteps.clear();

	list<int>& x = this->gsp_->getLines()[l1]->getStations();
	list<int>::iterator itL = x.begin();

	while (*itL != this->stat_b_) itL++;
	list<int>::iterator itR = itL;
	//ove dve liste idu u dva smera istom linijom, 
	//pa koja prva naidje na stanicu na koju staje naredna linija
	list<step> leftL; leftL.push_back(make_pair(*itL, l1));
	list<step> rightL; rightL.push_back(make_pair(*itR, l1));
	
	while (1) {
		if (itR != prev(x.end())) itR++; 
		if (itL != x.begin()) itL--;

		leftL.push_back(make_pair(*itL, l1));
		rightL; rightL.push_back(make_pair(*itR, l1));

		set<string>& setLR = this->gsp_->getStations()[*itR]->getLines();
		set<string>& setLL = this->gsp_->getStations()[*itL]->getLines();
		if (setLR.find(l2) != setLR.end() || *itR == this->stat_e_) {//drugi uslov jer verovatno ne menjam liniju u trenutku dolaska na poslednju stanicu
			rightL.pop_back(); //jer ce doci do tog u novoj liniji, a moja lista ima (posl. u preth, preth), pa (prvi u novoj, nova) koje mi ne ide tu nego u sledecoj iteraciji 
			lineSteps = rightL;
			this->stat_b_ = *itR;
			//cout << "ODRADIO" << endl;
			//for (auto& ittt : rightL) cout << "AJ R:" << ittt.first << endl;
			return;
		}
		if (setLL.find(l2) != setLL.end() || *itL == this->stat_e_) {
			leftL.pop_back(); 
			lineSteps = leftL;
			this->stat_b_ = *itL;
			//for (auto& ittt : leftL) cout << "AJ L:" << ittt.first << endl;
			return;
		}
	}
	//ovde ces srediti stat_b_ i stat_e_, verovatno samo stat_b_
}
void MinTransPathMaker::setWritePath(){
	try {
		this->setStations();
		map<int, Station*> x = this->gsp_->getStations();
		if (x.find(stat_b_) == x.end())
			throw NonExistentStationError("Pocetna stanica sa datom sifrom ne postoji.");
		if (x.find(stat_e_) == x.end())
			throw NonExistentStationError("Krajnja stanica sa datom sifrom ne postoji.");
		if (this->stat_b_ == this->stat_e_)
			throw EqualStationsError("Pocetna i krajnja stanica su iste.");
		int begin = this->stat_b_;
		int end = this->stat_e_;
		list<string> lPath;
		this->linePath(lPath); // to je samo spisak linija po redu kako treba ici
		if (lPath.empty())
			throw UnconnectedSystem("Ne postoji nacin da se dodje od pocetne do krajnje stanice.");

		//this->path_.push_back(make_pair(stat_b_, *lPath.begin()));
		list<string>::iterator it = lPath.begin();
		list<step> lineSteps;//ovaj ce sluziti

		for (it; it != prev(lPath.end()); it++) {
			this->stsLL(*it, *next(it), lineSteps);
			this->path_.splice(this->path_.end(), lineSteps);
		}
		//cout << "TU" << endl;
		this->stsLL(*it, " ", lineSteps);
		this->path_.splice(this->path_.end(), lineSteps);

		this->stat_b_ = begin;
		this->writePathFile();
	}
	catch (NonExistentStationError& e) {
		cout << e.what() << endl;
	}
	catch (UnconnectedSystem& e) {
		cout << e.what() << endl;
	}
	catch (EqualStationsError& e) {
		cout << e.what() << endl;
	}
}