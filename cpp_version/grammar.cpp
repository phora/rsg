/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: Grammar class
*/

#include "grammar.hpp"

Grammar::Grammar (string fname)
{
	Grammar::setUpFromFPath((char *)fname.c_str());
}

Grammar::Grammar (char *fname)
{
	Grammar::setUpFromFPath(fname);
}

string Grammar::getRandomRHS(string varname)
{
	transform(varname.begin(), varname.end(), varname.begin(), ::tolower);
	grammar_table::iterator it = gt.find(varname);
	//cout << varname << gt[varname].size() << '\n';
	if (it != gt.end()) {
		stringstream ss;
		vector<string> v = it->second;
		string prd = v[rand() % v.size()];
		return prd;
	}
	else {
		return NULL;
		//cerr << "Variable " << varname << " not found" << endl;
		//exit(1);
	}
}


void Grammar::setUpFromFPath(char *fname)
{
	ifstream infile;
	infile.open(fname);
	Grammar::setUpFromStream(infile);
	infile.close();
}

bool Grammar::containsNonTerminal(string nonTerm)
{
	transform(nonTerm.begin(), nonTerm.end(), nonTerm.begin(), ::tolower);
	grammar_table::iterator gti = gt.find(nonTerm);
	return (gti != gt.end());
}

void Grammar::addProduction(string nonTerm, string expansion)
{
	transform(nonTerm.begin(), nonTerm.end(), nonTerm.begin(), ::tolower);
	if (!Grammar::containsNonTerminal(nonTerm)) {
		gt[nonTerm] = vector<string>();
	}
	gt[nonTerm].push_back(expansion);
}

void Grammar::setUpFromStream(istream &infile)
{
	string line("");
	string current_var;
	bool in_variable_def = false;
	bool read_var_name = false;
	stringstream curprod;

	while(!infile.eof())
	{
		getline(infile, line);
		if (line.compare(0,1,"{") == 0 && !in_variable_def)
		{
			in_variable_def = true;
		}
		else if (line.compare(0,1,"}") == 0 && in_variable_def)
		{
			if (curprod.str().length() > 0 
				&& curprod.str().compare(" ") != 0
				&& curprod.str().compare("\t") != 0)
			{
				Grammar::addProduction(current_var,curprod.str());
				curprod.str("");
				curprod.clear();
			}
			in_variable_def = false;
			read_var_name = false;
			current_var = "";
		}
		else if (in_variable_def && !read_var_name)
		{
			read_var_name = true;
			//we only care about what's between the < and > on this line
			int var_start = line.find('<');
			int var_end = line.find('>')+1;
			current_var = line.substr(line.find('<'),var_end-var_start);
			transform(current_var.begin(), current_var.end(), current_var.begin(), ::tolower);
			gt[current_var] = vector<string>();
		}
		else if (in_variable_def)
		{
			//eliminate the last ; just in case we've got
			//multiple productions on the same line
			//or just one production on the line
			//build up token until we encounter semi-colon?
			replaceAll(line,"\r","",0);
			size_t pos = line.find_last_of(';');
			if (pos != string::npos) {
				bool putlastintonew = (pos != line.length()-1);
				vector<string> tokens;
				int end_here;
				if (!putlastintonew) {
					line.erase(pos);
					tokens = split(line,";");
					end_here = tokens.size();
				}
				else
				{
					tokens = split(line,";");
					end_here = tokens.size()-1;
				}
				for (int i = 0;i<end_here;i++)
				{
					if (i == 0) {
						curprod << tokens[i];
						Grammar::addProduction(current_var,curprod.str());
						curprod.str("");
						curprod.clear();
					}
					else {
						Grammar::addProduction(current_var,tokens[i]);
					}
				}
				if (putlastintonew)
				{
					replaceAll(tokens[tokens.size()-1],"\r","",0);
					curprod.str("");curprod.clear();
					curprod << tokens[tokens.size()-1];
				}
			}
			else
			{
				replaceAll(line,"\r","",0);
				curprod << line;
			}
		}
	}
}

ostream &operator<< (ostream &out, const Grammar &g)
{
	grammar_table gt = g.gt;
	for (grammar_table::iterator gti = gt.begin();gti!=gt.end();gti++)
	{
		out << gti->first << endl;
		vector<string> v = gti->second;
		for (vector<string>::iterator vpi = v.begin();vpi!=v.end();vpi++)
		{
			out << "\t" << *vpi;
			out << endl;
		}
		out << endl;
	}
	return out;
}
