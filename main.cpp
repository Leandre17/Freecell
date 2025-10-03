#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// Card structure
struct Card {
    char suit;  // 'H', 'D', 'C', 'S' (Hearts, Diamonds, Clubs, Spades)
    int rank;   // 1-13 (Ace-King)
    
    string toString() const {
        string rankStr;
        if (rank == 1) rankStr = "A";
        else if (rank == 11) rankStr = "J";
        else if (rank == 12) rankStr = "Q";
        else if (rank == 13) rankStr = "K";
        else rankStr = to_string(rank);
        return rankStr + suit;
    }
    
    bool isRed() const {
        return suit == 'H' || suit == 'D';
    }
};

// Game state
class FreecellGame {
private:
    vector<vector<Card>> tableau;      // 8 columns
    vector<Card> freecells;            // 4 free cells
    vector<vector<Card>> foundations;  // 4 foundation piles
    
    void initializeDeck(vector<Card>& deck) {
        char suits[] = {'H', 'D', 'C', 'S'};
        for (char suit : suits) {
            for (int rank = 1; rank <= 13; rank++) {
                deck.push_back({suit, rank});
            }
        }
    }
    
    void shuffleDeck(vector<Card>& deck) {
        random_device rd;
        mt19937 g(rd());
        shuffle(deck.begin(), deck.end(), g);
    }
    
    void dealCards(vector<Card>& deck) {
        int col = 0;
        for (const Card& card : deck) {
            tableau[col].push_back(card);
            col = (col + 1) % 8;
        }
    }
    
public:
    FreecellGame() {
        tableau.resize(8);
        freecells.resize(4);
        foundations.resize(4);
    }
    
    void newGame() {
        // Clear all data structures
        for (auto& col : tableau) col.clear();
        for (auto& foundation : foundations) foundation.clear();
        for (Card& cell : freecells) cell = {' ', 0};
        
        // Create and shuffle deck
        vector<Card> deck;
        initializeDeck(deck);
        shuffleDeck(deck);
        dealCards(deck);
    }
    
    void display() const {
        cout << "\n=== Freecell ===\n\n";
        
        // Display freecells
        cout << "Free Cells: ";
        for (const Card& cell : freecells) {
            if (cell.rank != 0) {
                cout << "[" << cell.toString() << "] ";
            } else {
                cout << "[ ] ";
            }
        }
        cout << "\n\n";
        
        // Display foundations
        cout << "Foundations: ";
        for (const auto& foundation : foundations) {
            if (!foundation.empty()) {
                cout << "[" << foundation.back().toString() << "] ";
            } else {
                cout << "[ ] ";
            }
        }
        cout << "\n\n";
        
        // Display tableau
        cout << "Tableau:\n";
        int maxHeight = 0;
        for (const auto& col : tableau) {
            maxHeight = max(maxHeight, (int)col.size());
        }
        
        for (int row = 0; row < maxHeight; row++) {
            for (int col = 0; col < 8; col++) {
                if (row < (int)tableau[col].size()) {
                    cout << tableau[col][row].toString() << "  ";
                } else {
                    cout << "    ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
        for (int i = 0; i < 8; i++) {
            cout << i << "   ";
        }
        cout << "\n";
    }
    
    bool isWon() const {
        for (const auto& foundation : foundations) {
            if (foundation.size() != 13) return false;
        }
        return true;
    }
};

int main() {
    FreecellGame game;
    
    cout << "Welcome to Freecell!\n";
    cout << "A classic card game implemented in C++\n";
    
    game.newGame();
    game.display();
    
    if (game.isWon()) {
        cout << "Congratulations! You won!\n";
    } else {
        cout << "\nThis is a basic display of the Freecell game.\n";
        cout << "Future versions will include:\n";
        cout << "- Move validation and execution\n";
        cout << "- Auto-move to foundations\n";
        cout << "- Undo/Redo functionality\n";
        cout << "- Win condition checking\n";
    }
    
    return 0;
}
