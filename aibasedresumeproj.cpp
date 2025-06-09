#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;
string toLower(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] - 'A' + 'a';
    }
    return s;
}
map<string, int> readKeywordsWithWeights(const string& filename) {
    map<string, int> keywords;
    ifstream fin(filename.c_str());
    if (!fin) {
        cout << "Error: Keyword file '" << filename << "' nahi khul pa rahi!\n";
        return keywords;
    }
    string word;
    int weight;
    while (fin >> word >> weight) {
        keywords[toLower(word)] = weight;
    }
    fin.close();
    return keywords;
}
string readResume(const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin) {
        cout << "Error: Resume file '" << filename << "' nahi khul pa rahi!\n";
        return "";
    }
    string line, content = "";
    while (getline(fin, line)) {
        content += toLower(line) + " ";
    }
    fin.close();
    return content;
}
map<string, int> analyzeResume(const string& resumeText, const map<string, int>& keywords, int &totalScore, int &maxScore) {
    map<string, int> counts;
    totalScore = 0;
    maxScore = 0;

    for (map<string,int>::const_iterator it = keywords.begin(); it != keywords.end(); ++it) {
        string key = it->first;
        int weight = it->second;

        size_t pos = resumeText.find(key);
        int count = 0;
        while (pos != string::npos) {
            count++;
            pos = resumeText.find(key, pos + 1);
        }
        counts[key] = count;
        totalScore += count * weight;
        maxScore += weight;
    }
    return counts;
}
void displayResults(const map<string,int>& counts, const map<string,int>& keywords, int totalScore, int maxScore) {
    cout << "\nKeyword Analysis Result:\n";
    for (map<string,int>::const_iterator it = counts.begin(); it != counts.end(); ++it) {
        string key = it->first;
        int count = it->second;
        int weight = 0;
        if (keywords.find(key) != keywords.end()) {
            weight = keywords.at(key);
        }
        cout << "- " << key << " : " << count << " (Weight: " << weight << ")\n";
    }
    cout << "\nFinal Weighted Score: " << totalScore << " / " << maxScore << "\n";

    float percent = 0.0f;
    if (maxScore > 0)
        percent = ((float)totalScore / maxScore) * 100;

    cout << fixed << setprecision(2);
    cout << "Score Percentage: " << percent << "%\n";

    if (percent >= 70.0)
        cout << "Resume is well optimized for the selected job role.\n";
    else
        cout << "Try to include more relevant keywords to improve your resume.\n";

    cout << "\nMissing Important Keywords:\n";
    for (map<string,int>::const_iterator it = counts.begin(); it != counts.end(); ++it) {
        if (it->second == 0) {
            cout << "- " << it->first << "\n";
        }
    }
    ofstream fout("output_report.txt");
    fout << "AI Resume Analyzer Report\n\n";
    for (map<string,int>::const_iterator it = counts.begin(); it != counts.end(); ++it) {
        fout << it->first << ": " << it->second << "\n";
    }
    fout << "\nFinal Weighted Score: " << totalScore << " / " << maxScore << "\n";
    fout << "Percentage: " << percent << "%\n";
    fout << "\nMissing Keywords:\n";
    for (map<string,int>::const_iterator it = counts.begin(); it != counts.end(); ++it) {
        if (it->second == 0) {
            fout << it->first << "\n";
        }
    }
    fout.close();
    cout << "\nReport saved to 'output_report.txt'\n";
}

int main() {
    cout << "===== AI Resume Analyzer (Dev C++ Compatible) =====\n";
    cout << "Select Job Role:\n";
    cout << "1. Web Developer\n";
    cout << "2. Data Scientist\n";
    cout << "3. Software Engineer\n";
    cout << "4. Android Developer\n";
    cout << "5. Cloud Engineer\n";
    cout << "Enter choice (1-5): ";

    int choice;
    cin >> choice;
    cin.ignore(); 

    string keywordFile;
    if (choice == 1) keywordFile ="D:\\aibasedresume\\web_developer.txt";
    else if (choice == 2) keywordFile = "D:\\aibasedresume\\Data_Scientist.txt";
 
    else if (choice == 3) keywordFile ="D:\\aibasedresume\\Software_Engineer.txt" ;
    else if (choice == 4) keywordFile = "D:\\aibasedresume\\Android_Developer.txt";
    else if (choice == 5) keywordFile = "D:\\aibasedresume\\Cloud_Engineer.txt";
 
    else {
        cout << "Invalid choice!\n";
        return 1;
    }

    cout << "Enter resume file path (e.g., my_resume.txt): ";
    string resumeFile;
    getline(cin, resumeFile);

    map<string,int> keywords = readKeywordsWithWeights(keywordFile);
    if (keywords.empty()) {
        cout << "Keywords nahi mile, program exit kar raha hai.\n";
        return 1;
    }

    string resumeText = readResume(resumeFile);
    if (resumeText == "") {
        cout << "Resume file nahi mili ya khali hai, program exit kar raha hai.\n";
        return 1;
    }

    int totalScore = 0, maxScore = 0;
    map<string,int> counts = analyzeResume(resumeText, keywords, totalScore, maxScore);

    displayResults(counts, keywords, totalScore, maxScore);

    return 0;
}

