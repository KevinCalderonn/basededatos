#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Configuración de la base de datos
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS ""
#define DB_NAME "olimpiadas_invierno"

// Definiciones de colores en español
#define RESET "\033[0m"
#define ROJO "\033[31m"
#define VERDE "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL "\033[34m"
#define MAGENTA "\033[35m"
#define CIAN "\033[36m"
#define BLANCO "\033[37m"

// Estructura para conexión global
MYSQL *conn;

// Funciones auxiliares
void setcolor(int color) {
    switch(color) {
        case 1: printf(ROJO); break;
        case 2: printf(VERDE); break;
        case 3: printf(AMARILLO); break;
        case 4: printf(AZUL); break;
        case 5: printf(MAGENTA); break;
        case 6: printf(CIAN); break;
        case 7: printf(BLANCO); break;
        default: printf(RESET); break;
    }
}

// Mueve el cursor a posición (x,y)
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Limpia la pantalla
void limpiar_pantalla() {
    system("clear");
}

// Pausa la ejecución
void pausar() {
    printf("\nPresione Enter para continuar...");
    getchar();
    getchar();
}

// Establece conexión con la base de datos
int conectar_bd() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        setcolor(1);
        printf("Error al inicializar MySQL: %s\n", mysql_error(conn));
        setcolor(0);
        return 0;
    }
    
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        setcolor(1);
        printf("Error al conectar con la base de datos: %s\n", mysql_error(conn));
        setcolor(0);
        mysql_close(conn);
        return 0;
    }
    
    setcolor(2);
    printf("Conexión exitosa a la base de datos!\n");
    setcolor(0);
    return 1;
}

// Cierra la conexión con la base de datos
void desconectar_bd() {
    if (conn != NULL) {
        mysql_close(conn);
    }
}

// Muestra el encabezado principal
void mostrar_encabezado() {
    limpiar_pantalla();
    gotoxy(1, 1);
    setcolor(6);
    printf("================================================================================\n");
    printf("                        SISTEMA OLIMPIADAS DE INVIERNO                        \n");
    printf("                            GESTIÓN DE PRUEBAS DE ESQUÍ                       \n");
    printf("================================================================================\n");
    setcolor(0);
}

// Muestra 3 registros de ejemplo de una tabla
void mostrar_ejemplos_tabla(const char* tabla) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[512];
    
    setcolor(3);
    printf("\n--- EJEMPLOS DE DATOS EXISTENTES EN %s ---\n", tabla);
    setcolor(0);
    
    snprintf(query, sizeof(query), "SELECT * FROM %s LIMIT 3", tabla);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al consultar ejemplos: %s\n", mysql_error(conn));
        setcolor(0);
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        return;
    }
    
    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    
    // Mostrar encabezados de columnas
    setcolor(4);
    for (int i = 0; i < num_fields; i++) {
        printf("%-15s ", fields[i].name);
    }
    printf("\n");
    setcolor(0);
    
    // Línea separadora
    for (int i = 0; i < num_fields * 16; i++) {
        printf("-");
    }
    printf("\n");
    
    // Mostrar datos
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%-15s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    
    mysql_free_result(result);
    printf("\n");
}


// ==================== OPERACIONES CRUD PARA FEDERACION ====================
void crear_federacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== CREAR NUEVA FEDERACIÓN ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("federacion");
    
    int id, num_federados;
    char nombre[101];
    char query[512];
    
    printf("Ingrese ID de la federación: ");
    if (scanf("%d", &id) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nombre de la federación: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese número de federados: ");
    if (scanf("%d", &num_federados) != 1) {
        setcolor(1);
        printf("Error: Número de federados debe ser un entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "INSERT INTO federacion (id, nombre, num_federados) VALUES (%d, '%s', %d)",
        id, nombre, num_federados);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al insertar federación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        setcolor(2);
        printf("Federación creada exitosamente!\n");
        setcolor(0);
    }
    
    pausar();
}

