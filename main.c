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


int main()
{
    HashMap* mapaTipo = createMap(200);
    HashMap* mapaMarca = createMap(200);
    HashMap* mapaNombre = createMap(200);
    HashMap* mapaCarritos = createMap(200);

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
            case 1: 
                importarProductos();
            break;  // Importar canciones
            case 2:
                e 
            break;  // Exportar canciones
            case 3:
                agregarCancion();
                break;  // Agregar cancion
            case 4:
                fflush(stdin);
                printf("Ingrese el nombre de la cancion: ");
                scanf("%[^\n]", nombre);
                resultadoBusqueda = buscarCancionNombre(nombre);
                if(resultadoBusqueda != NULL)
                {
                    printf("Cancion encontrada: \n");
                    mostrarInfoCancion(resultadoBusqueda);
                } else {
                    printf("No se encontro la cancion\n");
                }
                break;  // Buscar por nombre
            case 5: break;  // Buscar por artista
            case 6: break;  // Buscar por genero
            case 7: // Eliminar cancion
                printf("Ingrese el nombre de la cancion: ");
                scanf("%[^\n]", nombre);
                fflush(stdin);
                printf("Ingrese el artista de la cancion: ");
                scanf("%[^\n]", artista);
                fflush(stdin);
                printf("Ingrese el año de la cancion: ");
                scanf("%d", &anio);
                fflush(stdin);
                eliminarCancion(nombre, artista, anio);
                break;
            case 8: // Mostrar nombres listas
                mostrarListasReproduccion();
                break;  
            case 9: // Mostrar canciones lista
                printf("Ingrese el nombre de la lista: ");
                scanf("%[^\n]", nombre);
                fflush(stdin);
                mostrarCancionesPorLista(nombre);
                break; 
            case 10: // Mostrar todas las canciones
                mostrarCanciones();
                break;       
            case 11: exit(EXIT_SUCCESS); // Salir de la aplicacion
    }


    return 0;
}

void concretarCompra(tipoCarrito* carrito);
