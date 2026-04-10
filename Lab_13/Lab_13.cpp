#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

const double EPS = 1e-6;

bool is_zero(double x) {
    return fabs(x) < EPS;
}

void to_rref(vector < vector < double >> & matrix, int M, int N) {
    int row = 0;  

    for (int col = 0; col < N && row < M; ++col) {
        int pivot_row = row;
        double max_val = fabs(matrix[row][col]);
        for (int i = row + 1; i < M; ++i) {
            if (fabs(matrix[i][col]) > max_val) {
                max_val = fabs(matrix[i][col]);
                pivot_row = i;
            }
        }
 
        if (max_val < EPS)
            continue;

        // Перестановка строк 
        if (pivot_row != row) {
            swap(matrix[row], matrix[pivot_row]);
        }

        // делим на ведущий элемент
        double pivot = matrix[row][col];
        for (int j = col; j <= N; ++j) {
            matrix[row][j] /= pivot;
        }

        //Обнуление всех остальных строк в данном столбце 
        for (int i = 0; i < M; ++i) {
            if (i != row && !is_zero(matrix[i][col])) {
                double factor = matrix[i][col];
                for (int j = col; j <= N; ++j) {
                    matrix[i][j] -= factor * matrix[row][j];
                }
            }
        }

        ++row; 
    }
}

void print_matrix(const vector < vector < double>> & matrix, int M, int N, ofstream& out) {
    out << fixed << setprecision(6);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j <= N; ++j) {
            out << setw(12) << matrix[i][j] << " ";
        }
        out << "\n";
    }
}


bool has_unique_solution(vector < vector < double>> & matrix, int M, int N) {

    to_rref(matrix, M, N);

    int rank = 0;
    for (int i = 0; i < M; ++i) {
        bool all_zeros = true;
        for (int j = 0; j < N; ++j) {
            if (!is_zero(matrix[i][j])) {
                all_zeros = false;
                break;
            }
        }
        if (!all_zeros) {
            ++rank;
        }
        else {
             
            if (!is_zero(matrix[i][N])) {
                return false;
            }
        }
    }

    return (rank == N);
}

int main() {
    setlocale(LC_ALL, "RU");
    
    ifstream fin("test_7");
    if (!fin.is_open()) {
        cerr << "Ошибка: не удалось открыть файл input.txt" << endl;
        return 1;
    }

    int M, N;
    fin >> M >> N;

    // Создаём расширенную матрицу  
    vector < vector < double>> matrix(M, vector<double>(N + 1));

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j <= N; ++j) {
            fin >> matrix[i][j];
        }
    }
    fin.close();

    // Задание 1
    vector < vector < double>>  mat_for_step = matrix;  
    to_rref(mat_for_step, M, N);

    ofstream fstep("step_result");
    if (!fstep.is_open()) {
        cerr << "Ошибка: не удалось создать файл step_result" <<endl;
        return 1;
    }
    fstep <<"Ступенчатый вид расширенной матрицы системы:\n";
    print_matrix(mat_for_step, M, N, fstep);
    fstep.close();

    //  Задание 2 
    vector < vector < double>> mat_for_sol = matrix; 
    ofstream fsol("solution_result");
    if (!fsol.is_open()) {
        cerr << "Ошибка: не удалось создать файл solution_result" << endl;
        return 1;
    }

    if (!has_unique_solution(mat_for_sol, M, N)) {
        fsol << "Система не имеет единственного решения.\n";
    }
    else {
        fsol << "Система имеет единственное решение:\n";
        fsol << fixed << setprecision(6);
       
        for (int j = 0; j < N; ++j) {
            int row_with_pivot = -1;
            for (int i = 0; i < M; ++i) {
                if (!is_zero(mat_for_sol[i][j])) {
                    row_with_pivot = i;
                    break;
                }
            }
            if (row_with_pivot != -1) {
                fsol << "x" << j + 1 << " = " << mat_for_sol[row_with_pivot][N] << "\n";
            }
            else {
                fsol << "x" << j + 1 << " свободная переменная \n";
            }
        }
    }
    fsol.close();

    cout << "Результаты сохранены в файлы: step_result и solution_result";

    return 0;
}