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
            break;  
            case 2:
                exportarProductos(); 
            break;  
            case 3:
                agregaProducto();
                break;  
            case 4:
                
                break;  
            case 5: break;  
            case 6: break;  
            case 7: 
                
                break;
            case 8: 
                
                break;  
            case 9: 
                
                break; 
            case 10: 
                
                break;       
            case 11: exit(EXIT_SUCCESS); // Salir de la aplicacion
    }


    return 0;
}

void concretarCompra(tipoCarrito* carrito);
