#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "HCTree.hpp"
#include "Helper.hpp"

/**
 * Compresses a file using Huffman coding.
 *
 * @param argc the number of command-line arguments
 * @param argv an array of command-line arguments
 * @return 0 on success, -1 on failure
 */
int main(int argc, char** argv) {
    // Check the number of command-line arguments
    if (argc != 3) {
        std::cout << "Usage: ./compress inputfile outputfile\n";
        return -1;
    }

    // Check if the input file is empty
    std::ifstream inputFileCheck(argv[1], std::ifstream::ate | std::ifstream::binary);
    if (!inputFileCheck.good() || inputFileCheck.tellg() == 0) {
        // Input file is empty or an error occurred, so create an empty output file and stop
        std::ofstream outputFile(argv[2]);
        std::cout << "Empty input file.\n";
        return 0;
    }

    // Open the input file for reading
    FancyInputStream inputFile(argv[1]);
    if (!inputFile.good()) {
        std::cout << "Error opening input file.\n";
        return -1;
    }

    // Prepare frequency vector by reading all bytes in the file
    std::vector<int> freqs(256, 0);
    int byte;
    while ((byte = inputFile.read_byte()) != -1) {
        freqs[byte]++;
    }

    // Build Huffman tree
    HCTree huffmanTree;
    huffmanTree.build(freqs);

    // Open the output file for writing
    FancyOutputStream outputFile(argv[2]);
    if (!outputFile.good()) {
        std::cout << "Error opening output file.\n";
        return -1;
    }

    // Count non-zero frequencies
    int nonZeroFreqs = std::count_if(freqs.begin(), freqs.end(), [](int freq) { return freq > 0; });

    // Write the number of non-zero frequencies to the output file
    outputFile.write_int(nonZeroFreqs);

    // Write non-zero frequencies with their symbols to the output file
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            outputFile.write_byte(i);       // Write symbol
            outputFile.write_int(freqs[i]); // Write frequency
        }
    }

    // Encode the input data using the Huffman tree and write to the output file
    inputFile.reset(); // Reset the input file to start encoding from the beginning
    while ((byte = inputFile.read_byte()) != -1) {
        huffmanTree.encode(byte, outputFile);
    }

    outputFile.flush();

    return 0;
}