// Lista todas las federaciones
void leer_federaciones() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== LISTADO DE FEDERACIONES ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    if (mysql_query(conn, "SELECT * FROM federacion ORDER BY id")) {
        setcolor(1);
        printf("Error al consultar federaciones: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-10s %-30s %-15s\n", "ID", "NOMBRE", "NUM_FEDERADOS");
    setcolor(0);
    printf("--------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%-10s %-30s %-15s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL");
    }
    
    mysql_free_result(result);
    pausar();
}

// Actualiza datos de federación
void actualizar_federacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ACTUALIZAR FEDERACIÓN ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("federacion");
    
    int id, num_federados;
    char nombre[101];
    char query[512];
    
    printf("Ingrese ID de la federación a actualizar: ");
    if (scanf("%d", &id) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nuevo nombre: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese nuevo número de federados: ");
    if (scanf("%d", &num_federados) != 1) {
        setcolor(1);
        printf("Error: Número de federados debe ser un entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "UPDATE federacion SET nombre='%s', num_federados=%d WHERE id=%d",
        nombre, num_federados, id);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al actualizar federación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Federación actualizada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró federación con ID %d\n", id);
            setcolor(0);
        }
    }
    
    pausar();
}

// Elimina una federación
void eliminar_federacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ELIMINAR FEDERACIÓN ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("federacion");
    
    int id;
    char query[256];
    
    printf("Ingrese ID de la federación a eliminar: ");
    if (scanf("%d", &id) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), "DELETE FROM federacion WHERE id=%d", id);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al eliminar federación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Federación eliminada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró federación con ID %d\n", id);
            setcolor(0);
        }
    }
    
    pausar();
}

// ==================== OPERACIONES CRUD PARA ESQUIADOR ====================
// Crea nuevo esquiador
void crear_esquiador() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== CREAR NUEVO ESQUIADOR ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("esquiador");
    
    char dni[21], nombre[101];
    int edad, id_federacion;
    char query[512];
    
    printf("Ingrese DNI del esquiador: ");
    scanf(" %20[^\n]", dni);
    
    printf("Ingrese nombre del esquiador: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese edad: ");
    if (scanf("%d", &edad) != 1) {
        setcolor(1);
        printf("Error: Edad debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese ID de federación: ");
    if (scanf("%d", &id_federacion) != 1) {
        setcolor(1);
        printf("Error: ID federación debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "INSERT INTO esquiador (dni, nombre, edad, id_federacion) VALUES ('%s', '%s', %d, %d)",
        dni, nombre, edad, id_federacion);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al insertar esquiador: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        setcolor(2);
        printf("Esquiador creado exitosamente!\n");
        setcolor(0);
    }
    
    pausar();
}

// Lista todos los esquiadores
void leer_esquiadores() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== LISTADO DE ESQUIADORES ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    const char* query = "SELECT e.dni, e.nombre, e.edad, f.nombre as federacion "
                       "FROM esquiador e "
                       "LEFT JOIN federacion f ON e.id_federacion = f.id "
                       "ORDER BY e.nombre";
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al consultar esquiadores: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-15s %-25s %-8s %-20s\n", "DNI", "NOMBRE", "EDAD", "FEDERACIÓN");
    setcolor(0);
    printf("----------------------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%-15s %-25s %-8s %-20s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL",
            row[3] ? row[3] : "Sin federación");
    }
    
    mysql_free_result(result);
    pausar();
}

// Actualiza datos de esquiador
void actualizar_esquiador() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ACTUALIZAR ESQUIADOR ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("esquiador");
    
    char dni[21], nombre[101];
    int edad, id_federacion;
    char query[512];
    
    printf("Ingrese DNI del esquiador a actualizar: ");
    scanf(" %20[^\n]", dni);
    
    printf("Ingrese nuevo nombre: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese nueva edad: ");
    if (scanf("%d", &edad) != 1) {
        setcolor(1);
        printf("Error: Edad debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nuevo ID de federación: ");
    if (scanf("%d", &id_federacion) != 1) {
        setcolor(1);
        printf("Error: ID federación debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "UPDATE esquiador SET nombre='%s', edad=%d, id_federacion=%d WHERE dni='%s'",
        nombre, edad, id_federacion, dni);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al actualizar esquiador: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Esquiador actualizado exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró esquiador con DNI %s\n", dni);
            setcolor(0);
        }
    }
    
    pausar();
}

