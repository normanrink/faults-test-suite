/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdio.h>
#include <stdlib.h>

#include "mycheck.h"
#include "mycyc.h"


#define log0(...)      printf(__VA_ARGS__)
#define log1(...)      printf(__VA_ARGS__)
#define ADJ_PRINT(...) printf(__VA_ARGS__)


extern const long max_num_nodes;

extern void ___enc_init(unsigned long);
extern void ___enc_finish();
extern void ___enc_dijkstra(long, long, long);
extern long ___enc_get_node_dist(unsigned long);
extern long ___enc_get_node_pred(unsigned long);
extern long ___enc_get_matrix_elt(unsigned long, unsigned long);
extern void ___enc_print_path(long);


int main(int argc, char *argv[]) {
  uint64_t t1, t2, total = 0;
  const long num_nodes = (LENGTH < max_num_nodes) ? LENGTH : max_num_nodes;

#ifdef DEBUG
  fprintf(stderr, "LENGTH=%ld\n", num_nodes);

  ADJ_PRINT("Adjacency matrix:\n");
  for (long i = 0; i < num_nodes; i++) {
    for (long j = 0; j < num_nodes; j++) {
       ADJ_PRINT("(%02ld,%02ld)%4ld; ", i, j, ___enc_get_matrix_elt(i, j));
    }
    ADJ_PRINT("\n");
  }
  ADJ_PRINT("\n");
#endif

  __cs_fopen(argc, argv);
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_log(argc, argv);
  __cs_reset();
#endif

  ___enc_init(num_nodes);

  /* find NUM_NODES shortest paths between nodes */
  long j = num_nodes / 2;
  for (long i = 0; i < num_nodes; i++) {
    t1 = __cyc_rdtsc();
    ___enc_dijkstra(i, j, num_nodes);
    t2 = __cyc_rdtsc();
    total += t2 - t1;

    if (i == j) {
#ifdef DEBUG
      log0("Shortest path is 0 in cost. Just stay where you are.\n");
#endif
    } else {
      for(long k = 0; k < num_nodes; k++) {
        __cs_facc(___enc_get_node_dist(k));
        __cs_facc(___enc_get_node_pred(k));
#if (defined DEBUG) || (defined CHECKSUM)
        __cs_acc(___enc_get_node_dist(k));
        __cs_acc(___enc_get_node_pred(k));
#endif
      }
#ifdef DEBUG
      log1("Shortest path is %ld in cost. ", ___enc_get_node_dist(j));
      log0("Path is: ");
      ___enc_print_path(j);
      log0("\n");
#endif
    }
    j++;
    j = j % num_nodes;
  }

  ___enc_finish();

  __cs_fclose();
#if (defined DEBUG) || (defined CYCLES)
  __cyc_msg(total);
#endif
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_msg();
#endif

  return 0;
}
