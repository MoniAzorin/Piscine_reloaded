#!/usr/bin/env bash
# =============================================================================
#  Piscine Reloaded - tester
#  Uso: ./run_tests.sh [ruta/al/repo] [N ...] [--no-asan]
#  Ejecutalo desde FUERA de tu repo (o indicando la ruta) para no ensuciarlo.
# =============================================================================

TESTER_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="."
FILTER=()
SAN_MODE=auto

usage() {
  cat <<'EOF'
Uso: ./run_tests.sh [ruta/al/repo] [N ...] [--no-asan]

  ruta/al/repo   carpeta que contiene ex00/ ... ex27/ (por defecto: la actual)
  N              ejercicios a probar, p. ej.:  ./run_tests.sh ~/piscine 10 11 24
  --no-asan      no compilar los tests con AddressSanitizer/UBSan
EOF
}

for a in "$@"; do
  case "$a" in
    -h|--help) usage; exit 0 ;;
    --no-asan) SAN_MODE=off ;;
    --asan) SAN_MODE=on ;;
    [0-9]|[0-9][0-9]|ex[0-9]|ex[0-9][0-9]) n=${a#ex}; FILTER+=("$((10#$n))") ;;
    *) if [ -d "$a" ]; then REPO="$a"; else echo "Argumento desconocido o ruta inexistente: $a"; exit 2; fi ;;
  esac
done
REPO="$(cd "$REPO" && pwd)"

if [ -t 1 ]; then
  RED=$'\033[31m'; GRN=$'\033[32m'; YLW=$'\033[33m'; BLD=$'\033[1m'; DIM=$'\033[2m'; RST=$'\033[0m'; export PT_COLOR=1
else
  RED=""; GRN=""; YLW=""; BLD=""; DIM=""; RST=""; export PT_COLOR=0
fi

TMP="$(mktemp -d 2>/dev/null || mktemp -d -t piscine_tester)"
cleanup() { chmod -R u+rwx "$TMP" 2>/dev/null; rm -rf "$TMP"; }
trap cleanup EXIT
trap 'exit 130' INT TERM

PASS=0; FAIL=0; WARN=0; NORM_WARNED=0
MARVIN="\"\\?\$*'MaRViN'*\$?\\\""
TIMEOUT_BIN="$(command -v timeout || command -v gtimeout || true)"
CT_FLAGS=()
export ASAN_OPTIONS=detect_leaks=0:exitcode=77
export UBSAN_OPTIONS=print_stacktrace=0

# ---------------------------------------------------------------- utilidades --
section() { printf '\n%s== %s ==%s\n' "$BLD" "$1" "$RST"; }
indent()  { local l; for l in "$@"; do [ -n "$l" ] && printf '%s\n' "$l" | sed 's/^/         /'; done; }
ok()      { PASS=$((PASS+1)); printf '  %s[OK]%s   %s\n' "$GRN" "$RST" "$1"; }
ko()      { FAIL=$((FAIL+1)); printf '  %s[KO]%s   %s\n' "$RED" "$RST" "$1"; shift; indent "$@"; }
warn()    { WARN=$((WARN+1)); printf '  %s[WARN]%s %s\n' "$YLW" "$RST" "$1"; shift; indent "$@"; }
info()    { printf '  %s[i]%s    %s\n' "$DIM" "$RST" "$1"; }
mtime()   { stat -c %Y "$1" 2>/dev/null || stat -f %m "$1" 2>/dev/null; }
run_to()  { if [ -n "$TIMEOUT_BIN" ]; then "$TIMEOUT_BIN" 10 "$@"; else "$@"; fi; }
show()    { if [ -s "$1" ]; then od -An -c "$1" | head -n 3 | sed 's/^ *//'; else echo "(vacio)"; fi; }

