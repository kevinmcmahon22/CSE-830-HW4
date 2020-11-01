#include <iostream>
#include <map> // multimap
#include <unordered_map> // unordered_multimap
#include <utility> // make_pair
#include <chrono>
#include <cmath> // pow
#include <fstream>

int main(int argc, char** argv){

    if(argc != 2){
        std::cout << "Please supply maximum number of seconds to wait to insert elements\n"
                    << "Start with 100 insertions, then keep multiplying by 10 ( *= 10 )\n"
                    << "    a.exe [neconds (int)]\n\n";
        return 1;
    }
    int max_time = atoi(argv[1]) * 1000000;

    std::multimap<int,int> mm;
    std::unordered_multimap<int,int> u_mm;

    std::chrono::microseconds duration;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::time_point t2;

    int inserts = 10;
    int time_mm = 0, time_u_mm = 0;

    std::ofstream output;
    output.open("dicts.csv");
    
    while(time_mm < max_time && time_u_mm < max_time){

        // Multimap
        t1 = std::chrono::steady_clock::now();
        for(int i = 0; i < inserts; i++){
            mm.insert(std::make_pair(i,i));
        }
        t2 = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        time_mm = duration.count();

        // Unordered multimap
        t1 = std::chrono::steady_clock::now();
        for(int j = 0; j < inserts; j++){
            u_mm.insert(std::make_pair(j,j));
        }
        t2 = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        time_u_mm = duration.count();

        output << inserts << "," << time_mm << "," << time_u_mm << std::endl;

        inserts *= 10;
    }

}