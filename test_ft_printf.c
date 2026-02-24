/* ************************************************************************** */
/* test_ft_printf.c                                                           */
/* Comprehensive tester for ft_printf (mandatory part: no bonus)             */
/*                                                                            */
/* Compile: see Makefile  OR  cc -I../ft_printf -L../ft_printf               */
/*          -o test_ft_printf test_ft_printf.c -lftprintf                    */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

int	ft_printf(const char *format, ...);

/* ── Terminal colors ─────────────────────────────────────────────────────── */
#define GRN "\033[0;32m"
#define RED "\033[0;31m"
#define CYN "\033[0;36m"
#define BLD "\033[1m"
#define RST "\033[0m"

/* ── Global counters ─────────────────────────────────────────────────────── */
static int	g_pass  = 0;
static int	g_fail  = 0;
static int	g_total = 0;

/* ── Core test macro ─────────────────────────────────────────────────────── */
/*
 * Redirects fd 1 to a pipe, calls ft_printf, restores stdout, reads the
 * captured bytes, and compares them (and the return value) against what
 * snprintf produces (same semantics as printf for the return value).
 */
#define TEST(label, fmt, ...)								\
do {														\
	char	_exp[8192];										\
	char	_got[8192];										\
	int		_fd[2];											\
	int		_sv;											\
	ssize_t	_n;												\
	int		_fret;											\
	int		_eret;											\
	int		_ok_out;										\
	int		_ok_ret;										\
	\
	memset(_exp, 0, sizeof(_exp));							\
	memset(_got, 0, sizeof(_got));							\
	if (pipe(_fd) == -1) { perror("pipe"); break; }			\
	_sv   = dup(STDOUT_FILENO);								\
	dup2(_fd[1], STDOUT_FILENO);							\
	close(_fd[1]);											\
	_fret = ft_printf((fmt), ##__VA_ARGS__);				\
	fflush(stdout);											\
	dup2(_sv, STDOUT_FILENO);								\
	close(_sv);												\
	_n = read(_fd[0], _got, sizeof(_got) - 1);				\
	close(_fd[0]);											\
	if (_n < 0) _n = 0;										\
	_eret = snprintf(_exp, sizeof(_exp), (fmt), ##__VA_ARGS__); \
	if (_eret < 0) _eret = 0;								\
	_ok_out = ((int)_n == _eret)							\
		&& (_eret == 0 || memcmp(_got, _exp, (size_t)_eret) == 0); \
	_ok_ret = (_fret == _eret);								\
	g_total++;												\
	if (_ok_out && _ok_ret)									\
	{														\
		printf(GRN "[OK]" RST " %s\n", (label));			\
		g_pass++;											\
	}														\
	else													\
	{														\
		printf(RED "[KO]" RST " %s\n", (label));			\
		if (!_ok_out)										\
		{													\
			printf("     output  expected [%d bytes]: [%s]\n", \
				_eret, _exp);								\
			printf("     output  got      [%d bytes]: [%s]\n", \
				(int)_n, _got);								\
		}													\
		if (!_ok_ret)										\
		{													\
			printf("     return  expected : %d\n", _eret);	\
			printf("     return  got      : %d\n", _fret);	\
		}													\
		g_fail++;											\
	}														\
} while (0)

/* ── Section header ──────────────────────────────────────────────────────── */
static void	section(const char *name)
{
	printf("\n" BLD CYN
		"==============================================\n"
		"  %s\n"
		"==============================================" RST "\n", name);
}

/* ══════════════════════════════════════════════════════════════════════════ */
int	main(void)
{
	/* ---------------------------------------------------------------------- */
	/*  %c  – character                                                        */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %c  (character)");
	TEST("%c : 'A'",           "%c", 'A');
	TEST("%c : 'a'",           "%c", 'a');
	TEST("%c : 'Z'",           "%c", 'Z');
	TEST("%c : 'z'",           "%c", 'z');
	TEST("%c : '0'",           "%c", '0');
	TEST("%c : '9'",           "%c", '9');
	TEST("%c : space ' '",     "%c", ' ');
	TEST("%c : '!'",           "%c", '!');
	TEST("%c : '@'",           "%c", '@');
	TEST("%c : '#'",           "%c", '#');
	TEST("%c : '+'",           "%c", '+');
	TEST("%c : '-'",           "%c", '-');
	TEST("%c : '~'",           "%c", '~');
	TEST("%c : '\\t' tab",     "%c", '\t');
	TEST("%c : '\\n' newline", "%c", '\n');
	TEST("%c : 127 (DEL)",     "%c", 127);
	TEST("%c : 1   (SOH)",     "%c", 1);

	/* ---------------------------------------------------------------------- */
	/*  %s  – string                                                           */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %s  (string)");
	TEST("%s : \"Hello, World!\"",             "%s", "Hello, World!");
	TEST("%s : empty string \"\"",             "%s", "");
	TEST("%s : single char \"a\"",             "%s", "a");
	TEST("%s : digits \"0123456789\"",         "%s", "0123456789");
	TEST("%s : spaces \"   spaces   \"",       "%s", "   spaces   ");
	TEST("%s : special chars \"!@#$%^&*()\"",  "%s", "!@#$%^&*()");
	TEST("%s : newline inside",                "%s", "line1\nline2");
	TEST("%s : tab inside",                    "%s", "col1\tcol2");
	TEST("%s : 52-char string",                "%s",
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	TEST("%s : mixed \"abc 123 !@# XYZ\"",    "%s", "abc 123 !@# XYZ");
	TEST("%s : NULL  (nil / null expected)",   "%s", (char *)NULL);

	/* ---------------------------------------------------------------------- */
	/*  %p  – pointer                                                          */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %p  (pointer)");
	{
		int		i;
		char	c;
		void	*p1 = (void *)(uintptr_t)0x1;
		void	*p2 = (void *)(uintptr_t)0xDEADBEEF;

		i = 42;
		c = 'X';
		TEST("%p : NULL",              "%p", (void *)NULL);
		TEST("%p : address of int",    "%p", (void *)&i);
		TEST("%p : address of char",   "%p", (void *)&c);
		TEST("%p : (void *)0x1",       "%p", p1);
		TEST("%p : (void *)0xDEADBEEF","%p", p2);
		TEST("%p : address of main",   "%p", (void *)&main);
	}

	/* ---------------------------------------------------------------------- */
	/*  %d  – signed decimal integer                                           */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %d  (signed decimal integer)");
	TEST("%d : 0",                       "%d", 0);
	TEST("%d : 1",                       "%d", 1);
	TEST("%d : -1",                      "%d", -1);
	TEST("%d : 42",                      "%d", 42);
	TEST("%d : -42",                     "%d", -42);
	TEST("%d : 100",                     "%d", 100);
	TEST("%d : -100",                    "%d", -100);
	TEST("%d : 1000",                    "%d", 1000);
	TEST("%d : -1000",                   "%d", -1000);
	TEST("%d : 99999",                   "%d", 99999);
	TEST("%d : -99999",                  "%d", -99999);
	TEST("%d : INT_MAX  (2147483647)",   "%d", INT_MAX);
	TEST("%d : INT_MIN  (-2147483648)",  "%d", INT_MIN);
	TEST("%d : INT_MAX - 1",             "%d", INT_MAX - 1);
	TEST("%d : INT_MIN + 1",             "%d", INT_MIN + 1);

	/* ---------------------------------------------------------------------- */
	/*  %i  – signed integer (identical to %d)                                */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %i  (signed integer, same as %d)");
	TEST("%i : 0",         "%i", 0);
	TEST("%i : 1",         "%i", 1);
	TEST("%i : -1",        "%i", -1);
	TEST("%i : 42",        "%i", 42);
	TEST("%i : -42",       "%i", -42);
	TEST("%i : 12345",     "%i", 12345);
	TEST("%i : -12345",    "%i", -12345);
	TEST("%i : INT_MAX",   "%i", INT_MAX);
	TEST("%i : INT_MIN",   "%i", INT_MIN);

	/* ---------------------------------------------------------------------- */
	/*  %u  – unsigned decimal integer                                         */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %u  (unsigned decimal integer)");
	TEST("%u : 0",                            "%u", 0u);
	TEST("%u : 1",                            "%u", 1u);
	TEST("%u : 42",                           "%u", 42u);
	TEST("%u : 100",                          "%u", 100u);
	TEST("%u : 9999",                         "%u", 9999u);
	TEST("%u : INT_MAX as unsigned",          "%u", (unsigned int)INT_MAX);
	TEST("%u : INT_MAX + 1",                  "%u", (unsigned int)INT_MAX + 1u);
	TEST("%u : UINT_MAX  (4294967295)",       "%u", UINT_MAX);
	TEST("%u : UINT_MAX - 1",                 "%u", UINT_MAX - 1u);
	TEST("%u : (unsigned int)-1 == UINT_MAX", "%u", (unsigned int)-1);

	/* ---------------------------------------------------------------------- */
	/*  %x  – unsigned hexadecimal (lowercase)                                 */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %x  (unsigned hex, lowercase)");
	TEST("%x : 0       -> 0",          "%x", 0u);
	TEST("%x : 1       -> 1",          "%x", 1u);
	TEST("%x : 9       -> 9",          "%x", 9u);
	TEST("%x : 10      -> a",          "%x", 10u);
	TEST("%x : 15      -> f",          "%x", 15u);
	TEST("%x : 16      -> 10",         "%x", 16u);
	TEST("%x : 255     -> ff",         "%x", 255u);
	TEST("%x : 256     -> 100",        "%x", 256u);
	TEST("%x : 0xDEAD  -> dead",       "%x", 0xDEADu);
	TEST("%x : INT_MAX -> 7fffffff",   "%x", (unsigned int)INT_MAX);
	TEST("%x : UINT_MAX-> ffffffff",   "%x", UINT_MAX);
	TEST("%x : 0xCAFEBABE",            "%x", 0xCAFEBABEu);

	/* ---------------------------------------------------------------------- */
	/*  %X  – unsigned hexadecimal (uppercase)                                 */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %X  (unsigned hex, uppercase)");
	TEST("%X : 0       -> 0",          "%X", 0u);
	TEST("%X : 1       -> 1",          "%X", 1u);
	TEST("%X : 15      -> F",          "%X", 15u);
	TEST("%X : 255     -> FF",         "%X", 255u);
	TEST("%X : 256     -> 100",        "%X", 256u);
	TEST("%X : 0xDEAD  -> DEAD",       "%X", 0xDEADu);
	TEST("%X : INT_MAX -> 7FFFFFFF",   "%X", (unsigned int)INT_MAX);
	TEST("%X : UINT_MAX-> FFFFFFFF",   "%X", UINT_MAX);
	TEST("%X : 0xCAFEBABE",            "%X", 0xCAFEBABEu);

	/* ---------------------------------------------------------------------- */
	/*  %%  – literal percent sign                                             */
	/* ---------------------------------------------------------------------- */
	section("Specifier: %%  (literal percent sign)");
	TEST("%% : single        %%",      "%%");
	TEST("%% : double        %%%%",    "%%%%");
	TEST("%% : in number     100%%",   "100%%");
	TEST("%% : at start      %%text",  "%%text");
	TEST("%% : at end        text%%",  "text%%");
	TEST("%% : multiple      a%%b%%c", "a%%b%%c");

	/* ---------------------------------------------------------------------- */
	/*  Mixed / multiple specifiers                                            */
	/* ---------------------------------------------------------------------- */
	section("Mixed / multiple specifiers");
	TEST("mixed : %d and %s",
		"%d and %s", 42, "hello");
	TEST("mixed : %c %c %c",
		"%c %c %c", 'a', 'b', 'c');
	TEST("mixed : %s = %d",
		"%s = %d", "answer", 42);
	TEST("mixed : %x vs %X (same value)",
		"%x vs %X", 255u, 255u);
	TEST("mixed : %d  %i  %u  (same bits)",
		"%d %i %u", -1, -1, (unsigned int)-1);
	TEST("mixed : four %d",
		"%d %d %d %d", 1, 2, 3, 4);
	TEST("mixed : %p and %d",
		"%p and %d", (void *)(uintptr_t)0x42, 66);
	TEST("mixed : all mandatory specifiers",
		"c=%c s=%s d=%d i=%i u=%u x=%x X=%X p=%p",
		'A', "test", -42, 42, 42u, 255u, 255u, (void *)(uintptr_t)0x42);
	TEST("mixed : %% with %d",
		"100%% = %d", 100);
	TEST("mixed : %s and %c",
		"%s and %c", "hello", '!');
	TEST("mixed : INT_MIN and UINT_MAX",
		"%d and %u", INT_MIN, UINT_MAX);
	TEST("mixed : concatenated %s",
		"%s%s%s", "foo", "bar", "baz");

	/* ---------------------------------------------------------------------- */
	/*  Edge cases                                                             */
	/* ---------------------------------------------------------------------- */
	section("Edge cases");
	TEST("edge : empty format string",
		"");
	TEST("edge : plain text, no specifier",
		"Hello, 42 World!");
	TEST("edge : only text and %%",
		"Score: 100%%");
	TEST("edge : %d == 0",
		"%d", 0);
	TEST("edge : chain INT_MAX / INT_MIN / 0 / -1",
		"%d/%d/%d/%d", INT_MAX, INT_MIN, 0, -1);
	TEST("edge : %u == 0",
		"%u", 0u);
	TEST("edge : %x == 0  -> \"0\"",
		"%x", 0u);
	TEST("edge : %X == 0  -> \"0\"",
		"%X", 0u);
	TEST("edge : %s only spaces",
		"%s", "     ");
	TEST("edge : %c and %s same char",
		"%c%s", 'A', "A");
	TEST("edge : UINT_MAX in %x and %X",
		"%x %X", UINT_MAX, UINT_MAX);
	TEST("edge : %p NULL vs %d 0",
		"ptr=%p  int=%d", (void *)NULL, 0);
	TEST("edge : %s empty + %d",
		"%s%d", "", 42);
	TEST("edge : nested %%d looks like specifier",
		"%%d is not parsed as %%d");

	/* ---------------------------------------------------------------------- */
	/*  Summary                                                                */
	/* ---------------------------------------------------------------------- */
	printf("\n" BLD
		"==============================================\n"
		"  RESULTS : %d / %d tests passed\n"
		"==============================================\n" RST,
		g_pass, g_total);
	if (g_fail == 0)
		printf(BLD GRN "  All tests passed!\n" RST "\n");
	else
		printf(BLD RED "  %d test(s) failed.\n" RST "\n", g_fail);

	return (g_fail > 0 ? 1 : 0);
}
