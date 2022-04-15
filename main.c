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
    int stock;
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
    int precioTotal;
} tipoCarrito;

void importarProductos(char* nombreArchivo);

void exportarProductos(char* nombreArchivo);

void agregarProducto(char* nombre, char* tipo, char* marca, int stock, int precio);

void buscarProductosTipo(HashMap* map, char* tipo);

void buscarProductosMarca(HashMap* map, char* marca);

void buscarProductosNombre(HashMap* map, char* nombre);

void mostrarProductos(HashMap* map);

void agregarProductoCarrito(char *nombreProducto, int cantidad, char *nombreCarrito);

void eliminarProductoCarrito(tipoCarrito* carrito);

void concretarCompra(char* carrito);

void mostrarCarritos();

// Funciones auxiliares
void mostrarInfoProducto(tipoProducto *producto);
tipoProducto *crearProducto(char* nombre, char* tipo, char* marca, int stock, int precio);
tipoCarrito *crearCarrito(char *nombre);

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

    char nombreProducto [30];
    char tipo [30];
    char marca [30];
    int stock;
    int precio;
    char nombreCarrito [30];
    int cantidad;

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
                scanf("%[^\n]", nombreProducto);
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
                agregarProducto(nombreProducto, tipo, marca, stock, precio);
                printf("\n");
                break;
            case 4:
                printf("Ingrese el tipo de producto: ");
                scanf("%[^\n]", tipo);
                fflush(stdin);
                buscarProductosTipo(mapaTipo, tipo);
                break;
            case 8:
                printf("Ingrese el nombre del producto: ");
                scanf("%[^\n]", nombreProducto);
                fflush(stdin);
                printf("Ingrese la cantidad del producto: ");
                scanf("%d", &cantidad);
                fflush(stdin);
                printf("Ingrese el nombre del carrito: ");
                scanf("%[^\n]", nombreCarrito);
                fflush(stdin);
                agregarProductoCarrito(nombreProducto, cantidad, nombreCarrito);
                break;

            case 10:
                printf("Ingrese el nombre del carrito: ");
                scanf("%[^\n]", nombreCarrito);
                fflush(stdin);

                concretarCompra(nombreCarrito);
                break;
            case 12: 
                exit(EXIT_SUCCESS); // Salir de la aplicacion
        }
    }

    return 0;
}

tipoProducto *crearProducto(char* nombre, char* tipo, char* marca, int stock, int precio)
{
    tipoProducto *producto = (tipoProducto *) malloc(sizeof(tipoProducto));
    strcpy(producto->nombre, nombre);
    strcpy(producto->tipo, tipo);
    strcpy(producto->marca, marca);
    producto->stock = stock;
    producto->precio = precio;
    return producto;
}

void agregarProducto(char* nombre, char* tipo, char* marca, int stock, int precio)
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

tipoCarrito *crearCarrito(char *nombre)
{
    tipoCarrito *carrito = (tipoCarrito *) malloc(sizeof(tipoCarrito *));
    carrito->listaProductos = createList();
    carrito->cantidadProductos = 0;
    carrito->precioTotal = 0;
    insertMap(mapaCarritos, nombre, carrito);
    return carrito;
}

void agregarProductoCarrito(char *nombreProducto, int cantidad, char *nombreCarrito)
{
    Pair *pair = searchMap(mapaNombre, nombreProducto);
    if(!pair)
    {
        printf("El producto ingresado no existe.\n\n");
        return;
    }

    tipoProducto *producto = pair->value;
    if(producto->stock < cantidad)
    {
        printf("No hay suficiente stock del producto.\n\n");
        return;
    }

    tipoProductoCompra *productoCompra = (tipoProductoCompra*) malloc(sizeof(tipoProductoCompra));
    strcpy(productoCompra->nombre, nombreProducto);
    productoCompra->cantidad = cantidad;

    pair = searchMap(mapaCarritos, nombreCarrito);
    tipoCarrito *carrito;
    if(pair)
        carrito = pair->value;
    else
        carrito = crearCarrito(nombreCarrito);

    pushFront(carrito->listaProductos, productoCompra);
    carrito->cantidadProductos++;
    carrito->precioTotal += (cantidad * producto->precio);
    printf("Se agregó el producto al carrito.\n\n");
}

void concretarCompra(char* nombreCarrito)
{
    tipoCarrito* carrito;
    tipoProductoCompra* productoCarrito;
    tipoProducto* productoStock;
    int respuesta;

    carrito = (tipoCarrito *) searchMap(mapaCarritos, nombreCarrito);

    if(carrito!=NULL)
    {
        printf("Total a pagar: %d\n", carrito->precioTotal);

        printf("Productos dentro del carrito: \n");

        while (productoCarrito!=NULL)
        {
            productoCarrito = (tipoProductoCompra *) firstList(carrito->listaProductos);
            productoStock = (tipoProducto *) searchMap(mapaNombre, productoCarrito->nombre);

            printf("-%s, cantidad: %d\n", productoCarrito->nombre, productoCarrito->cantidad);

            productoStock->stock -= productoCarrito->cantidad;
            popFront(carrito->listaProductos); 
        }

        printf("¿Quiere concretar la compra?\n");
        printf("1.- Sí // 2.- No");
        scanf("%d", &respuesta);

        if(respuesta == 1)
            eraseMap(mapaCarritos, nombreCarrito);
    }
    else
        printf("No se encontró el carrito buscado");
    
}