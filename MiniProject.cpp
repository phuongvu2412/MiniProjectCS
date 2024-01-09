#include <iostream>
#include <string>
#include <vector>
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
            if (text[i + j] != pattern[j])//so sanh cac phan tu text[i..M-1] voi word[0,1,2,3]
                break;
        if (j == M)// khi vong lap dung voi j = 3, j++ thuc hien 1 lan nua, dung lai va thoa man text[i..M-1]= word[0,1,2,3]
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
    // Calculate k = d^(m-1)%q
    for (int i = 0; i < m - 1; i++) {
        k = (k * d) % q;
    }
    // Calculate hash value for the pattern and the first window in the text
    for (int i = 0; i < m; i++) {
        hash_pattern = (d * hash_pattern + pattern[i]) % q;
        hash_text = (d * hash_text + text[i]) % q;
    }
    // Iterate through the sliding windows
    for (int i = 0; i <= n - m; i++) {
        // Compare hash values
        if (hash_text == hash_pattern) {
            // Maybe 2 different strings have same hash value
            // Check each characters 
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
        // Update hash value for the next window
        if (i < n - m) {
            hash_text = (d * (hash_text - text[i] * k) + text[i + m]) % q;
            // Ensure that hash_text is non-negative
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
    // Check if the pattern exists in the given text
    if (text.find(pattern) == string::npos) {
        cout << "Pattern not found in text." << endl;
        return;
    }
    // Record the starting time of the execution
    auto start = chrono::high_resolution_clock::now();
    // Call the provided search function to find indices
    vector<int> indices = func(text, pattern);
    // Record the ending time of the execution
    auto end = chrono::high_resolution_clock::now();
    // Calculate the duration of the execution
    auto duration = duration_cast<microseconds>(end - start);
    // Print the execution time in milliseconds
    cout << "Execution time: " << duration.count() << " msec" << endl;
}
// Function to display the menu
void menu() {
    system("cls");
    cout << "\n============================= MENU =============================" << endl;
    cout << "1. Load Text File" << endl;
    cout << "2. Choose and Run Search Algorithm (Naive Search or Rabin Karp)" << endl;
    cout << "3. Compare Execution Time of Both Algorithms" << endl;
    cout << "4. Exit" << endl;
    cout << "================================================================\n" << endl;
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
    // Main loop to display the menu and process user input
    while (true) {
        // Display the menu to the user
        menu();
        cout << "Please enter your choice (1-4): ";
        getline(cin, choice);
        if (choice == "1") {
            // Option 1: Load a text file
            system("cls");
            cout << "Text File:\n";
            cout << "- text1.txt (1000 chars)\n";
            cout << "- text2.txt (5000 chars)\n";
            cout << "- text3.txt (10000 chars)\n";
            cout << "Please enter the filename of the text file: ";
            getline(cin, filename);
            // Read the content of the text file
            text = readFileText(filename);
            // Check if the file was successfully loaded
            if (text.empty()) {
                cout << "File not found. Please try again." << endl;
            }
            else {
                cout << "File loaded successfully." << endl;
            }
            system("pause");
        }
        else if (choice == "2") {
            // Option 2: Choose and run a search algorithm
            system("cls");
            // Check if a text file is loaded
            if (text.empty()) {
                cout << "Please load a text file first (Option 1)." << endl;
            }
            else {
                string algorithm;
                cout << "Please enter the search algorithm (Naive Search or Rabin Karp): ";
                getline(cin, algorithm);
                // Perform the selected search algorithm
                if (algorithm == "Naive Search" || algorithm == "naive search") {
                    if (findWord(text, naiveSearch)) {
                        executionTime(naiveSearch, text, pattern);
                    }
                }
                else if (algorithm == "Rabin Karp" || algorithm == "rabin karp") {
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
            // Option 3: Compare execution time of Naive Search and Rabin Karp
            system("cls");
            // Check if a text file is loaded
            if (text.empty()) {
                cout << "Please load a text file first (Option 1)." << endl;
            }
            else {
                cout << "Enter the pattern: ";
                getline(cin, pattern);
                // Check if the pattern exists in the text
                if (text.find(pattern) == string::npos) {
                    cout << "Pattern not found in text file. Please try again with a different pattern." << endl;
                }
                else {
                    // Compare execution time of Naive Search and Rabin Karp
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
            // Option 4: Exit the program
            cout << "Exiting the program. Goodbye!" << endl;
            return 0;
        }
        else {
            // Invalid choice entered by the user
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            system("cls");
        }
    }
    return 0;
}



