#include <iostream>
#include <string>
#include "Interface.h"
#include "PathMaker.h"
#include "SystemGSP.h"
using namespace std;

int main() {

	Interface* X = new Interface();
	X->openTerminal();
	delete X;

	//SystemGSP* X = new SystemGSP("stajalista.txt", "linije.txt");
	//X->deleteLine("12");
	//X->deleteStation(3112);
	//ImportantPathMaker* P = new ImportantPathMaker(X);
	//PathMaker* P = new PathMaker(X);
	//MinTransPathMaker* P = new MinTransPathMaker(X);
	//P->setWritePath();

	return 0;
}