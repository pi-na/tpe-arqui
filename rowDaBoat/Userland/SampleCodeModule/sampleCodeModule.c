// Necesito hacer que si encuentro un \n haga el salto
// Ademas necesito verificar que si llego al final de una fila tmb haga el salto
// Consta de 80 columnas y 25 filas

#define BUFFERADDRESS (char*) 0xB8000
char * bufferPointer = BUFFERADDRESS;

int printScreen(char* string, char* color);

int main() {
    char* string = "hola arquitectura de la compu";
    char color[] = {0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70};
    printScreen(string, color);
	return 0xFAFAFA;
}

int printScreen(char* string, char* color){
    for(int i = 0; string[i]; i++){
        *(bufferPointer++) = string[i];
        *(bufferPointer++) = color[i];
    }
    return 0;
}

