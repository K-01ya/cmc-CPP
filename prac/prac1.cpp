#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

class Matrix {
  private:
    vector<vector<int>> matrix;
  public:
    Matrix() = default;
    Matrix(int rows, int columns = 0) : matrix(rows, vector<int>(columns)) {}
    Matrix(vector<vector<int>> &matrix) : matrix(matrix) {}
    void resize(int rows, int columns) {
        matrix.resize(rows);
        for (vector<int> &row : matrix)
            row.resize(columns);
    }
    friend ostream& operator<<(ostream& stream, const Matrix &matrix);
    void print(int begin_rows, int begin_columns, int end_rows, int end_columns) {
        if (end_rows < 0 || end_columns < 0 ||
            end_rows >= matrix.size() || end_columns >= matrix[0].size() ||
            begin_columns < 0 || begin_rows < 0)
            throw out_of_range ("out of range");
        for (int i = begin_rows; i <= end_rows; i++) {
            for (int j = begin_columns; j <= end_columns; j++)
                cout << matrix[i][j] << "\t";
            cout << "\n";
        }
    }
    vector<int>& operator[](int idx) {
        return matrix[idx];
    }
};
ostream& operator<<(ostream& stream, const Matrix &matrix) {
    for (const vector<int> &row : matrix.matrix) {
        for (int element : row)
            stream << element << "\t";
        stream << "\n";
    }
    return stream;
}

int main() { //menu
    Matrix matrix (5, 6);
    vector<vector<int>> matr = {{1, 2, 3}, {3, 3, 1}};
    Matrix matrix1 (matr);
    matrix[1][2] = 100;
    matrix.resize(3, 3);
    cout << matrix;
    matrix.print(0, 2, 1, 2);
    cout << matrix1;
}