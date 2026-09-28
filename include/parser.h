#ifndef PARSER_H
#define PARSER_H

// -> de palabra a tokens <-- //

typedef struct{
    char **argv;   //arreglo de tokens
    int argc;      //cantidad de tokens
    int capacidad; //cantidad de punteros
}t_comando;

//funciones a usar :

t_comando *parsear_linea(const char *linea); // parsea la linea -> devuelve el comando o devuelve null a cualquier error sintactico o de memoria

void libera_comando(t_comando *cmd); // mas declaritivo imposible -> libero la mem dinamica q se usa

void imprimir_comando(const t_comando *cmd); // imprimimos los tokens en panatalla

#endif
