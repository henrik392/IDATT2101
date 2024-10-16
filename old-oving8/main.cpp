#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Class for compressed or uncompressed files.

class File {
  private:
    string inputFileName, outputFileName;
    vector<string> compressionMethods;
    int initSize;

    unordered_map<string, uint16_t> initializeStringToCodeLookup() {
        unordered_map<string, uint16_t> dictionary;
        for (uint16_t i = 0; i < 256; i++) {
            dictionary[string(1, (char)i)] = i;
        }
        return dictionary;
    }

    unordered_map<uint16_t, string> initializeCodeToStringLookup() {
        unordered_map<uint16_t, string> dictionary;
        for (uint16_t i = 0; i < 256; i++) {
            dictionary[i] = string(1, (char)i);
        }

        return dictionary;
    }

    vector<unsigned char> readRawData() {
        ifstream file(inputFileName, ios::binary);
        vector<unsigned char> rawData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return rawData;
    }

    vector<uint16_t> readCompressedData() {
        // Bruker en iterator for å lese filen byte for byte
        ifstream file(inputFileName, ios::binary);
        vector<unsigned char> fileBytes((istreambuf_iterator<char>(file)),
                                        istreambuf_iterator<char>());
        file.close();

        // Vector for å lagre de komprimerte dataene slik at vi kan returnere den til decompressLZW()
        vector<uint16_t> compressedData;

        // Data fra filen blir lest inn i bitbuffer, og bitcount holder styr på hvor mange bits som er lest inn
        // når bitcount >= bitwidth, så har vi en kode som kan lagres i compressedData og vi kan fjerne tegnene fra bitbuffer
        // bitwidth økes når vi har brukt opp alle kodene som kan lagres med den nåværende bitbredden
        int bitBuffer = 0;
        int bitCount = 0;
        int maxBits = 512;
        int bitWidth = 9;
        int dictSize = 256;

        for (size_t i = 0; i < fileBytes.size(); ++i) {
            bitBuffer |= (static_cast<int>(fileBytes[i]) << bitCount);
            bitCount += 8;

            while (bitCount >= bitWidth) {
                uint16_t code = bitBuffer & (maxBits - 1);

                compressedData.push_back(code);
                bitBuffer >>= bitWidth;
                bitCount -= bitWidth;

                if (dictSize >= maxBits) {
                    bitWidth++;
                    maxBits <<= 1;
                    cout << "Increased bit width to " << bitWidth << endl;
                }

                // Om det er siste iterasjon og det er mindre enn 8 bits igjen i bitbufferen, så er det ikke noe mer data å lese
                // grunnen til at dette kan skje er at i c++ må vi skrive ut byte for byte, og om antall koder ikke er delelig på 8
                // så må vi fylle ut de resterende bitene med 0
                if (i == fileBytes.size() - 1 && bitCount < 8 && bitBuffer == 0)
                    break;

                dictSize++;
            }
        }

        return compressedData;
        // Jeg brukte en evighet for å få den til å lese og skrive med dynamisk bitbredde.
    }

    void writeCompressedOutputToFile(const string &outputFileName, const vector<uint16_t> &output) {
        ofstream file(outputFileName, ios::binary);

        int bitBuffer = 0;
        int bitCount = 0;
        int maxBits = 512;
        int bitWidth = 9;
        int dictSize = 256;

        for (uint16_t code : output) {
            bitBuffer |= (code << bitCount);
            bitCount += bitWidth;

            // If we have to increase number of bits to store more codes, increase the bit width after you store the word
            // I am not sure if this is the correct way to do it
            if (dictSize >= maxBits) {
                bitWidth++;
                maxBits <<= 1;
                cout << "Increased bit width to " << bitWidth << endl;
            }

            while (bitCount >= 8) {
                char toWrite = bitBuffer & 0xff; // 0xff er 11111111, en maske for å få ut de 8 første bitene
                file.write(&toWrite, sizeof(char));
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
            file.write(&toWrite, sizeof(char));
        }

        file.close();
    }

  public:
    File(string inputFileName, string outputFileName, vector<string> compressionMethods = {}) : inputFileName(inputFileName), outputFileName(outputFileName), compressionMethods(compressionMethods) {
        ifstream file(inputFileName);

        if (!file.is_open()) {
            cerr << "Failed to open file: " << inputFileName << endl;
            exit(EXIT_FAILURE);
        }

        // Get file size
        file.seekg(0, ios::end);
        initSize = file.tellg();

        file.close();
    }

    int fileSize() {
        ifstream file(outputFileName);
        file.seekg(0, ios::end);
        return file.tellg();
    }

    vector<string> getCompressionMethods() {
        return compressionMethods;
    }

    void compressLZW() {
        compressionMethods.push_back("LZW");

        // Read file into memory
        vector<unsigned char> fileContents;
        fileContents = readRawData();

        // Initialize lookup tables
        unordered_map<string, uint16_t> stringToCodeLookup = initializeStringToCodeLookup();
        uint16_t dictSize = 256;

        // Compression
        vector<uint16_t> output;

        string p = "";
        for (size_t i = 0; i < fileContents.size(); i++) {
            char c = fileContents[i];
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

        // Write output to file with dynamic bit width
        writeCompressedOutputToFile(outputFileName, output);
    }

    void decompressLZW() {
        if (compressionMethods.back() != "LZW") {
            cerr << "File was not compressed with LZW" << endl;
            exit(EXIT_FAILURE);
        }

        compressionMethods.pop_back();

        unordered_map<uint16_t, string> codeToStringLookup = initializeCodeToStringLookup();
        uint16_t dictSize = 256;

        vector<uint16_t> compressedData = readCompressedData();

        vector<string> output;
        string p = string(1, (char)compressedData[0]);
        output.push_back(p);

        for (size_t i = 1; i < compressedData.size(); i++) {
            uint16_t c = compressedData[i];

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

        // Write output to file
        ofstream outFile(outputFileName);
        for (string data : output) {
            outFile << data;
        }

        outFile.close();
    }

    void printCompressionResults() {
        int compressedSize = fileSize();
        cout << "Compression results for file: " << inputFileName << endl;
        cout << "Initial size: " << initSize << " bytes" << endl;
        cout << "Compressed size: " << compressedSize << " bytes" << endl;
        cout << "Compression ratio: " << (double)compressedSize / initSize << endl;
        cout << "Compression methods used: ";
        for (string method : compressionMethods) {
            cout << method << " ";
        }
        cout << endl
             << endl;
    }
};

int main() {
    File file("data/diverse.txt", "data/out.txt");

    // Compress with methods here
    file.compressLZW();
    file.printCompressionResults();

    file = File("data/out.txt", "data/out2.txt", file.getCompressionMethods());

    file.decompressLZW();
    file.printCompressionResults();

    return 0;
}