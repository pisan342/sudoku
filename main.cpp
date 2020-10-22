#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class Sudoku {
  friend ostream &operator<<(ostream &out, const Sudoku &cb) {
    int hcount = 0;
    int vcount = 0;
    for (int i = 1; i <= cb.size; i++) {
      for (int j = 1; j <= cb.size; j++) {
        if (cb.board[i][j] == Sudoku::EMPTY) {
          out << " ";
        } else {
          out << cb.board[i][j];
        }
        if (++hcount == cb.numberOfBoxes) {
          hcount = 0;
          out << "|";
        }
      }
      out << endl;
      if (++vcount == cb.numberOfBoxes) {
        vcount = 0;
        for (int dash = 1; dash <= cb.size + cb.numberOfBoxes; dash++) {
          out << "-";
        }
        out << endl;
      }
    }
    return out;
  }

private:
  int size;
  int numberOfBoxes;
  vector<vector<int>> board;

public:
  const static int EMPTY = 0;

  explicit Sudoku(int size)
      : size{size}, numberOfBoxes{static_cast<int>(sqrt(size))},
        board{vector<vector<int>>(size + 1, vector<int>(size + 1))} {}

  Sudoku &add(int row, int col, int num) {
    assert(board[row][col] == EMPTY);
    board[row][col] = num;
    return *this;
  }
  Sudoku &add(int row, const vector<int> &nums) {
    assert(nums.size() == size);
    for (int i = 1; i <= size; i++) {
      board[row][i] = nums[i - 1];
    }
    return *this;
  }
  bool checkValid(int row, int col, int num) const {
    // cannot have same number in row or column
    for (int i = 1; i <= size; ++i) {
      if (board[row][i] == num || board[i][col] == num) {
        return false;
      }
    }
    // cannot have same number in Box
    // row 1,2,3 -> 1, row 4,5,6 -> 3, row 7,8,9 -> 6
    // integer division to get rid of after decimal point
    int minRow = 1 + (row - 1) / numberOfBoxes * numberOfBoxes;
    int minCol = 1 + (col - 1) / numberOfBoxes * numberOfBoxes;
    for (int r = 0; r < numberOfBoxes; ++r) {
      for (int c = 0; c < numberOfBoxes; ++c) {
        if (board[r + minRow][c + minCol] == num) {
          return false;
        }
      }
    }
    return true;
  }

  bool solve() {
    for (int row = 1; row <= size; ++row) {
      for (int col = 1; col <= size; ++col) {
        if (board[row][col] == EMPTY) {
          for (int option = 1; option <= size; ++option) {
            if (checkValid(row, col, option)) {
              board[row][col] = option;
              // cout << "Try: " << row << "," << col << ": " << option <<
              // endl;
              if (solve()) {
                return true;
              }
              // try next option
              board[row][col] = EMPTY;
            }
          }
          // none of the options worked
          return false;
        }
      }
    }
    return true;
  }
};

int main() {
  Sudoku small(4);
  auto v1 = vector<int>{1, 2, 0, 0};
  auto v2 = vector<int>{0, 0, 0, 2};
  auto v3 = vector<int>{0, 0, 4, 0};
  auto v4 = vector<int>{0, 3, 0, 0};
  small.add(1, v1);
  small.add(2, v2);
  small.add(3, v3);
  small.add(4, v4);
  small.solve();
  cout << small << endl;

  Sudoku regular(9);
  regular.add(1, vector<int>{6, 9, 0, 0, 2, 0, 3, 8, 0});
  regular.add(2, vector<int>{0, 0, 2, 6, 0, 0, 4, 7, 9});
  regular.add(3, vector<int>{0, 4, 3, 9, 1, 7, 0, 0, 0});
  regular.add(4, vector<int>{2, 1, 8, 0, 6, 0, 0, 0, 3});
  regular.add(5, vector<int>{9, 0, 0, 1, 7, 4, 0, 0, 6});
  regular.add(6, vector<int>{7, 0, 0, 0, 8, 0, 9, 1, 5});
  regular.add(7, vector<int>{0, 0, 0, 8, 9, 1, 2, 5, 0});
  regular.add(8, vector<int>{5, 2, 1, 0, 0, 3, 6, 0, 0});
  regular.add(9, vector<int>{0, 8, 9, 0, 5, 0, 0, 3, 7});
  regular.solve();
  cout << regular << endl;
}