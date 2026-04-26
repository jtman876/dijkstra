#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define VERTICES 100
#define INF 1000

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

typedef struct destination_node {
    int vertex;
    int distance;

    struct destination_node *next;
} DestinationNode;

typedef struct {
    int size;
    int max;

    DestinationNode *min_heap;
} PriorityQueue;

Graph input();
void  swap_nodes(DestinationNode *a, DestinationNode *b);
void  insert_queue(PriorityQueue *queue, DestinationNode node);
void  heapify(PriorityQueue *queue, int i);

DestinationNode dequeue(PriorityQueue *queue);

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

/** Input functions **/
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

/** Graph functions **/
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

Graph create_graph() {
    Graph graph;
    int   vertices = VERTICES;
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
void swap_nodes(DestinationNode *a, DestinationNode *b) {
    DestinationNode temp = *a;
    *a = *b;
    *b = temp;
}

/** Queue functions **/
void insert_queue(PriorityQueue *queue, DestinationNode node) {
    queue->min_heap[queue->size] = node;
    queue->size += 1;
    int temp;

    int current = queue->size - 1;
    while (current != 0) {
        int parent = (current - 1) / 2;
        if (queue->min_heap[current].distance <
            queue->min_heap[parent].distance) {
            swap_nodes(&queue->min_heap[current], &queue->min_heap[parent]);
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

    if (l < queue->size &&
        queue->min_heap[l].distance > queue->min_heap[largest].distance) {
        largest = l;
    }
    if (r < queue->size &&
        queue->min_heap[r].distance > queue->min_heap[largest].distance) {
        largest = r;
    }
    if (largest != i) {
        swap_nodes(&queue->min_heap[i], &queue->min_heap[largest]);
        heapify(queue, largest);
    }
}

/** Dijkstra's Algorithm **/
void dijkstra(Graph graph) {
    PriorityQueue queue;
    for (int i = 0; i < graph.vertices_count; i++) {
        Node *parent = NULL;
        int   path = (i == graph.source) ? 0 : INF;

        DestinationNode dest = {i, path, NULL};
        insert_queue(&queue, dest);
    }

    // init tree
    for (int i = 0; i < graph.vertices_count; i++) {
        DestinationNode node = dequeue(&queue);
        // insert the next node into the tree from its source
        // loop over neighbors of u
        Node *neighbor = graph.adj_lists[node.vertex];
        while (neighbor) {
            // if current node distance + the edge is better than neighbor dist
            if (node.distance + neighbor.path < neighbor.distance) {
            }
        }
    }
}
