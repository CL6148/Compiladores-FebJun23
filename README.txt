*******************************************
* Proyecto Final - Diseño de Compiladores *
*					  *
* Carlos Andrés Luna Leyva		  *
* A00826148				  *
*******************************************

La finalidad de este proyecto es crear un lenguaje orientado a objetos con las funcionalidades básicas de cualquier lenguaje de programación tal como estatutos, expresiones matemáticas, modulos locales y globales, y estructuras de datos sencillas. Para el aspecto de objectos se busca desarrollar mecanismos tal como clases, atributos, métodos, y herencia (1 solo nível).

El proyecto está diseñado para ser ejecutado por medio de Flex y Bison a base del lenguaje C.

*****************************************************************************

Instrucciones de Ejecución:
  Dentro del proyecto se encuentra el archivo 'compile.bat' para la compilación de los archivos del lexer y parser, pero de igual manera se pueden ejecutar los siguientes comandos de manera individual para la compilación.
	./compile.bat

		o

	bison -d parser.y
	flex scanner.l
	gcc -o main parser.tab.c lex.yy.c

  Una vez compilado, se ejecuta el archivo main.exe con el archivo de texto con el código a ejecutar.
	./main {Archivo_de_Prueba.extension}

*****************************************************************************

v0.0: Esqueleto de Lexer y Parser 'Little Duck'
	- scanner.l	:	Archivo Lexer
	- parser.y	: 	Archivo Parser
	- compile.bat	:	Ejecución y compilación de archivos en consola