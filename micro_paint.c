#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct	s_zone
{
	int		w, h;
	char	*zone;
}				t_zone;

typedef struct	s_shape
{
	char	type;
	float	x, y, w, h;
	char	c;
}				t_shape;

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_put_error(char *s)
{
	write(1, s, ft_strlen(s));
	return (1);
}

int	ft_quit(FILE *fp, t_zone *z, char *s)
{
	if (fp)
		fclose(fp);
	if (z->zone)
		free(z->zone);
	if (s)
		ft_put_error(s);
	return (1);
}

int	set_zone(FILE *fp, t_zone *z)
{
	char	c;
	int		i;

	if (fscanf(fp, "%d %d %c\n", &z->w, &z->h, &c) != 3)
		return (1);
	if (!(z->w > 0 && z->w <= 300 && z->h > 0 && z->h <= 300))
		return (1);
	if (!(z->zone = calloc(z->w * z->h + 1, sizeof(char))))
		return (1);
	i = 0;
	while (i < z->w * z->h)
		z->zone[i++] = c;
	return (0);
}

void	put_zone(t_zone z)
{
	int	y;

	y = 0;
	while (y < z.h)
	{
		write(1, &z.zone[y * z.w], z.w);
		write(1, "\n", 1);
		y++;
	}
}

int	is_in_rectangle(float x, float y, t_shape s)
{
	if (x - s.x < 0.0 || s.x - x + s.w < 0.0 || y - s.y < 0.0 || s.y - y + s.h < 0.0)
		return (0);
	if (x - s.x < 1.0 || s.x - x + s.w < 1.0 || y - s.y < 1.0 || s.y - y + s.h < 1.0)
		return (1);
	return (2);
}

void	draw_shape(t_zone *z, t_shape s)
{
	int	x, y, ret;

	y = 0;
	while (y < z->h)
	{
		x = 0;
		while (x < z->w)
		{
			ret = is_in_rectangle(x, y, s);
			if (ret == 1 || (s.type == 'R' && ret == 2))
				z->zone[y * z->w + x] = s.c;
			x++;
		}
		y++;
	}
}

int	draw_shapes(FILE *fp, t_zone *z)
{
	t_shape	s;
	int		ret;

	while ((ret = fscanf(fp, "%c %f %f %f %f %c\n", &s.type, &s.x, &s.y, &s.w, &s.h, &s.c)) == 6)
	{
		if (!((s.type == 'r' || s.type == 'R') && s.w > 0.0 && s.h > 0.0))
			return (1);
		draw_shape(z, s);
	}
	if (ret != -1)
		return (1);
	return (0);
}

int	main(int argc, char *argv[])
{
	FILE	*fp;
	t_zone	z;

	if (argc != 2)
		return (ft_put_error("Error: argument\n"));
	if (!(fp = fopen(argv[1], "r")))
		return (ft_put_error("Error: Operation file corrupted\n"));
	memset(&z, 0, sizeof(t_zone));
	if (set_zone(fp, &z))
		return (ft_quit(fp, &z, "Error: Operation file corrupted\n"));
	if (draw_shapes(fp, &z))
		return (ft_quit(fp, &z, "Error: Operation file corrupted\n"));
	put_zone(z);
	ft_quit(fp, &z, NULL);
	return (0);
}
