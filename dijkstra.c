#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define VERTICES 100
#define INF 1000

typedef struct node {
    int          vertex;
    int          weight;
    int          distance;
    struct node *next;
} Node;

typedef struct {
    Node **adj_lists;
    int   *visited;
    int    vertices_count;
    int    source;
} Graph;

typedef struct {
    int   size;
    int   max;
    Node *min_heap;
} PriorityQueue;

typedef struct tree_node {
    int                vertex;
    int                path;
    struct tree_node  *parent;
    struct tree_node **children;
} TreeNode;

Graph input();
void  swap_nodes(Node *a, Node *b);
void  insert_queue(PriorityQueue *queue, Node node);
void  heapify(PriorityQueue *queue, int i);

Node dequeue(PriorityQueue *queue);

Node *create_node(int v, int w);
Graph create_graph();
void  print_graph(Graph graph);

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
    graph.visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph.adj_lists[i] = NULL;
        graph.visited[i] = 0;
    }
    return graph;
}

Node *create_node(int v, int w) {
    Node *new_node = malloc(sizeof(Node));
    new_node->vertex = v;
    new_node->weight = w;
    new_node->distance = INF;
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
void insert_queue(PriorityQueue *queue, Node node) {
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
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < queue->size &&
        queue->min_heap[l].distance < queue->min_heap[smallest].distance) {
        smallest = l;
    }
    if (r < queue->size &&
        queue->min_heap[r].distance < queue->min_heap[smallest].distance) {
        smallest = r;
    }
    if (smallest != i) {
        swap_nodes(&queue->min_heap[i], &queue->min_heap[smallest]);
        heapify(queue, smallest);
    }
}

Node dequeue(PriorityQueue *queue) {
    Node min = queue->min_heap[0];
    queue->min_heap[0] = queue->min_heap[queue->size - 1];
    queue->size -= 1;
    heapify(queue, 0);
    return min;
}

void update_queue(PriorityQueue *queue, Node *node) {
    int current;
    for (int i = 0; i < queue->size; i++) {
        if (queue->min_heap[i].vertex == node->vertex) {
            queue->min_heap[i].distance = node->distance;
            // heapify(queue, i);
            // return;
            current = i;
            break;
        }
    }
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

void print_path(int parent[], int v) {
    if (v == -1) {
        return;
    }
    print_path(parent, parent[v]);
    printf("%c ", v);
}

/** Dijkstra's Algorithm **/
void dijkstra(Graph graph) {
    printf("Starting dijkstra's\n");
    print_graph(graph);
    PriorityQueue queue;
    queue.size = 0;
    queue.max = graph.vertices_count;
    queue.min_heap = malloc(sizeof(Node) * graph.vertices_count);
    int parent[graph.vertices_count];
    int visited[graph.vertices_count];
    for (int i = 0; i < graph.vertices_count; i++) {
        int path = (i == graph.source) ? 0 : INF;
        parent[i] = -1;
        visited[i] = 0;

        // TODO: nodes in queue will be destoryed at end of function
        Node dest = {i, 0, path, NULL};
        printf("Build vars.\n");
        insert_queue(&queue, dest);
        printf("Inserted queue.\n");
    }
    printf("Priority queue initialized.\n");

    // for (int i = 0; i < graph.vertices_count; i++) {
    //     if (queue.min_heap[i].distance == 0) {
    //         printf("Source: %d", queue.min_heap[i].vertex);
    //     }
    // }

    // init tree

    Node     *head;
    TreeNode *treeHead;
    for (int i = 0; i < graph.vertices_count; i++) {
        // printf("Popping min node.\n");
        Node node = dequeue(&queue);
        visited[node.vertex] = 1;
        // insert the next node into the tree from its source
        // loop over neighbors of u
        /*
         * All neighbors are initialized to have a distance of INF
         * All nodes are initialized with no weight and no next node
         * Not all vertices will be initialized with a distance of INF?
         * Once a node is dequeued, it should have a parent. If it doesn't, it
         * is the head
         *
         */

        // Node children[VERTICES];
        // if (!node.next) { // TODO: what is the lifetime of node? pointer?
        //     head = &node;
        // } else {
        //     children[i] = node;
        // }
        //
        // TreeNode *treeNode = (TreeNode *)malloc(sizeof(TreeNode *));
        // treeNode->vertex = node.vertex;
        // treeNode->parent = node.next;
        // treeNode->parent->children[i] = &treeNode;
        //
        // if (!node.next) {
        // }
        //
        // TODO: INPUT GRAPH
        // a b 5 a c 2 b c 1 b d 4 b e 2 c e 7 d e 6 d f 3 e f 1
        // 1 2 5 1 3 2 2 3 1 2 4 4 2 5 2 3 5 7 4 5 6 4 6 3 5 6 1

        // printf("Checking min node %d with distance %d\n", node.vertex,
        //        node.distance);
        Node *neighbor = graph.adj_lists[node.vertex];
        // Update distance for every neighbor
        while (neighbor) {
            // if current node distance + the edge is better than neighbor dist
            printf("%c has neighbor %c\n", node.vertex, neighbor->vertex);
            printf("%d < %d\n", node.distance + neighbor->weight,
                   neighbor->distance);
            if (!visited[neighbor->vertex] &&
                node.distance + neighbor->weight < neighbor->distance) {
                neighbor->distance = node.distance + neighbor->weight;
                printf("New distance for %d: %d\n", neighbor->vertex,
                       neighbor->distance);
                parent[neighbor->vertex] = node.vertex;
                // neighbor->next = &node; // Neighbor's parent is node
                // Problem: we are altering the graph. We need to change the
                // queue Or, we can have a predecessor array
                update_queue(&queue, neighbor);
                printf("Now the first node is %d\n", queue.min_heap[0].vertex);
            }
            neighbor = neighbor->next;
        }
    }
    printf("dijkstra complete\n");
    for (int i = '2'; i < '7'; i++) {
        print_path(parent, i);
        printf("\n");
    }
    // return tree
}

void traversal() {
    // All weights start at 0
    // Have a list of children to recursively traverse up the tree
    // Print each node backward as recursion finishes.
    // Set the weight to 1 if a node has been visited.
    // You can't start a recursion on a node that equals 1
}
