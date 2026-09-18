# Práctica 1: vector dinámico de especies

Este documento explica, paso a paso, cómo se ha resuelto la práctica y cómo se relaciona cada parte con el enunciado.

## 1. Objetivo

La práctica pide crear un contenedor genérico llamado `VDinamico<T>`, parecido a un `std::vector`, sin utilizar `std::vector`. Después se usa con objetos `Especie` cargados desde un CSV.

Las clases y archivos importantes son:

| Archivo | Responsabilidad |
| --- | --- |
| `VDinamico.h` | Implementa el vector dinámico genérico. |
| `Especie.h` | Representa una especie de árbol. |
| `LectorCSV.cpp` | Lee el fichero CSV y añade especies al vector. |
| `main.cpp` | Ejecuta todas las pruebas que exige el enunciado. |

`VDinamico` es una plantilla, por eso se implementa por completo en el fichero `.h`: el compilador necesita ver su código al crear, por ejemplo, un `VDinamico<Especie>` o un `VDinamico<Especie*>`.

## 2. Estructura interna de `VDinamico<T>`

La clase guarda tres datos privados:

```cpp
T* datos_;                 // Array dinámico donde se guardan los elementos.
unsigned int tamFisico_;   // Capacidad reservada.
unsigned int tamLogico_;   // Número de elementos realmente almacenados.
```

El tamaño lógico siempre cumple `tamLogico_ <= tamFisico_`. Por ejemplo, un vector con capacidad 8 y tres especies almacenadas tiene tamaño físico 8 y lógico 3.

La capacidad se mantiene como potencia de dos (`1`, `2`, `4`, `8`, ...). Esto permite ampliar el vector sin reservar memoria en cada inserción.

## 3. Constructores, copia y destructor

### Constructor por defecto

```cpp
VDinamico() : datos_(new T[1]), tamFisico_(1), tamLogico_(0) {}
```

Reserva sitio para un elemento y deja el vector vacío. Cumple el requisito de tamaño físico inicial 1 y tamaño lógico 0.

### Constructor con tamaño inicial

```cpp
VDinamico(unsigned int tamlog, const T& dato)
```

Calcula la primera potencia de dos que puede albergar `tamlog` elementos y rellena esas posiciones lógicas con `dato`.

### Constructor de copia

```cpp
VDinamico(const VDinamico& origen)
```

Reserva memoria propia y copia los elementos del vector origen. Es importante que no copie solo el puntero: de ese modo los dos vectores son independientes y no liberan la misma memoria dos veces.

### Constructor de copia parcial

```cpp
VDinamico(const VDinamico& origen,
          unsigned int posicionInicial,
          unsigned int numElementos)
```

Primero comprueba que el intervalo solicitado pertenece al vector origen. Después reserva la capacidad necesaria y copia desde `posicionInicial` exactamente `numElementos` elementos.

### Operador de asignación

```cpp
VDinamico& operator=(VDinamico otro)
```

Usa la técnica *copy-and-swap*: recibe una copia del origen e intercambia sus punteros y tamaños con el objeto actual. Al terminar, el parámetro temporal se destruye y libera la memoria antigua. Esto evita fugas y también funciona correctamente en una autoasignación (`v = v`).

### Destructor

```cpp
~VDinamico() { delete[] datos_; }
```

Libera el array que fue reservado con `new[]`. Sin este destructor se produciría una fuga de memoria.

## 4. Acceso y control de errores

El operador `[]` se ofrece en dos versiones:

```cpp
T& operator[](unsigned int pos);
const T& operator[](unsigned int pos) const;
```

Antes de acceder se llama a `comprobarPosicion`. Si `pos >= tamLogico_`, se lanza `std::out_of_range`. La versión constante permite leer desde vectores constantes; la otra permite leer y modificar.

También se validan:

- Borrado de un vector vacío.
- Inserciones fuera del intervalo válido.
- Rangos inválidos en la copia parcial.
- Desbordamiento al duplicar la capacidad.

## 5. Inserción y aumento de capacidad

El método pedido es:

```cpp
void insertar(const T& dato, unsigned int pos = UINT_MAX);
```

`UINT_MAX` actúa como valor especial: significa «insertar al final». Si se recibe otra posición, debe estar entre `0` y `tamLogico_`, ambos incluidos para permitir insertar justo después del último elemento.

Antes de insertar se llama a `asegurarCapacidad(tamLogico_ + 1)`. Cuando no hay hueco:

1. Se calcula la siguiente potencia de dos necesaria.
2. Se reserva un array nuevo.
3. Se copian los elementos existentes.
4. Se libera el array antiguo.
5. Se sustituye el puntero y se actualiza la capacidad.

Después se desplazan una posición a la derecha los elementos desde el final hasta `pos`, se escribe el dato y se incrementa el tamaño lógico.

