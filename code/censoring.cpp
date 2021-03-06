#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

vector<int> cmp_lps(char pat[], int m) {
  vector<int> lps(m);

  int len = 0;
  int i = 1;

  while (i < m) {
    if (pat[i] == pat[len]) {
      len++;
      lps[i] = len;
      i++;
    } else {
      if (len != 0) {
        len = lps[len - 1];
      } else {
        lps[i] = 0;
        i++;
      }
    }
  }
  return lps;
}

string stack_to_str(stack<pair<char, int>>& stack) {
  string ret;
  int n = stack.size();
  ret.resize(stack.size(), ' ');
  int k = 0;
  while (not stack.empty()) {
    ret[n - k - 1] = stack.top().first;
    ++k;
    stack.pop();
  }
  return ret;
}

string censor(char pat[], char str[], int m, int n) {
  vector<int> lps = cmp_lps(pat, m);
  stack<pair<char, int>> stack;

  int i = 0, j = 0;  // i length of match, j scanning through str

  while (j < n) {
    if (str[j] == pat[i]) {
      stack.push(make_pair(str[j++], ++i));
    } else {
      if (i != 0) {
        i = lps[i - 1];
      } else {
        stack.push(make_pair(str[j++], 0));
      }
    }

    if (i == m) {
      for (int k = 0; k < m; ++k) {
        stack.pop();
      }

      i = stack.empty() ? 0 : stack.top().second;
    }
  }

  return stack_to_str(stack);
}

int main() {
  string txt, w;
  ifstream fin("testdata/censoring/15.in", ifstream::in);
  fin >> txt >> w;

  int n = txt.length(), m = w.length();
  char str[n + 1];
  strcpy(str, txt.c_str());

  char pat[m + 1];
  strcpy(pat, w.c_str());

  cout << censor(pat, str, m, n);
}