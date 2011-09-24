#replace with your own GCC path instead if necessary
CC = gcc

all: dynarray

dynarray:
	$(CC) src/cds_core.c src/cds_dynamic_array.c examples/dynarray_test.c -o bin/dynarray_test -I include

dlist:
	$(CC) src/cds_core.c src/cds_dlnode.c src/cds_dlist.c examples/dlist_test.c -o bin/dlist_test -I include -Wpadded

slist:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_slist.c examples/slist_test.c -o bin/slist_test -I include -Wpadded
	
stack:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_stack.c examples/stack_test.c -o bin/stack_test -I include -Wpadded

queue:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_queue.c examples/queue_test.c -o bin/queue_test -I include -Wpadded

binary_tree:
	$(CC) src/cds_core.c src/cds_binary_node.c src/cds_binary_tree.c examples/binary_tree_test.c -o bin/binary_tree_test -I include -Wpadded
	
clean:
	del bin/test.exe