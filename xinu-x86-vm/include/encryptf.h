
// function definitions.
void updateRotors(void);
int32 encryptForwardRotors(int32, bool8);
int32 encryptBackwardRotors(int32, bool8);
int32 encryptReflector(int32, bool8);
int32 findCharInEncryptionList(char, bool8);

// defines for easer reading of code

#define isEncryptionValueGood(c) ((c >= 0) && (c < g_enigma_encrypt_char_count))