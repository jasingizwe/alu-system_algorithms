#include "pathfinding.h"

/**
 * get_min_distance - Finds the vertex with the
 * lowest distance from the source.
 * @graph: Pointer to the graph.
 * @distance: Array of distances from the start vertex.
 * @visited: Array indicating if a vertex has been visited.
 * @index: Pointer to store the index of the
 * vertex with the minimum distance.
 *
 * Return: Pointer to the vertex with the minimum distance,
 * or NULL if none found.
 */
vertex_t *get_min_distance(graph_t *graph, size_t *distance,
			   size_t *visited, size_t *index)
{
	size_t min = INT_MAX;
	size_t i;
	vertex_t *vertex = graph->vertices;

	if (vertex == NULL)
		return (NULL);

	*index = INT_MAX;

	for (i = 0; i < graph->nb_vertices; i++)
	{
		if (visited[i] == 0 && min > distance[i])
		{
			min = distance[i];
			*index = i;
		}
	}

	if (*index == INT_MAX)
		return (NULL);

	while (vertex)
	{
		if (vertex->index == *index)
			return (vertex);
		vertex = vertex->next;
	}
	return (NULL);
}

/**
 * insert_into_queue - Inserts vertices into the queue to form the path.
 * @graph: Pointer to the graph with vertices.
 * @path: Pointer to the path queue.
 * @path_via: Array of vertices representing the path.
 * @start: Pointer to the start vertex.
 * @target: Pointer to the target vertex.
 */
void insert_into_queue(graph_t *graph, queue_t *path, vertex_t **path_via,
		       vertex_t const *start, vertex_t const *target)
{
	size_t i = target->index;

	if (!path_via[i])
		return;

	if (!queue_push_front(path, strdup(target->content)))
		queue_delete(path);

	while (path_via[i] && i < graph->nb_vertices)
	{
		if (!queue_push_front(path, strdup(path_via[i]->content)))
			queue_delete(path);
		i = path_via[i]->index;
		if (i == start->index)
			return;
	}
}

/**
 * recursive_dijkstra - Recursively finds the
 * shortest path using Dijkstra's algorithm.
 * @graph: Pointer to the graph.
 * @distance: Array of distances from the start vertex.
 * @visited: Array indicating which vertices have been visited.
 * @path_via: Array of vertices representing the path.
 * @start: Pointer to the start vertex.
 * @target: Pointer to the target vertex.
 * @idx: Current index.
 */
void recursive_dijkstra(graph_t *graph, size_t *distance,
			size_t *visited,
			vertex_t **path_via, vertex_t const *start,
			vertex_t const *target, size_t idx)
{
	vertex_t *current;
	edge_t *edge;
	size_t i = 0, temp;

	current = get_min_distance(graph, distance, visited, &idx);
	if (!current)
		return;

	printf("Checking %s, distance from %s is %ld\n",
	       current->content, start->content, distance[current->index]);
	i = current->index;
	edge = current->edges;
	while (edge && visited[i] == 0)
	{
		temp = distance[i] + edge->weight;
		if (distance[edge->dest->index] > temp)
		{
			distance[edge->dest->index] = temp;
			path_via[edge->dest->index] = current;
		}
		edge = edge->next;
	}
	visited[i] = 1;
	if (visited[target->index] == 1)
		return;

	recursive_dijkstra(graph, distance, visited, path_via, start, target, idx);
}

/**
 * init_dijkstra - Initializes data structures needed for Dijkstra's algorithm.
 * @graph: Pointer to the graph.
 * @distance: Pointer to the array of distances.
 * @visited: Pointer to the array of visited vertices.
 * @path_via: Pointer to the array of path vertices.
 *
 * Return: 0 on success, 1 on failure.
 */
int init_dijkstra(graph_t *graph, size_t **distance, size_t **visited,
		  vertex_t ***path_via)
{
	size_t i;

	*visited = calloc(graph->nb_vertices, sizeof(**visited));
	if (!*visited)
		return (1);

	*path_via = calloc(graph->nb_vertices, sizeof(**path_via));
	if (!*path_via)
	{
		free(*visited);
		return (1);
	}

	*distance = malloc(graph->nb_vertices * sizeof(**distance));
	if (!*distance)
	{
		free(*visited);
		free(*path_via);
		return (1);
	}

	for (i = 0; i < graph->nb_vertices; i++)
		(*distance)[i] = INT_MAX;

	return (0);
}

/**
 * dijkstra_graph - Finds the shortest path
 * from a start to a target vertex in a graph.
 * @graph: Pointer to the graph.
 * @start: Pointer to the start vertex.
 * @target: Pointer to the target vertex.
 *
 * Return: Queue containing the path from
 * start to target, or NULL if no path found.
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	size_t *distance = NULL, *visited = NULL;
	queue_t *queue = NULL;
	vertex_t **path_via = NULL;

	if (!graph || !start || !target)
		return (NULL);

	if (init_dijkstra(graph, &distance, &visited, &path_via) != 0)
		return (NULL);

	queue = queue_create();
	if (!queue)
	{
		free(visited);
		free(distance);
		free(path_via);
		return (NULL);
	}

	distance[start->index] = 0;
	recursive_dijkstra(graph, distance, visited, path_via, start,
			   target, 0);

	insert_into_queue(graph, queue, path_via, start, target);
	free(visited);
	free(distance);
	free(path_via);

	if (!queue->front)
	{
		queue_delete(queue);
		return (NULL);
	}
	return (queue);
}
