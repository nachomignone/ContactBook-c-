#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void translateKeyword(char *line, FILE *outputFile) { // defino funcion que recibe una linea de texto y el archivo de salida
    struct { // arreglo de estructura (sin nombre) contiene las palabras claves en ingles y español
        char *english;
        char *spanish;
    } keywords[] = { // defino cada traduccion
        {"#include", "#incluir"},
        {"int", "entero"},
        {"main", "principal"},
        {"void", "funcion"},
        {"printf", "imprimir"},
        {"scanf", "escanear"},
        {"if", "si"},
        {"else", "sino"},
        {"while", "mientras"},
        {"return", "retornar"},
        {"char", "caracter"},
        {"file", "archivo"},
        {"fopen", "abrir"},
        {"fwrite", "escribir"},
        {"fclose", "cerrar"},
        {"fread", "leer"},
        {"ftell", "indicar"},
        {"fseek", "desplazar"},
        {"CUR", "actual"},
        {"END", "final"},
        {"NULL", "NULO"},
        {"const", "constante"},
        {"choice", "elegir"},
        {"do", "hacer"},
        {"switch", "cambiar"},
        {"case", "caso"},
        {"break", "romper"},
        {"found", "encontro"},
        {"strcmp", "comparar"},
        {"temp", "temporal"},
        {"remove", "remover"},
        {"rename", "renombrar"},
        {"size", "tamaño"},
        {"rewind", "rebobinar"},
        {"malloc", "reservarMemoria"},
        {"array", "arreglo"},
        {"qsort", "ordenarRapido"},
        {"free", "liberar"},
        {"of", "de"},
};

    int keywordCount = sizeof(keywords) / sizeof(keywords[0]); // calcula el numero de palabras clave en el arreglo (keywords)
    for (int i = 0; i < keywordCount; i++) { // itera sobre cada palabra clave en keywords
        char *pos = strstr(line, keywords[i].english); // busca en "line" la palabra clave en ingles. strstr devuelve un puntero a la primera aparicion de la palabra clave en line o NULL si no la encuentra
        if (pos != NULL) { // si se encuentra la palabra en line, se reemplaza
            char buffer[256]; // se declara un buffer temporal para construir la linea traducida
            strncpy(buffer, line, pos - line); // copia en buffer el contenido de line hasta justo antes de la palabra clave encontrada
            buffer[pos - line] = '\0'; // agrega caracter nulo al final de lo copiado en buffer
            strcat(buffer, keywords[i].spanish); // añade al buffer la palabra clave en español
            strcat(buffer, pos + strlen(keywords[i].english)); // agrega el resto de line a buffer despues de la palabra clave en ingles
            strcpy(line, buffer); // copia el contenido del buffer de vuelta en line, reemplazando la palabra en ingles a su traduccion
        }
    }
    fprintf(outputFile, "%s", line); // escribe line, con la traduccion ya aplicada en el archivo de salida
}

int main() { // inicia funcion principal
    FILE *inputFile = fopen("code.txt", "r"); // abre el archivo en modo lectura. inputFile es un puntero al archivo de entrada
    FILE *outputFile = fopen("codigo.txt", "w"); // abre el archivo en modo escritura donde se guardara el codigo traducido

    if (inputFile == NULL || outputFile == NULL) { // si los archivos de salida o entrada no se pudieron abrir indica error
        printf("Error al abrir el archivo.\n");
        return 1; // termina el programa
    }

    char line[256]; // declara un arreglo line con su tamaño para almacenar linea de texto
    while (fgets(line, sizeof(line), inputFile)) { // lee cada linea de inputFile y la guarda en line hasta alcanzar el final del archivo
        translateKeyword(line, outputFile); // llamo a la funcion para traducir las palabras clave en la linea y escribe la linea traducida en outputFile
    }

    fclose(inputFile); // cierra archivo entrada
    fclose(outputFile); // cierra archivo salida

    printf("Traducción completa. El archivo traducido es codigo.txt\n");
    return 0;
}
