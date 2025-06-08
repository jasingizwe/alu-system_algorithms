#include "pathfinding.h"

/**
 * recursive_backtrack_graph - Recursive
 * function to backtrack a path in a graph.
 * @path: Pointer to the current path queue.
 * @visited: Array marking the visited vertices.
 * @current: Pointer to the current vertex being explored.
 * @target: Pointer to the target vertex.
 *
 * Return: 1 on success (path found), 0 on failure (no path).
 */
int recursive_backtrack_graph(queue_t **path, int *visited,
			      vertex_t const *current, vertex_t const *target)
{
	char *city;
	edge_t *edges;

	/* If current vertex is NULL or already visited, return failure */
	if (current == NULL || visited[current->index] == 1)
		return (0);

	printf("Checking %s\n", current->content);

	/* If current vertex matches target, add to path and return success */
	if (strcmp(current->content, target->content) == 0)
	{
		city = strdup(current->content);
		queue_push_front(*path, city);
		return (1);
	}

	/* Mark current vertex as visited */
	visited[current->index] = 1;

	/* Explore all edges (adjacent vertices) of the current vertex */
	for (edges = current->edges; edges; edges = edges->next)
	{
		/* Recursive call to explore each adjacent vertex */
		if (recursive_backtrack_graph(path, visited, edges->dest, target))
		{
			city = strdup(current->content);
			queue_push_front(*path, city);
			return (1);
		}
	}

	/* Unmark current vertex if no path is found */
	visited[current->index] = 0;
	return (0);
}

/**
 * backtracking_graph - Initiates backtracking search for a path in a graph.
 * @graph: Pointer to the graph structure.
 * @start: Pointer to the starting vertex.
 * @target: Pointer to the target vertex.
 *
 * Return: Queue containing the path from start to target,
 *         or NULL if no path exists.
 */
queue_t *backtracking_graph(graph_t *graph, vertex_t const *start,
			    vertex_t const *target)
{
	int *visited = NULL;
	int success;
	queue_t *path;

	/* Validate input parameters */
	if (!graph || !start || !target)
		return (NULL);

	/* Create a new path queue */
	path = queue_create();
	if (!path)
		return (NULL);

	/* Allocate memory for visited array */
	visited = calloc(graph->nb_vertices, sizeof(*visited));
	if (!visited)
	{
		queue_delete(path);
		return (NULL);
	}

	/* Perform recursive backtracking to find the path */
	success = recursive_backtrack_graph(&path, visited, start, target);

	/* Free the visited array */
	free(visited);

	/* Return path if success, else delete the path and return NULL */
	if (success)
		return (path);

	queue_delete(path);
	return (NULL);
}
