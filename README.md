# EdaCal

EdaCal es una calculadora interactiva en C++11 que implementa la Tarea 3 de Estructuras de Datos y Algoritmos. El programa ofrece un REPL con soporte completo para expresiones aritméticas, variables, conversión a posfija mediante Shunting-yard, evaluación con pila y generación de árboles de expresión.

## Requisitos

- `g++` con soporte para C++11.
- `make`.

## Compilación y ejecución

```bash
make
./EdaCal
```

Comandos útiles del `Makefile`:

- `make` o `make all`: compila el binario `EdaCal`.
- `make run`: compila y ejecuta `./EdaCal`.
- `make clean`: elimina el ejecutable y archivos intermedios.

## Uso básico

Ejemplo de sesión:

```
>> 6 + 5
>> ans -> 11
>> x = 7 * ans
>> x -> 77
>> x + ans
>> ans -> 88
>> tree
...
>> posfix
...
>> prefix
...
```

Características destacadas:

- Expresiones con `+ - * / ^ sqrt()`.
- Unario negativo (`-5`, `-ans`).
- Variables con asignación `nombre = expresion`.
- Símbolo especial `ans` actualizado tras cada evaluación.
- Árbol de expresión ASCII (`tree`), notación posfija (`posfix` / `postfix`) y prefija (`prefix`).
- Manejo robusto de errores: variables indefinidas, divisiones por cero, paréntesis desbalanceados, `sqrt` inválidos.

## Script de prueba

Se incluye `tests/script.txt` con un recorrido rápido de funcionalidades y errores representativos. Para ejecutarlo:

```bash
./EdaCal < tests/script.txt
```

## Diseño

- **Tokenización** mediante `Tokenizer`, que produce una `LinkedList<Token>`.
- **Listas enlazadas y pilas** implementadas manualmente (`linked_list`, `stack`).
- **Conversión a posfija** con Shunting-yard (`Parser::toPostfix`).
- **Evaluación** desde posfija con una pila (`Evaluator`).
- **Árbol de expresión** construido desde posfija (`Parser::buildTreeFromPostfix`) y recorrido por `Printer`.
- **Tabla de símbolos** (`SymbolTable`) basada en `std::unordered_map`, con `ans` permanente.

Toda la lógica se encuentra en `src/` organizada en módulos `.hpp/.cpp` tal como se solicita en la tarea.
