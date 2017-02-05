
#if __GNUC__ == 4 && __GNUC_MINOR__ == 4

#define __fetch_and_add(a,b) __sync_fetch_and_add(a,b)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

typedef unsigned int 	uco_seq_t;
typedef int 		uco_cell_ref_t;
typedef int		uco_state_t; 		
typedef int (*uco_inv_t)(void**,void*);

struct uco_threading_state { 
	int head;
	int announce;
};

struct uco {
	unsigned consensus_number; /**<< consensus number */
	int (*do_invocation)(int,int,int);
	struct uco_threading_state threading_state[];
};

/** The universal consensus object cell */

struct uco_cell {
	long int      seq;   /**< Sequence number for operation ordering */

	int           after; /**< A link to the next cell */

	unsigned int  gc;   /**< GC info. Reference count */
        int           before; /** A link for garbage collecting */

	int          state; 
};

#define UCO_INITIAL_CELL (struct uco_cell) {.seq = 1, .after = -1, .gc = -1, .before = 0, .state = 0}

/** UCO initialization */

#define UCO_INIT ((struct uco_cell) {0, NULL, NULL})

struct uco_cell* uco_initialize_cell(struct uco_cell* cell, int n)
{
	cell -> seq    = 0;
	cell -> after  = -1;
	cell -> before = -1;
	cell -> state  = -1;
#if 0 
	cell -> gc  = n + 1;
#else
	cell -> gc = (1 << (n + 1)) - 1;
#endif
	
	
	return cell;
}

#define UCO_CELLS_NO(N) (N*N + N + 1)
#define UCO_STATE_NO(N) (N*N + 2*N)

/** Decision procedures */

/* Since all atomic objects can be allocated in a single one, these procedures perform each operation */

int uco_cell_decide_after(struct uco_cell* cell, int prefer)
{
	int ret;
	ret = __sync_val_compare_and_swap(&cell->after, -1, prefer);
	return (ret == -1) ? prefer : ret;
}

int uco_cell_decide_state(struct uco_cell* cell, int prefer)
{
	__sync_val_compare_and_swap(&cell->state, -1, prefer);
	return cell->state;
}

int uco_cell_decrement_counter(struct uco_cell* cell)
{
	return __sync_fetch_and_add(&cell->gc, -1);
}

#if 0
void uco_release_cells(int head, int n, struct uco_cell cell_pool[])
{
	struct uco_cell *cell = & cell_pool[cell_pool[head].before];
	int prev;
	
	do {

		prev = uco_cell_decrement_counter(cell);
		assert(prev > 0 || cell == & cell_pool[cell->before] );
		cell = & cell_pool[cell->before];
	} while(n--);
}

#else

void uco_release_cells(int head, int consensus_number, struct uco_cell cell_pool[])
{
	int n = consensus_number;

	struct uco_cell *cell = & cell_pool[cell_pool[head].before];

	int mask = 1;
	
	do {
		int prev;
		
		prev = __sync_fetch_and_and(&cell->gc, ~mask);

		assert(prev & mask);
		
		cell = & cell_pool [cell->before];
		mask <<= 1;
		
	} while (n--);
}
#endif

int uco_find_free_cell(struct uco_cell cell_pool[], int pool_size)
{
	int i;

	for (i=0; i<pool_size; ++i)
		if (cell_pool[i].gc == 0)
			break;

	assert (i < pool_size);
	
	return i;
}

/* Per-process info:
 * - process id 
 * - invocation context
 * - free cell (from its cell pool)

 */

int uco_thread_setup(struct uco *uco, int p, struct uco_cell cell_pool[], int pool_size)
{
	int cell;
	int consensus_number = uco->consensus_number;

	cell = 1 + p*pool_size + uco_find_free_cell(&cell_pool[1 + p*pool_size], pool_size);
	uco_initialize_cell(&cell_pool[cell], consensus_number);

	return cell;
}

