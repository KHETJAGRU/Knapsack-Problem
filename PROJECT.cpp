#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
using namespace std;

// Structure to hold investment information.
struct Investment {
    string name;
    int cost;   // Cost required to invest.
    int ret;    // Expected return (profit) from the investment.
};

// Function to perform the UNBOUNDED Knapsack algorithm
pair<int, map<int, int>> knapsackUnbounded(const vector<Investment>& investments, int budget) {
    int n = investments.size();
    vector<int> dp(budget + 1, 0);
    vector<int> parent(budget + 1, -1); // track selected company index

    for (int w = 0; w <= budget; w++) {
        for (int i = 0; i < n; i++) {
            if (investments[i].cost <= w) {
                int potential = dp[w - investments[i].cost] + investments[i].ret;
                if (potential > dp[w]) {
                    dp[w] = potential;
                    parent[w] = i;
                }
            }
        }
    }

    // Trace back how many times each company was selected
    map<int, int> selectedCount;  // company index -> count
    int w = budget;
    while (w > 0 && parent[w] != -1) {
        int idx = parent[w];
        selectedCount[idx]++;
        w -= investments[idx].cost;
    }

    return { dp[budget], selectedCount };
}

// Read investments from file
vector<Investment> readInvestmentsFromFile(const string& filename) {
    vector<Investment> investments;
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Could not open " << filename << endl;
        return investments;
    }
    string line;
    while (getline(fin, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        Investment inv;
        iss >> inv.name >> inv.cost >> inv.ret;
        investments.push_back(inv);
    }
    fin.close();
    return investments;
}

// Manual investment entry
vector<Investment> enterInvestmentsManually() {
    vector<Investment> investments;
    int n;
    cout << "Enter the number of companies you want to add: ";
    cin >> n;
    cin.ignore(); // Flush newline before using getline

    for (int i = 0; i < n; i++) {
        Investment inv;
        cout << "\nEnter details for company " << i + 1 << ":\n";
        cout << "Company Name: ";
        getline(cin, inv.name);
        cout << "Investment Cost: ";
        cin >> inv.cost;
        cout << "Expected Return: ";
        cin >> inv.ret;
        cin.ignore(); // Flush newline for next loop
        investments.push_back(inv);
    }
    return investments;
}

// Append a new investment to the file
void appendInvestmentToFile(const string& filename) {
    Investment inv;
    cout << "Enter the company name: ";
    cin.ignore(); // Flush leftover newline
    getline(cin, inv.name);
    cout << "Enter the investment cost: ";
    cin >> inv.cost;
    cout << "Enter the expected return: ";
    cin >> inv.ret;

    ofstream fout(filename, ios::app);
    if (!fout) {
        cerr << "Error: Could not open " << filename << " for appending." << endl;
        return;
    }
    fout << inv.name << " " << inv.cost << " " << inv.ret << "\n";
    fout.close();
    cout << "Company " << inv.name << " has been added to the file." << endl;
}

int main() {
    const string filename = "investments.txt";
    int choice, budget;
    bool exitProgram = false;

    do {
        cout << "\n========== Investment Optimization Menu ==========\n";
        cout << "1. Use pre-stored investment options (from file)\n";
        cout << "2. Manually enter investment options\n";
        cout << "3. Add a new company to the file\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        vector<Investment> investments;

        switch (choice) {
        case 1: {
            investments = readInvestmentsFromFile(filename);
            if (investments.empty()) {
                cout << "No investment data found in file." << endl;
                break;
            }

            cout << "\nAvailable Companies to Invest In:\n";
            cout << "-------------------------------------\n";
            for (size_t i = 0; i < investments.size(); ++i) {
                const Investment& inv = investments[i];
                cout << "Company: " << inv.name
                    << " | Cost: $" << inv.cost
                    << " | Expected Return: $" << inv.ret << endl;
            }

            cout << "\nEnter your total investment budget: ";
            cin >> budget;

            cout << "\nCalculating best investment combination...\n";
            pair<int, map<int, int>> result = knapsackUnbounded(investments, budget);

            cout << "\nMaximum Expected Return: $" << result.first << endl;
            cout << "Investment Plan (Company : Times Invested):\n";
            cout << "-------------------------------------------\n";
            if (result.second.empty()) {
                cout << "No investments can be made within the budget.\n";
            }
            else {
                for (map<int, int>::iterator it = result.second.begin(); it != result.second.end(); ++it) {
                    int idx = it->first;
                    int count = it->second;
                    cout << investments[idx].name
                        << " (Cost: $" << investments[idx].cost
                        << ", Return: $" << investments[idx].ret
                        << ") - Invest " << count << " times\n";
                }
            }

            cout << "\nThis tool uses a smart optimization algorithm to suggest the best investments based on your budget.\n";
            break;
        }
        case 2: {
            investments = enterInvestmentsManually();
            cout << "Enter your total investment budget: ";
            cin >> budget;
            pair<int, map<int, int>> result = knapsackUnbounded(investments, budget);

            cout << "\nMaximum Expected Return: $" << result.first << endl;
            cout << "Investment Plan (Company : Times Invested):\n";
            if (result.second.empty()) {
                cout << "No investments can be made within the budget.\n";
            }
            else {
                for (map<int, int>::iterator it = result.second.begin(); it != result.second.end(); ++it) {
                    int idx = it->first;
                    int count = it->second;
                    cout << investments[idx].name
                        << " (Cost: $" << investments[idx].cost
                        << ", Return: $" << investments[idx].ret
                        << ") - Invest " << count << " times\n";
                }
            }
            break;
        }

        case 3: {
            appendInvestmentToFile(filename);
            break;
        }
        case 4: {
            exitProgram = true;
            break;
        }
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    } while (!exitProgram);

    cout << "Exiting program. Thank you!" << endl;
    return 0;
}
