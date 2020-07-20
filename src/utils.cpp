//contains all the other non-RAMBO functions
#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include "MyBloom.h"
#include "MurmurHash3.h"
#include "utils.h"
#include "spdlog/spdlog.h"
#include <map>
#include <cassert>
using namespace std;
namespace fs = std::filesystem;

//readlines from a file
std::vector<string> getsets( string path){
  //get the size of Bloom filter by count
  ifstream cntfile (path);
  std::vector <std::string> allKeys;
  int linecnt = 0;
  while (cntfile.good())
      {
          string line1, vals;
          while( getline ( cntfile, line1 ) ){
              stringstream is;
              is<<line1;
              if (linecnt==0 ){
                while(getline (is, vals, ' ' )){
                  allKeys.push_back(vals);}
              }
              else{
                while(getline (is, vals, ' ' )){
                  allKeys.push_back(vals);
                  break;}
              }
                linecnt++;
          }
      }
      std::cout<<"total lines from this file: "<<linecnt-3<<std::endl;
      return allKeys;
}

// parse string into array given delimiter
std::vector<string> line2array( string line, char d){
  stringstream is;
  is<<line;
  std::vector <std::string> op;
  string vals;
  while( getline (is, vals, d)){
    op.push_back(vals);
  }
  return op;
}

//file write
void writeRAMBOresults(string path, int rows, int cols, float* values){
  ofstream myfile;
  myfile.open (path);
  for (int i =0;i<rows; i++){
    for (int j =0;j<cols; j++){
      myfile << to_string(values[i*cols +j])<<",";
    }
    myfile <<"\n";
  }
  myfile.close();
}

//use array set intersection and union
std::vector<int> arrayunion(std::vector<int> &v1, std::vector<int> &v2) {
  std::vector<int> v;
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 std::back_inserter(v));
  return v;
}

std::vector<int> arrayintersection(std::vector<int> &v1, std::vector<int> &v2) {
  std::vector<int> v;
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(v));
  return v;
}

set<int> takeunion(set<int> set1, set<int> set2){
  set<int> res;
  res.insert(set1.begin(), set1.end());
  res.insert(set2.begin(), set2.end());
  return res;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::vector<std::string> get_kmers(const fs::path input_file) {
    auto ext = input_file.extension().string();
    vector<std::string> keys;
    if (ext == ".ctx") {
        keys = getctxdata(input_file);
    } else if (ext == ".txt" || ext == ".out") {
        keys = gettxtdata(input_file);
    } else if (ext == ".fna" || ext == ".fasta" || ext == ".fa" || ext == ".ffn") {
        // TODO
    } else {
        spdlog::warn("File extension {} not recognized! Defaulting to .txt format", ext);
        keys = gettxtdata(input_file);
    }  
    return keys;
}

std::vector <std::string> gettxtdata(fs::path txtfile){
  ifstream txtstream (txtfile);
  std::vector <std::string> keys;
  std::string line;
  std::vector<std::string> values;
  while(txtstream.good()) {
    getline(txtstream, line);
    std::stringstream line_stream(line);
    getline(line_stream, line, ' ');
    if (line.size() > 0) {
        keys.push_back(line);
    }
  }
  return keys;
}

std::vector <std::string> getctxdata(fs::path ctxfile){
    std::vector<std::string> keys;
    auto cortexpy_out = exec((
        "cortexpy view graph " + ctxfile.string() + " > " + (fs::temp_directory_path() / ctxfile.stem()).string()
    ).c_str());
    keys = gettxtdata(fs::temp_directory_path() / ctxfile.stem());
    return keys;
}

//num control how many lines to get
std::vector<string> readlines( string path, int num){
  ifstream pathfile (path);
  std::vector <std::string> allfiles;
  int count=0;
  while ( pathfile.good() )
    {
      string line1;
      while( getline (pathfile, line1 ) ){
        count++;
        allfiles.push_back(line1);
        if (count >num && num){
          break;
        }
        }
  std::cout << count<< '\n';
  return allfiles;
  }
}

std::vector<string> getRandomTestKeys(int keysize, int n){
  static const char alphanum[] = "ATGC";
  std::vector<string> s;

  for (int j = 0; j < n; ++j){
    string st;
      for (int i = 0; i < keysize; ++i) {
          st = st + alphanum[rand()%4];
      }
      s.push_back(st);
  }
return s;
}

//std::map<std::string, vector<int>> makeInvIndex(int n, vector<std::string> foldernames){
  //std::map<std::string, vector<int>> m;
  //for (uint f=0; f<foldernames.size(); f++){
    //string foldername = foldernames[f];
    //for (int batch=0; batch<47; batch++){
      //string dataPath = foldername + to_string(batch)+ "_indexed.txt";
      //std::vector<string> setIDs = readlines(dataPath, 0);
      //cout<<setIDs[0]<<endl;

      //for (uint ss=0; ss<setIDs.size(); ss++){
          //char d = ',';
          //vector<std::string> setID = line2array(setIDs[ss], d);
          //string mainfile = foldername + setID[1]+ ".out";
	  //cout<<"getting keys"<<endl;
          //vector<std::string> keys = getctxdata(mainfile);
          //cout<<"gotkeys"<<endl;

	  //if (ss==0 && batch ==0 && f ==0){
              //for (int i =0; i<n; i++){
               //m[keys[i]].push_back(std::stoi(setID[0]));
              //}
                  //cout<<"completed first itr"<<endl;
         //for(map<string, vector<int> >::iterator it = m.begin(); it != m.end(); ++it){
		   //std::cout << it->first <<it->second[0]<<"\n";
		//}
           //}
          //else{

            //for (uint i =0; i<keys.size(); i++){
                 //if (m.find(keys[i]) != m.end()){
                 //std::cout << "map contains the key!\n";
                 //m[keys[i]].push_back(std::stoi(setID[0]));
                 //}
            //}
	   //}
          //cout<<ss<<endl;
      //}
     //}
  //}
  //return m;
//}
