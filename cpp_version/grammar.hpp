/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: Grammar class
*/

#include "util.hpp"

#ifndef GRAMMAR_H
#define GRAMMAR_H
class Grammar {
	friend ostream& operator<< (ostream &, const Grammar &);
	private:
		grammar_table gt;
	public:
		Grammar (string);
		Grammar (char *);
		Grammar (){};
		void addProduction(string, string);
		void setUpFromFPath(char *);
		void setUpFromStream(istream&);
		string getRandomRHS(string);
		bool containsNonTerminal(string);
};
#endif
