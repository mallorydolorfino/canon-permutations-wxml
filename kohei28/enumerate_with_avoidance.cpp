#include <bits/stdc++.h>
using namespace std;

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
    vector<int> current_permutation;
    vector<vector<int>> tableau;
    vector<int>prev_list;
    vector<int>next_list;

    Information(Input& input) {
        current_permutation.resize(input.N * input.K, -1);
        tableau.resize(input.N, vector<int>(input.K, -1));
        prev_list.resize(input.N+1, -1);
        next_list.resize(input.N+1, -1);
        prev_list[0] = 0;
        next_list[0] = 0;
    }
};

class Answer {
    vector<vector<int>> answers;

public:
    void add_answer(const vector<int>& answer) {
        answers.push_back(answer);
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
            for (int right = 0; right < (int)information.current_permutation.size(); right++) {
                if (information.current_permutation[right] == converted_pattern[left]) {
                    left++;
                    if (left == (int)converted_pattern.size()) {
                        return true;
                    }
                }
            }
        } while (next_permutation(numbers.begin(), numbers.end()));
    }
    return false;
}

void rec(Input& input, Information& information, Answer& answer, int number , int copy, int prev_copy_pos) {
    int N = input.N;
    int K = input.K;

    if (number > N) {
        //record answer
        answer.add_answer(information.current_permutation);
        return;
    }

    if (copy == 1) {
        int cur = 0;
        for (int i = 0; i < N * K; i++) {
            if (information.current_permutation[i] == information.next_list[cur]) {
                cur = information.next_list[cur];
            } else if (information.current_permutation[i] == -1) {
                information.current_permutation[i] = number;
                information.tableau[number-1][copy-1] = i;

                //insert number after cur in the linked list
                information.prev_list[number] = cur;
                information.next_list[number] = information.next_list[cur];
                information.next_list[cur] = number;
                information.prev_list[information.next_list[number]] = number;

                //call recursion
                if (copy != K || (copy == K && !has_avoidance(input, information, number))) {
                    rec(input, information, answer, (K == copy ? number + 1 : number), (K == copy ? 1 : copy + 1), i);
                }

                //remove number from the linked list
                information.prev_list[information.next_list[number]] = cur;
                information.next_list[cur] = information.next_list[number];
                information.prev_list[number] = -1;
                information.next_list[number] = -1;

                //backtrack Information
                information.tableau[number-1][copy-1] = -1;
                information.current_permutation[i] = -1;
            }
        }
    } else {
        int lower = (information.prev_list[number] == 0? -1: information.tableau[information.prev_list[number]-1][copy-1]);
        int upper = (information.next_list[number] == 0? N * K: information.tableau[information.next_list[number]-1][copy-1]);
        for (int i = max(prev_copy_pos, lower) + 1; i < min(N * K, upper); i++) {
            if (information.current_permutation[i] == -1) {
                    //update Information
                    information.current_permutation[i] = number;
                    information.tableau[number-1][copy-1] = i;

                    //recursive call
                    int next_number = (copy == K? number+1: number);
                    int next_copy = (copy == K? 1: copy+1);
                    int next_prev_copy_pos = (copy == K? -1: i);

                    if (copy != K || (copy == K && !has_avoidance(input, information, number))) {
                        rec(input, information, answer, next_number, next_copy, next_prev_copy_pos);
                    }

                    //backtrack Information
                    information.tableau[number-1][copy-1] = -1;
                    information.current_permutation[i] = -1;
            }
        }
    }
}

void solve() {
    Input input;
    Information information(input);
    Answer answer;
    rec(input, information, answer, 1, 1, -1);
    answer.print_answers();
}

int main() {
    //ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}