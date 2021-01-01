#ifndef KEYWORDS_IN_FILE_H
#define KEYWORDS_IN_FILE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

class KeywordsInFile {
private:
	//unordered_map that contains keyword and how many times it's found in text_file
	std::unordered_map<std::string, int> keywords;
	//set of keywords (NO DUPLICATES AND NO INT COUNT) used for << operator
	std::unordered_set<std::string> keywords_set;
	//vector that stores each line in text_file as a string
	std::vector<std::string> v;
	//vector of unordered_maps of keywords from every individual line in text_file
	std::vector<std::unordered_map<std::string, int>> lines;

public:
	KeywordsInFile() = delete;

	//constructor that takes "filename_with_keywords" is a text file that contains the list of keywords
	//"filename_with_text" is a text file that contains the text where the keywords must be found
	//words from filename_with_words is denoted by N
	//number of words from filename_with_text is denoted by M
	//constructor should be done in O(N+M) runtime
	KeywordsInFile(const std::string &filename_with_keywords, const std::string &filename_with_text);

	//returns true if keyword is found in the text otherwise false
	//O(1) 
	bool KeywordFound(const std::string &keyword);

	//returnns SPECIFIC number of times keyword appears in the specific line number
	//worst runtime should be O(log L) where L is the number of times keyword was found in the text
	int KeywordInLine(const std::string &keyword, int line_number);

	//returns number of times keyword appears
	//worst runtime should be O(log L) where L is the number of times keyword was found in the text
	int TotalOccurrences(const std::string &keyword);

	//overload
	friend std::ostream& operator << (std::ostream& os, const KeywordsInFile& object);
};



#endif
