/* Solver core */

#include <bits/stdc++.h>
using namespace std;

#include "solver.h"
#include "interface.h"

namespace solverGlobalVariables {
    const int MXN = 55;
    struct Node {
        bool type;
        int x, y, val;
        Node() {}
        Node(bool type, int x, int y, int val) : type(type), x(x), y(y), val(val) {}
        bool operator < (const Node& b) const {
            return abs(val) > abs(b.val);
        }
    };
    int r[MXN][MXN], d[MXN][MXN], *a[MXN], n, m, mxdep, dfscnt, _TIME_;
    bool VISIBLE;

    struct Dsu {
        int f[10005], siz[10005];
        vector <pair <int*, int> > stk;
        inline void Init() {
            for (int i = 0;i <= (n + 1) * (m + 1);i++) {
                f[i] = i; siz[i] = 1;
            }
            stk.clear();
        }
        inline int GetRoot(int v) {
            if (f[v] == v) return v;
            return GetRoot(f[v]);
        }
        inline bool Merge(int x, int y) {
            int u = GetRoot(x), v = GetRoot(y);
            if (u != v) {
                if (siz[u] < siz[v]) swap(u, v);
                stk.emplace_back(&f[v], f[v]); stk.emplace_back(&siz[u], siz[u]);
                f[v] = u; siz[u] += siz[v];
                return 1;
            }
            return 0;
        }
        inline void Rollback(int pos) {
            while (stk.size() > pos) {
                *(stk.back().first) = stk.back().second;
                stk.pop_back();
            }
        }
    };
    Dsu dsu;

	struct extEdge {
		bool tp, val;
		int x, y, depv;
		extEdge() {}
		extEdge(bool tp, int x, int y, bool val, int depv) : tp(tp), x(x), y(y), val(val), depv(depv) {}
		bool operator < (const extEdge& b) const {
			return make_pair(depv, make_pair(x, make_pair(y, make_pair(tp, val))))
					 > make_pair(b.depv, make_pair(b.x, make_pair(b.y, make_pair(b.tp, b.val))));
		}
	};
	set <extEdge> g[2][MXN][MXN][2];
	vector <extEdge> tmp[2][MXN][MXN][2];
	int adjSiz[2][MXN][MXN][2];
}

// set all unknown edges as non-cycle edges
void rmvMinus() {
    using namespace solverGlobalVariables;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j < m;j++) r[i][j] = max(r[i][j], 0);
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j <= m;j++) d[i][j] = max(d[i][j], 0);
	}
}

// Get the value of a specific position in the puzzle
int getVal(int x, int y, int tp) {
    using namespace solverGlobalVariables;
	if (tp == 0) return r[x][y];
	else return d[x][y];
}

// Check whether a complete solution is valid
// solution is saved in r[][] and d[][]
bool chkValid() {
    using namespace solverGlobalVariables;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) {
			if (a[i][j] >= 0) {
				if (max(r[i][j], 0) + max(d[i][j], 0) + max(r[i + 1][j], 0) + max(d[i][j + 1], 0) != a[i][j]) return 0;
			}
		}
	}
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j <= m;j++) {
			int sum = 0;
			if (i) sum += max(d[i - 1][j], 0);
			if (j) sum += max(r[i][j - 1], 0);
			if (i < n) sum += max(d[i][j], 0);
			if (j < m) sum += max(r[i][j], 0);
			if (sum != 0 && sum != 2) return 0;
		}
	}
	int cur = -1;
	for (int i = 0;i <= (n + 1) * (m + 1);i++) {
		if (dsu.GetRoot(i) == i && dsu.siz[i] == 1) continue;
		if (cur < 0) cur = dsu.GetRoot(i);
		else if (cur != dsu.GetRoot(i)) return 0;
	}
	return 1;
}