want() {
  local n=$((10#$1)) f
  [ ${#FILTER[@]} -eq 0 ] && return 0
  for f in "${FILTER[@]}"; do [ "$f" -eq "$n" ] && return 0; done
  return 1
}

# Simbolos no definidos (funciones externas usadas) de un .o / binario
undefined_syms() {
  local raw
  raw=$(nm -u "$1" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//')
  if [ "$(uname)" = "Darwin" ]; then raw=$(printf '%s\n' "$raw" | sed 's/^_//'); fi
  printf '%s\n' "$raw" | grep -v '^_' | grep -v '^$' | grep -v '^dyld_stub_binder$' | sort -u
}

# forbidden_check <fichero.o|binario> "<permitidas separadas por espacios>"
forbidden_check() {
  local s bad=""
  for s in $(undefined_syms "$1"); do
    case " $2 " in *" $s "*) ;; *) bad="$bad $s" ;; esac
  done
  if [ -z "$bad" ]; then ok "solo usa funciones permitidas (${2:-ninguna})"
  else ko "funciones NO permitidas:$bad" "permitidas: ${2:-ninguna}"; fi
}

run_norm() {
  local soft=0 out det
  if [ "$1" = "--soft" ]; then soft=1; shift; fi
  if ! command -v norminette >/dev/null 2>&1; then
    if [ $NORM_WARNED -eq 0 ]; then info "norminette no esta instalada: se omite (pip install norminette)"; NORM_WARNED=1; fi
    return
  fi
  out=$(norminette "$@" 2>&1)
  if printf '%s\n' "$out" | grep -q 'Error'; then
    det=$(printf '%s\n' "$out" | grep -v ': OK!' | head -n 6)
    if [ $soft -eq 1 ]; then warn "norminette (el enunciado lo permite en este ejercicio)" "$det"
    else ko "norminette" "$det"; fi
  else
    ok "norminette"
  fi
}

# check_layout <NN> <nombres esperados...>  -> 1 si falta el dir o algun fichero
check_layout() {
  local n=$1; shift
  local dir="$REPO/ex$n" missing=0 name entry base found e
  local extras=()
  if [ ! -d "$dir" ]; then ko "no existe el directorio ex$n/"; return 1; fi
  ok "directorio ex$n/ existe"
  for name in "$@"; do
    if [ ! -e "$dir/$name" ] && [ ! -L "$dir/$name" ]; then ko "falta el archivo: $name"; missing=1; fi
  done
  while IFS= read -r -d '' entry; do
    base=${entry##*/}; found=0
    for e in "$@"; do [ "$base" = "$e" ] && { found=1; break; }; done
    [ $found -eq 0 ] && extras+=("$base")
  done < <(find "$dir" -mindepth 1 -maxdepth 1 -print0)
  if [ ${#extras[@]} -gt 0 ]; then ko "archivos NO pedidos en ex$n/ (la Moulinette lo penaliza)" "${extras[*]}"; fi
  [ $missing -eq 0 ] && [ ${#extras[@]} -eq 0 ] && ok "contenido de ex$n/ exacto: $*"
  return $missing
}

# ------------------------------------------------------- sanitizers y setup ---
SANFLAGS=""
if [ "$SAN_MODE" != off ]; then
  echo 'int main(void){return 0;}' > "$TMP/probe.c"
  if cc -fsanitize=address,undefined "$TMP/probe.c" -o "$TMP/probe" 2>/dev/null && "$TMP/probe" 2>/dev/null; then
    SANFLAGS="-fsanitize=address,undefined"
  elif [ "$SAN_MODE" = on ]; then
    echo "AddressSanitizer no disponible en este sistema"; exit 2
  fi
fi
cc -c "$TESTER_DIR/support/ft_putchar.c" -o "$TMP/ft_putchar.o" || { echo "no compila support/ft_putchar.c"; exit 2; }

# -------------------------------------------------------- tests unitarios C ---
# run_c_test <NN> <fuentes del alumno...>   (usa CT_FLAGS para -I extra)
run_c_test() {
  local n=$1; shift
  local bin="$TMP/test_ex$n" out res s
  if ! cc $SANFLAGS -g -w -include "$TESTER_DIR/moulinette.h" -I "$TESTER_DIR" "${CT_FLAGS[@]}" \
       "$@" "$TESTER_DIR/c/ex$n.c" "$TESTER_DIR/support/ft_putchar.c" -o "$bin" 2>"$TMP/build.err"; then
    ko "no se pudo compilar/enlazar el test unitario" "$(head -n 6 "$TMP/build.err")"; return
  fi
  out=$("$bin" 2>"$TMP/san.err")
  printf '%s\n' "$out" | grep -v '^@@RESULT'
  res=$(printf '%s\n' "$out" | grep '^@@RESULT' | tail -n 1)
  if [ -z "$res" ]; then
    ko "el test termino de forma inesperada"
  else
    set -- $res
    PASS=$((PASS+$2)); FAIL=$((FAIL+$3)); WARN=$((WARN+$4))
  fi
  if [ -s "$TMP/san.err" ]; then
    s=$(grep -E 'ERROR: |runtime error|SUMMARY' "$TMP/san.err" | sort -u | head -n 6)
    if [ -n "$s" ]; then warn "avisos del sanitizer (memoria / comportamiento indefinido)" "$s"; fi
  fi
}

# test_c_func <NN> <fichero.c> "<funciones permitidas>" '<prototipo;>'
test_c_func() {
  local n=$1 file=$2 allowed=$3 proto=$4
  local fn="${file%.c}" src="$REPO/ex$n/$file" obj="$TMP/ex$n.o"
  section "ex$n - $fn"
  check_layout "$n" "$file" || return
  run_norm "$src"
  if ! cc -Wall -Wextra -Werror -include "$TESTER_DIR/moulinette.h" -c "$src" -o "$obj" 2>"$TMP/cc.err"; then
    ko "compila con -Wall -Wextra -Werror" "$(head -n 6 "$TMP/cc.err")"; return
  fi
  ok "compila con -Wall -Wextra -Werror"
  forbidden_check "$obj" "$allowed"
  local has_main=0
  if nm "$obj" 2>/dev/null | grep -Eq ' T _?main$'; then ko "no debe haber main() (no es un programa)"; has_main=1; else ok "sin main()"; fi
  if nm "$obj" 2>/dev/null | grep -Eq " T _?${fn}\$"; then ok "define $fn"; else ko "no define la funcion $fn"; return; fi
  printf '#include "moulinette.h"\n%s\n#include "%s"\n' "$proto" "$src" > "$TMP/proto.c"
  if cc -Wall -Wextra -Werror -I "$TESTER_DIR" -c "$TMP/proto.c" -o "$TMP/proto.o" 2>"$TMP/proto.err"; then
    ok "prototipo correcto: $proto"
  else
    ko "prototipo INCORRECTO, debe ser: $proto" "$(grep -E 'error' "$TMP/proto.err" | head -n 3)"
    info "tests unitarios omitidos"; return
  fi
  if [ $has_main -eq 1 ]; then info "tests unitarios omitidos (el main() sobrante choca con el del test)"; return; fi
  case "$n" in
    12) if [ "$(grep -v '^/\*' "$src" | grep -c 'ft_iterative_factorial')" -gt 1 ]; then
          warn "ft_iterative_factorial parece llamarse a si misma: el enunciado pide una version iterativa"; fi ;;
    13) if grep -v '^/\*' "$src" | grep -Eqw 'while|for'; then
          warn "ex13 usa bucles: el enunciado pide una funcion recursiva"; fi ;;
  esac
  run_c_test "$n" "$src"
}

# ----------------------------------------------- programas (ex18, ex19) -------
expect_file() { # desc fichero_esperado bin args...
  local desc=$1 expf=$2 bin=$3 rc; shift 3
  run_to "$bin" "$@" >"$TMP/got.out" 2>"$TMP/got.err"; rc=$?
  if [ $rc -eq 77 ]; then
    ko "$desc" "error de memoria (sanitizer):" "$(grep -E 'ERROR|SUMMARY' "$TMP/got.err" | head -n 3)"
  elif [ $rc -ge 124 ]; then
    ko "$desc" "el programa termino con codigo $rc (timeout 124 / senal 128+N)"
  elif cmp -s "$expf" "$TMP/got.out"; then
    ok "$desc"
  else
    ko "$desc" "esperado: $(show "$expf")" "obtenido: $(show "$TMP/got.out")"
  fi
}
expect_prog() { # desc "texto %b" bin args...
  local desc=$1 exp=$2; shift 2
  printf '%b' "$exp" > "$TMP/exp.out"
  expect_file "$desc" "$TMP/exp.out" "$@"
}
expect_sorted() { # desc bin args...
  local desc=$1 bin=$2; shift 2
  if [ $# -eq 0 ]; then : > "$TMP/exp.sorted"; else printf '%s\n' "$@" | LC_ALL=C sort > "$TMP/exp.sorted"; fi
  expect_file "$desc" "$TMP/exp.sorted" "$bin" "$@"
}

test_c_prog() {
  local n=$1 file=$2 fn="${2%.c}"
  local src="$REPO/ex$n/$file" obj="$TMP/ex$n.o" bin="$TMP/prog$n"
  section "ex$n - $fn (programa)"
  check_layout "$n" "$file" || return
  run_norm "$src"
  if ! cc -Wall -Wextra -Werror -include "$TESTER_DIR/moulinette.h" -c "$src" -o "$obj" 2>"$TMP/cc.err"; then
    ko "compila con -Wall -Wextra -Werror" "$(head -n 6 "$TMP/cc.err")"; return
  fi
  ok "compila con -Wall -Wextra -Werror"
  forbidden_check "$obj" "ft_putchar"
  if nm "$obj" 2>/dev/null | grep -Eq ' T _?main$'; then ok "tiene main()"; else ko "falta main() (es un programa)"; return; fi
  if ! cc $SANFLAGS -g -w -include "$TESTER_DIR/moulinette.h" "$src" "$TMP/ft_putchar.o" -o "$bin" 2>"$TMP/build.err"; then
    ko "no se pudo enlazar con el ft_putchar de la Moulinette" "$(head -n 4 "$TMP/build.err")"; return
  fi
  local big words i w
  big=$(head -c 10000 /dev/zero | tr '\0' 'x')
  if [ "$n" = "18" ]; then
    expect_prog "test1 test2 test3" 'test1\ntest2\ntest3\n' "$bin" test1 test2 test3
    expect_prog "sin argumentos -> no imprime nada" '' "$bin"
    expect_prog "un solo argumento" 'solo\n' "$bin" solo
    expect_prog "argumentos con espacios" 'hello world\nx y\n' "$bin" "hello world" "x y"
    expect_prog "argumento vacio -> linea vacia" '\na\n' "$bin" "" a
    expect_prog "argumentos raros: -n, --help, *, \$HOME, %s" '-n\n--help\n*\n$HOME\n%s\n' "$bin" -n --help '*' '$HOME' '%s'
    words=(); i=0; while [ $i -lt 100 ]; do words+=("arg$i"); i=$((i+1)); done
    printf '%s\n' "${words[@]}" > "$TMP/exp.many"
    expect_file "100 argumentos" "$TMP/exp.many" "$bin" "${words[@]}"
    expect_prog "argumento de 10000 caracteres" "$big\n" "$bin" "$big"
    expect_prog "no imprime argv[0] (solo los argumentos)" 'a\n' "$bin" a
  else
    expect_sorted "c b a" "$bin" c b a
    expect_sorted "sin argumentos -> no imprime nada" "$bin"
    expect_sorted "un solo argumento" "$bin" solo
    expect_sorted "orden ASCII (mayusculas antes que minusculas)" "$bin" B a A b
    expect_sorted "orden ASCII, no numerico (10 < 2 < 9)" "$bin" 10 9 2 1
    expect_sorted "prefijos y cadena vacia" "$bin" abc ab a "" abcd
    expect_sorted "duplicados" "$bin" b a b a c c
    expect_sorted "ya ordenado" "$bin" a b c d e
    expect_sorted "orden inverso" "$bin" e d c b a
    expect_sorted "bytes > 127 (comparacion unsigned)" "$bin" $'\xc3\xa9' z a Z
    expect_sorted "argumentos con espacios" "$bin" "b c" "a b" "a"
    words=(); i=0
    while [ $i -lt 60 ]; do
      w=$(head -c 12 /dev/urandom | base64 | tr -d '/+=\n' | cut -c1-$((RANDOM%9+1)))
      words+=("$w"); i=$((i+1))
    done
    expect_sorted "60 argumentos aleatorios" "$bin" "${words[@]}"
  fi
}

# --------------------------------------------------------------- ex00 - ex05 --
ls_check() { # name mode links size mon day time   ("-" = ignorar)
  local name=$1 mode=$2 links=$3 size=$4 mon=$5 day=$6 tm=$7
  local line m l s mo d t p=""
  line=$(LC_ALL=C ls -ld "$W/$name" 2>/dev/null)
  m=$(printf '%s' "$line" | awk '{print substr($1,1,10)}')
  l=$(printf '%s' "$line" | awk '{print $2}')
  s=$(printf '%s' "$line" | awk '{print $5}')
  mo=$(printf '%s' "$line" | awk '{print $6}')
  d=$(printf '%s' "$line" | awk '{print $7}')
  t=$(printf '%s' "$line" | awk '{print $8}')
  if [ "$name" = "test6" ] && [ "$(uname)" = "Darwin" ]; then
    [ "${m:0:1}" = "l" ] || p="$p tipo=${m:0:1}(esperado l);"
  else
    [ "$m" = "$mode" ] || p="$p permisos=$m (esperado $mode);"
  fi
  [ "$links" = "-" ] || [ "$l" = "$links" ] || p="$p enlaces=$l (esperado $links);"
  [ "$size" = "-" ] || [ "$s" = "$size" ] || p="$p tamano=$s (esperado $size);"
  [ "$mo $d" = "$mon $day" ] || p="$p fecha=$mo $d (esperado $mon $day);"
  if printf '%s' "$t" | grep -Eq '^[0-9]{2}:[0-9]{2}$'; then
    if [ "$t" != "$tm" ]; then
      if [ "$name" = "test6" ]; then warn "test6: hora=$t (esperada $tm); algunos tar no restauran la hora de los enlaces simbolicos"
      else p="$p hora=$t (esperada $tm);"; fi
    fi
  fi
  if [ -z "$p" ]; then ok "$name: $mode ${size/#-/} $mon $day $tm" ; else ko "$name:$p"; fi
}

test_ex00() {
  section "ex00 - Dame mas... (exo.tar)"
  check_layout 00 exo.tar || return
  local tarf="$REPO/ex00/exo.tar" got
  W="$TMP/ex00"; rm -rf "$W"; mkdir -p "$W"
  got=$(tar -tf "$tarf" 2>/dev/null | sed 's#^\./##; s#/$##' | grep -v '^\.$' | sort | tr '\n' ' ')
  if [ "$got" = "test0 test1 test2 test3 test4 test5 test6 " ]; then ok "el tar contiene exactamente test0 ... test6"
  else ko "contenido del tar incorrecto" "obtenido: $got" "esperado:  test0 test1 test2 test3 test4 test5 test6"; fi
  if ! (cd "$W" && tar -xpf "$tarf") 2>"$TMP/tar.err"; then ko "no se puede extraer el tar" "$(head -n 3 "$TMP/tar.err")"; return; fi
  ls_check test0 drwx--xr-x - - Jun 1 20:47
  ls_check test1 -rwx--xr-- 1 4 Jun 1 21:46
  ls_check test2 dr-x---r-- - - Jun 1 22:45
  ls_check test3 -r-----r-- 2 1 Jun 1 23:44
  ls_check test4 -rw-r----x 1 2 Jun 1 23:43
  ls_check test5 -r-----r-- 2 1 Jun 1 23:44
  ls_check test6 lrwxrwxrwx 1 5 Jun 1 22:20
  if [ "$W/test3" -ef "$W/test5" ]; then ok "test3 y test5 son el mismo archivo (enlace duro)"; else ko "test3 y test5 deben ser enlaces duros (mismo inodo)"; fi
  if [ "$(readlink "$W/test6" 2>/dev/null)" = "test0" ]; then ok "test6 es un enlace simbolico a test0"; else ko "test6 debe ser un enlace simbolico a test0"; fi
  info "si ls -l muestra el ano en vez de la hora, la hora no se puede comprobar (el enunciado lo tolera)"
}

test_ex01() {
  section "ex01 - Z"
  check_layout 01 z || return
  if printf 'Z\n' | cmp -s - "$REPO/ex01/z"; then ok "cat z -> \"Z\\n\" exacto"
  else ko "cat z debe devolver exactamente 'Z' + salto de linea" "obtenido: $(show "$REPO/ex01/z")"; fi
}

test_ex02() {
  section "ex02 - clean"
  check_layout 02 clean || return
  local f="$REPO/ex02/clean" T="$TMP/ex02" n x
  if grep -Eq '(^|[^\\]);|&&|\|\|' "$f"; then ko "solo se permite UN comando: nada de ';', '&&' ni '||'"; else ok "sin ';', '&&' ni '||'"; fi
  n=$(grep -Ev '^[[:space:]]*(#.*)?$' "$f" | wc -l | tr -d ' ')
  if [ "$n" -eq 1 ]; then ok "una sola linea de comando"; else warn "hay $n lineas de comando (¿una sola con '\\' al final?)"; fi
  if grep -v '^#!' "$f" | grep -v '||' | grep -q '|'; then warn "usa una tuberia (|): ¿cuenta como un unico comando?"; fi
  rm -rf "$T"; mkdir -p "$T/sub/deep" "$T/dir with space"
  local del=( 'a~' '#b#' 'sub/c~' 'sub/#d#' 'sub/deep/e~' '.hid~' 'my file~' '#my file#' 'dir with space/z~' )
  local keep=( 'keep.txt' '#only_start' 'only_end#' '~start' 'mid~dle' 'sub/keep2' 'sub/#nothash' 'sub/deep/file#' 'a~.txt' )
  for x in "${del[@]}" "${keep[@]}"; do echo x > "$T/$x"; done
  (cd "$T" && sh "$f") > "$TMP/clean.out" 2>&1
  local bad=""
  for x in "${del[@]}"; do [ -e "$T/$x" ] && bad="$bad [$x]"; done
  if [ -z "$bad" ]; then ok "borra todos los *~ y #*# (subdirectorios, ocultos y con espacios)"; else ko "no ha borrado:$bad"; fi
  bad=""
  for x in "${keep[@]}"; do [ -e "$T/$x" ] || bad="$bad [$x]"; done
  if [ -z "$bad" ]; then ok "no borra archivos que no deben borrarse (#x, x#, ~x, a~b...)"; else ko "ha borrado archivos que no tocaba:$bad"; fi
  bad=""
  for x in "${del[@]}"; do grep -qF -- "${x##*/}" "$TMP/clean.out" || bad="$bad [${x##*/}]"; done
  if [ -z "$bad" ]; then ok "muestra los archivos que elimina"; else ko "no muestra por pantalla:$bad" "salida: $(head -c 300 "$TMP/clean.out")"; fi
  (cd "$T" && sh "$f") > "$TMP/clean2.out" 2>&1
  if [ ! -s "$TMP/clean2.out" ]; then ok "segunda ejecucion (nada que borrar): sin salida ni errores"; else warn "segunda ejecucion con salida inesperada" "$(head -c 200 "$TMP/clean2.out")"; fi
}

test_ex03() {
  section "ex03 - find_sh"
  check_layout 03 find_sh.sh || return
  local f="$REPO/ex03/find_sh.sh" T="$TMP/ex03" x
  rm -rf "$T"; mkdir -p "$T/sub/deep"
  for x in 'a.sh' 'b.txt' 'sub/c.sh' 'sub/deep/d.sh' 'x.sh.bak' 'shfile' 'xsh' 'e.SH' 'my script.sh' 'name.with.dots.sh' 'sub/f.txt'; do echo x > "$T/$x"; done
  cp "$f" "$T/find_sh.sh"
  printf '%s\n' a c d find_sh 'my script' name.with.dots | LC_ALL=C sort > "$TMP/exp.f"
  (cd "$T" && sh ./find_sh.sh 2>"$TMP/f.err") | LC_ALL=C sort > "$TMP/got.f"
  if cmp -s "$TMP/exp.f" "$TMP/got.f"; then ok "lista solo los nombres sin ruta y sin .sh (subdirectorios incluidos)"
  else ko "salida incorrecta" "esperado: $(tr '\n' ',' < "$TMP/exp.f")" "obtenido: $(tr '\n' ',' < "$TMP/got.f")" "$(head -n 2 "$TMP/f.err")"; fi
  (cd "$T" && sh ./find_sh.sh 2>/dev/null) > "$TMP/raw.f"
  if [ -s "$TMP/raw.f" ] && [ "$(tail -c1 "$TMP/raw.f" | od -An -c | tr -d ' ')" = '\n' ]; then ok "cada linea termina en salto de linea"; else ko "la salida debe terminar con salto de linea"; fi
}

test_ex04() {
  section "ex04 - MAC"
  check_layout 04 MAC.sh || return
  local f="$REPO/ex04/MAC.sh" bad exp
  sh "$f" > "$TMP/mac.out" 2>"$TMP/mac.err"
  if [ ! -s "$TMP/mac.out" ]; then ko "no imprime ninguna direccion MAC" "$(head -n 3 "$TMP/mac.err")"
    command -v ifconfig >/dev/null 2>&1 || info "ifconfig no esta instalado (Ubuntu: sudo apt install net-tools)"; return; fi
  bad=$(grep -Ev '^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$' "$TMP/mac.out" | head -n 3)
  if [ -z "$bad" ]; then ok "cada linea es una direccion MAC (xx:xx:xx:xx:xx:xx) y nada mas"; else ko "lineas que no son una MAC pura" "$bad"; fi
  if [ "$(tail -c1 "$TMP/mac.out" | od -An -c | tr -d ' ')" = '\n' ]; then ok "cada linea termina en salto de linea"; else ko "la ultima linea no termina en salto de linea"; fi
  if command -v ifconfig >/dev/null 2>&1; then
    exp=$(ifconfig -a 2>/dev/null | awk '{for(i=1;i<=NF;i++) if ($i ~ /^([0-9a-fA-F][0-9a-fA-F]:){5}[0-9a-fA-F][0-9a-fA-F]$/) print tolower($i)}' | sort -u)
    if [ "$(tr 'A-F' 'a-f' < "$TMP/mac.out" | sort -u)" = "$exp" ]; then ok "coincide con las MAC de ifconfig -a"
    else warn "no coincide exactamente con las MAC de ifconfig -a" "esperado: $(printf '%s' "$exp" | tr '\n' ' ')" "obtenido: $(sort -u "$TMP/mac.out" | tr '\n' ' ')"; fi
  else
    info "ifconfig no esta instalado: no se puede comparar con el sistema (Ubuntu: sudo apt install net-tools)"
  fi
}

test_ex05() {
  section "ex05 - ¿Puedes crearlo?"
  check_layout 05 "$MARVIN" || return
  local f="$REPO/ex05/$MARVIN"
  if printf '42' | cmp -s - "$f"; then ok "contiene exactamente \"42\" (2 bytes, sin salto de linea)"
  else ko "debe contener unicamente 42 y NADA mas" "obtenido: $(show "$f")"; fi
}

# --------------------------------------------------------------------- ex22 ---
test_ex22() {
  section "ex22 - ft_abs.h"
  check_layout 22 ft_abs.h || return
  run_norm --soft "$REPO/ex22/ft_abs.h"
  CT_FLAGS=(-I "$REPO/ex22")
  run_c_test 22
  CT_FLAGS=()
}

test_ex23() {
  section "ex23 - ft_point.h"
  check_layout 23 ft_point.h || return
  run_norm "$REPO/ex23/ft_point.h"
  if cc -Wall -Wextra -Werror -I "$REPO/ex23" "$TESTER_DIR/c/ex23_main.c" -o "$TMP/ex23_main" 2>"$TMP/cc.err"; then
    if "$TMP/ex23_main"; then ok "el main del enunciado compila (-Wall -Wextra -Werror) y se ejecuta"; else ko "el main del enunciado devuelve != 0"; fi
  else
    ko "el main del enunciado NO compila con ft_point.h" "$(head -n 6 "$TMP/cc.err")"; return
  fi
  CT_FLAGS=(-I "$REPO/ex23")
  run_c_test 23
  CT_FLAGS=()
}

# --------------------------------------------------------------------- ex24 ---
test_ex24() {
  section "ex24 - Makefile (libft.a)"
  check_layout 24 Makefile || return
  local W="$TMP/ex24" mk out rc f objs m1 m2 m3
  rm -rf "$W"; mkdir -p "$W/srcs" "$W/includes"
  cp "$REPO/ex24/Makefile" "$W/Makefile"
  cat > "$W/includes/ft.h" <<'EOF'
#ifndef FT_H
# define FT_H
void	ft_putchar(char c);
void	ft_putstr(char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(char *str);
void	ft_swap(int *a, int *b);
#endif
EOF
  cat > "$W/srcs/ft_putchar.c" <<'EOF'
#include <unistd.h>
#include "ft.h"
void	ft_putchar(char c)
{
	ssize_t	r;

	r = write(1, &c, 1);
	(void)r;
}
EOF
  cat > "$W/srcs/ft_putstr.c" <<'EOF'
#include "ft.h"
void	ft_putstr(char *str)
{
	while (*str)
		ft_putchar(*str++);
}
EOF
  cat > "$W/srcs/ft_strcmp.c" <<'EOF'
#include "ft.h"
int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
EOF
  cat > "$W/srcs/ft_strlen.c" <<'EOF'
#include "ft.h"
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
EOF
  cat > "$W/srcs/ft_swap.c" <<'EOF'
#include "ft.h"
void	ft_swap(int *a, int *b)
{
	int	t;

	t = *a;
	*a = *b;
	*b = t;
}
EOF
  cat > "$W/main_test.c" <<'EOF'
#include <stdio.h>
#include "ft.h"
int	main(void)
{
	int	a;
	int	b;

	a = 1;
	b = 2;
	ft_putstr("hi");
	ft_putchar('!');
	ft_swap(&a, &b);
	printf("|%d|%d|%d|%d|%d\n", ft_strlen("hello"), ft_strcmp("a", "b") < 0,
		ft_strcmp("b", "a") > 0, ft_strcmp("a", "a"), a * 10 + b);
	return (0);
}
EOF
  mk() { make -C "$W" "$@"; }
  info "se prueba con archivos propios: srcs/{ft_putchar,ft_putstr,ft_strcmp,ft_strlen,ft_swap}.c e includes/ft.h"
  if grep -Eq 'wildcard|\*\.c' "$W/Makefile"; then ko "usa wildcards (*.c / \$(wildcard)): el enunciado los desaconseja"; else ok "sin wildcard ni *.c"; fi
  if grep -Eq '\*\.[oa]' "$W/Makefile"; then warn "usa *.o / *.a en alguna regla (wildcard): mejor listar los ficheros"; fi
  if grep -q '\.PHONY' "$W/Makefile"; then ok ".PHONY presente"; else warn "falta .PHONY (all clean fclean re)"; fi
  out=$(mk 2>&1); rc=$?
  if [ $rc -eq 0 ] && [ -f "$W/libft.a" ]; then ok "make (sin argumentos) genera libft.a en la raiz del ejercicio"
  else ko "make no genera libft.a" "$(printf '%s\n' "$out" | tail -n 8)"; return; fi
  for f in ft_putchar ft_putstr ft_strcmp ft_strlen ft_swap; do
    if nm "$W/libft.a" 2>/dev/null | grep -Eq " T _?$f\$"; then ok "libft.a contiene $f"; else ko "libft.a no contiene $f"; fi
  done
  if cc -Wall -Wextra -Werror -I "$W/includes" "$W/main_test.c" -L"$W" -lft -o "$W/t.bin" 2>"$TMP/cc.err"; then
    if [ "$("$W/t.bin")" = "hi!|5|1|1|0|21" ]; then ok "se puede enlazar con -lft y las funciones funcionan"; else ko "la libreria enlaza pero devuelve resultados incorrectos"; fi
  else
    ko "no se puede enlazar con libft.a" "$(head -n 4 "$TMP/cc.err")"
  fi
  mk fclean >/dev/null 2>&1
  out=$(mk -n 2>&1)
  if printf '%s' "$out" | grep -q -- '-Wall' && printf '%s' "$out" | grep -q -- '-Wextra' && printf '%s' "$out" | grep -q -- '-Werror'; then ok "compila con -Wall -Wextra -Werror"
  else ko "los comandos de compilacion no llevan -Wall -Wextra -Werror" "$(printf '%s\n' "$out" | head -n 3)"; fi
  mk >/dev/null 2>&1
  m1=$(mtime "$W/libft.a"); sleep 1; mk >/dev/null 2>&1; m2=$(mtime "$W/libft.a")
  if [ "$m1" = "$m2" ]; then ok "un segundo make no recompila ni reconstruye la libreria (sin relink)"; else ko "el segundo make vuelve a reconstruir libft.a (relink)"; fi
  sleep 1; touch "$W/srcs/ft_strlen.c"; mk >/dev/null 2>&1; m3=$(mtime "$W/libft.a")
  if [ "$m3" != "$m2" ]; then ok "make detecta un .c modificado y reconstruye"; else ko "make no reconstruye tras modificar un .c"; fi
  mk clean >/dev/null 2>&1; rc=$?
  objs=$(find "$W" -name '*.o' | wc -l | tr -d ' ')
  if [ $rc -eq 0 ] && [ "$objs" -eq 0 ]; then ok "make clean elimina los .o"; else ko "make clean no elimina los .o (quedan $objs) o falla (rc=$rc)"; fi
  if [ -f "$W/libft.a" ]; then ok "make clean conserva libft.a"; else ko "make clean no debe borrar libft.a"; fi
  local lost=""
  for f in srcs/ft_putchar.c srcs/ft_putstr.c srcs/ft_strcmp.c srcs/ft_strlen.c srcs/ft_swap.c includes/ft.h; do [ -f "$W/$f" ] || lost="$lost $f"; done
  if [ -z "$lost" ]; then ok "clean/fclean no tocan srcs/ ni includes/"; else ko "se han borrado archivos fuente:$lost"; fi
  mk clean >/dev/null 2>&1 && ok "make clean repetido no da error" || ko "make clean falla si no hay nada que limpiar"
  mk fclean >/dev/null 2>&1; rc=$?
  if [ $rc -eq 0 ] && [ ! -f "$W/libft.a" ]; then ok "make fclean borra libft.a"; else ko "make fclean no borra libft.a (rc=$rc)"; fi
  mk fclean >/dev/null 2>&1 && ok "make fclean repetido no da error" || ko "make fclean falla si no hay nada que borrar"
  mk all >/dev/null 2>&1
  if [ -f "$W/libft.a" ]; then ok "make all genera libft.a"; else ko "make all no genera libft.a"; fi
  m1=$(mtime "$W/libft.a"); sleep 1; mk re >/dev/null 2>&1; m2=$(mtime "$W/libft.a")
  if [ -f "$W/libft.a" ] && [ "$m1" != "$m2" ]; then ok "make re reconstruye desde cero"; else ko "make re no reconstruye libft.a"; fi
  mk fclean >/dev/null 2>&1; mk re >/dev/null 2>&1
  if [ -f "$W/libft.a" ]; then ok "make re funciona partiendo de un directorio limpio"; else ko "make re falla partiendo de un directorio limpio"; fi
}

# --------------------------------------------------------------------- ex27 ---
chk_msg() { # desc exp_stderr [soft]
  local desc=$1 exp=$2 soft=${3:-0}
  printf '%b' "$exp" > "$TMP/exp.err"
  if cmp -s "$TMP/exp.err" "$TMP/e" && [ ! -s "$TMP/o" ]; then ok "$desc"; return; fi
  local msg1 msg2
  if cmp -s "$TMP/exp.err" "$TMP/o"; then msg1="el mensaje sale por stdout: debe ir a la salida de error (stderr)"; msg2=""
  else msg1="stderr: $(show "$TMP/e")"; msg2="stdout: $(show "$TMP/o")"; fi
  if [ "$soft" = 1 ]; then warn "$desc" "$msg1" "$msg2"; else ko "$desc" "$msg1" "$msg2"; fi
}
chk_file() { # desc file
  run_to "$BIN" "$2" >"$TMP/o" 2>"$TMP/e"
  if cmp -s "$2" "$TMP/o" && [ ! -s "$TMP/e" ]; then ok "$1"
  else ko "$1" "archivo: $(wc -c < "$2" | tr -d ' ') bytes; salida: $(wc -c < "$TMP/o" | tr -d ' ') bytes; stderr: $(wc -c < "$TMP/e" | tr -d ' ') bytes"; fi
}

test_ex27() {
  section "ex27 - display_file"
  local dir="$REPO/ex27" W="$TMP/ex27" D="$TMP/ex27data" left out rc sz m1 m2
  if [ ! -d "$dir" ]; then ko "no existe el directorio ex27/"; return; fi
  ok "directorio ex27/ existe"
  if [ ! -f "$dir/Makefile" ]; then ko "falta el Makefile"; return; fi
  ok "Makefile presente"
  left=$(find "$dir" -type f \( -name '*.o' -o -name ft_display_file -o -name '*.a' -o -name 'a.out' \) 2>/dev/null | head -n 5)
  if [ -z "$left" ]; then ok "sin binarios ni objetos en ex27/"; else ko "binarios/objetos dentro de ex27/ (make fclean antes de entregar)" "$left"; fi
  if grep -rqw 'malloc' "$dir" --include='*.c' --include='*.h' 2>/dev/null; then ko "aparece 'malloc' en el codigo (esta prohibido)"; fi
  run_norm $(find "$dir" -type f \( -name '*.c' -o -name '*.h' \))
  rm -rf "$W" "$D"; mkdir -p "$D"; cp -R "$dir" "$W"
  find "$W" -type f \( -name '*.o' -o -name ft_display_file \) -delete
  out=$(make -C "$W" 2>&1); rc=$?
  BIN="$W/ft_display_file"
  if [ $rc -eq 0 ] && [ -x "$BIN" ]; then ok "make genera el binario ft_display_file"; else ko "make no genera ft_display_file" "$(printf '%s\n' "$out" | tail -n 8)"; return; fi
  forbidden_check "$BIN" "close open read write"
  make -C "$W" fclean >/dev/null 2>&1
  out=$(make -C "$W" -n 2>&1)
  if printf '%s' "$out" | grep -q -- '-Wall' && printf '%s' "$out" | grep -q -- '-Wextra' && printf '%s' "$out" | grep -q -- '-Werror'; then ok "compila con -Wall -Wextra -Werror"
  else ko "los comandos de compilacion no llevan -Wall -Wextra -Werror" "$(printf '%s\n' "$out" | head -n 3)"; fi
  make -C "$W" >/dev/null 2>&1
  [ -x "$BIN" ] || { ko "make no vuelve a generar el binario tras fclean"; return; }

  printf 'Hello 42\nsecond line\n' > "$D/text"
  printf 'no trailing newline' > "$D/nonl"
  : > "$D/empty"
  printf '\n' > "$D/onlynl"
  printf 'ab\0cd\0\0ef' > "$D/nul"
  printf 'con espacios\n' > "$D/with space.txt"
  for sz in 1 1023 1024 1025 4095 4096 4097 65536 65537 1000003; do head -c $sz /dev/urandom > "$D/rnd_$sz"; done

  run_to "$BIN" >"$TMP/o" 2>"$TMP/e";                       chk_msg "sin argumentos -> \"File name missing.\" (stderr)" 'File name missing.\n'
  run_to "$BIN" "$D/text" "$D/nonl" >"$TMP/o" 2>"$TMP/e";   chk_msg "2 argumentos -> \"Too many arguments.\" (stderr)" 'Too many arguments.\n'
  run_to "$BIN" "$D/text" a b c >"$TMP/o" 2>"$TMP/e";       chk_msg "4 argumentos -> \"Too many arguments.\" (stderr)" 'Too many arguments.\n'
  run_to "$BIN" "$D/no_existe" >"$TMP/o" 2>"$TMP/e";        chk_msg "archivo inexistente -> \"Cannot read file.\" (stderr)" 'Cannot read file.\n'
  if [ "$(id -u)" -ne 0 ]; then
    echo secret > "$D/noperm"; chmod 000 "$D/noperm"
    run_to "$BIN" "$D/noperm" >"$TMP/o" 2>"$TMP/e";         chk_msg "archivo sin permisos de lectura -> \"Cannot read file.\"" 'Cannot read file.\n'
  else
    info "ejecutandose como root: se omite la prueba de archivo sin permisos"
  fi
  run_to "$BIN" "$D" >"$TMP/o" 2>"$TMP/e";                  chk_msg "directorio como argumento -> \"Cannot read file.\"" 'Cannot read file.\n' 1

  chk_file "archivo de texto" "$D/text"
  chk_file "sin salto de linea final (no anade ninguno)" "$D/nonl"
  chk_file "archivo vacio (sin salida ni errores)" "$D/empty"
  chk_file "solo un salto de linea" "$D/onlynl"
  chk_file "bytes nulos en medio del archivo" "$D/nul"
  chk_file "nombre de archivo con espacios" "$D/with space.txt"
  for sz in 1 1023 1024 1025 4095 4096 4097 65536 65537 1000003; do chk_file "binario aleatorio de $sz bytes (limites de buffer)" "$D/rnd_$sz"; done

  m1=$(mtime "$BIN"); sleep 1; make -C "$W" >/dev/null 2>&1; m2=$(mtime "$BIN")
  if [ "$m1" = "$m2" ]; then ok "un segundo make no relinkea"; else ko "el segundo make vuelve a enlazar el binario (relink)"; fi
  make -C "$W" clean >/dev/null 2>&1; rc=$?
  if [ $rc -eq 0 ] && [ -z "$(find "$W" -name '*.o' | head -n 1)" ]; then ok "make clean elimina los .o"; else ko "make clean no elimina los .o o falla"; fi
  if [ -x "$BIN" ]; then ok "make clean conserva el binario"; else ko "make clean no debe borrar el binario"; fi
  make -C "$W" fclean >/dev/null 2>&1
  if [ ! -e "$BIN" ]; then ok "make fclean borra el binario"; else ko "make fclean no borra ft_display_file"; fi
  make -C "$W" fclean >/dev/null 2>&1 && ok "make fclean repetido no da error" || ko "make fclean falla si no hay nada que borrar"
  make -C "$W" all >/dev/null 2>&1
  if [ -x "$BIN" ]; then ok "make all funciona"; else ko "make all no genera el binario"; fi
}

# --------------------------------------------------------------------- main ---
printf '%sPiscine Reloaded - tester%s\n' "$BLD" "$RST"
info "repo: $REPO"
info "sanitizers: ${SANFLAGS:-desactivados}"

want 0  && test_ex00
want 1  && test_ex01
want 2  && test_ex02
want 3  && test_ex03
want 4  && test_ex04
want 5  && test_ex05
want 6  && test_c_func 06 ft_print_alphabet.c "ft_putchar" 'void ft_print_alphabet(void);'
want 7  && test_c_func 07 ft_print_numbers.c "ft_putchar" 'void ft_print_numbers(void);'
want 8  && test_c_func 08 ft_is_negative.c "ft_putchar" 'void ft_is_negative(int n);'
want 9  && test_c_func 09 ft_ft.c "" 'void ft_ft(int *nbr);'
want 10 && test_c_func 10 ft_swap.c "" 'void ft_swap(int *a, int *b);'
want 11 && test_c_func 11 ft_div_mod.c "" 'void ft_div_mod(int a, int b, int *div, int *mod);'
want 12 && test_c_func 12 ft_iterative_factorial.c "" 'int ft_iterative_factorial(int nb);'
want 13 && test_c_func 13 ft_recursive_factorial.c "" 'int ft_recursive_factorial(int nb);'
want 14 && test_c_func 14 ft_sqrt.c "" 'int ft_sqrt(int nb);'
want 15 && test_c_func 15 ft_putstr.c "ft_putchar" 'void ft_putstr(char *str);'
want 16 && test_c_func 16 ft_strlen.c "" 'int ft_strlen(char *str);'
want 17 && test_c_func 17 ft_strcmp.c "" 'int ft_strcmp(char *s1, char *s2);'
want 18 && test_c_prog 18 ft_print_params.c
want 19 && test_c_prog 19 ft_sort_params.c
want 20 && test_c_func 20 ft_strdup.c "malloc" 'char *ft_strdup(char *src);'
want 21 && test_c_func 21 ft_range.c "malloc" 'int *ft_range(int min, int max);'
want 22 && test_ex22
want 23 && test_ex23
want 24 && test_ex24
want 25 && test_c_func 25 ft_foreach.c "" 'void ft_foreach(int *tab, int length, void (*f)(int));'
want 26 && test_c_func 26 ft_count_if.c "" 'int ft_count_if(char **tab, int (*f)(char*));'
want 27 && test_ex27

printf '\n%s============================================%s\n' "$BLD" "$RST"
printf ' %sResultado:%s %s%d OK%s  %s%d KO%s  %s%d avisos%s\n' "$BLD" "$RST" "$GRN" "$PASS" "$RST" "$RED" "$FAIL" "$RST" "$YLW" "$WARN" "$RST"
[ "$FAIL" -eq 0 ]
