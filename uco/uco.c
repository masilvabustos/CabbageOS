
#if __GNUC__ == 4 && __GNUC_MINOR__ == 4

#define __fetch_and_add(a,b) __sync_fetch_and_add(a,b)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
struct _decision_queue {
	struct _decision_queue* dq_link;
};

struct _decision_queue* __queue_and_peek(struct _decision_queue* *q, struct _decision_queue *x)
{
	struct _decision_queue* peek = *q, *d;

	x -> dq_link = NULL;

	while((d = __sync_val_compare_and_swap(q, NULL, x)) != NULL)
		q = & d->dq_link;

	return peek;
}

void __free_dq(struct _decision_queue *q, void (*free)(struct _decision_queue *))
{
	if (q == NULL)
		return;

	__free_dq(q->dq_link, free);

	free(q);
}



typedef unsigned int 	uco_seq_t;
typedef int 		uco_cell_ref_t;
typedef int		uco_state_t; 		
typedef int (*uco_inv_t)(void**,void*);

struct uco_threading_state { 
	int head;
	int announce;
};

struct uco {
	unsigned cnum; /**<< consensus number */
	struct uco_threading_state threading_state[];
};

/** The universal consensus object cell */

struct uco_cell {
	long int 	  	 seq;   /**< Sequence number for operation ordering */
	struct _decision_queue*  state; /**< The object state at the cell operation */
	int                      after; /**< A link to the next cell */

	unsigned int            count;   /**< Reference count */
        int      		before; /** A link for garbage collecting */
};

/** UCO initialization */

#define UCO_INIT ((struct uco_cell) {0, NULL, NULL})

struct uco_cell* uco_initialize_cell(struct uco_cell* cell)
{
	cell -> seq   = 0;
	cell -> after = 0;
	cell -> before = 0;
	cell -> state = 0;
	
	return cell;
}

#define UCO_CELLS_NO(N) (N*N + N + 1)

/** Decision procedures */

/* Since all atomic objects can be allocated in a single one, these procedures perform each operation */

int uco_cell_decide_after(struct uco_cell* cell, int prefer)
{
	int ret;
	ret = __sync_val_compare_and_swap(&cell->after, 0, prefer);
	return (ret == 0) ? prefer : ret;
}

struct _decision_queue* uco_cell_decide_state(struct uco_cell* cell, struct _decision_queue* prefer)
{
	return __queue_and_peek(&cell->state, prefer);
}

int uco_cell_decrement_counter(struct uco_cell* cell)
{
	return __fetch_and_add(&cell->count, -1);
}

#if 1
void uco_release_cells(int head, int n, struct uco_cell cell_pool[])
{
	struct uco_cell *cell = & cell_pool[cell_pool[head].before];

#ifdef DEBUG
	printf("release begin: [%lx] %d\n", pthread_self(), head);
#endif

	do {
		
		struct uco_cell *before = & cell_pool[cell -> before];
		int count = uco_cell_decrement_counter(cell);
#ifdef DEBUG
		printf("release: %lx %ld %d\n", pthread_self(), cell->seq, count);
#endif
		if (count == 0){
			fflush(stdout);
			abort();
		}

#if 0
		if (count == 1){
			__free_dq(cell->state, free);
			cell->state = 0;
			uco_cell_decrement_counter(cell);
		}
#endif
	
		cell = before;
	} while(n--);
}
#endif

int uco_find_free_cell(struct uco_cell cell_pool[], int n)
{
	int i;

	for (i=0; i<n; ++i)
		if (cell_pool[i].count == 0)
			break;
	if (i == n)
		abort();
	__free_dq(cell_pool[i].state, (void (*)(struct _decision_queue*)) free);
	return i;
}

struct _decision_queue* uco_thread_cell(int p, int n, int initialized_cell, struct uco_cell cell_pool[], struct uco_threading_state ts[], void (*do_invocation)(int, struct _decision_queue**, struct _decision_queue*))
{
	int q;
	struct _decision_queue* new_state;

	cell_pool[initialized_cell].count = n +1;
	ts[p].announce = initialized_cell;
	
	for (q=n-1; q >= 0; --q)
		ts[p].head = ts[((cell_pool[ts[q].head].seq > cell_pool[ts[p].head].seq) ? q : p)].head;

	while(cell_pool[ts[p].announce].seq == 0) {
		int head    = ts[p].head;
		int help    = ts[cell_pool[head].seq % n].announce;
		int prefer  = (cell_pool[help].seq == 0) ? help : ts[p].announce;
	        int after   = uco_cell_decide_after(&cell_pool[head], prefer);

		do_invocation(after, &new_state, cell_pool[head].state);
		uco_cell_decide_state(&cell_pool[after], new_state);

		cell_pool[after].before   = head;
		cell_pool[after].seq      = cell_pool[head].seq + 1;
		
		ts[p].head	= after;
        }
      
