#include "KeywordsInFile.h"

//CONSTRUCTOR that takes both text files and converts them onto the map
//FIRST: Program reads KEYWORD file and inserts all keywords onto the map with a key of 0. O(N) runtime
//SECOND: Program reads TEXT file and does three tasks. ALL in O(M) runtime.
//------1. Program looks for keywords in TEXT file and increments the key counter if keyword is found.
//------2. Program then adds each line onto a vector of strings
//------3. Then program uses a vector of unordered maps to keep track of each keyword in each line.
KeywordsInFile::KeywordsInFile(const std::string &filename_with_keywords, const std::string &filename_with_text)
{
	//load KEYWORD and TEXT file
	std::ifstream keyword_file(filename_with_keywords);
	std::ifstream text_file(filename_with_text);

	//if KEYWORD or TEXT file fails terminate program immediately
	if (!keyword_file || !text_file)
	{
		exit(EXIT_FAILURE);
	}
	std::string str;
	//O(N) where N is the size of keywords_file
	//insert all the keywords from the file onto the map with key of 0
	//insert all the keywords from the file onto the set used for << operator
	while (keyword_file >> str)
	{
		keywords.insert( { str,0 } );
		keywords_set.insert(str);
	}
	keyword_file.close();
	
	//O(M) checking each word in textfile if keyword and increment key count
	while (text_file >> str)
	{
		//check if str_temp is a keyword
		if (keywords.count(str) > 0)
		{
			//increment key in primary map
			keywords.at(str)++;
		}
	}
	text_file.close();

	//reopen text_file so it beings from start
	text_file.open(filename_with_text);
	//push each line as a string onto vector v
	while (std::getline(text_file,str))
	{
		v.push_back(str);
	}
	text_file.close();

	//loop runs at O(M) runtime (M = number of words in text_file) v.size() == lines in text_file. (lines * words per line) = M
	for (int i{ 0 }; i < v.size(); i++)
	{
		//temporary unordered map consisting of keywords for each line ONLY
		std::unordered_map <std::string, int> temp;

		//stringstream each str line found in text_file. 
		//if line 1 of text_file (str) is "Sun is shining" ss becomes line 1 and then line 2 and so on.
		std::stringstream ss(v[i]);
		std::string str_temp;
		
		//while loop goes through each word in each line so if line 1 is "Sun is shining", str_temp is "Sun" then "is" then "shining"
		while (ss >> str_temp)
		{
			//if str_temp is a KEYWORD and is in temp map
			if (keywords.count(str_temp) > 0 && temp.count(str_temp) > 0)
			{
				//increment key in temp map
				temp.at(str_temp)++;
			}
			//if str in line is a KEYWORD but is not in temp map
			else if (keywords.count(str_temp) > 0)
			{
				//insert the keyword onto the temp map with a key of 1
				temp.insert({ str_temp,1 });
			}
		}
		//push back the unordered map onto the vector. The vector[index+1] indicates the line number. 
		//ex: lines[0+1] contains the unordered_map of keywords found in line 1
		lines.push_back(temp);
	}
}

//BOOL function
//O(1) returns true or false whether keyword was found in the file
bool KeywordsInFile::KeywordFound(const std::string &keyword)
{
	if (keywords.count(keyword) > 0)
	{
		return true;  
	}
	return false;
}

//KeywordInLine function returns vector[line_number-1]
//runtime is O(1) since "at" runtime is constant if keyword exists, if keyword doesn't exist simply return 0.
int KeywordsInFile::KeywordInLine(const std::string &keyword,const int line_number)
{
	//if line number is greater than the vector size return OR keyword doesn't exist in the specific unordered_map return 0
	if (line_number > lines.size() || lines[line_number-1].count(keyword) < 1)
	{
		return 0;
	}
	//vector[line_number-1] accesses the specific unordered_map from the specfic line_number and returns key value of keyword only from the specific line
	return lines[line_number-1].at(keyword);
}

//returns number of times keyword is found
//runtime is O(1) since all keywords exists on the map. Keywords that doesn't exist on the text file has a key of 0 on the map.
int KeywordsInFile::TotalOccurrences(const std::string &keyword)
{
	//if keyword doesn't exist in map return 0
	if (keywords.count(keyword) < 1)
	{
		return 0;
	}
	//return key value in O(1) runtime of keyword in map
	return keywords.at(keyword);
}

//OPERATOR << that takes the KeywordsInFile object and prints all the Keywords and number of times is occurs (including 0)
std::ostream &operator << (std::ostream &os, const KeywordsInFile &object)
{
	os << "##########Keywords Present#########" << "\n";
	//go through the set of keywords
	for (auto itr = object.keywords_set.begin(); itr != object.keywords_set.end(); ++itr)
	{
		//string key = keyword pointed to by iterator
		std::string keyword = *itr;
		//if keyword is in map print keyword and # of times it occurs
		if (object.keywords.count(keyword) > 0) 
		{
			os << keyword << ": " << object.keywords.at(keyword) << "\n";
		}
	}
	return os;
}