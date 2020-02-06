#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <Windows.h>
#include <fstream>
#include <ctime>
#include <algorithm>

#define mx_buf 10000
#define mx_char 32

using namespace std;

int main()
{
    string task;
    string file_name = "xx_net_deamon";
    ifstream in;
    FILE* out;
    bool is_running;
    int sleep_time;
    char time_now[mx_char] = { '\0' };
    time_t now = time(0);

    ctime_s(time_now, 32, &now);
    replace(time_now, time_now + mx_char, '\n', '\0');
    cout <<  "[?] " << time_now << " | 设置检查间隔时间（秒）：";
    cin >> sleep_time;
    cout << "==========================================================" << endl << endl;
    sleep_time *= 1000;

    while (1)
    {
        now = time(0);
        ctime_s(time_now, 32, &now);
        replace(time_now, time_now + mx_char, '\n', '\0');
        is_running = false;
        freopen_s(&out, file_name.c_str(), "w+", stdout);
        fflush(out);
        system("tasklist");
        freopen_s(&out, "CONOUT$", "w+", stdout);
        _fcloseall();
        in.open(file_name);
        if (!in.is_open())
        {
            cout << "Failed to open!" << endl;
            break;
        }
        in.seekg(0, ios::beg);
        while (getline(in, task))
        {
            if (task.find("pythonw.exe") != string::npos)
            {
                is_running = true;
                cout << "[+] " << time_now << " | XX-Net运行中..." << endl << endl;
                break;
            }
        }
        if (!is_running)
        {
            cout << "[!] " << time_now << " | XX-Net未运行，正在开启..." << endl << endl;
            system("C:\\Windows\\System32\\wscript.exe \"D:\\XX-Net-master\\start.vbs\"");
        }
        in.close();
        Sleep(sleep_time);
    }
    
    return 0;
}