	ts[p].head = ts[p].announce;

	uco_release_cells(ts[p].head, n, cell_pool);

        return new_state;
}

#define N 5


enum my_opcode {
	ENQ,
	DEQ
};

enum my_result {
	OK,
	FULL,
	EMPTY
};

struct my_state {
	struct _decision_queue dq;
	enum my_result result;
	int first;
	int last;
	struct {int p; int x;} queue[40];
};


struct my_op {
	enum my_opcode opcode;
	int p;
	int arg;
};


struct my_state initial_state = {{NULL}, OK};
struct uco_cell cell_pool[N*UCO_CELLS_NO(N) + 1] = {{1, &initial_state.dq, 0, -1, 0}};

struct my_op my_op_pool[sizeof(cell_pool)/sizeof(typeof(cell_pool[0]))];

 
#include <string.h>

void do_invocation(int op_cell, struct _decision_queue ** new_dq, struct _decision_queue *old_dq)
{
	struct my_state **new = (struct my_state**) new_dq;
	struct my_state *old = (struct my_state*) old_dq;
	struct my_op *op = & my_op_pool[op_cell];

	*new = (struct my_state*) malloc(sizeof(struct my_state));
	memcpy(*new, old, sizeof(struct my_state));

	switch (op -> opcode) {
		int last;
	case ENQ:
		last = (old->last + 1) % 40;
		if (last == old->first) {
			(*new) -> result = FULL;
			break;
		}
		(*new) -> last = last;
		(*new)->queue[(*new) -> last].x = op -> arg;
		(*new)->queue[(*new) -> last].p = op -> p;
		(*new) -> result = OK;
		break;
	case DEQ:
		if (old->first == old->last) {
			(*new) -> result = EMPTY;
			break;
		}
		(*new) -> first += 1;
		(*new) -> first %= 40;
		(*new) -> result = OK;

		break;
	
	}
}

#include <stdio.h>

struct uco_threading_state threading_state[N];


struct uco_cell initial_cell = {1, &initial_state.dq, 0, -1, 0};

void* producer(void* x)
{
	int p = * (int*) x;
	int i = 0;
	struct my_state *s;
	/*initial_cell.state = &initial_state.dq;*/
        int op_cell;

	printf("producer start\n");
	while (i>=0) {
		op_cell = 1 + p*UCO_CELLS_NO(N) + uco_find_free_cell(&cell_pool[1+p*UCO_CELLS_NO(N)], UCO_CELLS_NO(N));	
		uco_initialize_cell(&cell_pool[op_cell]);
		my_op_pool[op_cell].opcode = ENQ;
		my_op_pool[op_cell].p = p;
		my_op_pool[op_cell].arg = i;
		s = (struct my_state*) uco_thread_cell(p, N, op_cell, cell_pool, threading_state, do_invocation);
		
		if (s->result == FULL){
			sched_yield();
			continue;
		}
		printf("[%07ld] producer: (%d) %d\n", cell_pool[op_cell].seq, p, i);
		fflush(stdout);
		++i;
	}

	return NULL;
}

void* consumer(void* x)
{
	int p = * (int*) x;
	int i = 0;
	struct my_state *s;
	/*initial_cell.state = &initial_state.dq;*/
	int op_cell;

	while (i>=00) {
		op_cell	= 1 + p*UCO_CELLS_NO(N) + uco_find_free_cell(& cell_pool[1 + p*UCO_CELLS_NO(N)], UCO_CELLS_NO(N));
		uco_initialize_cell(& cell_pool[op_cell]);
		my_op_pool[op_cell].p = p;
		my_op_pool[op_cell]. opcode = DEQ;
		s = (struct my_state*) uco_thread_cell(p, N, op_cell, cell_pool, threading_state, do_invocation);

		if (s->result == EMPTY){
			sched_yield();
			continue;
		}
		printf("[%07ld] consumer[%d]: (%d) %d\n", cell_pool[op_cell].seq, p, s->queue[s->first].p, s->queue[s->first].x);
		++ i;
	}

	return NULL;

}


#include <unistd.h>

int main()
{
	
	int i;
	pthread_t t[N];
	int p[N];
	
	
	for (i=0; i<N; i++)
		threading_state[i].head = threading_state[i].announce = 0;

	for (i=0; i<N; ++i)
		p[i] = i;

	
	pthread_create(&t[0], NULL, consumer, &p[0]);
	pthread_create(&t[1], NULL, producer, &p[1]);
	pthread_create(&t[2], NULL, producer, &p[2]);	
	pthread_create(&t[3], NULL, consumer, &p[3]);
	
	pause();
	
	return 0;
}
