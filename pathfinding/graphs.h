#ifndef GRAPHS_HEADER
#define GRAPHS_HEADER

#include <stddef.h>

/**
 * enum edge_type_e - Describes the types of connections between two vertices
 *
 * @UNIDIRECTIONAL: Connection in only one direction
 * @BIDIRECTIONAL: Connection in both directions (two-way)
 */
typedef enum edge_type_e
{
	UNIDIRECTIONAL = 0,
	BIDIRECTIONAL
} edge_type_t;

/* Temporary forward declaration of vertex_t for usage in edge_t */
typedef struct vertex_s vertex_t;

/**
 * struct edge_s - Represents an edge in the adjacency list
 * Each vertex can be connected to multiple other vertices (edges)
 *
 * @dest: Pointer to the vertex this edge connects to
 * @next: Pointer to the next edge in the list
 * @weight: Weight of this edge (for weighted graphs)
 */
typedef struct edge_s
{
	vertex_t *dest;
	struct edge_s *next;
	int weight;
} edge_t;

/**
 * struct vertex_s - Represents a vertex in the adjacency list
 *
 * @index: The vertex's index in the adjacency list (assigned when added)
 * @content: Custom data (here, it's a string)
 * @x: X coordinate of the vertex
 * @y: Y coordinate of the vertex
 * @nb_edges: Number of edges (connections) this vertex has
 * @edges: Pointer to the head of the linked list of edges for this vertex
 * @next: Pointer to the next vertex in the list
 *
 * Note: @next only points to the next vertex in the list,
 *       NOT to a connected vertex (that's what the edges are for).
 */
struct vertex_s
{
	size_t index;
	char *content;
	int x;
	int y;
	size_t nb_edges;
	edge_t *edges;
	vertex_t *next;
};

/**
 * struct graph_s - The full graph represented as an adjacency list
 * The graph is a linked list of vertices, where each vertex has its own
 * linked list of edges.
 *
 * @nb_vertices: Number of vertices in the graph
 * @vertices: Pointer to the head of the linked list of vertices
 *
 * Graph visualization (vertices linked by @next, edges linked by @edges):
 *
 * @vertices
 *     |
 *  -----------      -----------      -----------
 * | vertex_t | -> edges | edge_t | -> | edge_t | -> ...
 *  -----------      -----------      -----------
 *     |
 *    next
 *  -----------      -----------      -----------
 * | vertex_t | -> edges | edge_t | -> | edge_t | -> ...
 *  -----------      -----------      -----------
 *     |
 *    next
 *  ...
 */
typedef struct graph_s
{
	size_t nb_vertices;
	vertex_t *vertices;
} graph_t;

/*
 * FUNCTIONS
 */

/**
 * graph_create - Initializes an empty graph structure
 *
 * Return: Pointer to the newly created graph, or NULL on failure
 */
graph_t *graph_create(void);

/**
 * graph_add_vertex - Adds a new vertex to the graph
 *
 * @graph: Pointer to the graph structure
 * @str: String content of the vertex
 * @x: X coordinate of the vertex
 * @y: Y coordinate of the vertex
 *
 * Return: Pointer to the created vertex, or NULL on failure
 */
vertex_t *graph_add_vertex(graph_t *graph, char const *str, int x, int y);

/**
 * graph_add_edge - Adds an edge between two vertices
 *
 * @graph: Pointer to the graph structure
 * @src: Source vertex (string representing it)
 * @dest: Destination vertex (string representing it)
 * @weight: Weight of the edge (0 for unweighted)
 * @type: Type of connection (Unidirectional or Bidirectional)
 *
 * Return: 1 on success, 0 on failure
 */
int graph_add_edge(graph_t *graph, char const *src, char const *dest,
		   int weight, edge_type_t type);

/**
 * graph_delete - Frees all the memory associated with the graph
 *
 * @graph: Pointer to the graph to be deleted
 */
void graph_delete(graph_t *graph);

/**
 * graph_display - Displays the graph in an adjacency list format
 *
 * @graph: Pointer to the graph structure
 */
void graph_display(graph_t const *graph);

#endif