// Check whether there exists a connected component of unknown edges that have odd numbers of cycle edges connecting to it
bool chkOddDegrees() {
    using namespace solverGlobalVariables;
    bool cvis[MXN][MXN] = {0};
	//return 1;
	queue <pair <int, int> > que;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j <= m;j++) {
			if (!cvis[i][j]) {
				que.push(make_pair(i, j));
				cvis[i][j] = 1;
				int cnt = 0;
				while (!que.empty()) {
					int x = que.front().first, y = que.front().second;
					que.pop();
					if (x) {
						if (d[x - 1][y] == -1 && !cvis[x - 1][y]) {
							cvis[x - 1][y] = 1;
							que.push(make_pair(x - 1, y));
						} else cnt += (d[x - 1][y] == 1);
					}
					if (y) {
						if (r[x][y - 1] == -1 && !cvis[x][y - 1]) {
							cvis[x][y - 1] = 1;
							que.push(make_pair(x, y - 1));
						} else cnt += (r[x][y - 1] == 1);
					}
					if (x < n) {
						if (d[x][y] == -1 && !cvis[x + 1][y]) {
							cvis[x + 1][y] = 1;
							que.push(make_pair(x + 1, y));
						} else cnt += (d[x][y] == 1);
					}
					if (y < m) {
						if (r[x][y] == -1 && !cvis[x][y + 1]) {
							cvis[x][y + 1] = 1;
							que.push(make_pair(x, y + 1));
						} else cnt += (r[x][y] == 1);
					}
				}
				if (cnt & 1) return 0;
			}
		}
	}
	return 1;
}

