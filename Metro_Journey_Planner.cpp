#include <bits/stdc++.h>
using namespace std;

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

class Graph_M {
public:
    class Vertex {
    public:
        unordered_map<string, double> nbrs;
    };

    unordered_map<string, Vertex> vtces;

    int num_vertex() {
        return vtces.size();
    }

    bool contains_vertex(const string& v_name) {
        return vtces.find(v_name) != vtces.end();
    }

    void add_vertex(const string& v_name) {
        vtces[v_name] = Vertex();
    }

    void remove_vertex(const string& v_name) {
        Vertex& vtx = vtces[v_name];
        for (const auto& key : vtx.nbrs) {
            Vertex& nbr_vtx = vtces[key.first];
            nbr_vtx.nbrs.erase(v_name);
        }
        vtces.erase(v_name);
    }

    int num_edges() {
        int count = 0;
        for (const auto& key : vtces) {
            count += key.second.nbrs.size();
        }
        return count / 2;
    }

    bool contains_edge(const string& v_name1, const string& v_name2) {
        return (vtces.find(v_name1) != vtces.end() &&
            vtces.find(v_name2) != vtces.end() &&
            vtces[v_name1].nbrs.find(v_name2) != vtces[v_name1].nbrs.end());
    }

    void add_edge(const string& v_name1, const string& v_name2, double value) {
        if (vtces.find(v_name1) == vtces.end() || vtces.find(v_name2) == vtces.end() ||
            vtces[v_name1].nbrs.find(v_name2) != vtces[v_name1].nbrs.end()) {
            return;
        }
        vtces[v_name1].nbrs[v_name2] = value;
        vtces[v_name2].nbrs[v_name1] = value;
    }

    void remove_edge(const string& v_name1, const string& v_name2) {
        if (!contains_edge(v_name1, v_name2)) {
            return;
        }
        vtces[v_name1].nbrs.erase(v_name2);
        vtces[v_name2].nbrs.erase(v_name1);
    }

    void display_map() {
        cout << "\t Hyderabad Metro Map\n";
        cout << "\t------------------\n";
        cout << "----------------------------------------------------\n\n";
        for (auto& key : vtces) {
            string strn = key.first + " =>\n";
            Vertex& vtx = key.second;
            for (const auto& nbr : vtx.nbrs) {
                strn += "\t" + nbr.first + "\t";
                if (nbr.first.length() < 16) strn += "\t";
                if (nbr.first.length() < 8) strn += "\t";
                strn += to_string(nbr.second) + "\n";
            }
            cout << strn;
        }
        cout << "\t------------------\n";
        cout << "---------------------------------------------------\n\n";
    }

    void display_stations() {
        cout << "\n*\n";
        int i = 1;
        for (const auto& key : vtces) {
            cout << i << ". " << key.first << "\n";
            i++;
        }
        cout << "\n*\n";
    }

    bool has_path(const string& v_name1, const string& v_name2, unordered_map<string, bool>& processed) {
        if (contains_edge(v_name1, v_name2)) {
            return true;
        }
        processed[v_name1] = true;
        Vertex& vtx = vtces[v_name1];
        for (const auto& nbr : vtx.nbrs) {
            if (processed.find(nbr.first) == processed.end() && has_path(nbr.first, v_name2, processed)) {
                return true;
            }
        }
        return false;
    }

    class DijkstraPair {
    public:
        string vname;
        string psf;
        double cost;

        bool operator<(const DijkstraPair& other) const {
            return cost < other.cost;
        }
    };

    class Pair {
    public:
        string v_name;
        string psf;
        double min_dis;
        double min_time;
    };

    string get_minimum_distance(const string& src, const string& dst) {
        double mini = numeric_limits<double>::infinity();
        string ans = "";
        unordered_map<string, bool> processed;
        vector<Pair> stack;
        Pair sp;
        sp.v_name = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        stack.push_back(sp);
        while (!stack.empty()) {
            Pair rp = stack.back();
            stack.pop_back();
            if (processed.find(rp.v_name) != processed.end()) {
                continue;
            }
            processed[rp.v_name] = true;
            if (rp.v_name == dst) {
                double temp = rp.min_dis;
                if (temp < mini) {
                    ans = rp.psf;
                    mini = temp;
                }
                continue;
            }
            Vertex& rpvtx = vtces[rp.v_name];
            vector<string> nbrs;
            for (const auto& nbr : rpvtx.nbrs) {
                nbrs.push_back(nbr.first);
            }
            for (const auto& nbr : nbrs) {
                if (processed.find(nbr) == processed.end()) {
                    Pair np;
                    np.v_name = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
                    stack.push_back(np);
                }
            }
        }
        ans += to_string(mini);
        return ans;
    }

    vector<string> get_interchanges(const string& s) {
        vector<string> arr;
        istringstream iss(s);
        string token;
        getline(iss, token, ' ');
        arr.push_back(token);
        int count = 0;
        while (getline(iss, token, ' ')) {
            if (token.find("~") != string::npos) {
                string s = token.substr(token.find("~") + 1);
                if (s.length() == 2) {
                    string prev = arr[arr.size() - 1].substr(arr[arr.size() - 1].find("~") + 1);
                    string next;
                    if (getline(iss, next, ' ')) {
                        next = next.substr(next.find("~") + 1);
                        if (prev == next) {
                            arr.push_back(token);
                        }
                        else {
                            arr.push_back(token + " ==> " + next);
                            count++;
                        }
                    }
                }
                else {
                    arr.push_back(token);
                }
            }
            else {
                arr.push_back(token);
            }
        }
        arr.push_back(to_string(count));
        return arr;
    }

