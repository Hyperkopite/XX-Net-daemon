#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <Windows.h>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <io.h>

#define mx_buf 10000
#define mx_char 32
#define path_num 5

using namespace std;

string task, xx_net_path, cmd;
string file_name = "xx_net_daemon", path_hstry = "path_hstry";
string paths[path_num] = { "" };
ifstream in;
ofstream write_to;
FILE* out;
bool is_running;
bool is_set_by_input = false;
int sleep_time;
char time_now[mx_char] = { '\0' };
time_t now = time(0);

void set_path()
{
	cmd = "C:\\Windows\\System32\\wscript.exe \"" + xx_net_path + "\\start.vbs\"";
	cout << endl << "您设置的路径是：" << "\"" << xx_net_path << "\\start.vbs\"" << "，请确认该文件是否存在！" << endl << endl;
	system("pause");
	if (is_set_by_input)
	{
		write_to.open(path_hstry, ios::app);
		write_to << xx_net_path << endl;
		write_to.close();
	}
}

void get_current_time()
{
	now = time(0);
	ctime_s(time_now, mx_char, &now);
	replace(time_now, time_now + mx_char, '\n', '\0');
}

int get_num_of_lines(string file_name)
{
	string buff;
	int cntr = 0;
	ifstream in(file_name);
	while (getline(in, buff))
	{
		cntr++;
	}
	return cntr;
}

int main()
{
	int op;
	if (_access(path_hstry.c_str(), 0) == -1)
	{
		get_current_time();
		cout << "[?] " << time_now << " | 未检测到输入历史，输入XX-Net-master文件夹路径并按下回车：";
		getline(cin, xx_net_path);
		is_set_by_input = true;
		set_path();
	}
	else
	{
	l1:
		in.open(path_hstry);
		cout << "[?] 选择或输入XX-Net-master文件夹路径-最近" << path_num << "条输入历史：" << endl << "-----------------------------------------------------------------------------------------------------" << endl;
		if (get_num_of_lines(path_hstry) > path_num)
		{
			for (int i = 0; i < get_num_of_lines(path_hstry) - path_num; i++)
			{
				getline(in, xx_net_path);
			}
		}
		for (int i = 0; i < path_num; i++)
		{
			getline(in, xx_net_path);
			paths[path_num - i - 1] = xx_net_path;
		}
		for (int i = 0; i < path_num; i++)
		{
			cout << '[' << i + 1 << "] " << paths[i] << endl;
		}
		in.close();
		cout << '[' << path_num + 1 << "] 输入新路径" << endl << "-----------------------------------------------------------------------------------------------------" << endl;
		cin >> op;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		if (cin.fail())
		{
			system("cls");
			cout << "[!] 输入编号有误！" << endl << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			goto l1;
		}
		if (op == path_num + 1)
		{
			get_current_time();
			getchar();
			cout << "[?] " << time_now << " | 输入XX-Net-master文件夹路径并按下回车：";
			getline(cin, xx_net_path);
			is_set_by_input = true;
			set_path();
		}
		else if (op >= 1 && op <= path_num)
		{
			xx_net_path = paths[op - 1];
			is_set_by_input = false;
			set_path();
		}
		else
		{
			system("cls");
			cout << "[!] 输入编号有误！" << endl << endl; 
			goto l1;
		}
	}

	get_current_time();
	cout << endl << "[?] " << time_now << " | 输入检查间隔时间并按下回车（秒）：";
	cin >> sleep_time;
	cout << "=====================================================================================================" << endl << endl;
	sleep_time *= 1000;

	while (1)
	{
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
				get_current_time();
				cout << "[+] " << time_now << " | XX-Net运行中..." << endl << endl;
				break;
			}
		}
		if (!is_running)
		{
			get_current_time();
			cout << "[!] " << time_now << " | XX-Net未运行，正在开启..." << endl << endl;
			system(cmd.c_str());
		}
		in.close();
		Sleep(sleep_time);
	}

	return 0;
}
