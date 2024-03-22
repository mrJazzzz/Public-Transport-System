#include "Interface.h"
#include <iostream>

using namespace std;


Interface::Interface():gsp_(nullptr){
	options1_[0] = "Kraj rada.";
	options1_[1] = "Ucitavanje podataka o mrezi gradskog prevoza.";

	options2_[0] = "Kraj rada.";
	options2_[1] = "Prikaz informacija o stajalistu.";
	options2_[2] = "Prikaz informacija o liniji gradskog prevoza.";
	options2_[3] = "Pronalazak putanje izmedju dva stajalista";
	//options2_[4] = "Dodavanje stajalista u mrezu gradskog prevoza.";

	options3_[1] = "Najkraci put izmedju dve stanice.";
	options3_[2] = "Put sa najmanje presedanja.";
	options3_[3] = "Put koji obilazi sve vazne stanice.";
}

void Interface::printOptions(const std::map<int, std::string>& options) {
	cout << "_______________________________________________" << endl;
	cout << "Molimo Vas, odaberite opciju: " << endl;
	for (auto it = options.begin(); it != options.end(); it++)
		cout << to_string(it->first) << "." << it->second << endl;
	cout << endl;
};

int Interface::readingFiles() {
	cout << "Molimo Vas, unesite putanju do fajla\nsa stajalistima ili pritisnite "
		"ENTER\nza ucitavanje podrazumevanog fajla\n(ulazi/stajalista.txt): ";
	cin.ignore();
	string fileS="";
	getline(cin, fileS);
	if (fileS.size()==0)
		fileS = "stajalista.txt";
	cout << endl << "Molimo Vas, unesite putanju do fajla\nsa linijama ili pritisnite "
		"ENTER za\nucitavanje podrazumevanog fajla\n(ulazi/linije.txt): ";
	//cin.ignore(); da bi lepo radilo kod upisa linije
	string fileL="";
	getline(cin, fileL);
	if (fileL.size()==0)
		fileL = "linije.txt";

	this->gsp_ = new SystemGSP(fileS, fileL);
	if (!this->gsp_->getStations().size())
		return 0;
	cout << endl << "Uspesno ucitana mreza gradskog prevoza!" << endl;
	return 1;
}

void Interface::stationInfo() {
	cout << "Molimo Vas, unesite oznaku stanice cije informacije zelite: ";
	int stat; cin >> stat;
	this->gsp_->printStation(stat);
}

void Interface::lineInfo() {
	cout << "Molimo Vas, unesite oznaku linije cije informacije zelite: ";
	string line;
	cin.ignore();
	getline(cin, line);
	this->gsp_->printLine(line);
}

void Interface::findPath() {
	this->printOptions(this->options3_);
	int option; cin >> option;
	PathMaker* X;
	switch (option) {
	case 1:
		X = new PathMaker(this->gsp_);
		X->setWritePath();
		X = nullptr;
		break;
	case 2:
		X = new MinTransPathMaker(this->gsp_);
		X->setWritePath();
		X = nullptr;
		break;
	case 3:
		X = new ImportantPathMaker(this->gsp_);
		X->setWritePath();
		X = nullptr;
		break;
	default:
		cout << "Ne postoji opcija." << endl;
		break;
	}
}

void Interface::endOfWork() {
	cout << "Hvala Vam sto ste koristili ovaj program. Prijatna voznja!" << endl;
	return;
}

void Interface::openTerminal(){

	cout << "Dobrodosli u simulator mreze gradskog prevoza!" << endl;

	int option;
	int validOption = false;
	while (!validOption) {
		this->printOptions(this->options1_);
		cin >> option;
		switch (option) {
		case 0:
			this->endOfWork();
			return;
		case 1:
			validOption = this->readingFiles(); //=1 za uspesno =0 za neuspesno
			break;
		default:
			cout << "Nema te opcije." << endl;
			break;
		}
	}
	
	
	while (1) {
		this->printOptions(this->options2_);
		cin >> option;
		switch (option) {
		case 0:
			this->endOfWork();
			return;
			break;
		case 1:
			this->stationInfo();
			break;
		case 2:
			this->lineInfo();
			break;
		case 3:
			this->findPath();
			break;
		default:
			cout << endl << "Ne postoji ta opcija." << endl;
			break;
		}
	}
}