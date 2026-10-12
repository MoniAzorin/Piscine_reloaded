#ifndef HARNESS_H
# define HARNESS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/mman.h>

# define MSG_SZ 2048
# define TIMEOUT_SEC 2

typedef int	(*t_test)(void);

static char			*g_msg;
static int			g_pass;
static int			g_fail;
static int			g_warn;
static int			g_pipe[2];
static int			g_saved_out;
static char			g_esc[2][512];
static const char	*c_ok = "";
static const char	*c_ko = "";
static const char	*c_wn = "";
static const char	*c_rs = "";

static void	harness_init(void)
{
	char	*col;

	g_msg = mmap(NULL, MSG_SZ, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	col = getenv("PT_COLOR");
	if (col && col[0] == '1')
	{
		c_ok = "\033[32m";
		c_ko = "\033[31m";
		c_wn = "\033[33m";
		c_rs = "\033[0m";
	}
}

/* Version imprimible de una cadena (\n, \t, bytes no ASCII) */
static const char	*esc(const char *s, int slot)
{
	char			*o;
	size_t			i;
	size_t			j;
	unsigned char	c;

	if (!s)
		return ("(null)");
	o = g_esc[slot];
	i = 0;
	j = 0;
	while (s[i] && j < 200)
	{
		c = (unsigned char)s[i++];
		if (c == '\n')
			j += sprintf(o + j, "\\n");
		else if (c == '\t')
			j += sprintf(o + j, "\\t");
		else if (c < 32 || c > 126)
			j += sprintf(o + j, "\\x%02x", c);
		else
			o[j++] = (char)c;
	}
	if (s[i])
		j += sprintf(o + j, "...");
	o[j] = '\0';
	return (o);
}

# define FAIL(...) \
	do { snprintf(g_msg, MSG_SZ, __VA_ARGS__); return (1); } while (0)

# define TRY(call) \
	do { if (call) return (1); } while (0)

# define EXPECT_INT(got, exp) \
	do { \
		long long g__ = (long long)(got); \
		long long e__ = (long long)(exp); \
		if (g__ != e__) \
			FAIL("l.%d: %s -> obtenido %lld, esperado %lld", \
				__LINE__, #got, g__, e__); \
	} while (0)

# define EXPECT_STR(got, exp) \
	do { \
		const char *gs__ = (got); \
		const char *es__ = (exp); \
		if (!gs__ || strcmp(gs__, es__) != 0) \
			FAIL("l.%d: obtenido \"%s\", esperado \"%s\"", \
				__LINE__, esc(gs__, 0), esc(es__, 1)); \
	} while (0)

# define EXPECT_TRUE(cond, what) \
	do { if (!(cond)) FAIL("l.%d: %s", __LINE__, what); } while (0)

static void	run_one(const char *name, t_test fn, int soft)
{
	pid_t		pid;
	int			st;
	const char	*why;

	g_msg[0] = '\0';
	pid = fork();
	if (pid == 0)
	{
		alarm(TIMEOUT_SEC);
		_exit(fn() ? 1 : 0);
	}
	waitpid(pid, &st, 0);
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
	{
		g_pass++;
		printf("  %s[OK]%s   %s\n", c_ok, c_rs, name);
		return ;
	}
	why = "";
	if (WIFEXITED(st) && WEXITSTATUS(st) == 77)
		why = "ERROR DE MEMORIA (sanitizer, ver abajo)";
	else if (WIFSIGNALED(st) && WTERMSIG(st) == SIGALRM)
		why = "TIMEOUT (> 2 s)";
	else if (WIFSIGNALED(st) && WTERMSIG(st) == SIGSEGV)
		why = "SEGFAULT";
	else if (WIFSIGNALED(st) && WTERMSIG(st) == SIGBUS)
		why = "BUS ERROR";
	else if (WIFSIGNALED(st) && WTERMSIG(st) == SIGABRT)
		why = "ABORT (double free / memoria corrupta)";
	else if (WIFSIGNALED(st) && WTERMSIG(st) == SIGFPE)
		why = "SIGFPE (division por cero)";
	else if (WIFSIGNALED(st))
		why = "terminado por una senal";
	if (soft)
		g_warn++;
	else
		g_fail++;
	printf("  %s%s%s %s%s%s\n", soft ? c_wn : c_ko, soft ? "[WARN]" : "[KO]  ",
		c_rs, name, why[0] ? "  <- " : "", why);
	if (g_msg[0])
		printf("         %s\n", g_msg);
}

# define T(desc, fn) run_one(desc, fn, 0)
# define TW(desc, fn) run_one(desc, fn, 1)

/* Captura de lo que se escribe en la salida estandar (fd 1) */
static void	cap_start(void)
{
	if (pipe(g_pipe) != 0)
		_exit(2);
	g_saved_out = dup(1);
	dup2(g_pipe[1], 1);
	close(g_pipe[1]);
}

static int	cap_end(char *buf, int size)
{
	int	n;
	int	total;

	dup2(g_saved_out, 1);
	close(g_saved_out);
	total = 0;
	n = read(g_pipe[0], buf, size - 1);
	while (n > 0)
	{
		total += n;
		if (total >= size - 1)
			break ;
		n = read(g_pipe[0], buf + total, size - 1 - total);
	}
	buf[total] = '\0';
	close(g_pipe[0]);
	return (total);
}

static int	harness_end(void)
{
	printf("@@RESULT %d %d %d\n", g_pass, g_fail, g_warn);
	return (g_fail != 0);
}

#endif