int uco_thread_cell(struct uco *uco, int p, int initialized_cell, struct uco_cell cell_pool[])
{
	int q;
	int new_state;

	int n = uco -> consensus_number;
	struct uco_threading_state *ts = uco -> threading_state;

	ts[p].announce = initialized_cell;
	
	for (q=n-1; q >= 0; --q)
		ts[p].head = ts[((cell_pool[ts[q].head].seq > cell_pool[ts[p].head].seq) ? q : p)].head;

	while(cell_pool[ts[p].announce].seq == 0) {
		int head    = ts[p].head;
		int help    = ts[cell_pool[head].seq % n].announce;
		int prefer  = (cell_pool[help].seq == 0) ? help : ts[p].announce;
	        int after   = uco_cell_decide_after(&cell_pool[head], prefer);

		new_state = uco->do_invocation(p, after, cell_pool[head].state);
		uco_cell_decide_state(&cell_pool[after], new_state);

		cell_pool[after].before   = head;
		cell_pool[after].seq      = cell_pool[head].seq + 1;
		
		ts[p].head	= after;
        }
      
	ts[p].head = ts[p].announce;

	uco_release_cells(ts[p].head, n, cell_pool);
	

        return cell_pool[ts[p].head].state;
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


#define NELEM(a) (sizeof(a)/sizeof((a)[0]))

struct uco_cell cell_pool[1 + N*UCO_CELLS_NO(N)] = {UCO_INITIAL_CELL};
struct my_state state_pool[NELEM(cell_pool)][N] = {OK};
struct my_op my_op_pool[NELEM(cell_pool)];

 
#include <string.h>

int do_invocation(int p, int cell, int prev)
{
	struct my_state *new = &state_pool[cell][p];
	struct my_state *old = &state_pool[prev/N][prev%N];
	struct my_op    *op = & my_op_pool[cell];

      
	memcpy(new, old, sizeof(struct my_state));

	switch (op -> opcode) {
		int last;
	case ENQ:
		last = (old->last + 1) % 40;
		if (last == old->first) {
			new -> result = FULL;
			break;
		}
		new -> last = last;
		new->queue[new -> last].x = op -> arg;
		new->queue[new -> last].p = op -> p;
		new -> result = OK;
		break;
	case DEQ:
		if (old->first == old->last) {
			new -> result = EMPTY;
			break;
		}
		new -> first += 1;
		new -> first %= 40;
		new -> result = OK;

		break;
	
	}

	return cell*N + p;
}

#include <stdio.h>

struct my_uco {
	struct uco uco;
	struct uco_threading_state ts[N];
} my_uco = {{N, do_invocation}};

void* producer(void* x)
{
	int p = * (int*) x;
	int i = 0;
	struct my_state *s;
        int op_cell;

	printf("producer start\n");
	while (i>=0) {
		int x;
		op_cell = uco_thread_setup(&my_uco.uco, p, cell_pool, UCO_CELLS_NO(N));
		my_op_pool[op_cell].opcode = ENQ;
		my_op_pool[op_cell].p = p;
		my_op_pool[op_cell].arg = i;
		x = uco_thread_cell(&my_uco.uco, p, op_cell, cell_pool);
		assert(x%N < NELEM(state_pool[0]) && x/N < NELEM(state_pool));
		s = & state_pool[x/N][x%N];
		
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
		int x;
		op_cell	= uco_thread_setup(&my_uco.uco, p, cell_pool, UCO_CELLS_NO(N));
		uco_initialize_cell(& cell_pool[op_cell], N);
		my_op_pool[op_cell].p = p;
		my_op_pool[op_cell]. opcode = DEQ;
	        x = uco_thread_cell(&my_uco.uco, p, op_cell, cell_pool);
		assert(x%N < NELEM(state_pool[0]) && x/N < NELEM(state_pool));
		s = & state_pool[x/N][x%N];

		if (s->result == EMPTY){
			sched_yield();
			continue;
		}
		printf("[%07ld] consumer @ proc=%d : (from %d) value=%d\n", cell_pool[op_cell].seq, p, s->queue[s->first].p, s->queue[s->first].x);
		++ i;
	}

	return NULL;

}


#include <unistd.h>

int main()
{
	pthread_t t[N];
	int p[N] = {1,2,3,4,5};

	//cell_pool[0].gc = (1 << (N+1)) - 1;

	
	pthread_create(&t[0], NULL, consumer, &p[0]);
	//pthread_create(&t[1], NULL, producer, &p[1]);
	//pthread_create(&t[2], NULL, producer, &p[2]);	
	//pthread_create(&t[3], NULL, consumer, &p[3]);
	
	pause();
	
	return 0;
}
