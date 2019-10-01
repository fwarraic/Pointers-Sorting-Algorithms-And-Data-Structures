#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Note: the names in this code use a number of standard prefixes, which you should know.  I've thrown
// in a few others in this list. They can be used with CamelCase or underscore_separated naming conventions.
//
//		min		minimum
//		max		maximum
//		avg		average
//		std		standard
//		temp	temporary (try not to use "temp" by itself as a variable name.  Say what the temporary holds.)
//		num		number of (when used as a prefix only.  Used as a suffix, it indicates an index or ID number.
//					       for example, numFiles is the number of files you have.  fileNum is the ID number
//						   of one particular file.)
//
//	Some people, notably people who learned to use C-style in C++, also use "in" as a prefix for "input", 
//  and "out" as a prefix for "output".  My preference is to not use these prefixes.
//
//  In general, except in the case of the standard prefixes, I advocate writing words out fully.  Don't use
//  "rng" for "range", for instance.  As another example, it would be disastrous to abbreviate "comparisons"
//  in the following main() function as "comp".  When the brain sees "comp" it has to sort out what that
//  means--it could be "computer" or "comprehensive" or "complete" or "computation", etc.--so writing it
//  out is essential.  Most of the comments in the code are meant to help out students and could be eliminated
//  in professional code.
//
//  Note that the correct specification of what a function may throw requires parentheses around the type, as
//  with writeArray() below.

class FileOpeningException {
private:
	string errorMessage;
public:
	FileOpeningException(string message) {
		errorMessage = message;
	}
	string getMessage() {
		return errorMessage;
	}
};

string* readWords(string filename, int & arraySize);
void writeArray(string filename, float *arr, int arraySize) throw (FileOpeningException);
bool isInAlphabeticalOrder(string s, int count);
int numOfComparisonsIsInAlphabeticalOrder(string s, int count);


int main (void) {
	int numWords;
	string wordListFilename = "./wordlist";
	string outputFilename = "average_comps.txt";

	// This reads the words from wordListFilename.
	// numWords will be the number of words in the array words
	// words[i] will be the ith word
	string *words = readWords(wordListFilename, numWords);


	// TO DO:: You can remove/comment out this.
	// Print out the list.
	//for (int i=0; i<numWords; i++) {
	//	cout << words[i] << endl;
	//}


	// TO DO:: You should compute these values.
	float avgWordLength;
	float avgNumComparisons;
	int maxWordLength = 30;  
	float *avgComparisonsForLength;  // avgComparisonsForLength[i] is the average number of comparisons for a word of length i.


	// TO DO:: Insert your code here.
	// 1.The average length of a word.
	float sumOfLength = 0;
	for (int i = 0; i < numWords; ++i)
	{
		sumOfLength += words[i].length();
	}

	avgWordLength = sumOfLength / numWords;
	cout << "average word length = " << avgWordLength << endl;

	// 2.The average number of character comparisons performed by isInAlphabeticalOrder
	int numOfComparisons = 0;
	float sumOfComparisons = 0;
	int count = 0;
	
	for (int i = 0; i < numWords; ++i) {
		int numOfComparisons = numOfComparisonsIsInAlphabeticalOrder(words[i], count);
		sumOfComparisons += numOfComparisons;
	}
	
	avgNumComparisons = sumOfComparisons / numWords;
	cout << "average number of comparisons = " << avgNumComparisons << endl;


	// 3.The average number of character comparisons as a function of the word length.	
	avgComparisonsForLength = new float[maxWordLength];

	int numOfComparisonsL = 0;
	float sumOfComparisonsL = 0;
	int countL = 0;
	int wordCount = 0;

	for (int i = 0; i < maxWordLength; ++i) {
		for (int j = 0; j < numWords; ++j) {
			if (words[j].length() == i) {
				int numOfComparisonsL = numOfComparisonsIsInAlphabeticalOrder(words[j], countL);
				sumOfComparisonsL += numOfComparisonsL;
				wordCount++;
			}
		}
		avgComparisonsForLength[i] = sumOfComparisonsL / wordCount;
		cout << "avg comparisons of length[" << i << "] = " << avgComparisonsForLength[i] << endl;

		sumOfComparisonsL = 0;
		wordCount = 0;
	}


	// Print average number of comparisons for each length to a file for plotting.
	writeArray(outputFilename, avgComparisonsForLength, maxWordLength);

	// Clean up memory and return.
	delete[] avgComparisonsForLength;
	delete[] words;
	return 0;
}



// Writes the array arr in text format to a file named filename.
// each element of the array has its own line in the file, 
// which contains the element's index and value, separated by a space.
void writeArray(string filename, float *arr, int arraySize) throw (FileOpeningException) {
	ofstream outputStream(filename.c_str());

	if (outputStream.fail()) {
		throw new FileOpeningException("Error opening " + filename);
	}

	for (int i = 0; i<arraySize; i++) {
		outputStream << i << " " << arr[i] << endl;
	}
	outputStream.close();
}

int numLinesInStream(ifstream& stream) {
	int numLines = 0;
	string tempLine;

	// getline returns 0 at end of file.
	while (getline(stream, tempLine)) {
		numLines++;
	}
	return numLines;
}

// note how the following function is all about one thing:
// the handling of the ifstream called inputStream().
// the exact computation done using inputStream() is abstracted
// to a single call (to numLinesInStream).
// Here, I don't declare the thrown object with a "throw ()" on the
// first line.  It's optional, so I'm leaving it off this time.
int numLinesInFile(string filename) {
	ifstream inputStream(filename.c_str());

	if (inputStream.fail()) {
		throw new FileOpeningException(filename + "... file not found ...");
	}

	int numLines = numLinesInStream(inputStream);

	inputStream.close();
	return numLines;
}

// Reads lines from a file named filename.
// Returns an array of strings, one per line of the file filename, 
// and places the size of the array in the reference parameter (output parameter) arraySize
string* readWords(string filename, int & arraySize) {
	string* result = NULL;
	arraySize = numLinesInFile(filename);
	ifstream inputStream(filename.c_str());

	// Read file contents into result, now that size is known
	result = new string[arraySize];			//create results array
	for (int i=0; i < arraySize; i++){
		getline(inputStream, result[i]);
	}
	inputStream.close(); //don't forget to close file

    return result;
}



//  PRE: s is composed of letters from the English alphabet, with no other characters.
bool isInAlphabeticalOrder(string s, int count) {
	int length = s.size();
	for (int i = 0; i < length - 1; ++i) {
		count++;
		if (s[i] > s[i+1]) {
			cout << count << endl;
			return false;
		}
	}
	cout << count << endl;
	return true;
}


//  PRE: s is composed of letters from the English alphabet, with no other characters.
// returns number of comparisons.
int numOfComparisonsIsInAlphabeticalOrder(string s, int count) {
	int length = s.size();
	for (int i = 0; i < length - 1; ++i) {
		count++;
		if (s[i] > s[i+1]) {
			return count;
		}
	}
	return count;
}



