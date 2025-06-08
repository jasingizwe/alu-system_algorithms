#ifndef QUEUES_H
#define QUEUES_H

/**
 * struct queue_node_s - Represents a node in the queue
 *
 * @ptr: Pointer to the data stored in the node
 * @next: Pointer to the next node in the queue
 * @prev: Pointer to the previous node in the queue
 */
typedef struct queue_node_s
{
	void *ptr;
	struct queue_node_s *next;
	struct queue_node_s *prev;
} queue_node_t;

/**
 * struct queue_s - Represents the full queue
 *
 * @front: Pointer to the node at the front of the queue
 * @back: Pointer to the node at the back of the queue
 */
typedef struct queue_s
{
	queue_node_t *front;
	queue_node_t *back;
} queue_t;

/*
 * FUNCTIONS
 */

/**
 * queue_create - Initializes and allocates memory for a new queue
 *
 * Return: Pointer to the newly created queue, or NULL on failure
 */
queue_t *queue_create(void);

/**
 * queue_push_back - Adds a new element to the back of the queue
 *
 * @queue: Pointer to the queue
 * @ptr: Data to be stored in the new node
 *
 * Return: Pointer to the newly created node, or NULL on failure
 */
queue_node_t *queue_push_back(queue_t *queue, void *ptr);

/**
 * queue_push_front - Adds a new element to the front of the queue
 *
 * @queue: Pointer to the queue
 * @ptr: Data to be stored in the new node
 *
 * Return: Pointer to the newly created node, or NULL on failure
 */
queue_node_t *queue_push_front(queue_t *queue, void *ptr);

/**
 * dequeue - Removes the front element from the queue and returns its data
 *
 * @queue: Pointer to the queue
 *
 * Return: Pointer to the data of the dequeued node, or NULL on failure
 */
void *dequeue(queue_t *queue);

/**
 * queue_delete - Frees all memory used by the queue
 *
 * @queue: Pointer to the queue to be deleted
 */
void queue_delete(queue_t *queue);

#endif
