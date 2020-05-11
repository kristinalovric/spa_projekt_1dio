#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <chrono>
#include "MovieOrShow.h"
#include "Date.h"
#include <stack>
#include <queue>
#include <cmath>
#include "Utils.h"

using namespace std;
using namespace std::chrono;


void copy_to_stack(vector<MovieOrShow>& main_data, ofstream& out1)
{
	stack<MovieOrShow, vector<MovieOrShow>> s(main_data);

	while (!s.empty())
	{
		out1 << s.top().to_string();
		s.pop();
	}
}

void copy_to_queue(vector<MovieOrShow>& main_data, ofstream& out2)
{
	queue<MovieOrShow> q;

	for (auto it = main_data.begin(); it != main_data.end(); ++it)
	{
		q.emplace(*it);

		while (!q.empty())
		{
			out2 << q.front().to_string();
			q.pop();
		}
	}
}

int gen_rand(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void print(MovieOrShow& m)
{
	cout << m.to_string() << endl;
}

void parse_file(ifstream& in, vector<MovieOrShow>& v)
{
	string line;

	auto begin = high_resolution_clock::now();

	//eliminate header
	getline(in, line);

	//int i = 0;
	while (getline(in, line))
	{
		//cout << ++i << endl;

		string temp;
		vector<string> aux_parsed;

		size_t end = 0, beg = 0;
		while (end != string::npos)
		{
			string column;

			end = line.find(',', beg);

			if (line.at(beg) == '"')
			{
				end = line.find("\",", beg);
				column = line.substr(beg + 1, end - beg - 1);
				aux_parsed.push_back(column);
				beg = end + 2;
			}
			else
			{
				column = line.substr(beg, end - beg);
				aux_parsed.push_back(column);
				beg = end + 1;
			}
		}

		MovieOrShow ms = MovieOrShow
		(
			aux_parsed.at(0),
			aux_parsed.at(1),
			aux_parsed.at(2),
			aux_parsed.at(3),
			aux_parsed.at(4),
			aux_parsed.at(5),
			aux_parsed.at(6),
			aux_parsed.at(7),
			aux_parsed.at(8),
			aux_parsed.at(9),
			aux_parsed.at(10),
			aux_parsed.at(11)
		);

		v.push_back(ms);
	}

	auto end = high_resolution_clock::now();
	cout << "Time to load main_data: " << duration_cast<milliseconds>(end - begin).count() << " ms" << endl;
}

void load_indian_list(vector <MovieOrShow>& v, list<MovieOrShow>& l)
{
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		vector<string> countries = it->get_countries();
		for (auto it1 = countries.begin(); it1 != countries.end(); ++it1)
		{
			if (Utils::trim(*it1) == "India")
			{
				l.emplace_back(*it);
				break;
			}
		}
	}
}

void list_to_stack(list<MovieOrShow>& l, stack<MovieOrShow>& s)
{
	for (auto it = l.rbegin(); it != l.rend(); ++it)
	{
		if (it->get_directors().empty())
		{
			s.push(*it);
		}
		else
		{
			if (s.empty())
			{
				continue;
			}
			else
			{
				s.pop();
			}
		}
	}

	if (!s.empty())
	{
		cout << "In stack: " << s.top().to_string() << endl;
		s.pop();
	}
	else
	{
		cout << "The stack is empty" << endl;
	}
}

bool ending_in_seven(MovieOrShow& ms)
{
	if (ms.get_show_id().size() >= 7  && ms.get_show_id().at(7) == '7')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void remove_id_ending_with_seven(list<MovieOrShow>& l)
{
	for (auto it = l.begin(); it != l.end();)
	{
		if (ending_in_seven(*it))
		{
			l.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void statistic_by_release_decade(vector<MovieOrShow>& l)
{
	list<int> decades;
	for (auto it = l.begin(); it != l.end(); ++it)
	{
		decades.push_back(it->get_release_decade());
	}

	decades.sort();

	vector<int> show_decade;
	vector<int> shows_by_decade;

	show_decade.push_back(decades.front());
	shows_by_decade.push_back(1);
	decades.pop_front();

	for (auto it = decades.begin(); it != decades.end(); ++it)
	{
		if (*it == show_decade.back())
		{
			shows_by_decade.back()++;
		}
		else
		{
			show_decade.push_back(*it);
			shows_by_decade.push_back(1);
		}
	}

	for (int i = 0; i < show_decade.size(); i++)
	{
		cout << show_decade.at(i) << "-" << show_decade.at(i) + 9 << "  ";

		int hash_number = ceil(shows_by_decade.at(i) / 10.0);
		for (int j = 0; j < hash_number; j++)
		{
			cout <<  "#";
		}
		cout << "   (" << shows_by_decade.at(i) << ")" << endl;
	}
}

int menu()
{
	//system("cls");

	cout << endl;
	cout << "Welcome to SPA_Netflix console application." << endl;
	cout << "What do you want to do?" << endl << endl;
	cout << "1 - Load data from file" << endl;
	cout << "2 - Copy file" << endl;
	cout << "3 - Do stuff with indian content" << endl;
	cout << "4 - View satistics" << endl;
	cout << "Enter option: ";

	int n;
	cin >> n;
	cin.ignore();
	return n;
}

int main()
{
	srand(time(nullptr));

	int main_menu_choice;

	ifstream in("SPA_PROJ_004_NETFLIX_data.csv");
	ofstream out1("reversed_copy.txt");
	ofstream out2("copy.txt");

	if (!in || !out1 || !out2)
	{
		cout << "Unable to open file" << endl;
		return 1;
	}

	vector<string> aux_parsed;
	vector<MovieOrShow> main_data;
	stack<MovieOrShow> s;
	queue<MovieOrShow> q;
	list<MovieOrShow> indian_content;
	stack<MovieOrShow> directors_stack;
	
	do
	{
		main_menu_choice = menu();
		switch (main_menu_choice)
		{

		case 1:
			parse_file(in, main_data);
			//printing main_data:
			//for_each(main_data.begin(), main_data.end(), print);

			in.close();
			
			break;

		case 2:
			parse_file(in, main_data);
			copy_to_stack(main_data, out1);
			copy_to_queue(main_data, out2);

			out1.close();
			out2.close();

			break;

		case 3:
			parse_file(in, main_data);
			load_indian_list(main_data, indian_content);
			indian_content.reverse();
			remove_id_ending_with_seven(indian_content);
			for_each(indian_content.begin(), indian_content.end(), print);

			list_to_stack(indian_content, directors_stack);

			break;

		case 4:
			parse_file(in, main_data);
			statistic_by_release_decade(main_data);
			break;

		default:
			cout << "Press any key to continue or 6 to exit" << endl;
			cin.ignore();
		}

	} while (main_menu_choice != 6);


	return 0;
}