
#include <stdio.h>
#include <stdlib.h>


#define LOG_ENQ(node, dist, prev) \
  /* printf("en node: %ld dist: %ld pred: %ld -- \n", (node), (dist), (prev)) */
#define LOG_DEQ(node, dist, prev) \
  /* printf("de node: %ld dist: %ld pred: %ld -- \n", (node), (dist), (prev)) */


#define NONE        9999


extern const long max_num_nodes;
extern long AdjMatrix[];



// Represent a node along a path as a tuple of
//   1) its index into the adjacency matrix structure,
//   1) its distance from the start node,
//   2) the index of its predecessor along the path.
struct _PATH_NODE
{
  long index;
  long dist;
  long pred;
};
typedef struct _PATH_NODE PATH_NODE;
// Global array of visited nodes:
static PATH_NODE *g_visited;



// Simple implementation of a queue of PATH_NODEs':
struct _QITEM
{
  struct _PATH_NODE *node;
  struct _QITEM *next;
};
typedef struct _QITEM QITEM;
// Global variables storing the head of the queue
// and the number of elemtns in the queue:
static QITEM *g_qHead = NULL;
static long g_qCount = 0;
// Global variable to hold an array of the maximum number of
// 'QITEMs' that will be needed during the computation.
// (In our implementation we never enqueue nodes twice. Therefore
// we never need more 'QITEMs' than there are nodes.)
static QITEM *g_qItems = NULL;

QITEM *create_item(PATH_NODE *node) __attribute__((always_inline));
QITEM *create_item(PATH_NODE *node) {
  QITEM *qItem = g_qItems + node->index;
  qItem->node = node;    
  qItem->next = NULL;
  return qItem;
}

void destroy_item(QITEM *) __attribute__((always_inline));
void destroy_item(QITEM *item) {
  ;
}

void enqueue(PATH_NODE *) __attribute__((always_inline));
void enqueue(PATH_NODE *node) {
  LOG_ENQ(node->index, node->dist, node->pred);
  if (!g_qHead) {
    // The queue is empty. Hence add 'qItem' as the head
    // of the queue:
    g_qHead = create_item(node);
    // Update counter of elements in the queue:
    g_qCount++;
  } else {
    QITEM *qLast = g_qHead;
    while (qLast->next) {
      // Detect if 'node' is already in the queue to avoid
      // that nodes are enqueued twice:
      if (qLast->node == node) {
        return;
      }
      qLast = qLast->next;
    }
    // We have not yet checked the last node in the queue:
    if (qLast->node == node) {
      return;
    }
    // 'node' is NOT already in the queue:
    qLast->next = create_item(node);
    // Update counter of elements in the queue:
    g_qCount++;
  }
}

PATH_NODE *dequeue() __attribute__((always_inline));
PATH_NODE *dequeue() {
  PATH_NODE *result = NULL;

  if (g_qHead) {
    QITEM *qSecond = g_qHead->next;
    result = g_qHead->node;

    destroy_item(g_qHead);

    g_qHead = qSecond;
    g_qCount--;

    LOG_DEQ(result->index, result->dist, result->pred);
  }
  return result;
}

long qcount() __attribute__((always_inline));
long qcount() {
  return(g_qCount);
}



void ___enc_init(unsigned long num_nodes) {
  g_visited = (PATH_NODE*)malloc(sizeof(PATH_NODE) * num_nodes);
  for (long n = 0; n < num_nodes; n++)
    g_visited[n].index = n;

  g_qItems = (QITEM*)malloc(sizeof(QITEM) * num_nodes);
}

void ___enc_finish() {
  free(g_visited);
  free(g_qItems);
}

long ___enc_get_node_dist(unsigned long node) {
  return g_visited[node].dist;
}

long ___enc_get_node_pred(unsigned long node) {
  return g_visited[node].pred;
}



// Printing of that path which finishes at
// node 'nend':
// (Since prinitng of paths is implemented
// recursively, we need an '___enc' wrapper
// function.)
void print_path(long nend);

void ___enc_print_path(long nend) {
  print_path(nend);
}

// Recursive printing of nodes along
// a path finishing at node 'nend':
void print_path(long nend) {
  if (g_visited[nend].pred != NONE) {
    print_path(g_visited[nend].pred);
  }
  printf(" %ld", nend);
}



// Function which implements Dijkstra's algorithm:
void ___enc_dijkstra(long nstart, long nend, long num_nodes) __attribute__((noinline));
void ___enc_dijkstra(long nstart, long nend, long num_nodes) {
  for (long n = 0; n < num_nodes; n++) {
    g_visited[n].dist = NONE;
    g_visited[n].pred = NONE;
  }

  if (nstart == nend) {
    return;
  } else {
    g_visited[nstart].dist = 0;
    g_visited[nstart].pred = NONE;
    enqueue(&g_visited[nstart]);

    while (qcount() > 0) {
      PATH_NODE *pn = dequeue();
      const long pn_index = pn->index;
      const long pn_dist = pn->dist;

      for (long i = 0; i < num_nodes; i++) {
        const long adj_index = pn_index*max_num_nodes + i;
        const long cost = AdjMatrix[adj_index];
        // 'cost' equals 'NONE' if there is no edge between node 'pn'
        // and node 'i':
        if (cost != NONE) {
          const long dist = g_visited[i].dist;
          // Either if the distance to node 'i' has not yet been calculated
          // or if the distance to node 'i' via 'pn' is smaller than the
          // previously computed distance, update node 'i':
          if ((dist == NONE) || (dist > (pn_dist + cost))) {
            g_visited[i].dist = pn_dist + cost;
            g_visited[i].pred = pn_index;
            enqueue(&g_visited[i]);
          }
        }
      }
    }
  }
}



long ___enc_get_matrix_elt(unsigned long i, unsigned long j) {
  if (i < max_num_nodes && j < max_num_nodes)
    return AdjMatrix[i*max_num_nodes + j];
  else
    return -1;
}
