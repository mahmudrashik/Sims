#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    string name;
    int AT;
    int BT;
    int CT;
    int TT;
    int WT;
};

void showReadyQueue(queue<string> readyQueue) {
    cout << "Ready Queue: ";
    if (readyQueue.empty()) {
        cout << "[Empty]" << endl;
    } else {
        cout << "[";
        while (!readyQueue.empty()) {
            cout << readyQueue.front();
            readyQueue.pop();
            if (!readyQueue.empty()) cout << ", ";
        }
        cout << "]" << endl;
    }
}

void fcfs(vector<Process> &processes) {
    int time = 0;
    double totalTT = 0, totalWT = 0;
    vector<pair<int, string>> ganttChart;
    queue<string> readyQueue;

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        if (a.AT == b.AT)
            return a.name < b.name;
        return a.AT < b.AT;
    });

    for (auto &p : processes) {
        if (time < p.AT) {
            ganttChart.push_back({time, "Idle"});
            time = p.AT;
        }
        
        readyQueue.push(p.name);
        showReadyQueue(readyQueue);

        ganttChart.push_back({time, p.name});
        readyQueue.pop(); 

        p.CT = time + p.BT;
        p.TT = p.CT - p.AT;
        p.WT = p.TT - p.BT;
        time = p.CT;

        totalTT += p.TT;
        totalWT += p.WT;
    }

    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << time << endl;

    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT
             << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / processes.size();
    double avgWT = totalWT / processes.size();
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT;
    }

    cout << "\nExecuting FCFS Algorithm...\n";
    fcfs(processes);
    return 0;
}
