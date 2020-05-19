/* lock.h */

char *g_enigma_rotors;
char *g_enigma_encrypt_chars;
int32 g_enigma_rotor_count;
int32 g_enigma_encrypt_char_count;

struct rotor_settings {
    int16 type;
    int16 shift;
    int16 start;
    int16 position;
};

struct rotor_settings g_enigma_rotor_settings[10];
