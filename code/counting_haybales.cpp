#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

using interval = pair<int, int>;

int len(interval itv) { return itv.second - itv.first; }

string repr(interval itv) {
  char buf[100];
  sprintf(buf, "[%d, %d)", itv.first, itv.second);
  return string(buf);
}

int mid(interval itv) { return (itv.first + itv.second + 1) / 2; }

pair<interval, interval> half(interval itv) {
  int m = mid(itv);
  return make_pair(interval(itv.first, m), interval(m, itv.second));
}

bool is_disjoint(interval a, interval b) { return a.second <= b.first || b.second <= a.first; }
bool contains(interval a, interval b) { return a.first <= b.first and a.second >= b.second; }

struct Node {
  long sum;
  long min;
  long lazy;
  interval itv;
};

string node_repr(const Node& node) {
  char buf[100];
  if (len(node.itv) == 0) {
    return string("dummy");
  }
  sprintf(buf, "(sum: %ld, min: %ld, lazy: %ld, itv: %s)", node.sum, node.min, node.lazy,
          repr(node.itv).c_str());
  return string(buf);
}

void build(vector<Node>& t, long arr[], int i, interval itv) {
  t[i].itv = itv;
  if (len(itv) == 1) {
    t[i].sum = arr[itv.first];
    t[i].min = arr[itv.first];
  } else if (len(itv) == 0) {
    t[i].sum = 0;
    t[i].min = numeric_limits<int>::max();
  } else {
    auto halfs = half(itv);

    build(t, arr, i * 2 + 1, halfs.first);
    build(t, arr, i * 2 + 2, halfs.second);
    t[i].sum = t[i * 2 + 1].sum + t[i * 2 + 2].sum;
    t[i].min = min(t[i * 2 + 1].min, t[i * 2 + 2].min);
  }
}

long range_sum(vector<Node>& t, int i, interval qitv, long lazy_sum) {
  if (contains(qitv, t[i].itv)) {
    return t[i].sum + lazy_sum * len(t[i].itv);
  } else if (is_disjoint(qitv, t[i].itv)) {
    return 0;
  } else {
    lazy_sum += t[i].lazy;
    return range_sum(t, i * 2 + 1, qitv, lazy_sum) + range_sum(t, i * 2 + 2, qitv, lazy_sum);
  }
}

long range_min(vector<Node>& t, int i, interval qitv, long lazy_sum) {
  if (contains(qitv, t[i].itv)) {
    return t[i].min + lazy_sum;
  } else if (is_disjoint(qitv, t[i].itv)) {
    return numeric_limits<int>::max();
  } else {
    lazy_sum += t[i].lazy;
    return min(range_min(t, i * 2 + 1, qitv, lazy_sum), range_min(t, i * 2 + 2, qitv, lazy_sum));
  }
}

void range_update(vector<Node>& t, int i, interval qitv, long value) {
  if (contains(qitv, t[i].itv)) {
    t[i].lazy += value;
    t[i].sum += len(t[i].itv) * value;
    t[i].min += value;
  } else if (is_disjoint(qitv, t[i].itv)) {
    return;
  } else {
    range_update(t, i * 2 + 1, qitv, value);
    range_update(t, i * 2 + 2, qitv, value);
    t[i].sum = t[i * 2 + 1].sum + t[i * 2 + 2].sum + len(t[i].itv) * t[i].lazy;
    t[i].min = min(t[i * 2 + 1].min, t[i * 2 + 2].min) + t[i].lazy;
  }
}

void print_tree(const vector<Node>& t) {
  for (int j = 0; j < t.size(); ++j) {
    cout << j << ": " << node_repr(t[j]) << endl;
  }
}

int main() {
  ifstream fin("testdata/counting_haybales/4.in", ifstream::in);
  int n;
  int q;
  fin >> n >> q;
  long arr[n];
  for (int i = 0; i < n; i++) {
    fin >> arr[i];
  }

  vector<Node> t(4 * n);
  build(t, arr, 0, interval(0, n));
  for (int i = 0; i < q; i++) {
    char letter;
    fin >> letter;
    if (letter == 'M') {
      long a, b;
      fin >> a >> b;
      cout << range_min(t, 0, interval(a - 1, b), 0) << endl;
    } else if (letter == 'S') {
      long a, b;
      fin >> a >> b;
      cout << range_sum(t, 0, interval(a - 1, b), 0) << endl;
    } else {
      long a, b, value;
      fin >> a >> b >> value;
      range_update(t, 0, interval(a - 1, b), value);
    }
  }
}