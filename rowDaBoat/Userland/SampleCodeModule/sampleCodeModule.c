/* sampleCodeModule.c */

char * buffer = (char*)0xB8000;

int main() {
    *buffer = 'A';
    *(buffer+1) = 0x74;

    *(buffer+2) = 'R';
    *(buffer+3) = 0x56;

	return 0xFAFAFA;
}