// Elimina un esquiador
void eliminar_esquiador() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ELIMINAR ESQUIADOR ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("esquiador");
    
    char dni[21];
    char query[256];
    
    printf("Ingrese DNI del esquiador a eliminar: ");
    scanf(" %20[^\n]", dni);
    
    snprintf(query, sizeof(query), "DELETE FROM esquiador WHERE dni='%s'", dni);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al eliminar esquiador: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Esquiador eliminado exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró esquiador con DNI %s\n", dni);
            setcolor(0);
        }
    }
    
    pausar();
}

// ==================== OPERACIONES CRUD PARA ESTACION ====================
// Crea nueva estación
void crear_estacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== CREAR NUEVA ESTACIÓN DE ESQUÍ ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("estacion");
    
    char codigo[21], nombre[101], direccion[151], telefono[21], personas_contacto[151];
    float km_esquiables;
    char query[1024];
    
    printf("Ingrese código de la estación: ");
    scanf(" %20[^\n]", codigo);
    
    printf("Ingrese nombre de la estación: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese dirección: ");
    scanf(" %150[^\n]", direccion);
    
    printf("Ingrese teléfono: ");
    scanf(" %20[^\n]", telefono);
    
    printf("Ingrese kilómetros esquiables: ");
    if (scanf("%f", &km_esquiables) != 1) {
        setcolor(1);
        printf("Error: Kilómetros esquiables debe ser un número decimal.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese personas de contacto: ");
    scanf(" %150[^\n]", personas_contacto);
    
    snprintf(query, sizeof(query), 
        "INSERT INTO estacion (codigo, nombre, direccion, telefono, km_esquiables, personas_contacto) "
        "VALUES ('%s', '%s', '%s', '%s', %.2f, '%s')",
        codigo, nombre, direccion, telefono, km_esquiables, personas_contacto);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al insertar estación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        setcolor(2);
        printf("Estación creada exitosamente!\n");
        setcolor(0);
    }
    
    pausar();
}

// Lista todas las estaciones
void leer_estaciones() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== LISTADO DE ESTACIONES DE ESQUÍ ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    if (mysql_query(conn, "SELECT * FROM estacion ORDER BY nombre")) {
        setcolor(1);
        printf("Error al consultar estaciones: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-10s %-20s %-25s %-15s %-10s\n", "CÓDIGO", "NOMBRE", "DIRECCIÓN", "TELÉFONO", "KM_ESQUI");
    setcolor(0);
    printf("--------------------------------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%-10s %-20s %-25s %-15s %-10s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL",
            row[3] ? row[3] : "NULL",
            row[4] ? row[4] : "NULL");
    }
    
    mysql_free_result(result);
    pausar();
}

// Actualiza datos de estación
void actualizar_estacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ACTUALIZAR ESTACIÓN DE ESQUÍ ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("estacion");
    
    char codigo[21], nombre[101], direccion[151], telefono[21], personas_contacto[151];
    float km_esquiables;
    char query[1024];
    
    printf("Ingrese código de la estación a actualizar: ");
    scanf(" %20[^\n]", codigo);
    
    printf("Ingrese nuevo nombre: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese nueva dirección: ");
    scanf(" %150[^\n]", direccion);
    
    printf("Ingrese nuevo teléfono: ");
    scanf(" %20[^\n]", telefono);
    
    printf("Ingrese nuevos kilómetros esquiables: ");
    if (scanf("%f", &km_esquiables) != 1) {
        setcolor(1);
        printf("Error: Kilómetros esquiables debe ser un número decimal.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nuevas personas de contacto: ");
    scanf(" %150[^\n]", personas_contacto);
    
    snprintf(query, sizeof(query), 
        "UPDATE estacion SET nombre='%s', direccion='%s', telefono='%s', "
        "km_esquiables=%.2f, personas_contacto='%s' WHERE codigo='%s'",
        nombre, direccion, telefono, km_esquiables, personas_contacto, codigo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al actualizar estación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Estación actualizada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró estación con código %s\n", codigo);
            setcolor(0);
        }
    }
    
    pausar();
}