    double cost(double d) {
        if (0 <= d && d < 2) return 10;
        else if (2 <= d && d < 4) return 15;
        else if (4 <= d && d < 6) return 25;
        else if (6 <= d && d < 8) return 30;
        else if (8 <= d && d < 10) return 35;
        else if (10 <= d && d < 14) return 40;
        else if (14 <= d && d < 18) return 45;
        else if (18 <= d && d < 22) return 50;
        else if (22 <= d && d < 26) return 55;
        else return 60;
    }

    void create_metro_map() {
        string current_directory = ".";
        string stations_file_path = current_directory + "/stations.csv";
        string edges_file_path = current_directory + "/edges.csv";

        ifstream fptr(stations_file_path);
        string row;
        while (getline(fptr, row)) {
            add_vertex(row);
        }

        fptr.close();
        fptr.open(edges_file_path);
        string prev;
        bool flag = false;
        while (getline(fptr, row)) {
            row = trim(row);
            if (row.empty()) {
                flag = true;
                continue;
            }
            if (flag) {
                prev = row;
                flag = false;
                continue;
            }
            istringstream iss(row);
            string cur;
            double val;
            getline(iss, cur, ',');
            iss >> val;
            add_edge(prev, cur, val);
            prev = cur;
        }
    }
};

class MetroSystem {
public:
    unordered_map<string, vector<string>> data;

    MetroSystem() {
        data = get_elements();
    }

    unordered_map<string, vector<string>> get_elements() {
        vector<vector<string>> lis(3);
        string current_directory = ".";
        string edges_file_path = current_directory + "/edges.csv";
        ifstream fptr(edges_file_path);
        string row;
        int line_index = 0;
        while (getline(fptr, row)) {
            row = trim(row);
            if (row.empty()) {
                line_index++;
                continue;
            }
            lis[line_index].push_back(row.substr(0, row.find("~")));
        }
        return { {"BlueLine", lis[0]}, {"RedLine", lis[1]}, {"GreenLine", lis[2]} };
    }

    unordered_map<string, string> calculate_sum(const string& source_station, const string& destination_station) {
        Graph_M g;
        g.create_metro_map();
        vector<string> lis;
        for (const auto& key : g.vtces) {
            lis.push_back(key.first);
        }
        string st1, st2;
        for (const auto& st : lis) {
            if (st.find(source_station) != string::npos) {
                st1 = st;
                break;
            }
        }
        for (const auto& st : lis) {
            if (st.find(destination_station) != string::npos) {
                st2 = st;
                break;
            }
        }
        vector<string> arr = g.get_interchanges(g.get_minimum_distance(st1, st2));
        int length = arr.size();
        double dis = round(stod(arr[length - 1]) * 10) / 10;
        double ts = dis * 109;
        int tm = static_cast<int>(ts) / 60;
        int th = tm / 60;
        tm = tm - th * 60;
        ts = static_cast<int>(ts) - tm * 60;
        string time;
        if (th > 0) {
            time += (th == 1) ? "1 Hr " : to_string(th) + " Hrs ";
        }
        if (tm > 0) {
            time += (tm == 1) ? "1 Min " : to_string(tm) + " Mins ";
        }
        if (ts > 0) {
            time += (ts == 1) ? "1 Sec " : to_string(ts) + " Secs ";
        }
        cout << "Number of Interchanges : " << arr[length - 2] << "\n";
        cout << "~\n";
        cout << "Start  ==>  " << arr[0] << "\n";
        for (int i = 1; i < length - 3; i++) {
            cout << arr[i] << "\n";
        }
        cout << arr[length - 3] << "  ==>  End\n";
        cout << "~\n";
        if (time.empty()) {
            time = "0 Secs";
        }
        return {
            {"source_station", st1.substr(0, st1.find("~"))},
            {"destination_station", st2.substr(0, st2.find("~"))},
            {"distance", to_string(dis) + " KM"},
            {"time", time},
            {"cost", "Rs. " + to_string(g.cost(dis))}
        };
    }

    void display_metro_data() {
        cout << "Metro Lines Data:\n";
        for (const auto& line : data) {
            cout << line.first << ": ";
            for (int i = 0;i < line.second.size();i++) {
                cout << line.second[i] << ", "[i != line.second.size() - 1];
            }
            cout << "\n";
        }
    }
};

int main() {
    MetroSystem metro_system;
    int choice;
    do {
        cout << "\n=== Metro System Menu ===\n";
        cout << "1. Display Metro Lines Data\n";
        cout << "2. Calculate Time, Distance, and Cost Between Stations\n";
        cout << "3. Exit\n";

        cout << "Enter your choice (1/2/3): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                metro_system.display_metro_data();
                break;
            }
            case 2: {
                string source_station, destination_station;
                getline(cin, source_station);
                cout << "Enter the source station: ";
                getline(cin, source_station);
                cout << "Enter the destination station: ";
                getline(cin, destination_station);
                auto result = metro_system.calculate_sum(source_station, destination_station);
                cout << "\n--- Result ---\n";
                cout << "Source: " << result["source_station"] << "\n";
                cout << "Destination: " << result["destination_station"] << "\n";
                cout << "Distance: " << result["distance"] << "\n";
                cout << "Time: " << result["time"] << "\n";
                cout << "Cost: " << result["cost"] << "\n";
                break;
            }
            case 3: {
                cout << "Exiting... Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } while (choice != 3);
    return 0;
}
