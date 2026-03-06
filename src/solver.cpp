#include "solver.hpp"
#include "card.hpp"
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>

using namespace std;

static string serializeBoard(const FreecellBoard& b) {
    // Foundations
    std::ostringstream oss;
    for (const auto& f : b.foundations) {
        oss << "F:";
        for (const auto& c : f) {
            oss << c.suit << c.rank << ",";
        }
        oss << "|";
    }
    // Freecells
    oss << "C:";
    for (const auto& c : b.freecells) {
        oss << c.suit << c.rank << ",";
    }
    oss << "|";
    // Tableau
    for (const auto& col : b.tableau) {
        oss << "T:";
        for (const auto& c : col) {
            oss << c.suit << c.rank << ",";
        }
        oss << "|";
    }
    return oss.str();
}

// Helper: check if board is won (all foundations have 13 cards)
static bool isWon(const FreecellBoard& b) {
    size_t total = 0;
    for (const auto& f : b.foundations)
        total += f.size();
    return total == 52;
}

// Lowercase helper
static string toLower(const string& s) {
    string t = s;
    std::transform(t.begin(), t.end(), t.begin(), [](unsigned char c) { return std::tolower(c); });
    return t;
}

string Solver::hint(const FreecellBoard& board) const {
    // 1) prefer moves to foundation
    // Freecells -> foundations
    for (size_t i = 0; i < board.freecells.size(); ++i) {
        const auto& c = board.freecells[i];
        if (c.rank == 0)
            continue;
        for (size_t fi = 0; fi < board.foundations.size(); ++fi) {
            if (board.canMoveToFoundation(c, (int)fi)) {
                std::ostringstream oss;
                oss << "f" << (i + 1) << " o" << (fi + 1);
                return toLower(oss.str());
            }
        }
    }
    // Tableau -> foundations
    for (size_t ti = 0; ti < board.tableau.size(); ++ti) {
        if (board.tableau[ti].empty())
            continue;
        const Card c = board.tableau[ti].back();
        for (size_t fi = 0; fi < board.foundations.size(); ++fi) {
            if (board.canMoveToFoundation(c, (int)fi)) {
                std::ostringstream oss;
                oss << "t" << (ti + 1) << " o" << (fi + 1);
                return toLower(oss.str());
            }
        }
    }
    // Tableau -> Tableau (single card)
    for (size_t src = 0; src < board.tableau.size(); ++src) {
        if (board.tableau[src].empty())
            continue;
        Card c = board.tableau[src].back();
        for (size_t dst = 0; dst < board.tableau.size(); ++dst) {
            if (src == dst)
                continue;
            if (board.canMoveToTableau(c, (int)dst)) {
                std::ostringstream oss;
                oss << "t" << (src + 1) << " t" << (dst + 1);
                return toLower(oss.str());
            }
        }
    }
    // Tableau -> Freecell
    for (size_t src = 0; src < board.tableau.size(); ++src) {
        if (board.tableau[src].empty())
            continue;
        for (size_t fc = 0; fc < board.freecells.size(); ++fc) {
            if (board.freecells[fc].rank == 0) {
                std::ostringstream oss;
                oss << "t" << (src + 1) << " f" << (fc + 1);
                return toLower(oss.str());
            }
        }
    }
    // Freecell -> Tableau
    for (size_t fc = 0; fc < board.freecells.size(); ++fc) {
        const auto& c = board.freecells[fc];
        if (c.rank == 0)
            continue;
        for (size_t dst = 0; dst < board.tableau.size(); ++dst) {
            if (board.canMoveToTableau(c, (int)dst)) {
                std::ostringstream oss;
                oss << "f" << (fc + 1) << " t" << (dst + 1);
                return toLower(oss.str());
            }
        }
    }
    return string();
}