// Elimina una estación
void eliminar_estacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ELIMINAR ESTACIÓN DE ESQUÍ ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("estacion");
    
    char codigo[21];
    char query[256];
    
    printf("Ingrese código de la estación a eliminar: ");
    scanf(" %20[^\n]", codigo);
    
    snprintf(query, sizeof(query), "DELETE FROM estacion WHERE codigo='%s'", codigo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al eliminar estación: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Estación eliminada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró estación con código %s\n", codigo);
            setcolor(0);
        }
    }
    
    pausar();
}

// ==================== OPERACIONES CRUD PARA PRUEBA ====================
// Crea nueva prueba
void crear_prueba() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== CREAR NUEVA PRUEBA ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("prueba");
    
    int id_prueba;
    char nombre[101], tipo[20], participante_vencedor_tipo[20], participante_vencedor_id[51], tiempo[20];
    char query[1024];
    
    printf("Ingrese ID de la prueba: ");
    if (scanf("%d", &id_prueba) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nombre de la prueba: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese tipo (fondo/slalom/salto): ");
    scanf(" %19[^\n]", tipo);
    
    printf("Ingrese tipo de participante vencedor (individual/equipo): ");
    scanf(" %19[^\n]", participante_vencedor_tipo);
    
    printf("Ingrese ID del participante vencedor: ");
    scanf(" %50[^\n]", participante_vencedor_id);
    
    printf("Ingrese tiempo empleado (HH:MM:SS): ");
    scanf(" %19[^\n]", tiempo);
    
    snprintf(query, sizeof(query), 
        "INSERT INTO prueba (id_prueba, nombre, tipo, participante_vencedor_tipo, "
        "participante_vencedor_id, tiempo_empleado_participantevencedor) "
        "VALUES (%d, '%s', '%s', '%s', '%s', '%s')",
        id_prueba, nombre, tipo, participante_vencedor_tipo, participante_vencedor_id, tiempo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al insertar prueba: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        setcolor(2);
        printf("Prueba creada exitosamente!\n");
        setcolor(0);
    }
    
    pausar();
}

// Lista todas las pruebas
void leer_pruebas() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== LISTADO DE PRUEBAS ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    if (mysql_query(conn, "SELECT * FROM prueba ORDER BY id_prueba")) {
        setcolor(1);
        printf("Error al consultar pruebas: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-8s %-20s %-10s %-12s %-15s %-10s\n", "ID", "NOMBRE", "TIPO", "TIPO_GANADO", "ID_GANADOR", "TIEMPO");
    setcolor(0);
    printf("---------------------------------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%-8s %-20s %-10s %-12s %-15s %-10s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL",
            row[3] ? row[3] : "NULL",
            row[4] ? row[4] : "NULL",
            row[5] ? row[5] : "NULL");
    }
    
    mysql_free_result(result);
    pausar();
}

