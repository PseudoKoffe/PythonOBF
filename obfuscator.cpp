#include <iostream>
#include <fstream>
#include <string>

std::string base64Encode(const std::string& data) {
    const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int i = 0;
    unsigned char a, b, c;
    int n = data.size();
    
    while (i < n) {
        a = i < n ? data[i++] : 0;
        b = i < n ? data[i++] : 0;
        c = i < n ? data[i++] : 0;
        
        result.push_back(b64[a >> 2]);
        result.push_back(b64[((a & 0x03) << 4) | (b >> 4)]);
        result.push_back(b64[((b & 0x0F) << 2) | (c >> 6)]);
        result.push_back(b64[c & 0x3F]);
    }
    
    for (int j = 0; j < (3 - n % 3) % 3; ++j) {
        result[result.size() - 1 - j] = '=';
    }
    return result;
}

int main() {
    std::string dllUrl, inputPath, outputPath;
    int choice;
    
    std::cout << "========================================\n";
    std::cout << "Python Obfuscator\n";
    std::cout << "========================================\n\n";
    
    std::cout << "[1] Use default GitHub URL\n";
    std::cout << "[2] Enter custom URL\n";
    std::cout << "[CHOICE] > ";
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 1) {
        
        dllUrl = "https://github.com/PseudoKoffe/PythonOBF/releases/download/dll.OBF/libprotect.dll";
        std::cout << "[INFO] Using default URL\n";
    } else {
        std::cout << "\n[URL] > ";
        std::getline(std::cin, dllUrl);
        if (dllUrl.empty()) {
            dllUrl = "https://github.com/PseudoKoffe/PythonOBFdll/releases/download/v1.0/libprotect.dll";
            std::cout << "[INFO] Using default URL\n";
        }
    }
    
    std::cout << "\n[INPUT]  > ";
    std::getline(std::cin, inputPath);
    
    std::cout << "[OUTPUT] > ";
    std::getline(std::cin, outputPath);
    
    std::ifstream inputFile(inputPath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "[ERROR] Cannot open input file\n";
        return 1;
    }
    
    std::string code((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();
    
    const unsigned char xorKey[] = {0x4C, 0x6F, 0x76, 0x65, 0x5F, 0x43, 0x6F, 0x66, 0x66, 0x65};
    
    for (size_t i = 0; i < code.size(); i++) {
        code[i] = code[i] ^ xorKey[i % 8];
    }
    
    std::ofstream outputFile(outputPath);
    if (!outputFile) {
        std::cerr << "[ERROR] Cannot create output file\n";
        return 1;
    }
    
    outputFile << "import urllib.request, ctypes, sys, os, tempfile, base64\n\n";
    
    outputFile << "DLL_URL = '" << dllUrl << "'\n";
    outputFile << "dll_path = os.path.join(tempfile.gettempdir(), 'libprotect_' + str(os.getpid()) + '.dll')\n\n";
    outputFile << "try:\n";
    outputFile << "    urllib.request.urlretrieve(DLL_URL, dll_path)\n";
    outputFile << "except Exception as e:\n";
    outputFile << "    print(f'[ERROR] Failed to download DLL: {e}')\n";
    outputFile << "    sys.exit(1)\n\n";
    
    outputFile << "dll = ctypes.CDLL(dll_path)\n\n";
    
    outputFile << "# Setup functions\n";
    outputFile << "dll.ValidateKey.argtypes = [ctypes.c_char_p]\n";
    outputFile << "dll.ValidateKey.restype = ctypes.c_int\n";
    outputFile << "dll.ExecuteCode.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p]\n";
    outputFile << "dll.ExecuteCode.restype = ctypes.c_int\n\n";
    
    outputFile << "enc = base64.b64decode('" << base64Encode(code) << "')\n\n";
    
    outputFile << "LICENSE_KEY = b'XK9#mP2$vL5@nQ8'\n\n";
    
    outputFile << "if dll.ValidateKey(LICENSE_KEY):\n";
    outputFile << "    result = dll.ExecuteCode(enc, len(enc), LICENSE_KEY)\n";
    outputFile << "    if result != 0:\n";
    outputFile << "        print(f'Execution error: {result}')\n";
    outputFile << "        sys.exit(1)\n";
    outputFile << "else:\n";
    outputFile << "    print('Invalid license key!')\n";
    outputFile << "    sys.exit(1)\n";
    
    outputFile.close();
    
    std::cout << "\n[SUCCESS] Protected file created: " << outputPath << std::endl;
    std::cout << "[INFO] DLL URL: " << dllUrl << std::endl;
    std::cout << "[INFO] Make sure the DLL is uploaded to that URL!\n";
    
    return 0;
}
