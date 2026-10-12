#include "harness.h"
#include "ft_point.h"
#include "ft_point.h"

static int	fields(void)
{
	t_point	p;

	p.x = 42;
	p.y = 21;
	EXPECT_INT(p.x, 42);
	EXPECT_INT(p.y, 21);
	return (0);
}

static int	independent_fields(void)
{
	t_point	p;

	p.x = 1;
	p.y = 2;
	p.x = 100;
	EXPECT_INT(p.y, 2);
	p.y = -100;
	EXPECT_INT(p.x, 100);
	return (0);
}

static int	through_pointer(void)
{
	t_point	p;
	t_point	*ptr;

	ptr = &p;
	ptr->x = 42;
	ptr->y = 21;
	EXPECT_INT(p.x, 42);
	EXPECT_INT(p.y, 21);
	return (0);
}

static int	array_of_points(void)
{
	t_point	a[3];
	int		i;

	i = 0;
	while (i < 3)
	{
		a[i].x = i;
		a[i].y = i * 10;
		i++;
	}
	i = 0;
	while (i < 3)
	{
		EXPECT_INT(a[i].x, i);
		EXPECT_INT(a[i].y, i * 10);
		i++;
	}
	return (0);
}

static int	member_types(void)
{
	t_point	p;

	EXPECT_TRUE(_Generic(p.x, int: 1, default: 0), "x deberia ser int");
	EXPECT_TRUE(_Generic(p.y, int: 1, default: 0), "y deberia ser int");
	return (0);
}

int	main(void)
{
	harness_init();
	T("t_point tiene x e y asignables (y la cabecera se puede incluir 2 veces)", fields);
	T("x e y son independientes", independent_fields);
	T("acceso mediante puntero (->)", through_pointer);
	T("array de t_point", array_of_points);
	TW("x e y son de tipo int", member_types);
	return (harness_end());
}
