#pragma once
#include <exception>

class FileNotOpened : public std::exception {
public:
	FileNotOpened(const char* msg) : exception(msg) {};
};

class EqualStationsError : public std::exception {
public:
	EqualStationsError(const char* msg) : exception(msg) {};
};

class NonExistentStationError : public std::exception {
public:
	NonExistentStationError(const char* msg) :exception(msg) {};
};

class NonExistentLineError: public std::exception {
public:
	NonExistentLineError(const char* msg) : exception(msg) {};
};

class UnconnectedSystem :public std::exception {
public:
	UnconnectedSystem(const char* msg) : exception(msg) {};
};