// Search from the edge (stp, sx, sy) for all edges of which the status can be confirmed
// return 0 if contradiction, -1 if found solution, other values indicate edge count
int Bfs(int stp, int sx, int sy, bool chkOdd, int cdep, bool addG) {
    using namespace solverGlobalVariables;
	//bool DEBUG = (stp == 1 && sx == 0 && sy == 13 && d[sx][sy]);
	bool DEBUG = 0;
	int rtv = 0;
	queue <Node> que;
	que.push(Node(stp, sx, sy, getVal(sx, sy, stp)));
	while (!que.empty()) {
		//Print();
		Node cur = que.front();
		// assert(cur.val == 0 || cur.val == 1);
		// if (DEBUG) cout << "ext " << cur.type << " " << cur.x << " " << cur.y << " " << cur.val << endl;
		que.pop();
		if (addG) {
			tmp[stp][sx][sy][getVal(sx, sy, stp)].push_back(extEdge(cur.type, cur.x, cur.y, cur.val, cdep));
			tmp[cur.type][cur.x][cur.y][!cur.val].push_back(extEdge(stp, sx, sy, !getVal(sx, sy, stp), cdep));
		}
		rtv++;
		if (cur.val) {
			bool ok;
			if (cur.type == 0) ok = dsu.Merge(cur.x * (m + 1) + cur.y, cur.x * (m + 1) + cur.y + 1);
			else ok = dsu.Merge(cur.x * (m + 1) + cur.y, (cur.x + 1) * (m + 1) + cur.y);
			if (!ok) {
				if (!chkValid()) return 0;
				else {
					rmvMinus();
					return -1;
				}
			}
		}
		// extend known relationships
		for (extEdge e : g[cur.type][cur.x][cur.y][getVal(cur.x, cur.y, cur.type)]) {
			if (getVal(e.x, e.y, e.tp) == -1) {
				if (e.tp == 0) r[e.x][e.y] = e.val;
				else d[e.x][e.y] = e.val;
				que.push(Node(e.tp, e.x, e.y, e.val));
			} else if (getVal(e.x, e.y, e.tp) != e.val) return 0;
		}
		if (cur.type == 0) {
			// horizontal line
			if (cur.x) {
				// extend upper square
				if (a[cur.x - 1][cur.y] >= 0) {
					int sum = a[cur.x - 1][cur.y];
					int cnt = 0;
					if (r[cur.x - 1][cur.y] >= 0) sum -= r[cur.x - 1][cur.y];
					else cnt++;
					if (d[cur.x - 1][cur.y] >= 0) sum -= d[cur.x - 1][cur.y];
					else cnt++;
					if (r[cur.x][cur.y] >= 0) sum -= r[cur.x][cur.y];
					else cnt++;
					if (d[cur.x - 1][cur.y + 1] >= 0) sum -= d[cur.x - 1][cur.y + 1];
					else cnt++;
					if (sum < 0 || sum > cnt) return 0;
					else if (sum == 0) {
						if (r[cur.x - 1][cur.y] < 0) que.push(Node(0, cur.x - 1, cur.y, r[cur.x - 1][cur.y] = 0));
						if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 0));
						if (d[cur.x - 1][cur.y + 1] < 0) que.push(Node(1, cur.x - 1, cur.y + 1, d[cur.x - 1][cur.y + 1] = 0));
					} else if (sum == cnt) {
						if (r[cur.x - 1][cur.y] < 0) que.push(Node(0, cur.x - 1, cur.y, r[cur.x - 1][cur.y] = 1));
						if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 1));
						if (d[cur.x - 1][cur.y + 1] < 0) que.push(Node(1, cur.x - 1, cur.y + 1, d[cur.x - 1][cur.y + 1] = 1));
					}
				}
			}
			if (cur.x < n) {
				//extend lower square
				if (a[cur.x][cur.y] >= 0) {
					int sum = a[cur.x][cur.y];
					int cnt = 0;
					if (r[cur.x][cur.y] >= 0) sum -= r[cur.x][cur.y];
					else cnt++;
					if (d[cur.x][cur.y] >= 0) sum -= d[cur.x][cur.y];
					else cnt++;
					if (r[cur.x + 1][cur.y] >= 0) sum -= r[cur.x + 1][cur.y];
					else cnt++;
					if (d[cur.x][cur.y + 1] >= 0) sum -= d[cur.x][cur.y + 1];
					else cnt++;
					if (sum < 0 || sum > cnt) return 0;
					else if (sum == 0) {
						if (d[cur.x][cur.y] < 0) que.push(Node(1, cur.x, cur.y, d[cur.x][cur.y] = 0));
						if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 0));
						if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 0));
					} else if (sum == cnt) {
						if (d[cur.x][cur.y] < 0) que.push(Node(1, cur.x, cur.y, d[cur.x][cur.y] = 1));
						if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 1));
						if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 1));
					}
				}
			}
			//extend left node
			int cnt = 0, sum = cur.val;
			if (cur.y) {
				if (r[cur.x][cur.y - 1] >= 0) sum += r[cur.x][cur.y - 1];
				else cnt++;
			}
			if (cur.x) {
				if (d[cur.x - 1][cur.y] >= 0) sum += d[cur.x - 1][cur.y];
				else cnt++;
			}
			if (cur.x < n) {
				if (d[cur.x][cur.y] >= 0) sum += d[cur.x][cur.y];
				else cnt++;
			}
			if (sum > 2 || (sum > 0 && sum + cnt < 2)) return 0;
			if ((sum == 0 && cnt < 2) || sum == 2) {
				if (cur.y) {
					if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 0));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 0));
				}
				if (cur.x < n) {
					if (d[cur.x][cur.y] < 0) que.push(Node(1, cur.x, cur.y, d[cur.x][cur.y] = 0));
				}
			} else if (sum > 0 && sum + cnt == 2) {
				if (cur.y) {
					if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 1));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 1));
				}
				if (cur.x < n) {
					if (d[cur.x][cur.y] < 0) que.push(Node(1, cur.x, cur.y, d[cur.x][cur.y] = 1));
				}
			}
			//extend right node
			cnt = 0; sum = cur.val;
			if (cur.y < m - 1) {
				if (r[cur.x][cur.y + 1] >= 0) sum += r[cur.x][cur.y + 1];
				else cnt++;
			}
			if (cur.x) {
				if (d[cur.x - 1][cur.y + 1] >= 0) sum += d[cur.x - 1][cur.y + 1];
				else cnt++;
			}
			if (cur.x < n) {
				if (d[cur.x][cur.y + 1] >= 0) sum += d[cur.x][cur.y + 1];
				else cnt++;
			}
			if (sum > 2 || (sum > 0 && sum + cnt < 2)) return 0;
			if ((sum == 0 && cnt < 2) || sum == 2) {
				if (cur.y < m - 1) {
					if (r[cur.x][cur.y + 1] < 0) que.push(Node(0, cur.x, cur.y + 1, r[cur.x][cur.y + 1] = 0));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y + 1] < 0) que.push(Node(1, cur.x - 1, cur.y + 1, d[cur.x - 1][cur.y + 1] = 0));
				}
				if (cur.x < n) {
					if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 0));
				}
			} else if (sum > 0 && sum + cnt == 2) {
				if (cur.y < m - 1) {
					if (r[cur.x][cur.y + 1] < 0) que.push(Node(0, cur.x, cur.y + 1, r[cur.x][cur.y + 1] = 1));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y + 1] < 0) que.push(Node(1, cur.x - 1, cur.y + 1, d[cur.x - 1][cur.y + 1] = 1));
				}
				if (cur.x < n) {
					if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 1));
				}
			}
		} else {
			// vertical line
			if (cur.y) {
				// extend left square
				if (a[cur.x][cur.y - 1] >= 0) {
					int sum = a[cur.x][cur.y - 1];
					// cout << "sum = " << sum << endl;
					int cnt = 0;
					if (r[cur.x][cur.y - 1] >= 0) sum -= r[cur.x][cur.y - 1];
					else cnt++;
					if (d[cur.x][cur.y - 1] >= 0) sum -= d[cur.x][cur.y - 1];
					else cnt++;
					if (r[cur.x + 1][cur.y - 1] >= 0) sum -= r[cur.x + 1][cur.y - 1];
					else cnt++;
					if (d[cur.x][cur.y] >= 0) sum -= d[cur.x][cur.y];
					else cnt++;
					//cout << "sum = " << sum << " cnt = " << cnt << endl;
					if (sum < 0 || sum > cnt) return 0;
					else if (sum == 0) {
						if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 0));
						if (d[cur.x][cur.y - 1] < 0) que.push(Node(1, cur.x, cur.y - 1, d[cur.x][cur.y - 1] = 0));
						if (r[cur.x + 1][cur.y - 1] < 0) que.push(Node(0, cur.x + 1, cur.y - 1, r[cur.x + 1][cur.y - 1] = 0));
					} else if (sum == cnt) {
						if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 1));
						if (d[cur.x][cur.y - 1] < 0) que.push(Node(1, cur.x, cur.y - 1, d[cur.x][cur.y - 1] = 1));
						if (r[cur.x + 1][cur.y - 1] < 0) que.push(Node(0, cur.x + 1, cur.y - 1, r[cur.x + 1][cur.y - 1] = 1));
					}
				}
			}
			if (cur.y < m) {
				//extend right square
				if (a[cur.x][cur.y] >= 0) {
					int sum = a[cur.x][cur.y];
					int cnt = 0;
					if (r[cur.x][cur.y] >= 0) sum -= r[cur.x][cur.y];
					else cnt++;
					if (d[cur.x][cur.y] >= 0) sum -= d[cur.x][cur.y];
					else cnt++;
					if (r[cur.x + 1][cur.y] >= 0) sum -= r[cur.x + 1][cur.y];
					else cnt++;
					if (d[cur.x][cur.y + 1] >= 0) sum -= d[cur.x][cur.y + 1];
					else cnt++;
					if (sum < 0 || sum > cnt) return 0;
					else if (sum == 0) {
						if (r[cur.x][cur.y] < 0) que.push(Node(0, cur.x, cur.y, r[cur.x][cur.y] = 0));
						if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 0));
						if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 0));
					} else if (sum == cnt) {
						if (r[cur.x][cur.y] < 0) que.push(Node(0, cur.x, cur.y, r[cur.x][cur.y] = 1));
						if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 1));
						if (d[cur.x][cur.y + 1] < 0) que.push(Node(1, cur.x, cur.y + 1, d[cur.x][cur.y + 1] = 1));
					}
				}
			}
			//extend upper node
			int cnt = 0, sum = cur.val;
			if (cur.y) {
				if (r[cur.x][cur.y - 1] >= 0) sum += r[cur.x][cur.y - 1];
				else cnt++;
			}
			if (cur.x) {
				if (d[cur.x - 1][cur.y] >= 0) sum += d[cur.x - 1][cur.y];
				else cnt++;
			}
			if (cur.y < m) {
				if (r[cur.x][cur.y] >= 0) sum += r[cur.x][cur.y];
				else cnt++;
			}
			if (sum > 2 || (sum > 0 && sum + cnt < 2)) return 0;
			if ((sum == 0 && cnt < 2) || sum == 2) {
				if (cur.y) {
					if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 0));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 0));
				}
				if (cur.y < m) {
					if (r[cur.x][cur.y] < 0) que.push(Node(0, cur.x, cur.y, r[cur.x][cur.y] = 0));
				}
			} else if (sum > 0 && sum + cnt == 2) {
				if (cur.y) {
					if (r[cur.x][cur.y - 1] < 0) que.push(Node(0, cur.x, cur.y - 1, r[cur.x][cur.y - 1] = 1));
				}
				if (cur.x) {
					if (d[cur.x - 1][cur.y] < 0) que.push(Node(1, cur.x - 1, cur.y, d[cur.x - 1][cur.y] = 1));
				}
				if (cur.y < m) {
					if (r[cur.x][cur.y] < 0) que.push(Node(0, cur.x, cur.y, r[cur.x][cur.y] = 1));
				}
			}
			//extend lower node
			cnt = 0; sum = cur.val;
			if (cur.y < m) {
				if (r[cur.x + 1][cur.y] >= 0) sum += r[cur.x + 1][cur.y];
				else cnt++;
			}
			if (cur.y > 0) {
				if (r[cur.x + 1][cur.y - 1] >= 0) sum += r[cur.x + 1][cur.y - 1];
				else cnt++;
			}
			if (cur.x < n - 1) {
				if (d[cur.x + 1][cur.y] >= 0) sum += d[cur.x + 1][cur.y];
				else cnt++;
			}
			if (sum > 2 || (sum > 0 && sum + cnt < 2)) return 0;
			if ((sum == 0 && cnt < 2) || sum == 2) {
				if (cur.y < m) {
					if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 0));
				}
				if (cur.y > 0) {
					if (r[cur.x + 1][cur.y - 1] < 0) que.push(Node(0, cur.x + 1, cur.y - 1, r[cur.x + 1][cur.y - 1] = 0));
				}
				if (cur.x < n - 1) {
					if (d[cur.x + 1][cur.y] < 0) que.push(Node(1, cur.x + 1, cur.y, d[cur.x + 1][cur.y] = 0));
				}
			} else if (sum > 0 && sum + cnt == 2) {
				if (cur.y < m) {
					if (r[cur.x + 1][cur.y] < 0) que.push(Node(0, cur.x + 1, cur.y, r[cur.x + 1][cur.y] = 1));
				}
				if (cur.y > 0) {
					if (r[cur.x + 1][cur.y - 1] < 0) que.push(Node(0, cur.x + 1, cur.y - 1, r[cur.x + 1][cur.y - 1] = 1));
				}
				if (cur.x < n - 1) {
					if (d[cur.x + 1][cur.y] < 0) que.push(Node(1, cur.x + 1, cur.y, d[cur.x + 1][cur.y] = 1));
				}
			}
		}
	}
	if (!chkOdd) return rtv;
	else return rtv * chkOddDegrees();
}

