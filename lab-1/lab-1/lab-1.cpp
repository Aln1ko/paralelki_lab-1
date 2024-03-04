#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

const int n = 40000;
const int thread_num =128;
const int data_num = n*n / thread_num;


void calculate_amount(std::vector<short>& matrix1, std::vector<short>& matrix2, int start, int finish)
{
    for (int i = start; i < finish; i++)
    {
        matrix1[i] += matrix2[i];
    }
}

void calculate_amount1(int *a, int *b, int num)
{
    for (int i = 0; i < num; i++)
    {
        *a = *a + *b;
        a = a + 1;
        b = b + 1;

    }
}


void fill_matrix(std::vector<short>& matrix)
{
    for (int i = 0; i < n; i++)   
    {
        for (int j = 0; j < n; j++)
        {
            matrix[n * i + j] = j;
        }
    }
}

void print_matrix(std::vector<short>& matrix)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cout << matrix[n*i+j] << " ";
        }
        std::cout << "\n";
    }
}

int main()
{
    //int** matrix1 = new int*[n];
    std::vector<short> matrix1(n * n);
    fill_matrix(matrix1);
 //   print_matrix(matrix1);
 //   std::cout << "\n\n";
    
    //int** matrix2 = new int* [n];
    std::vector<short> matrix2(n * n);
    fill_matrix(matrix2);
 //   print_matrix(matrix2);
 //   std::cout << "\n\n";
  
    auto start_creation = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    threads.reserve(thread_num);

    for (int i = 0; i < thread_num-1; i++)
    {
        threads.emplace_back(calculate_amount, std::ref(matrix1), std::ref(matrix2), i * data_num, (i + 1) * data_num);
       // threads.emplace_back(calculate_amount1, &(matrix1[i * data_num]), &(matrix2[i * data_num]), data_num);
    }

    threads.emplace_back(calculate_amount, std::ref(matrix1), std::ref(matrix2), (thread_num - 1) * data_num, n*n);
    auto finish_creation = std::chrono::high_resolution_clock::now();

   // threads[thread_num-1] = std::thread(calculate_amount, matrix1, matrix2, (thread_num - 1 )* data_num, n);

    auto start_working = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < thread_num; i++)
    {
        threads[i].join();
    }
    auto finish_working = std::chrono::high_resolution_clock::now();
    
    auto creating_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_creation - start_creation);
    auto working_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_working - start_working);
    auto all_time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_working - start_creation);

    std::printf(" Creation time: %.7f seconds.\n", creating_time.count() * 1e-9);
    std::printf(" Function time: %.5f seconds.\n", working_time.count() * 1e-9);
    std::printf(" All time: %.5f seconds.\n", all_time.count() * 1e-9);

//    std::cout << "\n\n";
 //   print_matrix(matrix1);

    return 0;
}


