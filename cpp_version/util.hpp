/*
Chrisdona Joan C. Paraiso, Stanley Dotloe
11/23/2012
CSCE 343
Assignment 6  - Random Sentence Generator: Utility header
*/

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifndef UTIL_H
#define UTIL_H

using namespace std;

typedef map<string, vector<string> > grammar_table;


inline vector<string> split( const string& s, const string& f ) {
    vector<string> temp;
    if ( f.empty() ) {
        temp.push_back( s );
        return temp;
    }
    typedef string::const_iterator iter;
    const iter::difference_type f_size( distance( f.begin(), f.end() ) );
    iter i( s.begin() );
    for ( iter pos; ( pos = search( i , s.end(), f.begin(), f.end() ) ) != s.end(); ) {
        temp.push_back( string( i, pos ) );
        advance( pos, f_size );
        i = pos;
    }
    temp.push_back( string( i, s.end() ) );
    return temp;
}


inline void replaceAll(string &s, string search, string repl, int offset)
{
		int position = s.find(search); // find first space 
		while (position != string::npos ) 
		{
			s.replace(position, search.size(), repl);
			position = s.find(search, position + offset );
		}
}
#endif