// DFS through all branches of the puzzle-solving process
// return 1 if found solution, 0 if no solution
bool Dfs(int dep) {
    using namespace solverGlobalVariables;
	dfscnt++;
	//assert(dfscnt==1);
	int tr[MXN][MXN], td[MXN][MXN], rc[MXN][MXN][2], dc[MXN][MXN][2];
	vector <Node> ext;
	start:;
	int pos = dsu.stk.size(); memcpy(td, d, sizeof(td)); memcpy(tr, r, sizeof(tr));
	auto Revert = [&](bool flg) -> void {
		memcpy(d, td, sizeof(d)); memcpy(r, tr, sizeof(r));
		dsu.Rollback(pos);
		if (flg) {
			for (int s = 0;s < 2;s++) {
				for (int i = 0;i <= n;i++) {
					for (int j = 0;j <= m;j++) {
						for (int v = 0;v < 2;v++) {
							while (!g[s][i][j][v].empty() && g[s][i][j][v].begin()->depv > dep) g[s][i][j][v].erase(g[s][i][j][v].begin());
							adjSiz[s][i][j][v] = g[s][i][j][v].size();
						}
					}
				}
			}
		}
	};
	if (VISIBLE) Print(dep, mxdep, dfscnt, n, m, a, r, d, _TIME_);
	for (int chkOdd = 0;chkOdd < 2;chkOdd++) {
		//Print();
		//cout << tr[0][0] << endl;
		for (int i = 0;i <= n;i++) {
			for (int j = 0;j < m;j++) {
				Revert(0);
				if (r[i][j] >= 0) continue;
				r[i][j] = 0;
				if (!(rc[i][j][0] = Bfs(0, i, j, chkOdd, dep, 0))) {
					//cout << 0 << " " << i << " " << j << " 0" << endl;
					Revert(0);
					//if (!i && !j) Print();
					r[i][j] = 1;
					int tres = Bfs(0, i, j, chkOdd, dep, 0);
					if (tres == 0) return 0;
					else if (tres == -1) return 1;
					goto start;
				} else if (rc[i][j][0] == -1) {
					return 1;
				} else {
					Revert(0);
					r[i][j] = 1;
					if (!(rc[i][j][1] = Bfs(0, i, j, chkOdd, dep, 0))) {
						//cout << 0 << " " << i << " " << j << " 1" << endl;
						Revert(0);
						r[i][j] = 0;
						int tres = Bfs(0, i, j, chkOdd, dep, 0);
						if (tres == 0) return 0;
						else if (tres == -1) return 1;
						goto start;
					} else if (rc[i][j][1] == -1) {
						return 1;
					}
				}
			}
		}
		//Print();
		for (int i = 0;i < n;i++) {
			for (int j = 0;j <= m;j++) {
				Revert(0);
				if (d[i][j] >= 0) continue;
				d[i][j] = 0;
				if (!(dc[i][j][0] = Bfs(1, i, j, chkOdd, dep, 0))) {
					//cout << 1 << " " << i << " " << j << " 0" << endl;
					//if (i == 49 && j == 32) Print();
					Revert(0);
					d[i][j] = 1;
					int tres = Bfs(1, i, j, chkOdd, dep, 0);
					if (tres == 0) return 0;
					else if (tres == -1) return 1;
					goto start;
				} else if (dc[i][j][0] == -1) {
					return 1;
				} else {
					Revert(0);
					d[i][j] = 1;
					if (!(dc[i][j][1] = Bfs(1, i, j, chkOdd, dep, 0))) {
						//cout << 1 << " " << i << " " << j << " 1" << endl;
						Revert(0);
						d[i][j] = 0;
						int tres = Bfs(1, i, j, chkOdd, dep, 0);
						if (tres == 0) return 0;
						else if (tres == -1) return 1;
						goto start;
					} else if (dc[i][j][1] == -1) {
						return 1;
					}
				}
			}
		}
	}
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j < m;j++) {
			if (r[i][j] == -1) {
				Revert(0);
				r[i][j] = 0;
				assert(Bfs(0, i, j, 0, dep, 1) > 0);
				Revert(0);
				r[i][j] = 1;
				assert(Bfs(0, i, j, 0, dep, 1) > 0);
				Revert(0);
			}
		}
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j <= m;j++) {
			if (d[i][j] == -1) {
				Revert(0);
				d[i][j] = 0;
				assert(Bfs(1, i, j, 0, dep, 1) > 0);
				Revert(0);
				d[i][j] = 1;
				assert(Bfs(1, i, j, 0, dep, 1) > 0);
				Revert(0);
			}
		}
	}
	bool flgExt = 0;
	for (int s = 0;s < 2;s++) {
		for (int i = 0;i <= n;i++) {
			for (int j = 0;j <= m;j++) {
				for (int v = 0;v < 2;v++) {
					for (auto x : tmp[s][i][j][v]) g[s][i][j][v].insert(x);
					tmp[s][i][j][v].clear();
					if (g[s][i][j][v].size() != adjSiz[s][i][j][v]) {
						adjSiz[s][i][j][v] = g[s][i][j][v].size();
						flgExt = 1;
					}
				}
			}
		}
	}
	if (flgExt) goto start;
	ext.clear();
	//cout << "pass" << endl;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j < m;j++) {
			if (r[i][j] == -1) {
				ext.push_back(Node(0, i, j, rc[i][j][0]));
				ext.push_back(Node(0, i, j, -rc[i][j][1]));
			}
		}
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j <= m;j++) {
			if (d[i][j] == -1) {
				ext.push_back(Node(1, i, j, dc[i][j][0]));
				ext.push_back(Node(1, i, j, -dc[i][j][1]));
			}
		}
	}
	sort(ext.begin(), ext.end());
	if (ext.empty()) {
		if (chkValid()) return 1;
		else return 0;
	}
	for (Node x : ext) {
		Revert(0);
		if (x.type == 0) {
			if (x.val > 0) r[x.x][x.y] = 0;
			else r[x.x][x.y] = 1;
			Bfs(0, x.x, x.y, 0, dep, 0);
			if (Dfs(dep + 1)) return 1;
			Revert(1);
			if (x.val > 0) {
				r[x.x][x.y] = 1;
				if (!dsu.Merge(x.x * (m + 1) + x.y, x.x * (m + 1) + x.y + 1)) {
					if (!chkValid()) return 0;
					else {
						rmvMinus();
						return 1;
					}
				}
			} else r[x.x][x.y] = 0;
			goto start;
		} else {
			if (x.val > 0) d[x.x][x.y] = 0;
			else d[x.x][x.y] = 1;
			Bfs(1, x.x, x.y, 0, dep, 0);
			if (Dfs(dep + 1)) return 1;
			Revert(1);
			if (x.val > 0) {
				d[x.x][x.y] = 1;
				if (!dsu.Merge(x.x * (m + 1) + x.y, (x.x + 1) * (m + 1) + x.y)) {
					if (!chkValid()) return 0;
					else {
						rmvMinus();
						return 1;
					}
				}
			} else d[x.x][x.y] = 0;
			goto start;
		}
	}
	Revert(1);
	return 0;
}

bool Solve(int in, int im, int *ia[], bool _VISIBLE) {
	using namespace solverGlobalVariables;
	cerr << "DBG start solve" << endl;
	for (int i = 0;i < MXN;i++) a[i] = new int [MXN];
	n = in; m = im;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) a[i][j] = ia[i][j];
	}
	memset(r, -1, sizeof(r)); memset(d, -1, sizeof(d));
	VISIBLE = _VISIBLE;
	mxdep = dfscnt = 0;
	_TIME_ = clock();
	dsu.Init();
	if (Dfs(1)) {
		Print(mxdep, mxdep, dfscnt, n, m, a, r, d, _TIME_);
		return 1;
	}
	return 0;
}

