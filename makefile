#replace with your own GCC path instead if necessary
CC = gcc

all: dynarray

dynarray:
	$(CC) src/cds_core.c src/cds_dynamic_array.c examples/dynarray_test.c -o bin/dynarray_test -I include -Wpadded -Wall

dlist:
	$(CC) src/cds_core.c src/cds_dlnode.c src/cds_dlist.c examples/dlist_test.c -o bin/dlist_test -I include -Wpadded

dlist2:
	$(CC) src/cds_core.c src/cds_dlnode.c src/cds_dlist.c examples/dlist_test.c -o bin/dlist_test -I include -Wpadded -D CDS_DLIST_SEARCH_FOR_RNODE=0
    
slist:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_slist.c examples/slist_test.c -o bin/slist_test -I include -Wpadded -Wall -ansi
	
stack:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_stack.c examples/stack_test.c -o bin/stack_test -I include -Wpadded -Wall -ansi

queue:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_queue.c examples/queue_test.c -o bin/queue_test -I include -Wpadded -Wall -ansi

binary_tree:
	$(CC) src/cds_core.c src/cds_binary_node.c src/cds_binary_tree.c examples/binary_tree_test.c -o bin/binary_tree_test -I include -Wpadded -Wall -ansi

hash_table:
	$(CC) src/cds_core.c src/cds_hash_node.c src/cds_hash_table.c examples/hash_table_test.c -o bin/hash_table_test -I include -Wpadded

hash_table_safe:
	$(CC) examples/hash_table_test.c -o bin/hash_table_test -Wpadded
	
clean:
	del bin/test.exe