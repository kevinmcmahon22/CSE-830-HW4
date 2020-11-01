#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>
#include <fstream>
#include <string>
#include <algorithm> // copy_n


void InsertionSort(std::vector<double>& vec){
    for(int i = 1; i < vec.size(); ++i){
        double item = vec[i];
        int j = i-1;
        while( j >= 0 && item < vec[j]){
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
    // Deal with left over items in list
    while(index1 < midpoint){ new_vec.push_back(vec[index1++]); }
    while(index2 < right){ new_vec.push_back(vec[index2++]); }

    std::copy_n(new_vec.begin(), new_vec.size(), vec.begin()+left);
}

void MergeSort(std::vector<double> & vec, int left = 0, int right = -1){
    if (right == -1) right = vec.size();
    if (right - left < 2) return;
    int midpoint = (left + right)/2;
    MergeSort(vec, left, midpoint);
    MergeSort(vec, midpoint, right);
    Merge(vec, left, midpoint, right);
}


double time_sort(std::vector<double> vec, bool merge = false){
    // Function to time each sort
    // Allows same vector to be used for each sort because of copy
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    if (merge) MergeSort(vec);
    else InsertionSort(vec);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> insertion_time = end - start;
    return (double) insertion_time.count();
}


int main(int argc, char** argv)
{
    if(argc != 2){
        std::cout << "Error, please supply filename to output results\n a.exe [filename]\n";
        return 1;
    }

    std::string filename(argv[1]);
    filename.append(".csv");
    // std::string filename = "sorts.csv";

    // Cryptic line borrowed from https://ask.xiaolee.net/questions/1093917, time since 1970 in ms
    auto seed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    int repetitions = 5000;

    int vec_size_start = 50;
    int step_size = 50;
    int max_vec_size = 1200;

    std::ofstream output;
    output.open(filename);

    std::uniform_real_distribution<double> distribution(0.0, 1000.0);
    std::mt19937 generator(seed);

    output << repetitions << std::endl;

    for(int count = vec_size_start; count <= max_vec_size; count += step_size){
        
        double time_ins = 0.0, time_merge = 0.0;
        for(int r = 0; r < repetitions; ++r){

            // Populate a random vector
            std::vector<double> random(count);
            for(auto & value : random){
                value = distribution(generator);
            }
            
            // Pass true for merge sort, nothing for insertion
            time_merge += time_sort(random, true);
            time_ins += time_sort(random);
        }
        output << count << "," << time_ins << "," << time_merge << std::endl;   
    } 

    output.close();
}