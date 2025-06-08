#include "pathfinding.h"

/**
 * get_min_g_score - Finds the vertex with the lowest g_score from the source.
 *
 * @graph: Pointer to the graph.
 * @g_score: Array of g_scores from the start vertex.
 * @visited: Array indicating if a vertex has been visited.
 * @index: Current index.
 *
 * Return: Pointer to the vertex with the minimum g_score or NULL if not found.
 */
vertex_t *get_min_g_score(graph_t *graph, size_t *g_score, size_t *visited,
		size_t *index)
{
	size_t min = ULONG_MAX;
	size_t i;
	vertex_t *vertex;

	vertex = graph->vertices;
	if (vertex == NULL)
		return (NULL);

	*index = ULONG_MAX;

	for (i = 0; i < graph->nb_vertices; i++)
	{
		if (visited[i] == 0 && min > g_score[i])
		{
			min = g_score[i];
			*index = i;
		}
	}
	if (*index == ULONG_MAX)
		return (NULL);

	while (vertex->next)
	{
		if (vertex->index == *index)
			return (vertex);
		vertex = vertex->next;
	}
	return (vertex);
}


/**
 * reconstruct_path - Constructs the path by inserting vertices into a queue.
 *
 * @graph: Pointer to the graph containing vertices.
 * @path: Pointer to the queue to store the path.
 * @came_from: Array indicating the previous vertex for each vertex.
 * @start: Pointer to the starting vertex.
 * @target: Pointer to the target vertex.
 *
 * Return: void
 */
void reconstruct_path(graph_t *graph, queue_t *path, vertex_t **came_from,
			vertex_t const *start, vertex_t const *target)
{
	size_t i = target->index;

	if (!came_from[i])
		return;

	/* Insert target node content into the queue */
	if (!queue_push_front(path, strdup(target->content)))
		queue_delete(path);

	/* Traverse backwards from target to start */
	while (came_from[i] && i < graph->nb_vertices)
	{
		if (!queue_push_front(path, strdup(came_from[i]->content)))
			queue_delete(path);
		i = came_from[i]->index;
		if (i == start->index)
			return;
	}
}


/**
 * recursive_a_star - Recursively finds the shortest
 * path using the A* algorithm.
 * @graph: Pointer to the graph.
 * @g_score: Array of g_scores from the start vertex.
 * @visited: Array indicating if a vertex has been visited.
 * @came_from: Array indicating the previous vertex for each vertex.
 * @start: Pointer to the starting vertex.
 * @target: Pointer to the target vertex.
 * @f_score: Array of g_scores plus the heuristic distance to the target.
 * @idx: Current index.
 *
 * Return: void
 */
void recursive_a_star(graph_t *graph, size_t *visited, vertex_t **came_from,
	vertex_t const *start, vertex_t const *target,
	size_t *f_score, size_t *g_score, size_t idx)
{
	vertex_t *current, *neighbor;
	edge_t *edge;
	size_t i = 0, tentative_g_score;

	current = get_min_g_score(graph, f_score, visited, &idx);
	if (!current)
		return;

	f_score[current->index] = h(current->x, current->y, target->x, target->y);
	printf("Checking %s, distance to %s is %ld\n", current->content,
			target->content, f_score[current->index]);
	i = current->index;
	edge = current->edges;
	while (edge && visited[i] == 0)
	{
		neighbor = edge->dest;
		tentative_g_score = g_score[i] + edge->weight;
		if (g_score[neighbor->index] > tentative_g_score)
		{
			came_from[neighbor->index] = current;
			g_score[neighbor->index] = tentative_g_score;
			f_score[neighbor->index] = g_score[neighbor->index] +
				h(neighbor->x, neighbor->y, target->x, target->y);
		}
		edge = edge->next;
	}
	visited[i] = 1;
	if (visited[target->index] == 1)
		return;

	recursive_a_star(graph, visited, came_from, start, target,
		f_score, g_score, idx);
}


/**
 * initialize_a_star - Initializes arrays for A* search.
 *
 * @graph: Pointer to the graph.
 * @visited: Pointer to an array indicating if a vertex has been visited.
 * @came_from: Pointer to an array indicating
 * the previous vertex for each vertex.
 * @f_score: Pointer to an array of g_scores plus the heuristic
 * distance to the target.
 * @g_score: Pointer to an array of g_scores from the start vertex.
 * @start: Pointer to the starting vertex.
 * @target: Pointer to the target vertex.
 *
 * Return: 1 on success, -1 on failure.
 */
int initialize_a_star(graph_t *graph, size_t **visited, vertex_t ***came_from,
	size_t **f_score, size_t **g_score,
	const vertex_t *start, const vertex_t *target)
{
	size_t i;

	*visited = calloc(graph->nb_vertices, sizeof(**visited));
	if (!*visited)
		return (-1);
	*came_from = calloc(graph->nb_vertices, sizeof(***came_from));
	if (!*came_from)
	{
		free(*visited);
		return (-1);
	}
	*f_score = malloc(graph->nb_vertices * sizeof(**f_score));
	if (!*f_score)
	{
		free(*visited), free(*came_from);
		return (-1);
	}
	*g_score = malloc(graph->nb_vertices * sizeof(**g_score));
	if (!*g_score)
	{
		free(*visited), free(*came_from), free(*f_score);
		return (-1);
	}
	for (i = 0; i < graph->nb_vertices; i++)
	{
		(*f_score)[i] = ULONG_MAX;
		(*g_score)[i] = ULONG_MAX;
	}
	(*g_score)[start->index] = 0;
	(*f_score)[start->index] = h(start->x, start->y,
							target->x, target->y);
	return (1);
}

/**
 * a_star_graph - Performs A* search for the shortest
 * path from start to target in a graph.
 * @graph: Pointer to the graph.
 * @start: Pointer to the starting vertex.
 * @target: Pointer to the target vertex.
 *
 * Return: Queue containing the shortest path
 * from start to target as a sequence of vertex contents,
 * or NULL if no path is found.
 */
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
					vertex_t const *target)
{
	size_t *f_score, *g_score, *visited = 0;
	queue_t *queue = NULL;
	vertex_t **came_from = NULL;

	if (!graph || !start || !target)
		return (NULL);

	if (!initialize_a_star(graph, &visited, &came_from, &f_score, &g_score,
			start, target))
		return (NULL);

	queue = queue_create();

	recursive_a_star(graph, visited, came_from, start, target,
		f_score, g_score, 0);
	reconstruct_path(graph, queue, came_from, start, target);

	free(visited), free(f_score), free(g_score), free(came_from);

	if (!queue->front)
	{
		free(queue);
		return (NULL);
	}
	return (queue);
}
