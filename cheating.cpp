///* Checking for cheating
/// Print the identifier that appears in the program the maximum number of times. 
/// If there are several such identifiers, you should print the one that appears the 
/// first time. If the language in the input data is not case sensitive, then you can
/// output the identifier in any case.

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>

using ID_t = std::map<std::string, std::pair<int, int>>;

// Cheking for correct of identificators which depends on start with digit
static bool startWithDigit(const std::string& word, bool startDigit)
{
	if (!startDigit && isdigit(word[0]))
		return false;
	return true;
}

// Cheking for the numbers
static bool isDigit(const std::string& word)
{
	for (const auto& i : word)
	{
		if (!std::isdigit(i))
			return false;
	}
	return true;
}

// Read and write the keywords in set
static void writeKeyWord(std::ifstream& in, std::set<std::string>& keyWords,
	bool caseSence, int n)
{
	std::string keyWord;
	std::string keyWordLower;
	for (int i = 0; i < n; ++i)
	{
		in >> keyWord;
		keyWordLower = keyWord;

		if (!caseSence)
		{
			std::transform(keyWord.begin(), keyWord.end(), keyWordLower.begin(), ::tolower);
		}

		keyWords.insert(keyWordLower);
	}
}

// Replace wrong symbols (not inc. digits, alphas and '_' ) with space ' '
static void replace_symbols(std::string& word)
{
	for (size_t i = 0; i < word.size(); ++i)
	{
		if (static_cast<int>(word[i]) != 95 &&
			!std::isdigit(word[i]) && !std::isalpha(word[i]))
			word[i] = ' ';
	}
}

// Read programm and full the dict of identificators
static void read(std::ifstream& in, ID_t& identificators,
	const std::set<std::string>& keyWords,
	bool caseSence, bool startDigit)
{
	int pos{};
	std::string line;
	std::string word;
	while (std::getline(in, line))
	{
		replace_symbols(line);
		std::stringstream ss;
		ss << line;
		while (ss >> word)
		{
			if (!startWithDigit(word, startDigit))
				continue;

			if (isDigit(word))
				continue;

			if (keyWords.count(word))
				continue;

			std::string wordLower = word;
			if (!caseSence)
			{
				std::transform(word.begin(), word.end(), wordLower.begin(), ::tolower);
			}

			if (!identificators.count(wordLower))
				identificators[wordLower] = std::pair<int, int>{ 0, pos };
			++identificators[wordLower].first;
			++pos;
		}
	}
}

// Outputing the most repeating identificator
static std::string output(ID_t& identificators)
{
	std::string max_repeat{};
	int count{};
	int pos = identificators.begin()->second.second;

	for (auto& i : identificators)
	{
		if (count < i.second.first)
		{
			count = i.second.first;
			pos = i.second.second;
			max_repeat = i.first;
		}
		else if (count == i.second.first && pos > i.second.second)
		{
			pos = i.second.second;
			max_repeat = i.first;
		}
	}
	return max_repeat;
}

int main()
{
	std::ifstream in("input.txt");
	int n;
	std::string C{}, D{};

	std::set<std::string> keyWords;
	ID_t identificators;
	bool caseSence;
	bool startDigit;
	
	if (in.is_open())
	{
		in >> n >> C >> D;
		caseSence = C == "yes";
		startDigit = D == "yes";
		writeKeyWord(in, keyWords, caseSence, n);
		read(in, identificators, keyWords, caseSence, startDigit);
	}
	in.close();

	std::cout << output(identificators);

	return 0;
}