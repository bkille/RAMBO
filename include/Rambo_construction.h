#ifndef _RamboConstruction_
#define _RamboConstruction_
#include <vector>
#include <set>
#include <string>
#include <bitset>
#include <filesystem>
#include "constants.h"
#include "MyBloom.h"
#include "utils.h"
#include "constants.h"
#include "bitArray.h"
#include "MurmurHash3.h"

namespace fs = std::filesystem;



// vector<uint> hashfunc( void* key, int len, int R, int B){
// }

class RAMBO{
    public:

        RAMBO(int n, float fpr1, int r1, int b1, std::vector<fs::path> input_files);
        RAMBO(fs::path rambo_dir);
        std::vector<uint> hashfunc( std::string key, int len);
        void insertion (fs::path input_file);
        std::set<int> takeunion(std::set<int> set1, std::set<int>& set2);
        std::set<int> takeIntrsec(std::set<int>* setArray);
        std::vector <std::string> getdata(std::string filenameSet);
        bitArray query (std::string query_key, int len);
        void createMetaRambo(std::vector<fs::path> input_files);
        void serializeRAMBO(fs::path dir);
        void deserializeRAMBO(fs::path dir);
        void insertion2 (std::vector<std::string> alllines, int V, int mr);

        int R ;
        int B;
        int n;
        float p;
        int range;
        int k;
        int K;
        float FPR;
        BloomFilter** Rambo_array;
        std::vector<int>* metaRambo;
        std::unordered_map<std::string, int> name_to_idx;
        std::vector<std::string> idx_to_name;
};

#endif