// Actualiza datos de prueba
void actualizar_prueba() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ACTUALIZAR PRUEBA ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("prueba");
    
    int id_prueba;
    char nombre[101], tipo[20], participante_vencedor_tipo[20], participante_vencedor_id[51], tiempo[20];
    char query[1024];
    
    printf("Ingrese ID de la prueba a actualizar: ");
    if (scanf("%d", &id_prueba) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    printf("Ingrese nuevo nombre de la prueba: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese nuevo tipo (fondo/slalom/salto): ");
    scanf(" %19[^\n]", tipo);
    
    printf("Ingrese nuevo tipo de participante vencedor (individual/equipo): ");
    scanf(" %19[^\n]", participante_vencedor_tipo);
    
    printf("Ingrese nuevo ID del participante vencedor: ");
    scanf(" %50[^\n]", participante_vencedor_id);
    
    printf("Ingrese nuevo tiempo empleado (HH:MM:SS): ");
    scanf(" %19[^\n]", tiempo);
    
    snprintf(query, sizeof(query), 
        "UPDATE prueba SET nombre='%s', tipo='%s', participante_vencedor_tipo='%s', "
        "participante_vencedor_id='%s', tiempo_empleado_participantevencedor='%s' WHERE id_prueba=%d",
        nombre, tipo, participante_vencedor_tipo, participante_vencedor_id, tiempo, id_prueba);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al actualizar prueba: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Prueba actualizada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró prueba con ID %d\n", id_prueba);
            setcolor(0);
        }
    }
    
    pausar();
}

// Elimina una prueba
void eliminar_prueba() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ELIMINAR PRUEBA ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("prueba");
    
    int id_prueba;
    char query[256];
    
    printf("Ingrese ID de la prueba a eliminar: ");
    if (scanf("%d", &id_prueba) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), "DELETE FROM prueba WHERE id_prueba=%d", id_prueba);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al eliminar prueba: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Prueba eliminada exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró prueba con ID %d\n", id_prueba);
            setcolor(0);
        }
    }
    
    pausar();
}

// ==================== OPERACIONES CRUD PARA EQUIPO ====================
// Crea nuevo equipo
void crear_equipo() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== CREAR NUEVO EQUIPO ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("equipo");
    
    char id_equipo[21], nombre[101];
    int id_federacion;
    char query[512];
    
    printf("Ingrese ID del equipo: ");
    scanf(" %20[^\n]", id_equipo);
    
    printf("Ingrese nombre del equipo: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese ID de federación: ");
    if (scanf("%d", &id_federacion) != 1) {
        setcolor(1);
        printf("Error: ID federación debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "INSERT INTO equipo (id_equipo, nombre, id_federacion) VALUES ('%s', '%s', %d)",
        id_equipo, nombre, id_federacion);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al insertar equipo: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        setcolor(2);
        printf("Equipo creado exitosamente!\n");
        setcolor(0);
    }
    
    pausar();
}

// Lista todos los equipos
void leer_equipos() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== LISTADO DE EQUIPOS ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    const char* query = "SELECT e.id_equipo, e.nombre, f.nombre as federacion "
                       "FROM equipo e "
                       "LEFT JOIN federacion f ON e.id_federacion = f.id "
                       "ORDER BY e.nombre";
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al consultar equipos: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-15s %-25s %-20s\n", "ID_EQUIPO", "NOMBRE", "FEDERACIÓN");
    setcolor(0);
    printf("--------------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(result))) {
        printf("%-15s %-25s %-20s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "Sin federación");
    }
    
    mysql_free_result(result);
    pausar();
}

// Actualiza datos de equipo
void actualizar_equipo() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ACTUALIZAR EQUIPO ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("equipo");
    
    char id_equipo[21], nombre[101];
    int id_federacion;
    char query[512];
    
    printf("Ingrese ID del equipo a actualizar: ");
    scanf(" %20[^\n]", id_equipo);
    
    printf("Ingrese nuevo nombre: ");
    scanf(" %100[^\n]", nombre);
    
    printf("Ingrese nuevo ID de federación: ");
    if (scanf("%d", &id_federacion) != 1) {
        setcolor(1);
        printf("Error: ID federación debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "UPDATE equipo SET nombre='%s', id_federacion=%d WHERE id_equipo='%s'",
        nombre, id_federacion, id_equipo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al actualizar equipo: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Equipo actualizado exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró equipo con ID %s\n", id_equipo);
            setcolor(0);
        }
    }
    
    pausar();
}

