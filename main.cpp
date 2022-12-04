#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const double accuracy = 1.e-6;
const int maxValue = 20;
const int minValue = 1;

void printSystem(double **a, double *y, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%7.4f*x%i", a[i][j], j);
            if (j < n - 1) {
                printf(" + ");
            }
        }
        printf(" = %f\n", y[i]);
    }
}

void printResultVector(double* resultV, int n) {
    for (int i = 0; i < n; i++) {
        printf("%7.4f ", resultV[i]);
    }
}

void fillUpMatrixWithRandom(double **m, int n) {
    for (int i = 0; i < n; i++) {
        m[i] = (double*) malloc(sizeof(double)*n);
        for (int j = 0; j < n; j++) {
            m[i][j] = rand() % maxValue + minValue;
        }
    }
}

void fillUpArrayWithRandom(double *v, int n) {
    for (int j = 0; j < n; j++) {
        v[j] = rand() % maxValue + minValue;
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

    double* x = (double*) malloc(sizeof(double) * n); // значения неизвестных

    int iter = 0; // номер текущей строки
    while (iter < n) {
        // Поиск строки с максимальным a[i][iter]
        double max = fabs(a[iter][iter]); // сначала в качестве максимума берем ведущий элемент по диагонали на текущей строке

        index = iter;
        for (int i = iter + 1; i < n; i++) { // двигаясь по столбцу ищем ведущую строку
            if (fabs(a[i][iter]) > max) {
                max = fabs(a[i][iter]);
                index = i;
            }
        }

        // проверка, что найденный ведущий элемент ненулевой
        if (max < accuracy) {
            printf("Impossible to get a solution because of the zero column %i of the matrix\n", index);
            return 0;
        }

        // перестановка строк если ведущей стала другая
        if (index != iter) {
            swapMatrixRows(a, n, index, iter);

            double temp = y[iter];
            y[iter] = y[index];
            y[index] = temp;
        }

        // Нормализация уравнений всех нижних уравнений
        for (int i = iter; i < n; i++) {
            double temp = a[i][iter];

            // если уже нулевой, то продолжаем
            if (fabs(temp) < accuracy) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                a[i][j] = a[i][j] / temp;
            }

            y[i] = y[i] / temp;

            // не вычитать уравнение само из себя
            if (i == iter) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                a[i][j] = a[i][j] - a[iter][j];
            }

            y[i] = y[i] - y[iter];
        }
        iter++;
    }

    // обратный ход: начиная с нижней строки находим решения
    for (iter = n - 1; iter >= 0; iter--) {
        // к этому моменту перебора остается одна неизветсная в строке, которая и равна числу из столбца значений
        x[iter] = y[iter];

        for (int i = 0; i < iter; i++) {
            // вычитаем из столбца значений найденную неизвествую, умноженную на соответствующий элемент матрицы
            y[i] = y[i] - a[i][iter] * x[iter];
        }
    }

    return x;
}

int main(int argc, char* argv[]) {
    int n;
    printf_s("\nEnter the size of the matrix: ");
    scanf_s("%d", &n);

    double **a = (double**) malloc(sizeof(double*) * n);
    double *y = (double *) malloc(sizeof(double) * n);

//    srand(static_cast<unsigned int>(time(0)));

    fillUpMatrixWithRandom(a, n);
    fillUpArrayWithRandom(y, n);

    printSystem(a, y, n);

    clock_t startTime = clock();
    double *x = gauss(a, y, n);
    clock_t endTime = clock();

    printResultVector(x, n);

    printf("\nTime spent:  %f s\n", ((double)(endTime - startTime) / CLOCKS_PER_SEC));

    return 0;
}
