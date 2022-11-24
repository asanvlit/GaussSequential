#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const double accuracy = 0.00001;

void printSystem(double **a, double *y, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f*x%i", a[i][j], j);
            if (j < n - 1) {
                printf(" + ");
            }
        }
        printf(" = %f\n", y[i]);
    }
}

void fillUpMatrixWithRandom(double **m, int n, int min, int max) {
    for (int i = 0; i < n; i++) {
        m[i] = (double*) malloc(sizeof(double)*n);
        for (int j = 0; j < n; j++) {
            m[i][j] = rand() % max + min;
        }
    }
}

void fillUpArrayWithRandom(double *m, int n, int min, int max) {
    for (int j = 0; j < n; j++) {
        m[j] = rand() % max + min;
    }
}

void swapMatrixRows(double **a, int n, int r1, int r2) {
    for (int j = 0; j < n; j++) {
        double temp = a[r2][j];
        a[r2][j] = a[r1][j];
        a[r1][j] = temp;
    }
}

double* gauss(double **a, double *y, int n) {
    int index; // номер ведущей строки

    double* x = (double*) malloc(sizeof(double) * n);

    int k = 0; // номер текущей строки
    while (k < n) {
        // Поиск строки с максимальным a[i][k]
        double max = fabs(a[k][k]); // сначала в качестве максимума берем ведущий элемент по диагонали на текущей строке

        index = k;
        for (int i = k + 1; i < n; i++) { // двигаясь по столбцу ищем ведущую строку
            if (fabs(a[i][k]) > max) {
                max = fabs(a[i][k]);
                index = i;
            }
        }

        // проверка, что найденный ведущий элемент ненулевой
        if (max < accuracy) {
            printf("Impossible to get a solution because of the zero column %i of the matrix\n", index);
            return 0;
        }

        // перестановка строк если ведущей стала другая
        if (index != k) {
            swapMatrixRows(a, n, index, k);

            double temp = y[k];
            y[k] = y[index];
            y[index] = temp;
        }

        // Нормализация уравнений всех нижних уравнений
        for (int i = k; i < n; i++) {
            double temp = a[i][k];

            // если уже нулевой, то продолжаем
            if (fabs(temp) < accuracy) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                a[i][j] = a[i][j] / temp;
            }

            y[i] = y[i] / temp;

            // не вычитать уравнение само из себя
            if (i == k) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                a[i][j] = a[i][j] - a[k][j];
            }

            y[i] = y[i] - y[k];
        }
        k++;
    }

    // обратный ход: начиная с нижней строки находим решения
    for (k = n - 1; k >= 0; k--) {
        // к этому моменту перебора остается одна неизветсная в строке, которая и равна числу из столбца значений
        x[k] = y[k];

        for (int i = 0; i < k; i++) {
            // вычитаем из столбца значений найденную неизвествую, умноженную на соответствующий элемент матрицы
            y[i] = y[i] - a[i][k] * x[k];
        }
    }

    return x;
}

int main(int argc, char* argv[]) {
    int n = 1000;

    double **a = (double**) malloc(sizeof(double*) * n);
    double *y = (double *) malloc(sizeof(double) * n);

//    srand(static_cast<unsigned int>(time(0)));

    fillUpMatrixWithRandom(a, n, 1, 10);
    fillUpArrayWithRandom(y, n, 1, 10);

//    printSystem(a, y, n);

    clock_t startTime = clock();
    double *x = gauss(a, y, n);
    clock_t endTime = clock();

//    for (int i = 0; i < n; i++) {
//        printf("x[%i] = %f\n", i, x[i]);
//    }

    printf("Time spent:  %f s\n", ((double)(endTime - startTime) / CLOCKS_PER_SEC));

    return 0;
}
