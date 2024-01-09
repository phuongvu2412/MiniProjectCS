#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Naive Search algorithm
vector<int> naiveSearch(string text, string pattern)
{
    int M = pattern.size();
    int N = text.size();
    vector<int> indexList;
    for (int i = 0; i <= N - M; i++) {
        int j;
        for (j = 0; j < M; j++)
            if (text[i + j] != pattern[j])
                break;
        if (j == M)
            indexList.push_back(i);
    }
    return indexList;
}
// Rabin-Karp algorithm
vector<int> rabinKarp(string text, string pattern) {
    int d = 26;
    int q = 101;
    vector<int> res;
    int hash_pattern = 0;
    int hash_text = 0;
    int n = text.size();
    int m = pattern.size();
    int k = 1;
    for (int i = 0; i < m - 1; i++) {
        k = (k * d) % q;
    }
    for (int i = 0; i < m; i++) {
        hash_pattern = (d * hash_pattern + pattern[i]) % q;
        hash_text = (d * hash_text + text[i]) % q;
    }
    for (int i = 0; i <= n - m; i++) {
        if (hash_text == hash_pattern) {
            bool check = true;
            for (int j = 0; j < m; j++) {
                if (text[j + i] != pattern[j]) {
                    check = false;
                    break;
                }
            }
            if (check) {
                res.push_back(i);
            }
        }
        if (i < n - m) {
            hash_text = (d * (hash_text - text[i] * k) + text[i + m]) % q;
            if (hash_text < 0) {
                hash_text = (hash_text + q) % q;
            }
        }
    }
    return res;
}

// Function to read a text file
string readFileText(string fileName) {
    fstream text;
    text.open(fileName, ios::in);
    string temp;
    getline(text, temp);
    text.close();
    return temp;
}
// Function to execute the search and print the execution time and indices
void executionTime(vector<int>(*func)(string, string), string text, string pattern) {
    if (text.find(pattern) == string::npos) {
        cout << "Pattern not found in text." << endl;
        return;
    }
    auto start = chrono::high_resolution_clock::now();
    vector<int> indices = func(text, pattern);
    auto end = chrono::high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " msec" << endl;
}
// Function to display the menu
void menu() {
    system("cls");
    cout << "\n==================== MENU ====================" << endl;
    cout << "1. Load Text File" << endl;
    cout << "2. Choose and Run Search Algorithm (Naive Search or Rabin Karp)" << endl;
    cout << "3. Compare Execution Time of Both Algorithms" << endl;
    cout << "4. Exit" << endl;
    cout << "==============================================\n" << endl;
}

// Find index of a word in a text file
bool findWord(string text, vector<int>(*searchFunc)(string, string)) {
    vector<int> indexList;
    string word;
    cout << "Enter the word you want to find: ";
    cin >> word;
    indexList = searchFunc(text, word);
    int a = indexList.size();
    if (a == 0) {
        cout << "Your word was not found!!!";
        return false;
    }
    else {
        for (int i = 0; i < a; i++) {
            cout << "Your word was at index " << indexList[i] << endl;
        }
        return true;
    }
}

int main() {
    string choice;
    string filename;
    string pattern;
    string text;
    string algorithm;
    while (true) {
        menu();
        cout << "Please enter your choice (1-4): ";
        getline(cin, choice);
        if (choice == "1") {
            system("cls");
            cout << "Text File:\n";
            cout << "- text1.txt (1000 chars)\n";
            cout << "- text2.txt (5000 chars)\n";
            cout << "- text3.txt (10000 chars)\n";
            cout << "Please enter the filename of the text file: ";
            getline(cin, filename);
            text = readFileText(filename);
            if (text.empty()) {
                cout << "File not found. Please try again." << endl;
            }
            else {
                cout << "File loaded successfully." << endl;
            }
            system("pause");
        }
        else if (choice == "2") {
            system("cls");
            if (text.empty()) {
                cout << "Please load a text file first (Option 1)." << endl;
            }
            else {
                string algorithm;
                cout << "Please enter the search algorithm (Naive Search or Rabin Karp): ";
                getline(cin, algorithm);
                if (algorithm == "Naive Search") {
                    if (findWord(text, naiveSearch)) {
                        executionTime(naiveSearch, text, pattern);
                    }
                }
                else if (algorithm == "Rabin Karp") {
                    if (findWord(text, rabinKarp)) {
                        executionTime(rabinKarp, text, pattern);
                    }
                }
                else {
                    cout << "Invalid algorithm choice" << endl;
                }
            }
            system("pause");
        }
        else if (choice == "3") {
            system("cls");
            if (text.empty()) {
                cout << "Please load a text file first (Option 1)." << endl;
            }
            else {
                cout << "Enter the pattern: ";
                getline(cin, pattern);
                if (text.find(pattern) == string::npos) {
                    cout << "Pattern not found in text file. Please try again with a different pattern." << endl;
                }
                else {
                    cout << "Comparing Execution time of Naive Search and Rabin Karp: " << endl;
                    cout << "Time taken by Naive Search-";
                    executionTime(naiveSearch, text, pattern);
                    cout << "Time taken by Rabin Karp-";
                    executionTime(rabinKarp, text, pattern);
                }
            }
            system("pause");
        }
        else if (choice == "4") {
            cout << "Exiting the program. Goodbye!" << endl;
            return 0;
        }
        else {
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            system("cls");
        }
    }
    return 0;
}

