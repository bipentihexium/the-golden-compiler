#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define PREFIX(n) corelib_##n
extern "C" {
#define MEM_STRUCTS(t, ts) \
	struct memory_row##ts { t *data; size_t size; int64_t ptr; };\
	struct memory##ts { memory_row##ts a, b; };
#define IMPL(t, ts, pf) \
	void PREFIX(memory##ts##_movea)(void *mem, int64_t move) { static_cast<memory##ts *>(mem)->a.ptr += move; }\
	void PREFIX(memory##ts##_swap)(void *_mem) {\
		auto *mem = static_cast<memory##ts *>(_mem);\
		auto tmp = mem->a;\
		mem->a = mem->b;\
		mem->b = tmp;\
	}\
	t *memory_row##ts##_addr(void *_mr) {\
		auto *mr = static_cast<memory_row##ts *>(_mr);\
		if (mr->ptr >= int64_t(mr->size)) {\
			size_t ns = mr->size + mr->size / 2;\
			if (mr->ptr > int64_t(ns)) ns = mr->ptr;\
			mr->data = static_cast<t *>(realloc(mr->data, ns * sizeof(t)));\
			for (size_t i = mr->size; i < ns; i++) { mr->data[i] = 0; }\
			mr->size = ns;\
		}\
		return &mr->data[mr->ptr];\
	}\
	void memory_row##ts##_set(void *_mr, t val) { *memory_row##ts##_addr(_mr) = val; }\
	t memory_row##ts##_get(void *_mr) { return *memory_row##ts##_addr(_mr); }\
	void PREFIX(memory##ts##_seta)(void *mem, t val) { memory_row##ts##_set(&static_cast<memory##ts *>(mem)->a, val); }\
	t PREFIX(memory##ts##_geta)(void *mem) { return memory_row##ts##_get(&static_cast<memory##ts *>(mem)->a); }\
	t PREFIX(memory##ts##_getb)(void *mem) { return memory_row##ts##_get(&static_cast<memory##ts *>(mem)->b); }\
	void PREFIX(memory##ts##_printna)(void *mem) { printf(pf, memory_row##ts##_get(&static_cast<memory##ts *>(mem)->a)); }\
	void PREFIX(memory##ts##_scanna)(void *mem) { scanf(pf, memory_row##ts##_addr(&static_cast<memory##ts *>(mem)->a)); }\
	void PREFIX(memory##ts##_printca)(void *mem) { putchar(int(memory_row##ts##_get(&static_cast<memory##ts *>(mem)->a))); }\
	void PREFIX(memory##ts##_scanca)(void *mem) { memory_row##ts##_set(&static_cast<memory##ts *>(mem)->a, static_cast<t>(getchar())); }\
	void PREFIX(memory##ts##_idxa)(void *_mem) { auto *mem = static_cast<memory##ts *>(_mem); memory_row##ts##_set(&mem->a, static_cast<t>(mem->a.ptr)); }

#define MEM_POOL(t, ts, n, s) \
	struct memory_pool##s { memory##ts slots[n]; size_t free[n]; size_t free_c; };
#define IMPL_POOL(t, ts, n, mps) \
	void *PREFIX(memory_pool##mps##_new)() {\
		auto *pool = static_cast<memory_pool##mps *>(malloc(sizeof(memory_pool##mps)));\
		pool->free_c = (n);\
		for (size_t i = 0; i < (n); i++) {\
			pool->free[i] = i;\
		}\
		return pool;\
	}\
	void PREFIX(memory_pool##mps##_free)(void *pool) { free(pool); }\
	void *PREFIX(memory_pool##mps##_alloc)(void *_pool) {\
		auto *pool = static_cast<memory_pool##mps *>(_pool);\
		if (pool->free_c) {\
			auto *mem = &pool->slots[pool->free[--pool->free_c]];\
			mem->a.data = static_cast<t *>(malloc(8 * sizeof(t))); mem->a.size = 8; mem->a.ptr = 0;\
			mem->b.data = static_cast<t *>(malloc(8 * sizeof(t))); mem->b.size = 8; mem->b.ptr = 0;\
			for (size_t i = 0; i < 8; i++) { mem->a.data[i] = mem->b.data[i] = 0; }\
			mem->b.data[0] = 1;\
			return mem;\
		} else {\
			return nullptr;\
		}\
	}\
	void PREFIX(memory_pool##mps##_dealloc)(void *_pool, void *_mem) {\
		auto *pool = static_cast<memory_pool##mps *>(_pool);\
		auto *mem = static_cast<memory##ts *>(_mem);\
		const size_t id = mem - pool->slots;\
		free(mem->a.data);\
		free(mem->b.data);\
		pool->free[pool->free_c++] = id;\
	}

	void PREFIX(rand_init)() { srand(time(NULL)); }

	MEM_STRUCTS(double, d)
	IMPL(double, d, "%lf")
	MEM_POOL(double, d, 1024, d1k)
	IMPL_POOL(double, d, 1024, d1k)
	double PREFIX(floord)(double x) { return floor(x); }
	double PREFIX(ceilrd)(double x) { return ceil(x); }
	double PREFIX(randd)() { return double(rand()) / double(RAND_MAX); }
	double PREFIX(flrpowd)(double x, double y) { double z = pow(x, floor(abs(y))); return y < 0 ? 1/z : z; }
	MEM_STRUCTS(int64_t, li)
	IMPL(int64_t, li, "%" PRId64)
	MEM_POOL(int64_t, li, 1024, li1k)
	IMPL_POOL(int64_t, li, 1024, li1k)
	int64_t PREFIX(floorli)(int64_t x) { return x; }
	int64_t PREFIX(ceilrli)(int64_t x) { return x; }
	int64_t PREFIX(randli)() { return rand() % 2; }
	int64_t PREFIX(flrpowli)(int64_t x, int64_t y) {
		if (y < 0) return 0;
		int64_t o = 0;
		uint64_t uy = y;
		for (int i = 0; i < 63; i++) {
			uy <<= 1;
			o *= o;
			if (uy & 0x8000000000000000ull)
				o *= x;
		}
		return o;
	}

	void print_mem_row_data(memory_rowd *r) {
		for (size_t i = 0; i < r->size; i++) {
			char pfc = ' ';
			char dc = r->data[i];
			if (static_cast<unsigned char>(dc) < 8) { pfc = '\\'; dc += '0'; }
			else if (dc == '\n') { pfc = '\\'; dc = 'n'; }
			else if (dc == '\r') { pfc = '\\'; dc = 'r'; }
			else if (dc == '\t') { pfc = '\\'; dc = 't'; }
			else if (static_cast<unsigned char>(dc) < 32) { pfc = '?'; dc = '<'; }
			else if (dc == ' ') { pfc = 's'; dc = 'p'; }
			else if (static_cast<unsigned char>(dc) > 126) { pfc = '?'; dc = '>'; }
			printf("| %12.2f (%c%c) ", r->data[i], pfc, dc);
		}
		printf("|\n");
	}
	void print_ptr_pos(memory_rowd *r, char c) {
		for (size_t i = 0; i < r->size; i++) {
			if (int64_t(i) == r->ptr) {
				printf("  %c---- %6ld ----", c, r->ptr);
				break;
			} else {
				printf("                    ");
			}
		}
		putchar('\n');
	}
	void print_memory(void *_mem) {
		memoryd *mem = static_cast<memoryd *>(_mem);
		print_ptr_pos(&mem->a, 'v');
		print_mem_row_data(&mem->a);
		print_mem_row_data(&mem->b);
		print_ptr_pos(&mem->b, '^');
	}
	void print_all_memory(void *_pool) {
		memory_poold1k *pool = static_cast<memory_poold1k *>(_pool);
		printf("global:\n");
		print_memory(pool->slots + 1023);
		for (size_t i = 0; i < 1023; i++) {
			for (size_t j = 0; j < pool->free_c; j++) {
				if (pool->free[j] == i)
					goto skip;
			}
			printf("memory %zu:\n", i);
			print_memory(pool->slots + i);
		skip:;
		}
	}
	void dprint(void *str) { printf("\x1b[34m%s\x1b[0m", (char *)str); }
}
