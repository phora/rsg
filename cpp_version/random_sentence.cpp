/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: RandomSentenceGenerator class and program
*/

#include "util.hpp"
#include "grammar.hpp"
#include <stdlib.h>
#include <time.h>

class RandomSentenceGenerator
{
	public:
		RandomSentenceGenerator(char *);
		string randomSentence();
		void printGrammar();
	private:
		Grammar g;
};

RandomSentenceGenerator::RandomSentenceGenerator(char *fname)
{
	g = Grammar(fname);
}

string RandomSentenceGenerator::randomSentence()
{
	string sentence = "<start>";
	while(sentence.find('<') != string::npos)
	{
		int lhs_start = sentence.find_first_of('<');
		int lhs_end = sentence.find_first_of('>')+1;
		string lhs = sentence.substr(lhs_start,lhs_end-lhs_start);
		string rhs = g.getRandomRHS(lhs);
		sentence.replace(lhs_start, lhs.size(),rhs);
	}
	replaceAll(sentence, "  ", " ", 0);
	replaceAll(sentence, "\t", "", 0);
	replaceAll(sentence, " .", ".", 1);
	replaceAll(sentence, " ,", ",", 1);
	replaceAll(sentence, " ?", "?", 1);
	replaceAll(sentence, " !", "!", 1);
	return sentence;
}

void RandomSentenceGenerator::printGrammar()
{
	cout << g << endl;
}

int main (int argc, char *argv[])
{
	srand(time(NULL));
	for (int i = 1;i<argc;i++)
	{
		//grammar_table gt = parseGrammar(argv[i]);
		//cout << expandGrammar(gt, "<start>") << '\n';
		cout << argv[i] << endl;
		RandomSentenceGenerator rsg = RandomSentenceGenerator(argv[i]);
		//cout << g << endl;
		rsg.printGrammar();
		cout << rsg.randomSentence() << endl;
	}
}
