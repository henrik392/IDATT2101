#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class File {
private:
  string filename;
  void writeBytes(vector<unsigned char> data);

public:
  File(string filename);
  vector<unsigned char> readBytes();
  vector<uint32_t> readCodes();
  void writeCodes(vector<uint32_t> data);
  void writeStringVector(vector<string> data);
  int fileSize();
};