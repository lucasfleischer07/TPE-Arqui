#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>


static int shiftFlag = 0;
static int mayusFlag = 0;
static unsigned int keyboardBufferSize = 0;
static unsigned char keyboardBuffer[MAX_SIZE];
static uint64_t* registers;
static uint64_t* finalRegisters;



static char scanCodeToChar[KEYS][2] = {
    {0, 0}, {ESC, ESC}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}
	};


void keyboard_handler() {
    int scanCode;

    while (kbDetection()) {
        scanCode = kbScanCode();
        if(scanCode == TAB) {
            registers = get_regs();
        }
        if (scanCode == SHIFTL || scanCode == SHIFTR) {
            shiftFlag = 1;
        } else if (scanCode == SHIFTL_RELESED || scanCode == SHIFTR_RELESED) {
            shiftFlag = 0;
        } else if (scanCode == CAPSLOCKS) {
            mayusFlag = !mayusFlag;
        }
        if (scanCode >= 0 && scanCode < KEYS && scanCodeToChar[scanCode][0] != 0) {
            putCharInBuffer(scanToChar(scanCode));
        }
    }
}

char scanToChar(int scancode) {
    return scanCodeToChar[scancode][mayusFlag || shiftFlag];
}


unsigned char* getBuffer() {
	return keyboardBuffer;
}


void putCharInBuffer(char sc) {
    if (keyboardBufferSize <= MAX_SIZE) {
        keyboardBuffer[keyboardBufferSize++] = sc;
    }
}


char getChar() {	
    if (keyboardBufferSize <= 0) {
        return 0;
    }
    char key = keyboardBuffer[0];
    if (keyboardBufferSize > 0) {	
        for (int i = 1; i < keyboardBufferSize; i++) {
            keyboardBuffer[i - 1] = keyboardBuffer[i];
        }
    }
    keyboardBufferSize--;
    if(key == '\t') {   // Entra aca, esta verificado ya
        finalRegisters = registers;
    }
    return key;
}


uint64_t* getRegs() {
    return finalRegisters;
}
