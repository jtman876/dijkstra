#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int vertex;
    int weight;

    struct node *next;
} Node;

typedef struct {
    Node **adj_lists;
    int   *visited;
    int    vertices_count;
    int    source;
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

Node *create_node(int v);
Graph create_graph();
void  print_graph(Graph graph);

void add_node(Graph *graph, int s);
void add_edge(Graph *graph, int u, int v, int w);

void dijkstra(Graph graph);

int main() {
    Graph graph = input();
    dijkstra(graph);
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

void print_graph(Graph graph) {
    for (int v = 0; v < graph.vertices_count; v++) {
        Node *temp = graph.adj_lists[v];
        printf("\n Vertex %d:\n ", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

Graph input() {
    printf("Enter the number of vertices (n) and edges (m) there are: ");

    int num_vertices = next_number();
    int num_edges = next_number();

    Graph graph = create_graph();
    printf("Enter one edge per line with 2 vertices and the weight.\n");
    for (int i = 0; i < num_edges; i++) {
        int u = next_alnum();
        int v = next_alnum();
        int w = next_alnum();
        add_edge(&graph, u, v, w);
    }
    printf("Enter the source node: ");
    graph.source = next_alnum();
    return graph;
}

Graph create_graph() {
    Graph graph;
    int   vertices = 100;
    graph.vertices_count = vertices;
    graph.adj_lists = malloc(vertices * sizeof(Node *));
    graph.visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph.adj_lists[i] = NULL;
        graph.visited[i] = 0;
    }
    return graph;
}

Node *create_node(int v) {
    Node *new_node = malloc(sizeof(Node));
    new_node->vertex = v;
    new_node->next = NULL;
    return new_node;
}

void add_edge(Graph *graph, int u, int v, int w) {
    Node *new_node = create_node(u);
    new_node->next = graph->adj_lists[v];
    graph->adj_lists[v] = new_node;
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

void dijkstra(Graph graph) {}
