#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    string name;      
    int AT,BT,CT,TT,WT,priority,remainingBT;           
};

void priorityScheduling(vector<Process> &processes, vector<pair<int, string>> &ganttChart, vector<pair<int, vector<string>>> &readyQueueStatus, double &totalTT, double &totalWT) {
    int time = 0, completed = 0, n = processes.size();  
    
    for (auto &p : processes) {
        p.remainingBT = p.BT;
    }

    vector<string> readyQueue;
    int idx = -1;  
    string currentProcess = "Idle";

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && find(readyQueue.begin(), readyQueue.end(), processes[i].name) == readyQueue.end()) {
                readyQueue.push_back(processes[i].name);
            }
        }

        int highestPriority = INT_MAX;
        idx = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) { 
            if (currentProcess != "Idle") {
                ganttChart.push_back({time, "Idle"});
                currentProcess = "Idle";
            }
            time++;
            continue; 
        }

        if (currentProcess != processes[idx].name) {
            ganttChart.push_back({time, processes[idx].name});
            readyQueueStatus.push_back({time, readyQueue});
            currentProcess = processes[idx].name;
        }

        processes[idx].remainingBT--;
        time++;

        if (processes[idx].remainingBT == 0) {
            processes[idx].CT = time;
            processes[idx].TT = processes[idx].CT - processes[idx].AT;
            processes[idx].WT = processes[idx].TT - processes[idx].BT;
            totalTT += processes[idx].TT;
            totalWT += processes[idx].WT;
            completed++;

            readyQueue.erase(remove(readyQueue.begin(), readyQueue.end(), processes[idx].name), readyQueue.end());
        }
    }

    if (!ganttChart.empty() && ganttChart.back().first != time) {
        ganttChart.push_back({time, "Idle"});
    }
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter Process Name, Arrival Time, Burst Time, and Priority for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT >> processes[i].priority;
    }

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        return a.AT < b.AT;
    });

    vector<pair<int, string>> ganttChart;
    vector<pair<int, vector<string>>> readyQueueStatus;
    double totalTT = 0, totalWT = 0;

    priorityScheduling(processes, ganttChart, readyQueueStatus, totalTT, totalWT);

    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << "\n";

    cout << "\nReady Queue Status:\n";
    for (auto &status : readyQueueStatus) {
        cout << status.first << " - ";
        for (auto &process : status.second) {
            cout << process << " ";
        }
        cout << "\n";
    }
    sort(processes.begin(), processes.end(), [](Process &a, Process &b)
         { return a.name < b.name; });
    cout << "\nProcess\tAT\tBT\tPriority\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.priority << "\t\t" 
             << p.CT << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}