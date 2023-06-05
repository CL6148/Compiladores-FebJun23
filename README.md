### Proyecto Final - Diseño de Compiladores
### Carlos Andrés Luna Leyva A00826148
---

La finalidad de este proyecto es crear un lenguaje orientado a objetos con las funcionalidades básicas de cualquier lenguaje de programación tal como estatutos lineales, expresiones matemáticas, módulos locales y globales, y estructuras de datos sencillas. Para el aspecto de objetos se busca desarrollar el mecanismo de atributos sencillos.

El proyecto está diseñado para ser ejecutado por medio de Flex y Bison a base del lenguaje C.

---

### Instrucciones de Ejecución:
Dentro del proyecto se encuentra el archivo 'compile.bat' para la compilación de los archivos del lexer y parser, pero de igual manera se pueden ejecutar los siguientes comandos de manera individual para la compilación.

    ./compile.bat

o

	bison -d parser.y
	flex scanner.l
	gcc -o main parser.tab.c lex.yy.c

Una vez compilado, se ejecuta el archivo main.exe con el archivo de texto con el código a ejecutar.

    ./main {Archivo_de_Prueba.extension}

---
## v0.3.5: Verificación de Arreglos y Matrices
|  | Archivo | Descripción |
| - | - | - |
| M | parser.y | + Generación de cuadruplos para estatutos lineales |
| M | semantics.c | M Códigos y rutas operadores lineales |
| M | intermediate.c | + Separación generación de cuadruplos con espacios |

- Pendiente insertar tabla de variables a estructura global/local permanente
- Pendiente asignar dirección en memoria virtual a tabla de variables
- Pendiente indexación de variables en cuadruplos por medio de memoria virtual
- Pendiente impresión de cuadruplos a un archivo central (no consola)

## v0.3.4: Verificación de Arreglos y Matrices
|  | Archivo | Descripción |
| - | - | - |
| M | scanner.l | M Limpieza de tokens no utilizados |
| M | parser.y | + Generación de cuadruplos para expresiones aritmeticas |
| | | + Generación de cuadruplos para expresiones lógicas |
| | | + Lógica semántica al leer variables y busqueda en tabla de símbolos |
| M | semantics.c | M Limpieza de rutas |
| M | intermediate.c | + Método para generar cuadruplos |
| | | + Método para regresar símbolo operador utilizando código entero |

- Pendiente insertar tabla de variables a estructura global/local permanente
- Pendiente asignar dirección en memoria virtual a tabla de variables
- Pendiente indexación de variables en cuadruplos por medio de memoria virtual
- Pendiente impresión de cuadruplos a un archivo central (no consola)

## v0.3.3: Verificación de Arreglos y Matrices
|  | Archivo | Descripción |
| - | - | - |
| M | parser.y | M Verificación lectura de arreglos/matrices y sus dimensiones/rangos |
| | | + Códigos de error diferentes para semantica de arreglos/matrices |
| M | symtab.c | M cambio de estructura a struct |

- Pendiente insertar tabla de variables a estructura global/local permanente
- Pendiente asignar dirección en memoria virtual a tabla de variables
- Pendiente lectura general de variables para generación de cuádruplos


## v0.3.2: Modificación Tabla de Símbolos
|  | Archivo | Descripción |
| - | - | - |
| M | parser.y | M reflejar cambio a addVar |
| M | symtab.c | M cambio de estructura a struct |
| M | stack.c | M cambio a impresión de pilas |

- Pendiente insertar tabla de variables a estructura global/local permanente
- Pendiente asignar dirección en memoria virtual a tabla de variables
- Pendiente lectura general de variables para generación de cuádruplos

## v0.3.1: Generación Cuádruplos (Expresiones Aritméticas)
|  | Archivo | Descripción |
| - | - | - |
| M | scanner.l | M lectura de valores float
| M | parser.y | + #include "intermediate.c" |
| | | + push a pila de operadores y operaciones |
| M | symtab.c | M cambio de estructura a pilas (parcial) |
| M | semantics.c | + códigos int para cada operación especificados |
| + | stack.c | Archivo con estructura y metodos para la creación y manipulación de pilas |
| + | intermediate.c | Archivo con pilas de ejecución |

- Pendiente modificar estructura de tabla de variables a pilas
- Pendiente insertar tabla de variables a estructura global/local permanente
- Pendiente asignar dirección en memoria virtual a tabla de variables
- Pendiente lectura general de variables para generación de cuádruplos

## v0.2.2: Tabla de Variables
|  | Archivo | Descripción |
| - | - | - |
| M | parser.y | + #include <symtab.c> |
| | | + Llamada a symtab.c y sus funciones al declarar variables |
| + | symtab.c | Archivo con tabla de variables

- Creacion de Tabla de Variables
- Pendiente modificar estructura de arreglos a pilas para facilitar manipulacion
- Pendiente borrar estructura temporal y generar estructura global
- Pendiente generar estructura local para funciones

## v0.2.1: Consideraciones Semanticas
|  | Archivo | Descripción |
| - | - | - |
| M | parser.y | + #include <semantics.c> |
| + | semantics.c | Archivo con cubo semantico

- Creacion de cubo semantico

## v0.1: Lexer + Parser
|  | Archivo | Descripción |
| - | - | - |
| M | scanner.l |+ yylineno |
| M | parser.y | + yylineno |
| | | + UMINUS
| | | M varcte
| | | - callParam

- Limpieza total de lexer y parser para uso general
- Seguimiento de errores por medio de 'yylineno'
- Separación de 'varcte' a tipos específicos

## v0.0: Esqueleto de Lexer y Parser 'Little Duck'
|  | Archivo | Descripción |
| - | - | - |
| + | scanner.l | Archivo Lexer |
| + | parser.y | Archivo Parser |
| + | compile.bat | Ejecución y compilación de archivos en consola |