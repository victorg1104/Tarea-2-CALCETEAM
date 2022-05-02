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
    tipoProducto *producto;
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

void buscarProductosLista(HashMap* map, char* clave);

void buscarProductosNombre(HashMap* map, char* nombre);

void mostrarProductos(HashMap* map);

void agregarProductoCarrito(char *nombreProducto, int cantidad, char *nombreCarrito);

void eliminarProductoCarrito(char* carrito);

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
    // Mapas globales para la busqueda
    mapaTipo = createMap(200);
    mapaMarca = createMap(200);
    mapaNombre = createMap(200);
    mapaCarritos = createMap(200);

    // Variables auxiliares para c/opción del menú
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
        printf("7.-  Mostrar todos los productos disponibles\n");
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
                buscarProductosLista(mapaTipo, tipo);
                break;

            case 5:
                fflush(stdin);
                printf("Ingrese la marca del producto: ");
                scanf("%[^\n]", marca);
                fflush(stdin);
                buscarProductosLista(mapaMarca, marca);
                break;                

            case 6:
                fflush(stdin);
                printf("Ingrese el nombre del producto: ");
                scanf("%[^\n]", nombreProducto);
                fflush(stdin);
                buscarProductosNombre(mapaNombre, nombreProducto);
            break;

            case 7:
                mostrarProductos(mapaNombre);
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
            
            case 9:
                printf("Ingrese el nombre del carrito: ");
                scanf("%[^\n]", nombreCarrito);
                eliminarProductoCarrito(nombreCarrito);
                break;

            case 10:
                printf("Ingrese el nombre del carrito: ");
                scanf("%[^\n]", nombreCarrito);
                fflush(stdin);

                concretarCompra(nombreCarrito);
                break;
            
            case 11:
                mostrarCarritos();
                break;

            case 12: 
                exit(EXIT_SUCCESS); // Salir de la aplicacion
        }
    }

    return 0;
}

void mostrarProductos(HashMap* map)
{
    // Accedemos al primer elemento del mapa para iterar sobre el
    Pair *pairAux = firstMap(map);

    // Si pairAux no existe, el mapa está vacío
    if(!pairAux)
    {
        printf("No hay productos para mostrar.\n\n");
        return;
    }

    // Creamos un producto auxiliar para imprimir su información
    tipoProducto *productoAux;

    // Mientras pairAux sea distinto a NULL
    while(pairAux)
    {
        productoAux = pairAux->value;
        mostrarInfoProducto(productoAux); // Imprimimos el producto actual
        pairAux = nextMap(map);    // Siguiente producto en el mapa de clave Nombre
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
    // Variables auxiliares
    FILE* archivoEntrada;
    Pair* pair;
    tipoProducto* aux;

    archivoEntrada = fopen(nombreArchivo, "r"); // Abrimos el archivo csv en modo lectura

    if(archivoEntrada == NULL)
    {
        printf("Error al importar archivo .csv");
        exit(EXIT_FAILURE);
    }

    char linea[1024]; // String donde almacenaremos cada linea del archivo

    // Iteramos cada linea hasta llegar al final del archivo
    while (fgets (linea, 1023, archivoEntrada) != NULL) 
    {
        // Se guardan los datos según el campo que se recibe
        char *nombre = get_csv_field(linea, 0);
        char *tipo = get_csv_field(linea, 2);
        char *marca = get_csv_field(linea, 1);
        int stock = atoi(get_csv_field(linea, 3));  // Pasamos el string a entero
        int precio = atoi(get_csv_field(linea, 4)); // Pasamos el string a entero

        agregarProducto(nombre, tipo, marca, stock, precio);
    }

    fclose(archivoEntrada);
    printf("Productos importados correctamente.\n\n");
}

void exportarProductos(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "w"); // Abrimos el archivo destino en modo escritura
    
    if(!archivo)
    {
        printf("No se pudo crear el archivo\n");
        return;
    }

    // Creamos un par auxiliar para iterar desde el primer elemento del mapa
    Pair *pair = firstMap(mapaNombre);

    tipoProducto *producto;

    // Iteramos por todo el mapa
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
    Pair *pair = searchMap(mapaNombre, nombre); // Buscamos si el producto ya existe en el mapa con clave Nombres
    if(pair)
    {
        // Si ya existe, aumentamos el stock
        tipoProducto *producto = pair->value;
        producto->stock += stock;
    }
    else
    {
        // Si no existe, lo creamos y lo insertamos en el mapa
        tipoProducto *producto = crearProducto(nombre, tipo, marca, stock, precio);
        insertMap(mapaNombre, nombre, producto);

        // Buscamos si el producto ya existe en el mapa con clave Tipo
        pair = searchMap(mapaTipo, tipo);
        if(pair)
        {
            // Si ya existe, lo insertamos en la lista
            // Usamos una lista ya que un producto con el mismo nombre puede tener varios tipos
            List *listaTipo = pair->value;
            pushFront(listaTipo, producto);
        }
        else
        {
            // Si no existe, creamos la lista, insertamos el producto en ella e insertamos la lista en el mapa con clave Tipo
            List *listaTipo = createList();
            pushFront(listaTipo, producto);
            insertMap(mapaTipo, tipo, listaTipo);
        }

        // Buscamos si el producto ya existe en el mapa con clave Marca
        pair = searchMap(mapaMarca, marca);
        if(pair)
        {
            // Si ya existe, lo insertamos en la lista
            // Usamos una lista ya que un mismo producto puede tener varias marcas
            List *listaMarca = pair->value;
            pushFront(listaMarca, producto);
        }
        else
        {
            // Si no existe, creamos la lista, insertamos el producto en ella e insertamos la lista en el mapa con clave Marca
            List *listaMarca = createList();
            pushFront(listaMarca, producto);
            insertMap(mapaMarca, marca, listaMarca);
        }
    }
}