// Elimina un equipo
void eliminar_equipo() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ELIMINAR EQUIPO ===\n");
    setcolor(0);
    
    mostrar_ejemplos_tabla("equipo");
    
    char id_equipo[21];
    char query[256];
    
    printf("Ingrese ID del equipo a eliminar: ");
    scanf(" %20[^\n]", id_equipo);
    
    snprintf(query, sizeof(query), "DELETE FROM equipo WHERE id_equipo='%s'", id_equipo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al eliminar equipo: %s\n", mysql_error(conn));
        setcolor(0);
    } else {
        if (mysql_affected_rows(conn) > 0) {
            setcolor(2);
            printf("Equipo eliminado exitosamente!\n");
            setcolor(0);
        } else {
            setcolor(3);
            printf("No se encontró equipo con ID %s\n", id_equipo);
            setcolor(0);
        }
    }
    
    pausar();
}

// ==================== REPORTES Y CONSULTAS ====================
// Consulta esquiadores por federación
void consultar_esquiadores_por_federacion() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ESQUIADORES POR FEDERACIÓN ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    int id_federacion;
    char query[512];
    
    printf("Ingrese ID de la federación: ");
    if (scanf("%d", &id_federacion) != 1) {
        setcolor(1);
        printf("Error: ID debe ser un número entero.\n");
        setcolor(0);
        pausar();
        return;
    }
    
    snprintf(query, sizeof(query), 
        "SELECT e.dni, e.nombre, e.edad, f.nombre as federacion "
        "FROM esquiador e "
        "JOIN federacion f ON e.id_federacion = f.id "
        "WHERE e.id_federacion = %d "
        "ORDER BY e.nombre", id_federacion);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al consultar esquiadores: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-15s %-25s %-8s %-20s\n", "DNI", "NOMBRE", "EDAD", "FEDERACIÓN");
    setcolor(0);
    printf("----------------------------------------------------------------------\n");
    
    int count = 0;
    while ((row = mysql_fetch_row(result))) {
        printf("%-15s %-25s %-8s %-20s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL",
            row[3] ? row[3] : "NULL");
        count++;
    }
    
    setcolor(3);
    printf("\nTotal de esquiadores encontrados: %d\n", count);
    setcolor(0);
    
    mysql_free_result(result);
    pausar();
}

