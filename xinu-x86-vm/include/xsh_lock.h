/* lock.h */

#define MAX_ROTORS 10
#define NUM_CHARS 26
#define NUM_NUMS 10
#define NUM_WHITESPACE 3
#define NUM_SYMBOLS 32

#define PRINT_CHAR_LENGTH 33
#define PRINT_NUM_LENGTH 20

typedef int16 bool16;

char *g_enigma_encrypt_chars;
int32 g_enigma_rotor_count;
int32 g_enigma_encrypt_char_count;

struct rotor_settings
{
	int8 type;		/* there is only 9 types of rotors so only need 4-bits.	*/
	int16 shift;	/* This is the shift in the rotor configurations.		*/
	int16 start;	/* starting position of the rotors at initializations.	*/
	int16 position; /* position of the rotor as it goes through encryption.	*/
};

struct rotor_vals
{
	char forword;
	char backward;
};

struct rotor_vals *g_enigma_rotors;
struct rotor_settings g_enigma_rotor_settings[MAX_ROTORS];
char *g_enigma_reflector;

#define getBit1(x) (x % 2)
#define getBit2(x) ((x >> 1) % 2)
#define getBit3(x) ((x >> 2) % 2)
#define getBit4(x) ((x >> 3) % 2)
#define getBit5(x) ((x >> 4) % 2)
#define getBit6(x) ((x >> 5) % 2)
#define getBit7(x) ((x >> 6) % 2)
#define getBit8(x) ((x >> 7) % 2)
#define getBit9(x) ((x >> 8) % 2)
#define getBit10(x) ((x >> 9) % 2)
#define getBit11(x) ((x >> 10) % 2)
#define getBit12(x) ((x >> 11) % 2)
#define getBit13(x) ((x >> 12) % 2)
#define getBit14(x) ((x >> 13) % 2)
#define getBit15(x) ((x >> 14) % 2)
#define getBit16(x) ((x >> 15) % 2)

#define shouldEncryptLowerCase(x) (getBit1(x) == TRUE)
#define shouldEncryptUpperCase(x) (getBit2(x) == TRUE)
#define shouldEncryptNumbers(x) (getBit3(x) == TRUE)
#define shouldEncryptWhiteSpace(x) (getBit4(x) == TRUE)
#define shouldEncryptSymbols(x) (getBit5(x) == TRUE)
#define shouldEncryptLock(x) (getBit6(x) == TRUE)
#define shouldEncryptUnlock(x) (getBit7(x) == TRUE)
#define shouldEncryptVerbose(x) (getBit8(x) == TRUE)
#define shouldEncryptResetRotorsPositions(x) (getBit9(x) == TRUE)

#define setBit1(x) (x = x | 1)
#define setBit2(x) (x = x | 2)
#define setBit3(x) (x = x | 4)
#define setBit4(x) (x = x | 8)
#define setBit5(x) (x = x | 16)
#define setBit6(x) (x = x | 32)
#define setBit7(x) (x = x | 64)
#define setBit8(x) (x = x | 128)
#define setBit9(x) (x = x | 256)
#define setBit10(x) (x = x | 512)
#define setBit11(x) (x = x | 1024)
#define setBit12(x) (x = x | 2048)
#define setBit13(x) (x = x | 4096)
#define setBit14(x) (x = x | 8192)
#define setBit15(x) (x = x | 16384)
#define setBit16(x) (x = x | 32768)

#define setEncryptLowerCase(x) (setBit1(x))
#define setEncryptUpperCase(x) (setBit2(x))
#define setEncryptNumbers(x) (setBit3(x))
#define setEncryptWhiteSpace(x) (setBit4(x))
#define setEncryptSymbols(x) (setBit5(x))
#define setEncryptLock(x) (setBit6(x))
#define setEncryptUnlock(x) (setBit7(x))
#define setEncryptVerbose(x) (setBit8(x))
#define setEncryptResetRotorsPositions(x) (setBit9(x))
