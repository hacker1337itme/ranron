#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>

namespace fs = std::filesystem;

// Function prototypes for encryption methods
void encryptShift(std::string &data, int shift);
void encryptXOR(std::string &data, char key);
void encryptReverse(std::string &data);
void encryptCaesar(std::string &data, int shift);
void encryptSubstitution(std::string &data);
void encryptVigenere(std::string &data, const std::string &key);
void encryptBase64(std::string &data);
// Add more encryption methods as needed...

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
    std::vector<void(*)(std::string&, int)> methods = {
        encryptShift,      // Randomly chosen, you may change based on requirement
        encryptXOR,
        encryptReverse,
        encryptCaesar,
        encryptSubstitution,
        encryptVigenere,
        encryptBase64
        // Add more methods as needed...
    };

    // Seed random number generation
    std::srand(std::time(nullptr));

    for (const auto &entry : fs::directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.path())) {
            std::string content = readFile(entry.path());
            
            int methodIndex = std::rand() % methods.size();
            int randomShift = std::rand() % 26; // For shift-based methods
            
            // Call a random encryption method based on the chosen index
            if (methodIndex == 0) {  // Encrypt with Shift
                encryptShift(content, randomShift);
            } else if (methodIndex == 1) { // Encrypt with XOR
                encryptXOR(content, 'A'); // You can change key
            } else if (methodIndex == 2) { // Encrypt with Reverse
                encryptReverse(content);
            } else if (methodIndex == 3) { // Encrypt with Caesar
                encryptCaesar(content, randomShift);
            } else if (methodIndex == 4) { // Encrypt with Substitution
                encryptSubstitution(content);
            } else if (methodIndex == 5) { // Encrypt with Vigenere
                encryptVigenere(content, "KEY"); // Example key
            } else if (methodIndex == 6) { // Encrypt with Base64
                encryptBase64(content);
            }

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
