#include "file.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

File::File(string filename) : filename(filename) {}

vector<unsigned char> File::readBytes() {
  ifstream file(filename, ios::binary);
  vector<unsigned char> data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  file.close();
  return data;
}

vector<uint32_t> File::readCodes() {
  vector<unsigned char> fileBytes = readBytes();
  vector<uint32_t> data;

  // Vector for å lagre de komprimerte dataene slik at vi kan returnere den til decompressLZW()
  vector<uint32_t> compressedData;

  // Data fra filen blir lest inn i bitbuffer, og bitcount holder styr på hvor mange bits som er lest inn
  // når bitcount >= bitwidth, så har vi en kode som kan lagres i compressedData og vi kan fjerne tegnene fra bitbuffer
  // bitwidth økes når vi har brukt opp alle kodene som kan lagres med den nåværende bitbredden
  int bitBuffer = 0;
  int bitBufferCount = 0;
  int maxBits = 512;
  int bitWidth = 9;
  int dictSize = 256;

  for (size_t i = 0; i < fileBytes.size(); ++i) {
    bitBuffer |= (static_cast<int>(fileBytes[i]) << bitBufferCount);
    bitBufferCount += 8;

    if (bitBufferCount < bitWidth) {
      continue;
    }

    uint32_t code = bitBuffer & (maxBits - 1);

    compressedData.push_back(code);
    bitBuffer >>= bitWidth;
    bitBufferCount -= bitWidth;

    if (dictSize >= maxBits) {
      bitWidth++;
      maxBits <<= 1;
      cout << "Increased bit width to " << bitWidth << endl;
    }

    // Om det er siste iterasjon og det er mindre enn 8 bits igjen i bitbufferen, så er det ikke noe mer data å lese
    // grunnen til at dette kan skje er at i c++ må vi skrive ut byte for byte, og om antall koder ikke er delelig på 8
    // så må vi fylle ut de resterende bitene med 0
    // Tror ikke dette trengs
    // if (i == fileBytes.size() - 1 && bitBufferCount < 8 && bitBuffer == 0)
    //   break;

    dictSize++;
  }

  return compressedData;
  // Jeg brukte en evighet for å få den til å lese og skrive med dynamisk bitbredde.
}

void File::writeStringVector(vector<string> data) {
  ofstream file(filename);
  for (string str : data) {
    file << str;
  }
  file.close();
}

void File::writeCodes(vector<uint32_t> data) {
  vector<unsigned char> fileBytes;
  int bitBuffer = 0;
  int bitCount = 0;
  int maxBits = 512;
  int bitWidth = 9;
  int dictSize = 256;

  for (uint32_t code : data) {
    bitBuffer |= (code << bitCount);
    bitCount += bitWidth;

    if (dictSize >= maxBits) {
      bitWidth++;
      maxBits <<= 1;
      cout << "Increased bit width to " << bitWidth << endl;
    }

    while (bitCount >= 8) {
      char toWrite = bitBuffer & 0xff; // 0xff er 11111111, en maske for å få ut de 8 første bitene
      fileBytes.push_back(toWrite);
      bitBuffer >>= 8;
      bitCount -= 8;
    }

    // For hver ny kode som blir skrevet blir også en ny kode lagt til i lookup tabellen
    // Brukte litt for lang tid på å finne ut at det var derfor jeg ikke fikk dekomprimert filen riktig
    // Rakk ikke fristen :(
    dictSize++;
  }

  if (bitCount > 0) { // Write any remaining bits
    char toWrite = bitBuffer & 0xff;
    fileBytes.push_back(toWrite);
  }

  writeBytes(fileBytes);
}

void File::writeBytes(vector<unsigned char> data) {
  ofstream file(filename, ios::binary);
  for (unsigned char byte : data) {
    file << byte;
  }
  file.close();
}

int File::fileSize() {
  ifstream file(filename);
  file.seekg(0, ios::end);
  return file.tellg();
}