#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Process {
    string name;      
    int AT;           
    int BT;           
    int CT;           
    int TT;           
    int WT;           
    int remainingBT;  
};

void displayReadyQueue(const queue<int> &readyQueue, const vector<Process> &processes, int time) {
    cout << time << " - Ready Queue: "; 
    queue<int> tempQueue = readyQueue; 
    while (!tempQueue.empty()) {
        cout << processes[tempQueue.front()].name << " ";
        tempQueue.pop();
    }
    cout << "\n";
}

void roundRobin(vector<Process> &processes, int quantum, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT) {
    queue<int> readyQueue;  
    int time = 0, completed = 0, n = processes.size();
    vector<bool> inQueue(n, false); 

    for (auto &p : processes)
        p.remainingBT = p.BT; 
    
    for (int i = 0; i < n; i++) {
        if (processes[i].AT == 0) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }
    if (!readyQueue.empty()) displayReadyQueue(readyQueue, processes, time);

    while (completed < n) {
        if (readyQueue.empty()) { 
            ganttChart.push_back({time, "Idle"});
            time++;
            
            for (int i = 0; i < n; i++) {
                if (processes[i].AT <= time && processes[i].remainingBT > 0 && !inQueue[i]) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                    displayReadyQueue(readyQueue, processes, time); 
                }
            }
        } else {
            int idx = readyQueue.front(); 
            readyQueue.pop(); 

            ganttChart.push_back({time, processes[idx].name}); 
            int execTime = min(quantum, processes[idx].remainingBT); 
            processes[idx].remainingBT -= execTime;
            time += execTime;
            
            for (int i = 0; i < n; i++) {
                if (processes[i].AT <= time && processes[i].remainingBT > 0 && !inQueue[i]) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }

            if (processes[idx].remainingBT > 0) {
                
                readyQueue.push(idx);
            } else {
                
                processes[idx].CT = time;
                processes[idx].TT = processes[idx].CT - processes[idx].AT;
                processes[idx].WT = processes[idx].TT - processes[idx].BT;
                totalTT += processes[idx].TT;
                totalWT += processes[idx].WT;
                completed++;
            }
            displayReadyQueue(readyQueue, processes, time); 
        }
    }
}

int main() {
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the time quantum: ";
    cin >> quantum;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT;
    }

    
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        if (a.AT == b.AT) return a.name < b.name;
        return a.AT < b.AT;
    });

    vector<pair<int, string>> ganttChart;
    double totalTT = 0, totalWT = 0;

    
    roundRobin(processes, quantum, ganttChart, totalTT, totalWT);

    
    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++) {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << processes.back().CT << endl;
    sort(processes.begin(), processes.end(), [](Process &a, Process &b)
         { return a.name < b.name; });
    
    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &p : processes) {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT
             << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}
