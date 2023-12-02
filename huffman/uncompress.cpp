#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"

/**
 * Main function for the uncompression program.
 * Takes in the input file name and output file name as command line arguments.
 * Reads the encoded input file, builds a Huffman tree, decodes the symbols, and writes the result to the output file.
 * Returns 0 on successful execution, -1 otherwise.
 */
int main(int argc, char** argv) {
    // Check if the correct number of command line arguments is provided
    if (argc != 3) {
        std::cout << "Usage: ./uncompress inputfile outputfile\n";
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

    // Read the number of non-zero frequencies from the input file
    int nonZeroFreqs = inputFile.read_int();

    // Read frequencies from the input file
    std::vector<int> freqs(256, 0);
    int totalSymbols = 0; // Total number of symbols to decode
    std::cout << "Frequency values: ";
    for (int i = 0; i < nonZeroFreqs; i++) {
        unsigned char symbol = inputFile.read_byte();  // Read symbol
        int freq = inputFile.read_int();  // Read frequency
        freqs[symbol] = freq;
        totalSymbols += freq; // Update total number of symbols
        std::cout << freq << " ";
    }
    std::cout << std::endl;

    // Build Huffman tree
    HCTree huffmanTree;
    huffmanTree.build(freqs);

    // Open the output file for writing
    FancyOutputStream outputFile(argv[2]);
    if (!outputFile.good()) {
        std::cout << "Error opening output file.\n";
        return -1;
    }

    // Decode input file and write to output file
    for (int i = 0; i < totalSymbols; i++) { // Decode exactly totalSymbols symbols
        unsigned char symbol = huffmanTree.decode(inputFile);
        outputFile.write_byte(symbol);
    }

    // Flush the output file stream and close the files
    outputFile.flush();
    return 0;
}

