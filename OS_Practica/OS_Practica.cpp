#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>


using namespace std;

// Тип данных для матрицы
using Matrix = vector<vector<int>>;

void generateMatrix(Matrix &mas, int n) {
    int start = -1000;
    int end = 1000;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
             mas[i][j]= rand() % (end - start + 1) + start;
        } 
    }

}

void print(const Matrix &mas, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout.width(3);
            cout << mas[i][j] << " ";
        }
        cout << endl;
    }

}

void matrixMultiplication(const Matrix & A, Matrix & B, Matrix & C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


// Функция для вычисления одной строки результирующей матрицы
void multiplyRow(const Matrix & A, const Matrix & B, Matrix & C, int row, int n) {
    for (int j = 0; j < n; ++j) {
        //C[row][j] = 0; // Инициализируем элемент C[row][j]
        for (int k = 0; k < n; ++k) {
            C[row][j] += A[row][k] * B[k][j];
        }
    }
}


// Функция для умножения двух подматриц
void multiplySubMatrix(const Matrix& A, const Matrix& B, Matrix& C, int rowStart, int rowEnd, int colStart, int colEnd, int N) {
    for (int i = rowStart; i < rowEnd; ++i) {
        for (int j = colStart; j < colEnd; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


int main()
{
    setlocale(LC_ALL, "ru");
    int n;
    cin >> n;
    // Определяем границы для подматриц
    int midRow = n / 2;
    int midCol = n / 2;

    auto matrixA = Matrix(n, vector<int>(n));
    auto matrixB = Matrix(n, vector<int>(n));
    auto matrixС = Matrix(n, vector<int>(n));
    auto matrixD = Matrix(n, vector<int>(n));
  
    generateMatrix(matrixA, n);
    generateMatrix(matrixB, n);


    //Перемножение потоками
    auto start1 = chrono::high_resolution_clock::now();

    // Создание потоков для обработки каждой четверти
    thread t1(multiplySubMatrix, cref(matrixA), cref(matrixB), ref(matrixС), 0, midRow, 0, midCol, n);
    thread t2(multiplySubMatrix, cref(matrixA), cref(matrixB), ref(matrixС), 0, midRow, midCol, n, n);
    thread t3(multiplySubMatrix, cref(matrixA), cref(matrixB), ref(matrixС), midRow, n, 0, midCol, n);
    thread t4(multiplySubMatrix, cref(matrixA), cref(matrixB), ref(matrixС), midRow, n, midCol, n, n);

    // Ожидание завершения всех потоков
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    /*
     // Создаем и запускаем потоки для каждой строки результирующей матрицы
    vector<thread> threads;
    for (int i = 0; i < n; ++i) {
        threads.push_back(thread(multiplyRow, cref(matrixA), cref(matrixB), ref(matrixС), i, n));
    }
    
    // Ожидаем завершение всех потоков
    for (auto& th : threads) {
        th.join();
    }
    */
    auto end1 = chrono::high_resolution_clock::now();

    //вывод времени для thread
    double time_taken1 =
        chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();

    time_taken1 *= 1e-9;

    cout << "Time taken для thread is : " << fixed
        << time_taken1 << setprecision(9);
    cout << " sec" << endl;

    //print(matrixС, n);
    cout << "\n";





    //Перемножение обычное
    auto start2 = chrono::high_resolution_clock::now();
    matrixMultiplication(matrixA, matrixB, matrixD, n);
    auto end2 = chrono::high_resolution_clock::now();

    //вывод времени для обычного перемножения
    double time_taken2 =
        chrono::duration_cast<chrono::nanoseconds>(end2 - start2).count();

    time_taken2 *= 1e-9;

    cout << "Time taken для обычного перемножения is : " << fixed
        << time_taken2 << setprecision(9);
    cout << " sec" << endl;

    //print(matrixD, n);

    return 0;
}

