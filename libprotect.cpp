#include <windows.h>
#include <cstring>
#include <cstdio>

extern "C" {

__declspec(dllexport) int ValidateKey(const char* userKey) {
    const char* validKey = "XK9#mP2$vL5@nQ8";
    if (!userKey) return 0;
    return strcmp(userKey, validKey) == 0 ? 1 : 0;
}

__declspec(dllexport) int ExecuteCode(const char* encrypted, int len, const char* authKey) {
    if (!ValidateKey(authKey)) return -1;
    
    const unsigned char xorKey[] = {0x4C, 0x6F, 0x76, 0x65, 0x5F, 0x43, 0x6F, 0x66, 0x66, 0x65};
    
    char* decrypted = (char*)malloc(len + 1);
    if (!decrypted) return -2;
    
    for (int i = 0; i < len; i++) {
        decrypted[i] = encrypted[i] ^ xorKey[i % 8];
    }
    decrypted[len] = '\0';
    
    char tempPath[MAX_PATH];
    char tempFile[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    GetTempFileNameA(tempPath, "py", 0, tempFile);
    
    FILE* f = fopen(tempFile, "w");
    if (f) {
        fwrite(decrypted, 1, len, f);
        fclose(f);
        
        char cmd[MAX_PATH * 2];
        snprintf(cmd, sizeof(cmd), "python \"%s\"", tempFile);
        int result = system(cmd);
        DeleteFileA(tempFile);
        free(decrypted);
        return result;
    }
    
    free(decrypted);
    return -3;
}

}

BOOL APIENTRY DllMain(HMODULE h, DWORD r, LPVOID) {
    return TRUE;
}
