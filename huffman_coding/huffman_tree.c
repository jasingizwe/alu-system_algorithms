#include "huffman.h"

/**
 * freeNestedNode - program that is used as a free_data parameter
 * to heap_delete
 * this function frees memory allocated for a binary_tree_node_t node
 * containing a symbol_t struct
 * @data: the void pointer intended to be cast into binary_tree_node_t pointer
 * Return: nothing (void)
 */

void freeNestedNode(void *data)
{
	binary_tree_node_t *node_data = NULL;
	symbol_t *symbol_data = NULL;

	node_data = (binary_tree_node_t *)data;

	if (node_data)
	{
		symbol_data = (symbol_t *)(node_data->data);

		if (symbol_data)
			free(symbol_data);

		free(node_data);
	}
}

/**
 * huffman_tree - program that constructs a Huffman tree using character data
 * and their frequencies
 * @data: an array of characters
 * @freq: an array of frequencies associated with characters
 * @size: the size of the arrays
 * Return: a pointer to the root node of the constructed Huffman tree
 */

binary_tree_node_t *huffman_tree(char *data, size_t *freq, size_t size)
{
	heap_t *priority_queue = NULL;
	binary_tree_node_t *huffman_root = NULL;

	if (!data || !freq || size == 0)
		return (NULL);

	priority_queue = huffman_priority_queue(data, freq, size);

	if (!priority_queue)
		return (NULL);

	while (priority_queue->root && (priority_queue->root->left ||
					priority_queue->root->right))
	{
		if (!huffman_extract_and_insert(priority_queue))
		{
			heap_delete(priority_queue, freeNestedNode);
			return (NULL);
		}
	}

	huffman_root = (binary_tree_node_t *)(priority_queue->root->data);
	heap_delete(priority_queue, NULL);

	return (huffman_root);
}
