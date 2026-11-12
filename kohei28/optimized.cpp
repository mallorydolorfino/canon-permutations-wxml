#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
class Input {
public:
    int N;
    int K;
    vector<vector<int>> avoidance;

    Input() {
        cout << "Enter N: ";
        cin >> N;
        cout << "Enter K: ";
        cin >> K;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //Until inputted -1, read input line by line and separate each by blank space
        cout << "Enter avoidance, separating each integer by blank space.\n";
        //put instructions for input format
        cout << "Each avoidance must be constructed by consecutive integers from 1.\n"
             << "To stop input, enter -1.\n";
        
        string line;
        while (true) {
            cout << "Enter avoidance: ";
            if (!getline(cin, line) || line == "-1") {
                break;
            }
            vector<int> row;
            stringstream ss(line);
            int value;
            while (ss >> value) {
                row.push_back(value);
            }
            avoidance.push_back(row);
        }
    }
};

//All information shared throughout the entire code is stored here.
class Information {
public:
    vector<vector<pair<int, int>>> linked_list;
    vector<int>sigma_prev;
    vector<int>sigma_next;

    Information(Input& input) {
        linked_list.resize(input.N + 1, vector<pair<int, int>>(input.K + 1, {-1, -1}));
        //linked_list[0][0] is the head;
        //put all 1s in the linked list
        linked_list[0][0] = {1, 1};
        for (int j = 1; j < input.K; j++) {
            linked_list[1][j] = {1, j + 1};
        }
        linked_list[1][input.K] = {0, 0};

        sigma_prev.resize(input.N+1, -1);
        sigma_next.resize(input.N+1, -1);
        sigma_prev[0] = 1;
        sigma_prev[1] = 0;
        sigma_next[0] = 1;
        sigma_next[1] = 0;
    }
};

class Answer {
    vector<vector<int>> answers;

public:
    void add_answer(const Information& information) {
        pair<int, int> cur_pos = {0, 0};
        answers.push_back(vector<int>());
        while (true) {
            cur_pos = information.linked_list[cur_pos.first][cur_pos.second];
            if (cur_pos.first == 0 && cur_pos.second == 0) {
                break;
            }
            answers.back().push_back(cur_pos.first);
        }
    }

    void print_answers() const {
        cout << "All answers:\n";
        cout << answers.size() << "\n";
        for (const auto& answer : answers) {
            //cout << "[";
            for (int i = 0; i < (int)answer.size(); i++) {
                //cout << answer[i] << (i == (int)answer.size() - 1 ? "" : ", ");
            }
            //cout << "]\n";
        }
    }

};

bool has_avoidance(Input& input, Information& information, int number) {
    //for each avoidance pattern, check if it appears or not
    for (const auto& pattern : input.avoidance) {
        //first find the maximum number in the pattern
        int max_number = *max_element(pattern.begin(), pattern.end());

        //second, enumerate (number-1)C(max_number-1) by next_permutation
        if (max_number > number) {
            continue;
        }
        vector<int> numbers, convert_list;
        vector<int> converted_pattern(pattern.size());
        for (int i = 0; i < max_number - 1; i++) {
            numbers.push_back(1);
        }
        for (int i = 0; i < number - max_number; i++) {
            numbers.push_back(0);
        }
        
        sort(numbers.begin(), numbers.end());
        //check all patterns whether it is included in the permutation or not
        do {
            convert_list.clear();
            for (int i = 0; i < (int)numbers.size(); i++) {
                if (numbers[i] == 1) {
                    convert_list.push_back(i + 1);
                }
            }
            
            for (int i = 0; i < (int)pattern.size(); i++) {
                converted_pattern[i] = (pattern[i] == max_number ? number : convert_list[pattern[i] - 1]);
            }

            //check if the converted pattern is included in the current permutation by Two Pointers
            int left = 0;
            pair<int, int> cur_pos = {0, 0};
            while (true) {
                cur_pos = information.linked_list[cur_pos.first][cur_pos.second];
                if (cur_pos.first == converted_pattern[left]) {
                    left++;
                    if (left == (int)converted_pattern.size()) {
                        return true;
                    }
                } 
                if (cur_pos.first == 0 && cur_pos.second == 0) {
                    break;
                }
            }
        } while (next_permutation(numbers.begin(), numbers.end()));
    }
    return false;
}

