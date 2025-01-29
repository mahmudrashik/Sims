#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <climits>
using namespace std;

struct Process
{
    string name;
    int AT,BT,CT,TT,WT,remainingBT;
};

void displayReadyQueue(const vector<Process> &processes, int time)
{
    cout << "Time: " << time << " | Ready Queue: ";
    vector<string> readyQueue;
    for (auto &p : processes)
    {
        if (p.AT <= time && p.remainingBT > 0)
        {
            readyQueue.push_back(p.name);
        }
    }
    sort(readyQueue.begin(), readyQueue.end());
    for (const auto &processName : readyQueue)
    {
        cout << processName << " ";
    }
    cout << endl;
}

void srjf(vector<Process> &processes, vector<pair<int, string>> &ganttChart, double &totalTT, double &totalWT, int &finalTime)
{
    int time = 0, completed = 0, n = processes.size();

    for (auto &p : processes)
        p.remainingBT = p.BT;

    while (completed < n)
    {
        int idx = -1, minRemainingBT = INT_MAX;
        displayReadyQueue(processes, time);
        
        for (int i = 0; i < n; i++)
        {
            if (processes[i].remainingBT > 0 && processes[i].AT <= time && processes[i].remainingBT < minRemainingBT)
            {
                minRemainingBT = processes[i].remainingBT;
                idx = i;
            }
        }

        if (idx == -1)
        {
            ganttChart.push_back({time, "Idle"});
            time++;
        }
        else
        {
            ganttChart.push_back({time, processes[idx].name});
            processes[idx].remainingBT--;
            time++;

            if (processes[idx].remainingBT == 0)
            {
                processes[idx].CT = time;
                processes[idx].TT = processes[idx].CT - processes[idx].AT;
                processes[idx].WT = processes[idx].TT - processes[idx].BT;
                totalTT += processes[idx].TT;
                totalWT += processes[idx].WT;
                completed++;
            }
        }
    }
    finalTime = time;
}

int main()
{
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++)
    {
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processes[i].name >> processes[i].AT >> processes[i].BT;
    }

    sort(processes.begin(), processes.end(), [](Process &a, Process &b)
         {
        if (a.AT == b.AT)
            return a.name < b.name;
        return a.AT < b.AT; });

    vector<pair<int, string>> ganttChart;
    double totalTT = 0, totalWT = 0;
    int finalTime = 0;
    srjf(processes, ganttChart, totalTT, totalWT, finalTime);

    cout << "\nGantt Chart:\n";
    for (size_t i = 0; i < ganttChart.size(); i++)
    {
        cout << ganttChart[i].first << " | " << ganttChart[i].second << " | ";
    }
    cout << finalTime << endl;
    sort(processes.begin(), processes.end(), [](Process &a, Process &b)
         { return a.name < b.name; });
    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &p : processes)
    {
        cout << p.name << "\t" << p.AT << "\t" << p.BT << "\t" << p.CT
             << "\t" << p.TT << "\t" << p.WT << "\n";
    }

    double avgTT = totalTT / n;
    double avgWT = totalWT / n;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTT;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << endl;

    return 0;
}