## 6. Borrado

```cpp
T borrar(unsigned int pos = UINT_MAX);
```

También usa `UINT_MAX` para indicar el último elemento. Guarda una copia del elemento que se va a devolver, desplaza hacia la izquierda los que están después de él y decrementa `tamLogico_`.

El desplazamiento hace que el coste sea `O(n)` en el peor caso, que es lo solicitado en el enunciado.

## 7. Ordenación y búsqueda binaria

### Ordenación

```cpp
void ordenar() { std::sort(datos_, datos_ + tamLogico_); }
```

Se ordena únicamente el intervalo lógico. `std::sort` necesita que el tipo almacenado tenga `operator<`.

### Búsqueda dicotómica

```cpp
int busquedaDicotomica(const T& dato) const;
```

La búsqueda trabaja sobre un intervalo semiabierto `[inicio, fin)`. En cada paso compara el elemento central con el buscado y descarta la mitad que no puede contenerlo. Por eso su coste es `O(log n)`.

Devuelve el índice si lo encuentra y `-1` si no existe. Se debe llamar después de ordenar el vector.

## 8. Clase `Especie`

`Especie` contiene los cuatro campos del CSV:

```cpp
std::string codigoEspecie_;
std::string nombreComun_;
std::string nombreCientifico_;
std::string tipoPlanta_;
```

Incluye constructores, getters y setters. Los operadores se han definido por código de especie:

```cpp
bool operator<(const Especie& otra) const {
    return codigoEspecie_ < otra.codigoEspecie_;
}

bool operator==(const Especie& otra) const {
    return codigoEspecie_ == otra.codigoEspecie_;
}
```

Así, `ordenar()` ordena por `codigoEspecie` y `busquedaDicotomica(Especie("CTA", "", "", ""))` busca por ese código, sin necesitar rellenar los otros campos.

`mostrarInfo()` centraliza la salida de una especie por pantalla.

## 9. Lectura del CSV

`LectorCSV::cargar` abre `data/arbolado-especies.csv`, descarta la cabecera y procesa cada fila válida.

Para cada línea:

1. Comprueba que contiene cuatro campos.
2. Extrae código, nombre común, nombre científico y tipo de planta.
3. Construye un `Especie` con esos valores.
4. Lo inserta al final de `VDinamico<Especie>`.

El lector tiene en cuenta comillas y comas dentro de los campos. Además, elimina un posible BOM UTF-8 de la primera línea.

## 10. Filtro de especies con nombre común

La función del enunciado es:

```cpp
VDinamico<Especie*> getEspNComun(VDinamico<Especie>& vEspecies)
```

Se recorre el vector original. Cuando `getnombre_comun()` no devuelve una cadena vacía, se inserta la dirección del objeto existente:

```cpp
if (!vEspecies[i].getnombre_comun().empty()) {
    vectorEspNComun.insertar(&vEspecies[i]);
}
```

No se crean copias de `Especie`; el nuevo vector solo contiene punteros. Por eso cumple el requisito de apuntar a los objetos del vector original.

El vector original no debe volver a redimensionarse después de crear esos punteros, porque una redimensión movería los objetos a otra zona de memoria y los punteros dejarían de ser válidos. En `main.cpp` el filtro se crea al final, cuando ya se ha cargado y ordenado todo.

## 11. Pruebas realizadas en `main.cpp`

El flujo del programa es:

1. Muestra el directorio de trabajo actual.
2. Carga el CSV en `VDinamico<Especie>`.
3. Muestra los códigos de las primeras 50 especies leídas.
4. Ordena el vector y muestra las primeras 50 especies ordenadas.
5. Busca `CTA`, `DMD`, `HCN`, `NDOF` y `JAX` mediante búsqueda binaria.
6. Construye el vector de punteros de especies con nombre común.
7. Muestra cuántas hay y los primeros 50 resultados.

Los bucles de impresión comprueban también el tamaño lógico. Esto evita acceder fuera de rango si el CSV tuviera menos de 50 registros.

## 12. Compilación y ejecución en CLion

1. Abrir la carpeta del proyecto en CLion.
2. Esperar a que CMake cargue la configuración `codigo_prueba`.
3. Seleccionar `codigo_prueba` arriba a la derecha.
4. Ejecutar con el botón verde o con `Shift + F10`.
5. Comprobar que el *Working directory* es la raíz del proyecto:

   ```text
   C:\Users\admin\Downloads\codigo_prueba
   ```

El directorio de trabajo es importante porque la ruta del CSV es relativa:

```cpp
const string RUTA_FICHERO_ESPECIES = "data/arbolado-especies.csv";
```

## 13. Comprobación final

Se ha compilado el proyecto con C++14 y se han validado carga del CSV, ordenación, búsqueda binaria y filtrado de nombres comunes. La prueba completa finaliza correctamente.
