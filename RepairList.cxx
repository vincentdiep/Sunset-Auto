#include <string>     // string
#include <fstream>
#include <iterator>   // next()
#include <stdexcept>  // invalid_argument

#include "RepairList.hpp"

using std::string;
using std::ifstream;

/**********************************************
**  Public/private implementations, types, and objects
** for class BookList
************************************************/

/*************************************
**  Friend functions: read and write
**************************************/

std::ostream& operator<<(std::ostream& stream, const RepairList& repairlist)
{
	for (const auto& request : repairlist._dailyList)   stream << '\n' << std::setw(5) << request;

	return stream;
}


std::istream& operator>>(std::istream& stream, RepairList& repairlist)
{
	Repair request;

	while (stream) {
		stream >> request;
		repairlist.addToList(std::move(request));
	}

	return stream;
}

/***********************
** Constructor
************************/
// written by Vincent Diep
// TO DO
RepairList::RepairList()
{
	// guarantee that _dailyList is empty
	_dailyList.clear();
	// initialize iterator to point to past the end of list
	_nowServicing = _dailyList.end();
}

/*********************
** Destructor
*********************/
// written by Vincent Diep
// TO DO
RepairList::~RepairList() 
{
	// removes all elements from the list
	_dailyList.clear();
}

/************************
** Assignment operator
************************/
// written by Vincent Diep
// TO DO
RepairList& RepairList::operator+=(const RepairList& rhs)
{
	// Concatenate the right hand side list of repair requests to this list by repeatedly adding each repair at the end of the current repair list 
	for (auto const& i : rhs._dailyList)
	{
		//front of list will be the tail of the queue
		//back of list will be the head of the queue
		_dailyList.push_front(i);
	}
	// returns concatenated list
	return *this;
}

/***********************
** Queries or getters
************************/
// written by Vincent Diep
// TO DO
// return the size of the list
// If you decide to use std::forward_list, keep in mind that some implementations of a singly linked list maintain the size (number of elements in the list).  // std::forward_list does not. The size of singly linked list must be calculated on demand by walking the list from beginning to end counting the
// number of elements visited.  The STL's std::distance() function does that, or you can write your own loop.
std::size_t RepairList::size() const
{
	// return the size of the list
	return _dailyList.size();
}

// written by Vincent Diep
// TO DO
// return the current repair
Repair RepairList::currRepair() const
{
	// end of list is front of queue
	// thus element in back is current repair
	return _dailyList.back();
}

/***********************
** Mutators
************************/

// read the list of advanced repair requests from
// the specified file
void RepairList::loadAdvanceList(const std::string& filename)
{
	std::ifstream inFile(filename);
	Repair newRequest;

	if (inFile.is_open())
	{
		// Read each line
		while (inFile >> newRequest) {
			addToList(newRequest);
		}

		inFile.close();  // file closes 
		_nowServicing = _dailyList.begin();
	}
	else
	{
		throw std::invalid_argument("Could not open " + filename);
	}
}

// written by Vincent Diep
// TO DO
// the current repair has been serviced so 
// move the iterator to the next request
void RepairList::next()
{
	// removes finished repair
	_dailyList.pop_back();
	// updates end of list
	_nowServicing = _dailyList.end();
}

// written by Vincent Diep
// TO DO
// add a repair request to the current list
void RepairList::addToList(const Repair& newRequest)
{
	// adds newRequest to back of the queue (front of list)
	_dailyList.push_front(newRequest);
}

// written by Vincent Diep
// TO DO
// insert a repair request coming from a loyal customer
// right after the current iterator but do not make
// changes to the current iterator
void RepairList::insertLoyal(const Repair& newRequest)
{
	// create temp copy of current repair
	Repair temp = _dailyList.back();
	// remove current repair
	_dailyList.pop_back();
	// insert newRequest to front of queue
	_dailyList.push_back(newRequest);
	// insert the old current repair back to front of queue
	_dailyList.push_back(temp);
	// update iterator
	_nowServicing = _dailyList.end();
}
