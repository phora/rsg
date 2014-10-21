/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: Grammar class
*/

Grammar = function() {
	this.gt = new Object();
	
	this.containsNonTerminal = function (lhs)
	{
		//check if the nonterminal exists no matter the case
		//by transforming it to lowercase
		return this.gt[lhs.toLowerCase()] !== undefined;
	};
	
	this.prepareFromString = function (str)
	{
		//indicator of whether we found the variable definition
		in_variable_def = false;
		//indicator of whether we read the line with the variable name
		read_var_name = false;
		//the current production we are working on
		curprod="";
		//the current variable we are adding productions to
		current_var="";
		//filter out all \r's and iterate over lines
		lines=str.replace(/\r/gm,'').split('\n');
		for (var j = 0;j<lines.length;j++)
		{
			line=lines[j];
			//if we found a sole { on a line, then we found a variable definition
			if (line === "{" && !in_variable_def)
				in_variable_def = true;
			//if we found a sole } on a line and we were in a variable definition,
			//add whatever leftover production we have
			//and reset our state variables
			else if (line === "}" && in_variable_def)
			{
				if (curprod!==""&&curprod!==" "&&curprod!=="\t")
				{
					this.addProduction(current_var, curprod);
					curprod="";
				}
				in_variable_def = false;
				read_var_name = false;
				current_var="";
			}
			//if we're in a variable definition and we haven't read a variable name
			//assume it to be the first line in definition
			//and extract it's name between < and >.
			//then transform it into lowercase
			else if  (in_variable_def && !read_var_name)
			{
				read_var_name = true;
				var_start = line.indexOf('<');
				var_end = line.indexOf('>')+1;
				current_var = line.substring(var_start, var_end).toLowerCase();
			}
			//if we're in the variable definition and we read its name
			//then we take out the \r's
			else if (in_variable_def)
			{
				line = line.replace(/\r/gm,'');
				pos = line.lastIndexOf(';');
				//do we have a semicolon?
				if (pos >= 0)
				{
					//is there something after the last semicolon?
					putlastintonew = (pos != line.length-1);
					tokens = new Array();
					end_here = 0;
					//if not, take out the last semicolon
					//and split this line
					if(!putlastintonew)
					{
						line=line.substr(0,pos);
						tokens=line.split(';');
						end_here = tokens.length;
					}
					//otherwise, split it without taking out the last semicolon
					//and stop iterating over the available tokens just before the last one
					else
					{
						tokens=line.split(';');
						end_here = tokens.length-1;
					}
					for (var i = 0;i<end_here;i++)
					{
						//if this is the first token on this line we came across
						//slam it into our current production
						//then add it to our grammar table
						//then clear the current production
						if (i == 0) {
							curprod+=tokens[i];
							this.addProduction(current_var, curprod);
							curprod="";
						}
						//otherwise just add the production to the table
						else
							this.addProduction(current_var, tokens[i]);
					}
					if(putlastintonew)
					{
						//if we have a leftover token, let's make it the start
						//of a new production
						lasttkn = tokens[tokens.length-1];
						curprod = lasttkn.replace(/\r/gm,'');
					}
				}
				//if not, we should just add this to the current production
				else
				{
					curprod+=line.replace(/\r/gm,'');
				}
			}
		}
	};
	
	this.addProduction = function (lhs, rhs)
	{
		llhs=lhs.toLowerCase();
		//if the current lhs doesn't exist
		//let's add to our table
		//and set up an array for it
		if (!this.containsNonTerminal(lhs))
			this.gt[llhs]=new Array();
		//then we add the new production to the array
		this.gt[llhs].push(rhs);
	};

	this.getAllRHS = function(lhs)
	{
		//if we have the nonterminal
		//retrieve a list of all expansions
		//for that nonterminal
		if (this.containsNonTerminal(lhs)) {
			llhs = lhs.toLowerCase();
			return this.gt[llhs];
		}
	};
	
	this.getRandomRHS = function(lhs)
	{
		//if we have the nonterminal
		//retrieve a random expansion
		//for that nonterminal
		if (this.containsNonTerminal(lhs)) {
			llhs = lhs.toLowerCase();
			idx = Math.floor(Math.random()*this.gt[llhs].length);
			return this.gt[llhs][idx];
		}
	};

	this.toString = function()
	{
		str="";
		for (key in this.gt)
		{
			//set up the section for this nonterminal
			str+="<h3>"+htmlspecialchars(key)+"</h3>\n<div><ul>\n";
			for (production in this.gt[key])
			{
				//then list each expansion in a bulleted list
				str+="<li>"+htmlspecialchars(this.gt[key][production])+"</li>\n";
			}
			str+="</ul></div>\n"
		}
		return str;
	};
}
