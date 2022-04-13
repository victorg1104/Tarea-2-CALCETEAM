#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "list.h"
#include "hashmap.h"

typedef struct
{
    char nombre [30];
    char tipo [30];
    char marca [30];
    long stock;
    int precio;
} tipoProducto;

typedef struct
{
    char nombre [30];
    int cantidad;
} tipoProductoCompra;

typedef struct
{
    List* listaProductos;
    int cantidadProductos;
    long precioTotal;
} tipoCarrito;

void importarProductos(char* nombreArchivo);

void exportarProductos(char* nombreArchivo);

void agregaProducto(char* nombre, char* tipo, char* marca, long stock, int precio);

void buscarProductosTipo(HashMap* map, char* tipo);

void buscarProductosMarca(HashMap* map, char* marca);

void buscarProductosNombre(HashMap* map, char* nombre);

void mostrarProductos(HashMap* map);

void agregaProductosCarrito(tipoCarrito* carrito, tipoProductoCompra* producto);

void eliminaProductosCarrito(tipoCarrito* carrito);

void concretarCompra(tipoCarrito* carrito);

void mostrarCarritos(HashMap* map);

// Funciones auxiliares
void mostrarInfoProducto(tipoProducto *);

int main()
{
    HashMap* mapaTipo = createMap(200);
    HashMap* mapaMarca = createMap(200);
    HashMap* mapaNombre = createMap(200);
    HashMap* mapaCarritos = createMap(200);

    char tipo[30];

    int opcion = 0;

    while(opcion!= 12)
    {
        printf("1.-  Importar productos desde un archivo CSV\n");
        printf("2.-  Exportar productos CSV\n");
        printf("3.-  Agregar producto\n"); // Funcionando
        printf("4.-  Buscar producto por tipo\n"); // Funcionando
        printf("5.-  Buscar producto por marca\n");
        printf("6.-  Buscar producto por nombre\n");
        printf("7.-  Mostrar todos los productos diponibles\n"); // Funcionando
        printf("8.-  Agregar producto a un carrito\n"); // Funcionando
        printf("9.-  Eliminar producto de un carrito\n"); // Funcionando
        printf("10.- Concretar compra\n");
        printf("11.- Mostrar todos los carritos\n");
        printf("12.- Salir\n\n");

        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);

        switch(opcion)
        {
            case 4:
                printf("Ingrese el tipo de producto: ");
                scanf("%[^\n]", tipo);
                buscarProductosTipo(mapaTipo, tipo);
            case 11: exit(EXIT_SUCCESS); // Salir de la aplicacion
        }
    }

    return 0;
}

void buscarProductosTipo(HashMap *map, char *tipo)
{
    Pair *pair = searchMap(map, tipo);
    List *listaProductos = pair->value;
    tipoProducto *producto = firstList(listaProductos);
    while(producto)
    {
        mostrarInfoProducto(producto);
        producto = nextList(listaProductos);
    }
}

void mostrarInfoProducto(tipoProducto *producto)
{
    printf("Nombre: %s\n", producto->nombre);
    printf("Marca: %s\n", producto->marca);
    printf("Tipo: %s\n", producto->tipo);
    printf("Stock: %d\n", producto->stock);
    printf("Precio: %d\n\n", producto->precio);
}