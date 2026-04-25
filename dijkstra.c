#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int          vertex;
    struct Node *next;
} Node;

typedef struct {
    int   *visited;
    int    vertices_count;
    Node **adj_lists;
} Graph;

typedef struct {
    int *min_heap;
    int  size;
    int  max;
} PriorityQueue;

Graph input();
void  swap(int *a, int *b);
void  insert_queue(PriorityQueue *queue, int num);
void  heapify(PriorityQueue *queue, int i);

Node  *create_node(int v);
Graph *create_graph(int vertices);

void add_node(Graph *graph, int s);
void edge(Graph *graph, int s, int d);

int main() {
    input();
    return 0;
}

int next_number() {
    char current;
    do {
        current = getchar();
    } while (!isdigit(current));

    char buffer[10];
    int  num_digits = 0;

    do {
        buffer[num_digits] = current;
        num_digits++;
        current = getchar();
    } while (isdigit(current));
    buffer[num_digits] = '\0';

    return atoi(buffer);
}

int next_alnum() {
    char current;
    do {
        current = getchar();
    } while (!isalnum(current));
    return current;
}

Graph input() {
    printf("Enter the number of vertices (n) and edges (m) there are: ");

    int num_vertices = next_number();
    int num_edges = next_number();

    Graph graph = create_graph(num_vertices);
    printf("Enter one edge per line with 2 vertices and the weight.\n");
    for (int i = 0; i < num_edges; i++) {
        int u = next_alnum();
        int v = next_alnum();
        int w = next_alnum();
        add_edge(graph, u, v, w);
    }
    printf("Enter the source node: ");
    graph.source = next_alnum();
    return graph
}

/** Helper functions **/
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/** Queue functions */
void insert_queue(PriorityQueue *queue, int num) {
    queue->min_heap[queue->size] = num;
    queue->size += 1;
    int temp;

    int current = queue->size - 1;
    while (current != 0) {
        int parent = (current - 1) / 2;
        if (queue->min_heap[current] < queue->min_heap[parent]) {
            swap(&queue->min_heap[current], &queue->min_heap[parent]);
            current = parent;
        } else {
            break;
        }
    }
}

void heapify(PriorityQueue *queue, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < queue->size && queue->min_heap[l] > queue->min_heap[largest]) {
        largest = l;
    }
    if (r < queue->size && queue->min_heap[r] > queue->min_heap[largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(&queue->min_heap[i], &queue->min_heap[largest]);
        heapify(queue, largest);
    }
}
