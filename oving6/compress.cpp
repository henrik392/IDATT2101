#include "file.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<uint32_t> compressLZW(vector<unsigned char> data);
void compareFileSize(File uncompressedFile, File compressedFile);

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  string filename = argv[1];
  File file(filename);

  vector<unsigned char> data = file.readBytes();
  vector<uint32_t> compressedData = compressLZW(data);

  string outputFilename = argc > 2 ? argv[2] : filename + ".lzw";
  File outputFile(outputFilename);
  outputFile.writeCodes(compressedData);

  cout << "Compressed " << filename << " to " << outputFilename << endl;
  compareFileSize(file, outputFile);

  return 0;
}

void compareFileSize(File uncompressedFile, File compressedFile) {
  int uncompressedSize = uncompressedFile.fileSize();
  int compressedSize = compressedFile.fileSize();

  cout << "Uncompressed size: " << uncompressedSize << " bytes" << endl;
  cout << "Compressed size: " << compressedSize << " bytes" << endl;
  cout << "Compression ratio: " << (double)compressedSize / uncompressedSize << endl;
}

vector<uint32_t> compressLZW(vector<unsigned char> data) {
  unordered_map<string, uint32_t> stringToCodeLookup;
  for (uint32_t i = 0; i < 256; i++) {
    stringToCodeLookup[string(1, (char)i)] = i;
  }

  uint32_t dictSize = 256;

  vector<uint32_t> output;

  string p = "";
  for (size_t i = 0; i < data.size(); i++) {
    char c = data[i];
    string pc = p + c;

    if (stringToCodeLookup.count(pc)) {
      p = pc;
    } else {
      output.push_back(stringToCodeLookup[p]);
      stringToCodeLookup[pc] = dictSize++;
      p = string(1, c);
    }
  }

  if (!p.empty()) {
    output.push_back(stringToCodeLookup[p]);
  }

  return output;
}