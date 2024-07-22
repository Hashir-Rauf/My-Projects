#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<string> loadDictionary(const string& filePath) 
{
    vector<string> wordList;
    ifstream file(filePath);
    if (file.is_open()) {
        string word;
        while (file >> word) {
            wordList.push_back(word);
        }
        file.close();
    }
    return wordList;
}

void saveDictionary(const vector<string>& wordList, const string& filePath) {
    ofstream file(filePath);
    for (const auto& word : wordList) {
        file << word << endl;
    }
    file.close();
}

void addToDictionary(const string& word, vector<string>& wordList, const string& filePath) {
    wordList.push_back(word);
    saveDictionary(wordList, filePath);
}

bool spellCheck(const string& word, const vector<string>& wordList) {
    return find(wordList.begin(), wordList.end(), word) != wordList.end();
}

//function to guess the word
int levenshteinDistance(const string& word1, const string& word2) {
    int m = word1.length();
    int n = word2.length();

    // Create a 2D vector to store the distance
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // Initialize first row and column
    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    // Guessing the word
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }

    return dp[m][n];
}

string findClosestWord(const string& word, const vector<string>& wordList) {
    int minDistance = INT_MAX;
    string closestWord;

    for (const auto& dictWord : wordList) {
        int distance = levenshteinDistance(word, dictWord);
        if (distance < minDistance) {
            minDistance = distance;
            closestWord = dictWord;
        }
    }

    return closestWord;
}

int main() {
    string filePath = "dictionary.txt";
    vector<string> wordList = loadDictionary(filePath);

    string userInput;
    while (true) {
        cout << "Enter a word (type 'exit' to quit): ";
        cin >> userInput;
        if (userInput == "exit") {
            break;
        }
        if (spellCheck(userInput, wordList)) {
            cout << "The word is spelled correctly." << endl;
        } else {
            cout << "The word is misspelled." << endl;
            string closestWord = findClosestWord(userInput, wordList);
            cout << "Did you mean \"" << closestWord << "\"?" << endl;
            string addToDict;
            cout << "Do you want to add it to the dictionary? (yes/no): ";
            cin >> addToDict;
            if (addToDict == "yes") {
                addToDictionary(userInput, wordList, filePath);
                cout << "Word added to dictionary." << endl;
            }
        }
    }

    return 0;
}
