#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <fstream>
#include <string>
#include <algorithm> // copy_n


void InsertionSort(std::vector<double> & vec, int start_index, int end_index){
    for(int i = start_index; i < end_index; ++i){
        double item = vec[i];
        int j = i-1;
        while( j >= start_index && item < vec[j]){
            vec[j+1] = vec[j];
            --j;
        }
        vec[j+1] = item;
    }
}

void Merge(std::vector<double> & vec, int left, int midpoint, int right){
    std::vector<double> new_vec;
    int index1 = left;
    int index2 = midpoint;
    while(index1 < midpoint && index2 < right){
        if(vec[index1] < vec[index2]){
            new_vec.push_back(vec[index1++]);
        } else {
            new_vec.push_back(vec[index2++]);
        }
    }
    // Deal with leftoveritems in list
    while(index1 < midpoint){ new_vec.push_back(vec[index1++]); }
    while(index2 < right){ new_vec.push_back(vec[index2++]); }

    std::copy_n(new_vec.begin(), new_vec.size(), vec.begin()+left);
}

void MergeSort(std::vector<double> & vec, int K, int left = 0, int right = -1){
    if (right == -1) right = vec.size();
    if (right - left <= K){
        auto v = right - left;
        InsertionSort(vec, left, right);
    }
    else {
        int midpoint = (left + right)/2;
        MergeSort(vec, K, left, midpoint);
        MergeSort(vec, K, midpoint, right);
        Merge(vec, left, midpoint, right);
    }
}


double time_sort(std::vector<double> vec, int K){
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // Becomes Insertion sort when chunk to sort is less than K
    MergeSort(vec, K);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> insertion_time = end - start;
    return (double) insertion_time.count();
}


int main(int argc, char** argv)
{
    int K = 0;
    if(argc == 1){
        std::cout << "Error, please supply filename to output results and optional K \n\n\ta.exe [filename] [K (optional)]" << std::endl;
        return 1;
    }

    // Cryptic line borrowed from https://ask.xiaolee.net/questions/1093917, time since 1970 in ms
    auto seed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Set variables from shell args
    std::string filename(argv[1]);
    filename.append(".csv");

    if (argc == 3) K = atoi(argv[2]);

    std::ofstream output;
    output.open(filename);

    std::uniform_real_distribution<double> distribution(0.0, 1000.0);
    std::mt19937 generator(seed);

    if (!K) {
        // Finding optimal K code, K not specified in function call

        int vec_size = 5000;
        int min_k = 5,  max_k = 400, k_step = 5;
        int repetitions = 500;

        output << vec_size << "," << repetitions << std::endl;

        for(int K = min_k; K <= max_k; K += k_step){
            double timsort_time = 0.0;
            for(int r = 0; r < repetitions; r++){

                // Populate a random vector
                std::vector<double> random(vec_size);
                for(auto & value : random){
                    value = distribution(generator);
                }

                timsort_time += time_sort(random, K);
            }
            output << K << "," << timsort_time << std::endl;   
        }
    } else {
        // Compare timsort to insertion and merge, value of K given in function call

        // Use same vector size/step variables as problem 1
        int vec_size_start = 50;
        int step_size = 50;
        int max_vec_size = 1200;

        int repetitions = 5000;
        int Krange = 25;
        int Kminus = K - Krange;
        int Kplus = K + Krange;

        output << K << "," << Kminus << "," << Kplus << "," << repetitions << std::endl;

        for(int count = vec_size_start; count <= max_vec_size; count += step_size){
            
            double timsort_K = 0.0, timsort_Kminus = 0.0, timsort_Kplus = 0.0;
            for(int r = 0; r < repetitions; ++r){

                // Populate a random vector
                std::vector<double> random(count);
                for(auto & value : random){
                    value = distribution(generator);
                }
                timsort_K += time_sort(random, K);
                timsort_Kminus += time_sort(random, K);
                timsort_Kplus += time_sort(random, K);
            }
            output << count << "," << timsort_K << "," << timsort_Kminus << "," << timsort_Kplus << std::endl;   
        } 
    }
    
    output.close();
}