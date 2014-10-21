class ProdComponent {
	public:
		string content;
		bool is_variable;
		ProdComponent(string c, bool b)
		{
					content = c;
	is_variable = b;
		}
};

typedef vector<ProdComponent> production;

//typedef map<string, vector<production> > grammar_table;

string expandGrammar(grammar_table &gt, string varname)
{
	transform(varname.begin(), varname.end(), varname.begin(), ::tolower);
	grammar_table::iterator it = gt.find(varname);
	//cout << varname << gt[varname].size() << '\n';
	if (it != gt.end()) {
		stringstream ss;
		vector<production> v = it->second;
		production prd = v[rand() % v.size()];
		production::iterator prdi;
		for (prdi=prd.begin();prdi !=  prd.end(); ++prdi)
		{
			ProdComponent pc = *prdi;
			//cout << pc.content << "|, |";
			if (pc.is_variable)
			{
				ss << expandGrammar(gt, pc.content);
			}
			else
			{
				ss << pc.content;
			}
		}
		//cout << endl;
		string final_sentence = ss.str();
		replaceAll(final_sentence, "  ", " ", 0);
		replaceAll(final_sentence, "\t", "", 1);
		replaceAll(final_sentence, " .", ".", 1);
		replaceAll(final_sentence, " ,", ",", 1);
		replaceAll(final_sentence, " ?", "?", 1);
		replaceAll(final_sentence, " !", "!", 1);
		//cout << final_sentence << endl;
		return final_sentence;
	}
	else {
		cerr << "Variable " << varname << " not found" << endl;
		exit(1);
	}
}

production parseProduction(string prod)
{
	production prd;
	string::iterator prdc;
	stringstream ss;

	for (prdc = prod.begin(); prdc != prod.end(); prdc++)
	{
		if (*prdc == '<') {
			ProdComponent pc = ProdComponent(ss.str(),false);
			prd.push_back(pc);
			//cout << ss.str() << "|";
			//clear the buffer
			ss.str("");
			ss.clear();
			//start building variable name
			ss << *prdc;
		}
		else if (*prdc == '>')
		{
			ss << *prdc;
			ProdComponent pc = ProdComponent(ss.str(),true);
			prd.push_back(pc);
			//cout << ss.str() << "|";
			//clear the buffer
			ss.str("");
			ss.clear();
			//stop building variable name
		}
		else if (*prdc != '\r')
		{
			ss << *prdc;
		}
	}
	if (ss.str().length() > 0)
	{
		ProdComponent pc = ProdComponent(ss.str(),false);
		prd.push_back(pc);
		//cout << ss.str() << "|";
	}
	//cout << endl;
	return prd;
}

grammar_table parseGrammar(char *fname)
{
	string line("");
	string current_var;
	ifstream infile;
	infile.open(fname);
	bool in_variable_def = false;
	bool read_var_name = false;
	grammar_table gt;
	stringstream curprod;

	while(!infile.eof())
	{
		getline(infile, line);
		if (line.compare(0,1,"{") == 0 && !in_variable_def)
		{
			//cout << line << '\n';
			in_variable_def = true;
		}
		else if (line.compare(0,1,"}") == 0 && in_variable_def)
		{
			//cout << current_var << " | " << gt[current_var][0].size() << endl;
			if (curprod.str().length() > 0 
				&& curprod.str().compare(" ") != 0
				&& curprod.str().compare("\t") != 0)
			{
				gt[current_var].push_back(parseProduction(curprod.str()));
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
			current_var = line.substr(line.find('<'),line.find('>')+1);
			//cout << line << '\n';
			transform(current_var.begin(), current_var.end(), current_var.begin(), ::tolower);
			gt[current_var] = vector<production>();
		}
		else if (in_variable_def)
		{
			//cout << line.substr(0,line.size()-1) << '\n';
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
				/*
				istringstream iss(line);
				string token;
				while(!iss.eof())
				{
					//then tokenize based on semi-colons
					getline(iss, token, ';');
					//cout << token << endl;
					gt[current_var].push_back(parseProduction(token));
				} */
				for (int i = 0;i<end_here;i++)
				{
					//cout << tokens[i] << endl;
					if (i == 0) {
						curprod << tokens[i];
						gt[current_var].push_back(parseProduction(curprod.str()));
						curprod.str("");
						curprod.clear();
					}
					else
						gt[current_var].push_back(parseProduction(tokens[i]));
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
	infile.close();
	return gt;
}

void printGrammar(grammar_table &gt)
{
	for (grammar_table::iterator gti = gt.begin();gti!=gt.end();gti++)
	{
		cout << gti->first << endl;
		vector<production> v = gti->second;
		for (vector<production>::iterator vpi = v.begin();vpi!=v.end();vpi++)
		{
			cout << "\t";
			production p = *vpi;
			for (production::iterator pi = p.begin();pi!=p.end();pi++)
			{
				cout << ((ProdComponent)*pi).content;
			}
			cout << endl;
		}
	}
}

