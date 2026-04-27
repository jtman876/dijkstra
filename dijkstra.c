#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define VERTICES 200
#define INF 1000

typedef struct node {
    int          vertex;
    int          weight;
    struct node *next;
} Node;

typedef struct {
    Node **adj_lists;
    int    vertices_count;
    int    source;
} Graph;

typedef struct {
    int   size;
    int   max;
    Node *min_heap;
} PriorityQueue;

Graph input();
void  swap_nodes(Node *a, Node *b);
void  insert_queue(PriorityQueue *queue, Node node, int *distance);
void  heapify(PriorityQueue *queue, int i, int *distance);

Node dequeue(PriorityQueue *queue, int *distance);

Node *create_node(int v, int w);
Graph create_graph();
void  print_graph(Graph graph);

void add_edge(Graph *graph, int u, int v, int w);

void dijkstra(Graph graph);
void print_path(int parent[], int v, int arrow);

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
    /* Example input:
     * a b 5 | a c 2 | b c 1 | b d 4 | b e 2 | c e 7 | d e 6 | d f 3 | e f 1
     */
    for (int i = 0; i < num_edges; i++) {
        int u = next_alnum();
        int v = next_alnum();
        int w = next_number();
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

    for (int i = 0; i < vertices; i++) {
        graph.adj_lists[i] = NULL;
    }
    return graph;
}

Node *create_node(int v, int w) {
    Node *new_node = malloc(sizeof(Node));
    new_node->vertex = v;
    new_node->weight = w;
    new_node->next = NULL;
    return new_node;
}

void add_edge(Graph *graph, int u, int v, int w) {
    Node *new_node = create_node(v, w);
    new_node->next = graph->adj_lists[u];
    graph->adj_lists[u] = new_node;
}

/** Helper functions **/
void swap_nodes(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

/** Queue functions **/
void insert_queue(PriorityQueue *queue, Node node, int *distance) {
    queue->min_heap[queue->size] = node;
    queue->size += 1;
    int temp;

    int current = queue->size - 1;
    while (current != 0) {
        int parent = (current - 1) / 2;
        if (distance[queue->min_heap[current].vertex] <
            distance[queue->min_heap[parent].vertex]) {
            swap_nodes(&queue->min_heap[current], &queue->min_heap[parent]);
            current = parent;
        } else {
            break;
        }
    }
}

void heapify(PriorityQueue *queue, int i, int *distance) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < queue->size && distance[queue->min_heap[l].vertex] <
                               distance[queue->min_heap[smallest].vertex]) {
        smallest = l;
    }
    if (r < queue->size && distance[queue->min_heap[r].vertex] <
                               distance[queue->min_heap[smallest].vertex]) {
        smallest = r;
    }
    if (smallest != i) {
        swap_nodes(&queue->min_heap[i], &queue->min_heap[smallest]);
        heapify(queue, smallest, distance);
    }
}

Node dequeue(PriorityQueue *queue, int *distance) {
    Node min = queue->min_heap[0];
    queue->min_heap[0] = queue->min_heap[queue->size - 1];
    queue->size -= 1;
    heapify(queue, 0, distance);
    return min;
}

void update_queue(PriorityQueue *queue, Node *node, int *distance) {
    int current;
    for (int i = 0; i < queue->size; i++) {
        if (queue->min_heap[i].vertex == node->vertex) {
            distance[queue->min_heap[i].vertex] = distance[node->vertex];
            current = i;
            break;
        }
    }
    while (current != 0) {
        int parent = (current - 1) / 2;
        if (distance[queue->min_heap[current].vertex] <
            distance[queue->min_heap[parent].vertex]) {
            swap_nodes(&queue->min_heap[current], &queue->min_heap[parent]);
            current = parent;
        } else {
            break;
        }
    }
}

/** Dijkstra's Algorithm **/
void dijkstra(Graph graph) {
    PriorityQueue queue;
    queue.size = 0;
    queue.max = graph.vertices_count;
    queue.min_heap = malloc(sizeof(Node) * graph.vertices_count);

    int parent[graph.vertices_count];
    int visited[graph.vertices_count];
    int distance[graph.vertices_count];

    // Place all vertices in the priority queue
    for (int i = 0; i < graph.vertices_count; i++) {
        distance[i] = (i == graph.source) ? 0 : INF;
        parent[i] = -1;
        visited[i] = 0;

        Node dest = {i, 0, NULL};
        insert_queue(&queue, dest, distance);
    }

    // Make a path for each vertex
    for (int i = 0; i < graph.vertices_count; i++) {
        Node node = dequeue(&queue, distance);
        visited[node.vertex] = 1;

        // Update neighbors to the current vertex
        Node *neighbor = graph.adj_lists[node.vertex];
        while (neighbor) {
            // If current node distance + the edge is better than neighbor dist
            if (!visited[neighbor->vertex] &&
                distance[node.vertex] + neighbor->weight <
                    distance[neighbor->vertex]) {

                distance[neighbor->vertex] =
                    distance[node.vertex] + neighbor->weight;
                parent[neighbor->vertex] = node.vertex;
                update_queue(&queue, neighbor, distance);
            }
            neighbor = neighbor->next;
        }
    }
    printf("\nAll paths and distances:\n");
    for (int i = 0; i < graph.vertices_count; i++) {
        if (i == graph.source || parent[i] == -1) {
            continue;
        }
        print_path(parent, i, 0);
        printf(": %d", distance[i]);
        printf("\n");
    }
}

/** Print the full path from the source node to node v **/
void print_path(int parent[], int v, int arrow) {
    if (v == -1) {
        return;
    }
    print_path(parent, parent[v], 1);
    printf("%c", v);
    if (arrow) {
        printf(" -> ");
    }
}
