// Compile with c++ short-pathcpp -std=c++11 -o short-path
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <list>

using namespace std;

class Graph
{
    int V;
    list<int> *adjacency;

public:
    Graph(int V);
    void addEdge(int x, int y);
    void DFS(int first, int last);
};

Graph::Graph(int V)
{
    this->V = V;
    adjacency = new list<int>[V];
}

void Graph::addEdge(int x, int y)
{
    adjacency[x].push_back(y);
    adjacency[y].push_back(x);
}

void Graph::DFS(int first, int last)
{
    vector<bool> visited;
    for (int i = 0; i < V; i++)
        visited.push_back(false);

    vector<int> parent;
    for (int i = 0; i < V; i++)
        parent.push_back(-1);

    parent[last] = -2;
    visited[last] = true;

    list<int> queue;
    queue.push_back(last);

    list<int>::iterator i;

    bool finished = false;

    while (!queue.empty())
    {
        int current = queue.front();
        queue.pop_front();

        if (current == first)
        {
            if (first == last)
                cout << first << "-";
            while (parent[current] != -2)
            {
                cout << current << "-";
                current = parent[current];
            }
            cout << last << endl;
            finished = true;
        }
        for (i = adjacency[current].begin(); i != adjacency[current].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                parent[*i] = current;
                queue.push_back(*i);
            }
        }
        if (queue.empty() && (!finished))
        {
            cerr << "Error: No path exists" << endl;
        }
    }
}

int main()
{
    int v;
    v = 0;
    vector<int> e;
    // read from stdin until EOF
    while (!cin.eof())
    {
        // read a line of input until EOL and store in a string

        Graph g(15);

        string line;
        getline(cin, line);

        // this vector will store the numbers.
        vector<int> nums;

        if (!line.empty())
        {
            regex rgx(R"(\d+)");
            sregex_iterator iter(line.begin(), line.end(), rgx);
            sregex_iterator end;

            while (iter != end)
            {
                for (unsigned i = 0; i < iter->size(); ++i)
                {
                    nums.push_back(stoi((*iter)[i]));
                }
                ++iter;
            }

            if (line.at(0) == 'V')
            {
                v = nums[0];
                nums.clear();
                e.clear();
            }

            if (line.at(0) == 'E')
            {
                e.clear();
                for (auto i = nums.begin(); i != nums.end(); i++)
                {
                    if (*i >= v)
                    {
                        cerr << "Error: Edge outside of V" << endl;
                        e.clear();
                        break;
                    }

                    e.push_back(*i);
                }
                nums.clear();
                cout << "V " << v << endl;
                cout << line << endl;
            }
            if (line.at(0) == 's')
            {
                Graph g(v);
                for (auto i = e.begin(); i != e.end(); i = i + 2)
                {
                    g.addEdge(*i, *(i + 1));
                }
                if (nums[0] < v && nums[1] < v)
                {
                    g.DFS(nums[0], nums[1]);
                }
                else
                {
                    cerr << "Error: Outside of V" << endl;
                }
                nums.clear();
            }
        }
    }
    return 0;
}
