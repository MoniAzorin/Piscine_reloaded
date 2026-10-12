# Piscine Reloaded - tester

Tests automaticos para ex00-ex27 (sin soluciones, solo comprobaciones).

## Uso

    tar xzf piscine_tester.tar.gz
    cd piscine_tester
    ./run_tests.sh /ruta/a/tu/repo            # todo
    ./run_tests.sh /ruta/a/tu/repo 10 11 24   # solo algunos ejercicios
    ./run_tests.sh /ruta/a/tu/repo --no-asan  # sin sanitizers

El repo debe contener `ex00/ ... ex27/`. Ejecutalo desde fuera del repo: el
tester trabaja en un directorio temporal y no escribe nada en tu repo.

Requisitos: `cc`, `make`, `tar`, `nm`. Opcionales: `norminette` (pip install
norminette), `ifconfig` (Ubuntu: sudo apt install net-tools) y soporte de
AddressSanitizer (se detecta solo; sirve para pillar malloc de menos, overflows
y UB como `i * i` desbordando).

## Que comprueba

Para TODOS los ejercicios
- El directorio exNN/ existe y contiene EXACTAMENTE los archivos pedidos
  (ni faltan ni sobran: a.out, .o, binarios...).

ex00-ex05 (shell)
- ex00: contenido del tar, permisos, enlaces, tamanos, fechas, enlace duro y simbolico.
- ex01-ex05: contenido exacto, `clean` sobre un arbol de prueba (borra/no borra, un solo
  comando), `find_sh.sh`, formato de `MAC.sh`, nombre y contenido ("42", sin \n) de ex05.

ex06-ex21, ex25, ex26 (funciones)
- norminette, compilacion con -Wall -Wextra -Werror, solo funciones permitidas
  (via `nm -u`), sin main(), la funcion esta definida.
- Prototipo: se incluye tu .c tras declarar el prototipo del enunciado; si tus tipos
  difieren (retorno, punteros, const...) el compilador da "conflicting types".
- Tests unitarios con casos limite (INT_MIN/INT_MAX, negativos, NULL, vacio, enormes...).
  Cada caso corre en un proceso aparte, asi que un segfault o un bucle infinito
  (limite 2 s) solo marca ese caso.

ex18, ex19 (programas): salida exacta con 0, 1 y muchos argumentos, argumentos vacios,
con espacios, muy largos, orden ASCII frente a `LC_ALL=C sort`.

ex22, ex23 (cabeceras): ABS con precedencia de operadores, tipos, anidado; el main del
enunciado compila; doble inclusion; campos x e y.

ex24 (Makefile): se prueba con 5 fuentes propias en srcs/ y un includes/ft.h
(la cabecera se llama `ft.h`): genera libft.a en la raiz, simbolos, flags, sin relink,
reconstruye al tocar un .c, clean / fclean / re / all, sin wildcards.

ex27 (display_file): errores por stderr con el texto exacto, archivos de 0 a 1 MB
(limites de buffer, bytes nulos, binarios), sin malloc, Makefile (all/clean/fclean).

## Niveles

- [OK]   correcto
- [KO]   fallo (la Moulinette suspenderia)
- [WARN] caso no definido en el enunciado o aviso (p. ej. 13! con overflow, length
         negativo en ft_foreach, ex13 con bucles). No cuenta como fallo.

## Notas / suposiciones

- ft_print_alphabet / ft_print_numbers: se espera la salida sin salto de linea final.
- ft_strcmp: se compara el signo del resultado con el de strcmp.
- Los mensajes de ex27 deben ir a stderr ("salida reservada" de los errores).
- ft_putchar: se enlaza con un ft_putchar.c propio (como hace la Moulinette); no definas
  el tuyo en el .c si el enunciado lo autoriza.
