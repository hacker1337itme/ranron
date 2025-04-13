#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Include for std::reverse
#include <functional> // Include for std::function

namespace fs = std::filesystem;

// Function prototypes for encryption methods
void encryptShift(std::string &data, int shift);
void encryptXOR(std::string &data, char key);
void encryptReverse(std::string &data);
void encryptCaesar(std::string &data, int shift);
void encryptSubstitution(std::string &data);
void encryptVigenere(std::string &data, const std::string &key);
void encryptBase64(std::string &data);

// Encryption method implementations
void encryptShift(std::string &data, int shift) {
    for (char &c : data) {
        c += shift;
    }
}

void encryptXOR(std::string &data, char key) {
    for (char &c : data) {
        c ^= key;
    }
}

void encryptReverse(std::string &data) {
    std::reverse(data.begin(), data.end());
}

void encryptCaesar(std::string &data, int shift) {
    for (char &c : data) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
    }
}

void encryptSubstitution(std::string &data) {
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const std::string substitute = "qwertyuiopasdfghjklzxcvbnm";

    for (char &c : data) {
        if (isalpha(c)) {
            char lower = tolower(c);
            size_t index = alphabet.find(lower);
            char newChar = substitute[index];
            c = isupper(c) ? toupper(newChar) : newChar;
        }
    }
}

void encryptVigenere(std::string &data, const std::string &key) {
    std::string keyExtended = key;
    while (keyExtended.length() < data.length()) {
        keyExtended += key;
    }
    for (size_t i = 0; i < data.length(); ++i) {
        if (isalpha(data[i])) {
            char base = islower(data[i]) ? 'a' : 'A';
            data[i] = (data[i] - base + (keyExtended[i] - base)) % 26 + base;
        }
    }
}

void encryptBase64(std::string &data) {
    static const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    std::string encoded;
    int val = 0, valb = -6;
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    while (valb >= 0) {
        encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
        valb -= 6;
    }
    data = encoded;
}

// Helper function to read file content
std::string readFile(const fs::path &filepath) {
    std::ifstream file(filepath);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

// Helper function to write content to a file
void writeFile(const fs::path &filepath, const std::string &content) {
    std::ofstream file(filepath);
    file << content;
}

// Encrypt the contents of the folder
void encryptFolder(const fs::path &folderPath) {
    using EncryptFunc = std::function<void(std::string&)>;  // Define a type for encryption functions

    std::vector<EncryptFunc> methods = {
        [&](std::string &data) { encryptShift(data, std::rand() % 26); },     // Using lambda to capture shift
        encryptReverse,
        [&](std::string &data) { encryptCaesar(data, std::rand() % 26); },    // Using lambda to capture shift
        encryptSubstitution,
        [&](std::string &data) { encryptVigenere(data, "KEY"); },             // Using a constant key for Vigenere
        encryptBase64
    };

    // Seed random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (const auto &entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            std::string content = readFile(entry.path());
            
            int methodIndex = std::rand() % methods.size();

            // Call the randomly chosen encryption method
            methods[methodIndex](content);

            // Write the encrypted content back to the file
            writeFile(entry.path(), content);
        }
    }
}

int main() {
    fs::path folderPath = "path/to/your/folder";  // Change this to your target folder path
    encryptFolder(folderPath);
    std::cout << "Encryption completed." << std::endl;
    return 0;
}
