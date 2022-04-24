#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "list.h"
#include "hashmap.h"

typedef struct
{
    char nombre [60];
    char tipo [30];
    char marca [30];
    int stock;
    int precio;
} tipoProducto;

typedef struct
{
    char nombre [60];
    int cantidad;
} tipoProductoCompra;

typedef struct
{
    List* listaProductos;
    int cantidadProductos;
    int precioTotal;
} tipoCarrito;

char *get_csv_field (char * tmp, int k);

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

void mostrarTotalProductos();

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

    char nombreProducto [60];
    char tipo [30];
    char marca [30];
    int stock;
    int precio;
    char nombreCarrito [30];
    int cantidad;
    char nombreArchivo [30];

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
            case 1:// Importar productos
                fflush(stdin);
                printf("Ingrese el nombre del archivo: ");
                scanf("%[^\n]", nombreArchivo);
                importarProductos(nombreArchivo);
            break;

            case 2: // Exportar productos
                fflush(stdin);
                printf("Ingrese el nombre del archivo: ");
                scanf("%[^\n]", nombreArchivo);
                exportarProductos(nombreArchivo);
                break;

            case 3:
                fflush(stdin);
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
                fflush(stdin);
                printf("Ingrese el tipo de producto: ");
                scanf("%[^\n]", tipo);
                fflush(stdin);
                buscarProductosTipo(mapaTipo, tipo);
                break;

            case 5:
                fflush(stdin);
                printf("Ingrese la marca del producto: ");
                scanf("%[^\n]", marca);
                fflush(stdin);
                buscarProductosMarca(mapaMarca, marca);
                break;                

            case 6:
                fflush(stdin);
                printf("Ingrese el nombre del producto: ");
                scanf("%[^\n]", nombreProducto);
                fflush(stdin);
                buscarProductosNombre(mapaNombre, nombreProducto);
            break;

            case 7:
                mostrarTotalProductos();
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

void mostrarTotalProductos()
{
    Pair *pairAux = firstMap(mapaNombre);
    tipoProducto *productoAux = pairAux->value;
    while(pairAux)
    {
        mostrarInfoProducto(productoAux);
        pairAux = nextMap(mapaNombre);
        productoAux = pairAux->value;
    } else {
        return
    }
}

char *get_csv_field(char *linea, int indice)
{
    char *campo = (char *) malloc(100 * sizeof(char *)); // Guarda el string a retornar
    int i = 0; // Recorre la linea
    int k = 0; // Cuenta las comas
    int n = 0; // Recorre el campo
    bool comillas = false;

    while(linea[i] != '\0')
    {
        if(linea[i] == '\"')
        {
            comillas = !comillas;
        }

        if(k == indice)
        {
            if(linea[i] != '\"')
            {
                campo[n] = linea[i];
                n++;
            }
        }

        i++;

        if(linea[i] == ',' && !comillas)
        {
            k++;
            i++;
        }

        if(k > indice || linea[i] == '\0' || linea[i] == '\n')
        {
            campo[n] = '\0';
            return campo;
        }
    }

    return NULL;
}

void importarProductos(char* nombreArchivo)
{
    FILE* archivoEntrada;
    Pair* pair;
    tipoProducto* aux;

    archivoEntrada = fopen(nombreArchivo, "r");

    if(archivoEntrada == NULL)
    {
        printf("Error al importar archivo .csv");
        exit(EXIT_FAILURE);
    }

    char linea[1024];

    while (fgets (linea, 1023, archivoEntrada) != NULL) 
    {
        // Se guardan los datos según el campo que se recibe
        char *nombre = get_csv_field(linea, 0);
        char *tipo = get_csv_field(linea, 2);
        char *marca = get_csv_field(linea, 1);
        int stock = atoi(get_csv_field(linea, 3));
        int precio = atoi(get_csv_field(linea, 4));

        agregarProducto(nombre, tipo, marca, stock, precio);
    }

    fclose(archivoEntrada);
    printf("\n");
}

void exportarProductos(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "w"); 
    if(!archivo)
    {
        printf("No se pudo crear el archivo\n");
        return;
    }

    Pair *pair = firstMap(mapaNombre);
    tipoProducto *producto;
    while(pair)
    {
        producto = pair->value;
        fprintf(archivo, "%s,%s,%s,", producto->nombre, producto->marca, producto->tipo);
        fprintf(archivo, "%d,%d\n", producto->stock, producto->precio);
        pair = nextMap(mapaNombre);
    }

    fclose(archivo);
    printf("\n");
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
    if(!pair)
    {
        printf("No se encontraron productos del tipo ingresado\n\n");
        return;
    }

    List *listaProductos = pair->value;
    tipoProducto *producto = firstList(listaProductos);
    while(producto)
    {
        mostrarInfoProducto(producto);
        producto = nextList(listaProductos);
    }
}

void buscarProductosMarca(HashMap* map, char* marca)
{
    Pair *pair = searchMap(map, marca);
    if(!pair)
    {
        printf("No se encontraron productos de la marca ingresada\n\n");
        return;
    }

    List *listaProductos = pair->value;
    tipoProducto *producto = firstList(listaProductos);
    while(producto)
    {
        mostrarInfoProducto(producto);
        producto = nextList(listaProductos);
    }
}

void buscarProductosNombre(HashMap* mapa, char* nombre)
{
    Pair* pair = searchMap(mapa, nombre);
    if(pair)
    {
        tipoProducto *producto = pair->value;
        mostrarInfoProducto(producto);
    }
    else
    {
        printf("No se encontró un producto con el nombre ingresado\n\n");
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
    Pair* pair;
    tipoCarrito* carrito;
    tipoProductoCompra* productoCarrito;
    tipoProducto* productoStock;
    int respuesta;
    int cont = 0;

    pair = searchMap(mapaCarritos, nombreCarrito);

    if(pair)
    {
        carrito = pair->value;

        printf("Total a pagar: %d\n", carrito->precioTotal);

        printf("Productos dentro del carrito: \n");

        while (cont < carrito->cantidadProductos);
        {
            productoCarrito = (tipoProductoCompra *) firstList(carrito->listaProductos);

            if(productoCarrito != NULL)
            {
                productoStock = (tipoProducto *) searchMap(mapaNombre, productoCarrito->nombre);

                printf("-%s, cantidad: %d\n", productoCarrito->nombre, productoCarrito->cantidad);

                productoStock->stock -= productoCarrito->cantidad;
                popFront(carrito->listaProductos);

                cont++;
            }
        }

        if (carrito->cantidadProductos == 0)
            printf("El carrito está vacío");
        
        if(carrito->cantidadProductos != 0)
        {
            printf("¿Quiere concretar la compra?\n");
            printf("1.- Sí // 2.- No");
            scanf("%d", &respuesta);
        }

        if(respuesta == 1)
            eraseMap(mapaCarritos, nombreCarrito);
    }
    else
        printf("No se encontró el carrito buscado");
    
}