void rec(Input& input, Information& information, Answer& answer, pair<int, int>x, pair<int, int>y) {
    int N = input.N;
    int K = input.K;
    int number = x.first;
    int copy = x.second;

    if (number > N) {
        //record answer
        if (N == 1) {
            if (has_avoidance(input, information, 1)) {
                return;
            }
        }
        answer.add_answer(information);
        return;
    }

    if (copy == 1) {
        pair<int, int> cur_pos = {0, 0};
        int prev_element = 0;
        int cnt = INF;
        while (true) {

            if (information.sigma_next[prev_element] == cur_pos.first) {
                prev_element = cur_pos.first;
                cnt = 1;
            } else if (prev_element == cur_pos.first) {
                cnt++;
            }
            //insert {number, copy} after cur_pos in the linked list
            information.linked_list[number][copy] = information.linked_list[cur_pos.first][cur_pos.second];
            information.linked_list[cur_pos.first][cur_pos.second] = {number, copy};

            //insert number after prev_element in sigma linked_list
            information.sigma_prev[number] = prev_element;
            information.sigma_next[number] = information.sigma_next[prev_element];
            information.sigma_next[prev_element] = number;
            information.sigma_prev[information.sigma_next[number]] = number;

            //call recursion
            if (copy != K || (copy == K && !has_avoidance(input, information, number))) {
                rec(input, information, answer, { (K == copy ? number + 1 : number), (K == copy ? 1 : copy + 1) }, {prev_element, cnt});
            }

            //backtrack Information
            information.sigma_prev[information.sigma_next[number]] = prev_element;
            information.sigma_next[prev_element] = information.sigma_next[number];
            information.sigma_prev[number] = -1;
            information.sigma_next[number] = -1;

            information.linked_list[cur_pos.first][cur_pos.second] = information.linked_list[number][copy];
            information.linked_list[number][copy] = {-1, -1};

            cur_pos = information.linked_list[cur_pos.first][cur_pos.second];
            if (cur_pos.first == 0 && cur_pos.second == 0) {
                break;
            }
        }
    } else {
        pair<int, int> lower = (y.second >= copy? make_pair(x.first, x.second - 1): make_pair(y.first, copy));
        pair<int, int> upper = (information.sigma_next[number] == 0? make_pair(0, 0): make_pair(information.sigma_next[number], copy));
        pair<int, int>cur_pos = lower;
        
        pair<int, int>new_y = y;

        while (true) {
            if (cur_pos.first == y.first) {
                new_y.second = cur_pos.second;
            }
            //insert {number, copy} after cur_pos in the linked list
            information.linked_list[number][copy] = information.linked_list[cur_pos.first][cur_pos.second];
            information.linked_list[cur_pos.first][cur_pos.second] = {number, copy};

            //call recursion
            if (copy != K || (copy == K && !has_avoidance(input, information, number))) {
                rec(input, information, answer, { (K == copy ? number + 1 : number), (K == copy ? 1 : copy + 1) }, new_y);
            }
            //backtrack Information
            information.linked_list[cur_pos.first][cur_pos.second] = information.linked_list[number][copy];
            information.linked_list[number][copy] = {-1, -1};

            cur_pos = information.linked_list[cur_pos.first][cur_pos.second];
            if (cur_pos == upper) {
                break;
            }
        }
    }
}

void solve() {
    Input input;
    Information information(input);
    Answer answer;
    rec(input, information, answer, {2, 1}, {-1, -1});
    answer.print_answers();
}

int main() {
    //ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}