void buscarProductosLista(HashMap* map, char* clave)
{   
    // Se busca el producto con la clave ingresada en el mapa
    Pair *pair = searchMap(map, clave);
    if(!pair)
    {   
        // En caso que no se encuentre la clave imprime el mensaje por pantalla y termina la funcion
        printf("No se encontraron productos que coincidan\n\n");
        return;
    }

    // En caso de que la clave exista nos ubicamos en la primera posicion de la lista de productos
    List *listaProductos = pair->value;
    tipoProducto *producto = firstList(listaProductos);

    // Mostramos la informacion por pantalla del producto
    while(producto)
    {
        mostrarInfoProducto(producto);
        producto = nextList(listaProductos);
    }
}

void buscarProductosNombre(HashMap* mapa, char* nombre)
{   
    // Se busca el producto en el mapa con el nombre ingresado
    Pair* pair = searchMap(mapa, nombre);
    if(pair)
    {
        // En caso de que exista el producto con el nombre imprime la informacion por pantalla 
        tipoProducto *producto = pair->value;
        mostrarInfoProducto(producto);
    }
    else
    {   
        // En caso contrario, imprime por pantalla que no existe un producto con el nombre ingresado
        printf("No se encontró un producto con el nombre ingresado\n\n");
    }
}

// Imprime la informacion detallada de un producto
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
    // Se reserva memoria necesaria para el carrito
    tipoCarrito *carrito = (tipoCarrito *) malloc(sizeof(tipoCarrito *));

    // Se asignan e inicializan las variables correspondientes
    carrito->listaProductos = createList();
    carrito->cantidadProductos = 0;
    carrito->precioTotal = 0;

    // Se inserta en el mapa de carritos
    insertMap(mapaCarritos, nombre, carrito);
    return carrito;
}

void agregarProductoCarrito(char *nombreProducto, int cantidad, char *nombreCarrito)
{   
    // Se busca el nombre del producto ingresado en el mapa de productos
    Pair *pair = searchMap(mapaNombre, nombreProducto);

    // En caso de que el producto no se encuentre en el mapa
    if(!pair)
    {
        printf("El producto ingresado no existe.\n\n");
        return;
    }

    // Se verifica si hay stock suficiente para agregar el producto al carrito
    tipoProducto *producto = pair->value;
    if(producto->stock < cantidad)
    {
        printf("No hay suficiente stock del producto.\n\n");
        return;
    }

    // Se pasa a reservar memoria para agregar el producto al carrito
    // Se asignan los valores correspondientes
    tipoProductoCompra *productoCompra = (tipoProductoCompra*) malloc(sizeof(tipoProductoCompra));
    productoCompra->producto = producto;
    productoCompra->cantidad = cantidad;

    // Se busca si el nombre del carrito ya existe
    pair = searchMap(mapaCarritos, nombreCarrito);
    tipoCarrito *carrito;

    if(pair)
    {   
        // En caso que exista, se le asigna el valor al carrito
        carrito = pair->value;
    }
    else
    {   
        // En caso que no exista, se crea un nuevo carrito
        carrito = crearCarrito(nombreCarrito);
    }

    // Se debe verificar si el producto ya ha sido ingresado anteriormente al carrito
    // Nos ubicamos en la primera posicion de la lista de productos del carrito
    tipoProductoCompra* aux = firstList(carrito->listaProductos);
    bool existe = false;

    while (aux)
    {   
        // En caso de que el producto ya exista
        if(strcmp(productoCompra->producto->nombre, aux->producto->nombre) == 0)
        {   
            // Se aumenta la cantidad unicamente
            aux->cantidad += productoCompra->cantidad;
            existe = true;
            break;
        }
        aux = nextList(carrito->listaProductos);
    }

    // En caso de que el producto no exista
    if(existe == false) 
    {   
        // Se agrega el nuevo producto al carrito
        pushFront(carrito->listaProductos, productoCompra);
        carrito->cantidadProductos++;
    }

    // Se aumenta el precio total del carrito
    carrito->precioTotal += (cantidad * producto->precio);
    printf("Se agrego el producto al carrito.\n\n");
}

