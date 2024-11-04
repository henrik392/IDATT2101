#include "file.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> decompressLZW(vector<uint32_t> data);

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  string filename = argv[1];
  File file(filename);

  vector<uint32_t> data = file.readCodes();
  vector<string> decompressedData = decompressLZW(data);

  // Output the decompressed data to a file
  string outputFilename = argc > 2 ? argv[2] : filename + ".uncompressed";
  File outputFile(outputFilename);

  outputFile.writeStringVector(decompressedData);

  cout << "Decompressed " << filename << " to " << outputFilename << endl;

  return 0;
}

unordered_map<uint32_t, string> initializeCodeToStringLookup() {
  unordered_map<uint32_t, string> dictionary;
  for (uint32_t i = 0; i < 256; i++) {
    dictionary[i] = string(1, (char)i);
  }

  return dictionary;
}

vector<string> decompressLZW(vector<uint32_t> compressedData) {
  unordered_map<uint32_t, string> codeToStringLookup = initializeCodeToStringLookup();
  uint32_t dictSize = 256;

  vector<string> output;
  string p = string(1, (char)compressedData[0]);
  output.push_back(p);

  for (size_t i = 1; i < compressedData.size(); i++) {
    uint32_t c = compressedData[i];

    string pc;
    if (codeToStringLookup.count(c)) {
      pc = codeToStringLookup[c];
    } else if (c == dictSize) {
      pc = p + p[0];
    } else {
      cerr << "Bad compressed data" << endl;
      exit(EXIT_FAILURE);
    }

    output.push_back(pc);

    if (!p.empty())
      codeToStringLookup[dictSize++] = p + pc[0];

    p = pc;
  }

  return output;
}