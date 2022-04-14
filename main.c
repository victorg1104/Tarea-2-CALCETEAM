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

void agregarProducto(char* nombre, char* tipo, char* marca, long stock, int precio);

void buscarProductosTipo(HashMap* map, char* tipo);

void buscarProductosMarca(HashMap* map, char* marca);

void buscarProductosNombre(HashMap* map, char* nombre);

void mostrarProductos(HashMap* map);

void agregaProductosCarrito(tipoCarrito* carrito, tipoProductoCompra* producto);

void eliminaProductosCarrito(tipoCarrito* carrito);

void concretarCompra(tipoCarrito* carrito);

void mostrarCarritos(HashMap* map);

// Funciones auxiliares
void mostrarInfoProducto(tipoProducto *producto);
tipoProducto *crearProducto(char* nombre, char* tipo, char* marca, long stock, int precio);

// Variables globales
HashMap* mapaTipo;
HashMap* mapaMarca;
HashMap* mapaNombre;
HashMap* mapaCarritos;

int main()
{
    mapaTipo = createMap(200);
    mapaMarca = createMap(200);
    mapaNombre = createMap(200);
    mapaCarritos = createMap(200);

    char nombre [30];
    char tipo [30];
    char marca [30];
    long stock;
    int precio;

    int opcion = 0;

    while(opcion!= 12)
    {
        printf("1.-  Importar productos desde un archivo CSV\n");
        printf("2.-  Exportar productos CSV\n");
        printf("3.-  Agregar producto\n");
        printf("4.-  Buscar producto por tipo\n");
        printf("5.-  Buscar producto por marca\n");
        printf("6.-  Buscar producto por nombre\n");
        printf("7.-  Mostrar todos los productos diponibles\n");
        printf("8.-  Agregar producto a un carrito\n");
        printf("9.-  Eliminar producto de un carrito\n");
        printf("10.- Concretar compra\n");
        printf("11.- Mostrar todos los carritos\n");
        printf("12.- Salir\n\n");

        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);

        switch(opcion)
        {
            case 3:
                printf("Ingrese el nombre del producto: ");
                scanf("%[^\n]", nombre);
                fflush(stdin);
                printf("Ingrese el tipo del producto: ");
                scanf("%[^\n]", tipo);
                fflush(stdin);
                printf("Ingrese la marca del producto: ");
                scanf("%[^\n]", marca);
                fflush(stdin);
                printf("Ingrese el stock del producto: ");
                scanf("%d", &stock);
                printf("Ingrese el precio del producto: ");
                scanf("%d", &precio);
                agregarProducto(nombre, tipo, marca, stock, precio);
                printf("\n");
                break;
            case 4:
                printf("Ingrese el tipo de producto: ");
                scanf("%[^\n]", tipo);
                buscarProductosTipo(mapaTipo, tipo);
                break;
            case 11: 
                exit(EXIT_SUCCESS); // Salir de la aplicacion
        }
    }

    return 0;
}

tipoProducto *crearProducto(char* nombre, char* tipo, char* marca, long stock, int precio)
{
    tipoProducto *producto = (tipoProducto *) malloc(sizeof(tipoProducto));
    strcpy(producto->nombre, nombre);
    strcpy(producto->tipo, tipo);
    strcpy(producto->marca, marca);
    producto->stock = stock;
    producto->precio = precio;
    return producto;
}

void agregarProducto(char* nombre, char* tipo, char* marca, long stock, int precio)
{
    Pair *pair = searchMap(mapaNombre, nombre);
    if(pair)
    {
        tipoProducto *producto = pair->value;
        producto->stock += stock;
    }
    else
    {
        tipoProducto *producto = crearProducto(nombre, tipo, marca, stock, precio);
        insertMap(mapaNombre, nombre, producto);

        pair = searchMap(mapaTipo, tipo);
        if(pair)
        {
            List *listaTipo = pair->value;
            pushFront(listaTipo, producto);
        }
        else
        {
            List *listaTipo = createList();
            pushFront(listaTipo, producto);
            insertMap(mapaTipo, tipo, listaTipo);
        }

        pair = searchMap(mapaMarca, marca);
        if(pair)
        {
            List *listaMarca = pair->value;
            pushFront(listaMarca, producto);
        }
        else
        {
            List *listaMarca = createList();
            pushFront(listaMarca, producto);
            insertMap(mapaMarca, marca, listaMarca);
        }
    }
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