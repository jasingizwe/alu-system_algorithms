#include "graphs.h"

/**
 * graph_add_single_edge - program that adds a single directed edge to a vertex
 * @src_vertex: the source vertex for the edge
 * @dest_vertex: the destination vertex for the edge
 * Return: a pointer to the created edge, or NULL on failure
 * Arthor: Frank Onyema Orji
 */

edge_t *graph_add_single_edge(vertex_t *src_vertex, vertex_t *dest_vertex)
{
	edge_t *new_edge = NULL, *temp_edge = NULL;

	if (!src_vertex || !dest_vertex)
		return (NULL);

	new_edge = malloc(sizeof(edge_t));

	if (!new_edge)
		return (NULL);

	new_edge->dest = dest_vertex;
	new_edge->next = NULL;

	temp_edge = src_vertex->edges;

	while (temp_edge && temp_edge->next)
		temp_edge = temp_edge->next;

	if (temp_edge)
		temp_edge->next = new_edge;
	else
		src_vertex->edges = new_edge;

	src_vertex->nb_edges++;

	return (new_edge);
}



/**
 * graph_add_edge - program that adds an edge between vertices in the graph
 * @graph: the graph to which the edge should be added
 * @src: the content of the source vertex
 * @dest: the content of the destination vertex
 * @type: the type of edge (UNIDIRECTIONAL or BIDIRECTIONAL)
 * Return: 1 on success, 0 on failure
 */

int graph_add_edge(graph_t *graph, const char *src,
		   const char *dest, edge_type_t type)
{
	vertex_t *temp_vertex = NULL, *src_vertex = NULL, *dest_vertex = NULL;

	if (!graph || !src || !dest ||
	    type < UNIDIRECTIONAL || type > BIDIRECTIONAL)
		return (0);

	for (temp_vertex = graph->vertices; temp_vertex; temp_vertex =
		     temp_vertex->next)
	{
		if (strcmp(temp_vertex->content, src) == 0)
			src_vertex = temp_vertex;
		else if (strcmp(temp_vertex->content, dest) == 0)
			dest_vertex = temp_vertex;
	}

	if (!src_vertex || !dest_vertex)
		return (0);

	if (graph_add_single_edge(src_vertex, dest_vertex) == NULL)
		return (0);

	if (type == BIDIRECTIONAL)
	{
		if (graph_add_single_edge(dest_vertex, src_vertex) == NULL)
			return (0);
	}

	return (1);
}
