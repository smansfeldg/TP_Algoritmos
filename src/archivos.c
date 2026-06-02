#define _CRT_SECURE_NO_WARNINGS

#include "../include/archivos.h"

int abrirArchivo(FILE **arch, const char *nombreArchivo, const char *modoApertura, int mostrarError)
{
    *arch = fopen(nombreArchivo, modoApertura);

    if (*arch == NULL) {
        if (mostrarError == 1) {
            fprintf(stderr, "Error al abrir el archivo %s en modo %s.\n", nombreArchivo, modoApertura);
        }
        return ERROR_ARCH;
    }
    return OK;
}

int cerrarArchivo(FILE **arch, const char *nombreArchivo, int mostrarError)
{
    if (!arch || !*arch) {
        return OK;
    }

    if (fclose(*arch) == 0) {
        *arch = NULL;
        return OK;
    }

    if (mostrarError == 1) {
        fprintf(stderr, "Error al cerrar el archivo %s.\n", nombreArchivo);
    }

    return ERROR_ARCH;
}

int leerArchivoTxt(FILE *arch, void *estDatos, unsigned tam, unsigned tamLinea, accion Accion)
{
    void *aux;
    char *linea;

    if (!arch || !estDatos || !Accion) {
        return ERROR_ARCH;
    }

    aux = malloc(tam);
    if (aux == NULL) {
        return ERROR_MEM;
    }
    memcpy(aux, estDatos, tam);

    linea = (char *)malloc(tamLinea);
    if (linea == NULL) {
        free(aux);
        return ERROR_MEM;
    }

    rewind(arch);

    while (fgets(linea, tamLinea, arch) != NULL) {
        Accion(aux, linea);
    }

    memcpy(estDatos, aux, tam);

    free(linea);
    free(aux);
    return OK;
}

int leerArchivoBin(FILE *arch, void *estDatos, unsigned tam, accion Accion)
{
    void *reg;

    if (!arch || !Accion) {
        return ERROR_ARCH;
    }

    reg = malloc(tam);
    if (reg == NULL) {
        return ERROR_MEM;
    }

    rewind(arch);

    while (fread(reg, tam, 1, arch) == 1) {
        Accion(estDatos, reg);
    }

    free(reg);
    return OK;
}

int buscarRegistro(FILE *arch, void *reg, const void *dato, unsigned tam, cmp Cmp)
{
    void *aux;
    int pos = 0;

    aux = malloc(tam);
    if (aux == NULL) {
        return ERROR_MEM;
    }

    rewind(arch);

    while (fread(aux, tam, 1, arch) == 1) {
        if (Cmp(aux, dato) == 0) {
            fseek(arch, -(long)tam, SEEK_CUR);
            memcpy(reg, aux, tam);
            free(aux);
            return pos;
        }
        pos++;
    }

    free(aux);
    return NO_ENCONTRADO;
}

int actualizarRegistro(FILE *arch, void *reg, const void *dato, unsigned tam, accion Accion)
{
    Accion(reg, dato);
    return fwrite(reg, tam, 1, arch) == 1;
}

int leerPos(FILE *arch, void *dato, unsigned tam, int pos)
{
    if (fseek(arch, (long)tam * pos, SEEK_SET) != 0) {
        return ERROR_ARCH;
    }

    return fread(dato, tam, 1, arch) == 1;
}

int escribirPos(FILE *arch, const void *dato, unsigned tam, int pos)
{
    if (fseek(arch, (long)tam * pos, SEEK_SET) != 0) {
        return ERROR_ARCH;
    }

    return fwrite(dato, tam, 1, arch) == 1;
}

int escribirTxt(FILE *arch, void *dato, unsigned tamLinea, accion Accion)
{
    char *linea = (char *)malloc(tamLinea);
    if (linea == NULL) {
        return ERROR_MEM;
    }

    Accion(linea, dato);

    if (fprintf(arch, "%s\n", linea) < 0) {
        free(linea);
        return ERROR_ARCH;
    }

    free(linea);
    return OK;
}

int escribirNuevoReg(FILE *arch, const void *dato, unsigned tam)
{
    fseek(arch, 0, SEEK_END);
    return fwrite(dato, tam, 1, arch) == 1;
}
