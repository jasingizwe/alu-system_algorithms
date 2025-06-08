#include "pathfinding.h"

/**
 * backtracking_array - Implements a backtracking algorithm to find a path
 * @map: The 2D map (grid)
 * @rows: Total number of rows
 * @cols: Total number of columns
 * @start: Starting point on the map
 * @target: Target point we're trying to reach
 *
 * Return: A queue of points representing the path
 */
queue_t *backtracking_array(char **map, int rows, int cols,
			    point_t const *start, point_t const *target)
{
	queue_t *path = queue_create();
	queue_t *reverse_path = queue_create();
	char **mymap;
	int i;
	point_t *point;

	if (!path || !reverse_path)
		return (NULL);

	mymap = malloc(rows * sizeof(char *));
	if (!mymap)
		exit(1);

	for (i = 0; i < rows; i++)
	{
		mymap[i] = malloc(cols + 1);
		if (!mymap[i])
			exit(1);
		strcpy(mymap[i], map[i]);
	}

	if (backtrack(mymap, rows, cols, target, start->x, start->y, path))
	{
		while ((point = dequeue(path)))
			queue_push_front(reverse_path, point);
		free(path);
	}
	else
	{
		free(path);
		free(reverse_path);
		reverse_path = NULL;
	}

	for (i = 0; i < rows; i++)
		free(mymap[i]);
	free(mymap);

	return (reverse_path);
}

/**
 * backtrack - Recursively finds a path using backtracking
 * @map: 2D map
 * @rows: Number of rows
 * @cols: Number of columns
 * @target: Target point
 * @x: Current x coordinate
 * @y: Current y coordinate
 * @path: Queue to store the path
 *
 * Return: 1 if path is found, 0 if no path
 */
int backtrack(char **map, int rows, int cols, point_t const *target,
	      int x, int y, queue_t *path)
{
	point_t *point;

	if (x < 0 || x >= cols || y < 0 || y >= rows || map[y][x] != '0')
		return (0);

	map[y][x] = '1';

	point = calloc(1, sizeof(*point));
	if (!point)
		exit(1);

	point->x = x;
	point->y = y;

	queue_push_front(path, point);
	printf("Checking coordinates [%d, %d]\n", x, y);

	if (x == target->x && y == target->y)
		return (1);

	if (backtrack(map, rows, cols, target, x + 1, y, path) ||
	    backtrack(map, rows, cols, target, x, y + 1, path) ||
	    backtrack(map, rows, cols, target, x - 1, y, path) ||
	    backtrack(map, rows, cols, target, x, y - 1, path))
		return (1);

	free(dequeue(path));

	return (0);
}