vector<string> Solver::solve(const FreecellBoard& start, size_t maxNodes) const {
    struct Node {
        FreecellBoard b;
        vector<string> path;
    };
    unordered_set<string> visited;
    queue<Node> q;
    q.push(Node{start, {}});
    visited.insert(serializeBoard(start));
    size_t nodes = 0;
    // initial foundations count: allow returning a path that increases foundations
    size_t startFoundations = 0;
    for (const auto& f : start.foundations)
        startFoundations += f.size();

    while (!q.empty() && nodes < maxNodes) {
        Node cur = q.front();
        q.pop();
        ++nodes;
        if (isWon(cur.b))
            return cur.path;

        // Generate moves: priority similar to hint
        // 1) freecell -> foundation
        for (size_t i = 0; i < cur.b.freecells.size(); ++i) {
            const auto& c = cur.b.freecells[i];
            if (c.rank == 0)
                continue;
            for (size_t fi = 0; fi < cur.b.foundations.size(); ++fi) {
                if (cur.b.canMoveToFoundation(c, (int)fi)) {
                    FreecellBoard nb = cur.b;
                    try {
                        nb.moveCard(string("f") + char('0' + (i + 1)),
                                    string("o") + char('0' + (fi + 1)), false);
                    } catch (...) {
                        continue;
                    }
                    string key = serializeBoard(nb);
                    if (visited.insert(key).second) {
                        vector<string> p = cur.path;
                        p.push_back("f" + to_string(i + 1) + " o" + to_string(fi + 1));
                        // if foundations progressed, return this path as a partial solution
                        size_t fcount = 0;
                        for (const auto& ff : nb.foundations)
                            fcount += ff.size();
                        if (fcount > startFoundations)
                            return p;
                        q.push(Node{nb, p});
                    }
                }
            }
        }
        // 2) tableau -> foundation
        for (size_t ti = 0; ti < cur.b.tableau.size(); ++ti) {
            if (cur.b.tableau[ti].empty())
                continue;
            Card c = cur.b.tableau[ti].back();
            for (size_t fi = 0; fi < cur.b.foundations.size(); ++fi) {
                if (cur.b.canMoveToFoundation(c, (int)fi)) {
                    FreecellBoard nb = cur.b;
                    try {
                        nb.moveCard(string("t") + char('0' + (ti + 1)),
                                    string("o") + char('0' + (fi + 1)), false);
                    } catch (...) {
                        continue;
                    }
                    string key = serializeBoard(nb);
                    if (visited.insert(key).second) {
                        vector<string> p = cur.path;
                        p.push_back("t" + to_string(ti + 1) + " o" + to_string(fi + 1));
                        size_t fcount = 0;
                        for (const auto& ff : nb.foundations)
                            fcount += ff.size();
                        if (fcount > startFoundations)
                            return p;
                        q.push(Node{nb, p});
                    }
                }
            }
        }
        // 3) tableau -> tableau (single card)
        for (size_t src = 0; src < cur.b.tableau.size(); ++src) {
            if (cur.b.tableau[src].empty())
                continue;
            Card c = cur.b.tableau[src].back();
            for (size_t dst = 0; dst < cur.b.tableau.size(); ++dst) {
                if (src == dst)
                    continue;
                if (cur.b.canMoveToTableau(c, (int)dst)) {
                    FreecellBoard nb = cur.b;
                    try {
                        nb.moveCard(string("t") + char('0' + (src + 1)),
                                    string("t") + char('0' + (dst + 1)), false);
                    } catch (...) {
                        continue;
                    }
                    string key = serializeBoard(nb);
                    if (visited.insert(key).second) {
                        vector<string> p = cur.path;
                        p.push_back("t" + to_string(src + 1) + " t" + to_string(dst + 1));
                        q.push(Node{nb, p});
                    }
                }
            }
        }
        // 4) tableau -> freecell
        for (size_t src = 0; src < cur.b.tableau.size(); ++src) {
            if (cur.b.tableau[src].empty())
                continue;
            for (size_t fc = 0; fc < cur.b.freecells.size(); ++fc) {
                if (cur.b.freecells[fc].rank == 0) {
                    FreecellBoard nb = cur.b;
                    try {
                        nb.moveCard(string("t") + char('0' + (src + 1)),
                                    string("f") + char('0' + (fc + 1)), false);
                    } catch (...) {
                        continue;
                    }
                    string key = serializeBoard(nb);
                    if (visited.insert(key).second) {
                        vector<string> p = cur.path;
                        p.push_back("t" + to_string(src + 1) + " f" + to_string(fc + 1));
                        q.push(Node{nb, p});
                    }
                }
            }
        }
        // 5) freecell -> tableau
        for (size_t fc = 0; fc < cur.b.freecells.size(); ++fc) {
            const auto& c = cur.b.freecells[fc];
            if (c.rank == 0)
                continue;
            for (size_t dst = 0; dst < cur.b.tableau.size(); ++dst) {
                if (cur.b.canMoveToTableau(c, (int)dst)) {
                    FreecellBoard nb = cur.b;
                    try {
                        nb.moveCard(string("f") + char('0' + (fc + 1)),
                                    string("t") + char('0' + (dst + 1)), false);
                    } catch (...) {
                        continue;
                    }
                    string key = serializeBoard(nb);
                    if (visited.insert(key).second) {
                        vector<string> p = cur.path;
                        p.push_back("f" + to_string(fc + 1) + " t" + to_string(dst + 1));
                        q.push(Node{nb, p});
                    }
                }
            }
        }
    }

    return {};
}