// Consulta pruebas por tipo
void consultar_pruebas_por_tipo() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== PRUEBAS POR TIPO ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    char tipo[20];
    char query[512];
    
    printf("Tipos disponibles: fondo, slalom, salto\n");
    printf("Ingrese tipo de prueba: ");
    scanf(" %19[^\n]", tipo);
    
    snprintf(query, sizeof(query), 
        "SELECT id_prueba, nombre, participante_vencedor_tipo, "
        "participante_vencedor_id, tiempo_empleado_participantevencedor "
        "FROM prueba "
        "WHERE tipo = '%s' "
        "ORDER BY tiempo_empleado_participantevencedor", tipo);
    
    if (mysql_query(conn, query)) {
        setcolor(1);
        printf("Error al consultar pruebas: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    result = mysql_store_result(conn);
    if (result == NULL) {
        setcolor(1);
        printf("Error al obtener resultados: %s\n", mysql_error(conn));
        setcolor(0);
        pausar();
        return;
    }
    
    setcolor(4);
    printf("%-8s %-20s %-12s %-15s %-10s\n", "ID", "NOMBRE", "TIPO_GANADO", "ID_GANADOR", "TIEMPO");
    setcolor(0);
    printf("---------------------------------------------------------------------------------\n");
    
    int count = 0;
    while ((row = mysql_fetch_row(result))) {
        printf("%-8s %-20s %-12s %-15s %-10s\n", 
            row[0] ? row[0] : "NULL",
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL",
            row[3] ? row[3] : "NULL",
            row[4] ? row[4] : "NULL");
        count++;
    }
    
    setcolor(3);
    printf("\nTotal de pruebas encontradas: %d\n", count);
    setcolor(0);
    
    mysql_free_result(result);
    pausar();
}

// Muestra estadísticas generales del sistema
void reporte_estadisticas_generales() {
    mostrar_encabezado();
    gotoxy(1, 6);
    setcolor(2);
    printf("=== ESTADÍSTICAS GENERALES DEL SISTEMA ===\n");
    setcolor(0);
    
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    // Contar federaciones
    if (mysql_query(conn, "SELECT COUNT(*) FROM federacion")) {
        setcolor(1);
        printf("Error al consultar federaciones\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result && (row = mysql_fetch_row(result))) {
            printf("Total de Federaciones: %s\n", row[0]);
        }
        mysql_free_result(result);
    }
    
    // Contar esquiadores
    if (mysql_query(conn, "SELECT COUNT(*) FROM esquiador")) {
        setcolor(1);
        printf("Error al consultar esquiadores\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result && (row = mysql_fetch_row(result))) {
            printf("Total de Esquiadores: %s\n", row[0]);
        }
        mysql_free_result(result);
    }
    
    // Contar equipos
    if (mysql_query(conn, "SELECT COUNT(*) FROM equipo")) {
        setcolor(1);
        printf("Error al consultar equipos\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result && (row = mysql_fetch_row(result))) {
            printf("Total de Equipos: %s\n", row[0]);
        }
        mysql_free_result(result);
    }
    
    // Contar estaciones
    if (mysql_query(conn, "SELECT COUNT(*) FROM estacion")) {
        setcolor(1);
        printf("Error al consultar estaciones\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result && (row = mysql_fetch_row(result))) {
            printf("Total de Estaciones: %s\n", row[0]);
        }
        mysql_free_result(result);
    }
    
    // Contar pruebas
    if (mysql_query(conn, "SELECT COUNT(*) FROM prueba")) {
        setcolor(1);
        printf("Error al consultar pruebas\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result && (row = mysql_fetch_row(result))) {
            printf("Total de Pruebas: %s\n", row[0]);
        }
        mysql_free_result(result);
    }
    
    printf("\n");
    setcolor(3);
    printf("--- DISTRIBUCIÓN DE PRUEBAS POR TIPO ---\n");
    setcolor(0);
    
    if (mysql_query(conn, "SELECT tipo, COUNT(*) as cantidad FROM prueba GROUP BY tipo ORDER BY cantidad DESC")) {
        setcolor(1);
        printf("Error al consultar tipos de pruebas\n");
        setcolor(0);
    } else {
        result = mysql_store_result(conn);
        if (result) {
            setcolor(4);
            printf("%-15s %-10s\n", "TIPO", "CANTIDAD");
            setcolor(0);
            printf("-------------------------\n");
            while ((row = mysql_fetch_row(result))) {
                printf("%-15s %-10s\n", 
                    row[0] ? row[0] : "NULL",
                    row[1] ? row[1] : "0");
            }
        }
        mysql_free_result(result);
    }
    
    pausar();
}

// ==================== MENÚS ====================
// Menú para gestión de federaciones
void menu_federacion() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== GESTIÓN DE FEDERACIONES ===\n");
        setcolor(0);
        printf("1. Crear federación\n");
        printf("2. Listar federaciones\n");
        printf("3. Actualizar federación\n");
        printf("4. Eliminar federación\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: crear_federacion(); break;
            case 2: leer_federaciones(); break;
            case 3: actualizar_federacion(); break;
            case 4: eliminar_federacion(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú para gestión de esquiadores
void menu_esquiador() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== GESTIÓN DE ESQUIADORES ===\n");
        setcolor(0);
        printf("1. Crear esquiador\n");
        printf("2. Listar esquiadores\n");
        printf("3. Actualizar esquiador\n");
        printf("4. Eliminar esquiador\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: crear_esquiador(); break;
            case 2: leer_esquiadores(); break;
            case 3: actualizar_esquiador(); break;
            case 4: eliminar_esquiador(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú para gestión de estaciones
void menu_estacion() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== GESTIÓN DE ESTACIONES DE ESQUÍ ===\n");
        setcolor(0);
        printf("1. Crear estación\n");
        printf("2. Listar estaciones\n");
        printf("3. Actualizar estación\n");
        printf("4. Eliminar estación\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: crear_estacion(); break;
            case 2: leer_estaciones(); break;
            case 3: actualizar_estacion(); break;
            case 4: eliminar_estacion(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú para gestión de pruebas
void menu_prueba() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== GESTIÓN DE PRUEBAS ===\n");
        setcolor(0);
        printf("1. Crear prueba\n");
        printf("2. Listar pruebas\n");
        printf("3. Actualizar prueba\n");
        printf("4. Eliminar prueba\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: crear_prueba(); break;
            case 2: leer_pruebas(); break;
            case 3: actualizar_prueba(); break;
            case 4: eliminar_prueba(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú para gestión de equipos
void menu_equipo() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== GESTIÓN DE EQUIPOS ===\n");
        setcolor(0);
        printf("1. Crear equipo\n");
        printf("2. Listar equipos\n");
        printf("3. Actualizar equipo\n");
        printf("4. Eliminar equipo\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: crear_equipo(); break;
            case 2: leer_equipos(); break;
            case 3: actualizar_equipo(); break;
            case 4: eliminar_equipo(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú para consultas y reportes
void menu_consultas() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== CONSULTAS Y REPORTES ===\n");
        setcolor(0);
        printf("1. Esquiadores por federación\n");
        printf("2. Pruebas por tipo\n");
        printf("3. Estadísticas generales\n");
        printf("0. Volver al menú principal\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: consultar_esquiadores_por_federacion(); break;
            case 2: consultar_pruebas_por_tipo(); break;
            case 3: reporte_estadisticas_generales(); break;
            case 0: break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// Menú principal del sistema
void menu_principal() {
    int opcion;
    
    do {
        mostrar_encabezado();
        gotoxy(1, 6);
        setcolor(3);
        printf("=== MENÚ PRINCIPAL ===\n");
        setcolor(0);
        printf("1. Gestión de Federaciones\n");
        printf("2. Gestión de Esquiadores\n");
        printf("3. Gestión de Estaciones de Esquí\n");
        printf("4. Gestión de Pruebas\n");
        printf("5. Gestión de Equipos\n");
        printf("6. Consultas y Reportes\n");
        printf("0. Salir del sistema\n");
        printf("\nSeleccione una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            setcolor(1);
            printf("Error: Ingrese un número válido.\n");
            setcolor(0);
            pausar();
            continue;
        }
        
        switch(opcion) {
            case 1: menu_federacion(); break;
            case 2: menu_esquiador(); break;
            case 3: menu_estacion(); break;
            case 4: menu_prueba(); break;
            case 5: menu_equipo(); break;
            case 6: menu_consultas(); break;
            case 0: 
                setcolor(2);
                printf("¡Gracias por usar el sistema!\n");
                setcolor(0);
                break;
            default:
                setcolor(1);
                printf("Opción no válida.\n");
                setcolor(0);
                pausar();
        }
    } while(opcion != 0);
}

// ==================== FUNCIÓN PRINCIPAL ====================
int main() {
    setcolor(6);
    printf("Iniciando Sistema de Gestión de Olimpiadas de Invierno...\n");
    setcolor(0);
    
    if (!conectar_bd()) {
        setcolor(1);
        printf("No se pudo conectar a la base de datos. Terminando programa.\n");
        setcolor(0);
        return 1;
    }
    
    menu_principal();
    
    desconectar_bd();
    
    setcolor(2);
    printf("Sistema terminado correctamente.\n");
    setcolor(0);
    
    return 0;
}