void eliminarProductoCarrito(char* nombreCarrito)
{   
    // Se busca el nombre del carrito en el mapaCarritos
    Pair* pair = searchMap(mapaCarritos, nombreCarrito);

    // En caso que el carrito no exista
    if(!pair){
        printf("El carrito ingresado no existe.\n\n");
    }
    else{
        tipoCarrito* carrito = pair->value;

        // Si el carrito contiene productos se elimina el ultimo ingresado
        if(carrito->cantidadProductos > 0 ){
            popFront(carrito->listaProductos);
            carrito->cantidadProductos--;
            printf("El producto ha sido eliminado.\n\n");
        }
        else{
            // En caso que el carrito esté vacio
            printf("El carrito está vacio.\n\n");
        }
    }
}

void mostrarCarritos()
{
    // Nos ubicamos en la primera posicion del mapaCarritos
    Pair* pair = firstMap(mapaCarritos);

    // En caso que no existan carritos
    if(pair == NULL){
        printf("No hay carritos creados.\n");
    }

    // Se recorre el mapaCarritos imprimendo la información
    tipoCarrito *carrito;
    while(pair)
    {
        carrito = pair->value;

        // En caso que el carrito exista pero esté vacio
        if(carrito->cantidadProductos == 0){
            printf("\nNombre del carrito: %s\n", pair->key);
            printf("Cantidad de productos: vacio\n");
        }

        // En caso que el carrito exista y contenga productos
        if(carrito->cantidadProductos > 0){
            printf("\nNombre del carrito: %s\n", pair->key);
            printf("Cantidad de productos: %d\n", carrito->cantidadProductos);
        }
        
        // Se avanza a la siguiente posicion del mapaCarritos
        pair = nextMap(mapaCarritos);
    }
    printf("\n");
}

void concretarCompra(char* nombreCarrito)
{
    // Se busca el nombre del carrito en el mapaCarrito 
    Pair *pair = searchMap(mapaCarritos, nombreCarrito);

    // En caso que el carrito no exista
    if(!pair)
    {
        printf("No se encontró el carrito buscado\n\n");
        return;
    }

    // En caso que el carrito exista pero esté vacio
    tipoCarrito *carrito = pair->value;
    if(carrito->cantidadProductos == 0) {
        printf("El carrito está vacío\n\n");
        return;
    }

    // Se imprime el precio total a pagar 
    printf("\nTotal a pagar: %d\n", carrito->precioTotal);
    printf("Productos dentro del carrito: \n");

    List *listaProductosCarrito = carrito->listaProductos;
    tipoProductoCompra *productoCarrito = firstList(listaProductosCarrito);

    // Se recorre el carrito para mostrar los productos
    while(productoCarrito)
    {
        // Se muestran los productos del carrito
        printf("- %s, ", productoCarrito->producto->nombre);
        printf("cantidad: %d\n", productoCarrito->cantidad);
        productoCarrito = nextList(listaProductosCarrito);
    }

    int respuesta;

    // Confirma si quiere concretar la compra
    printf("\n¿Quiere concretar la compra?\n");
    printf("1.- Si // 2.- No\n");
    printf("Ingrese la opción: ");
    scanf("%d", &respuesta);
    printf("\n");

    // Caso que no quiera concretar la compra
    if(respuesta != 1) return;

    // Se recorre nuevamente el carrito para reducir el stock
    productoCarrito = firstList(listaProductosCarrito);
    while(productoCarrito)
    {
        productoCarrito->producto->stock -= productoCarrito->cantidad;
        productoCarrito = nextList(listaProductosCarrito);
    }
    // Se elimina el carrito del mapaCarritos
    eraseMap(mapaCarritos, nombreCarrito);
}