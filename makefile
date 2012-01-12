# replace with your own GCC path instead if necessary
# now supports g++ too!
CC = gcc

all: dynamic_array dlist dlist stack queue binary_tree hash_table

dynamic_array:
	$(CC) src/cds_core.c src/cds_dynamic_array.c examples/dynamic_array_test.c -o bin/dynamic_array_test -I include -Wpadded -Wall -ansi

dlist:
	$(CC) src/cds_core.c src/cds_dlnode.c src/cds_dlist.c examples/dlist_test.c -o bin/dlist_test -I include -Wpadded -Wall -ansi
    
slist:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_slist.c examples/slist_test.c -o bin/slist_test -I include -Wpadded -Wall -ansi
	
stack:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_stack.c examples/stack_test.c -o bin/stack_test -I include -Wpadded -Wall -ansi

queue:
	$(CC) src/cds_core.c src/cds_slnode.c src/cds_queue.c examples/queue_test.c -o bin/queue_test -I include -Wpadded -Wall -ansi

binary_tree:
	$(CC) src/cds_core.c src/cds_binary_node.c src/cds_slnode.c src/cds_queue.c src/cds_binary_tree.c examples/binary_tree_test.c -o bin/binary_tree_test -I include -Wpadded -Wall -ansi

hash_table:
	$(CC) src/cds_core.c src/cds_hash_node.c src/cds_hash_table.c examples/hash_table_test.c -o bin/hash_table_test -I include -Wpadded -Wall -ansi
	
clean:
	del bin\dynarray_test.exe bin\slist_test.exe bin\dlist_test.exe bin\stack_test.exe bin\queue_test.exe bin\binary_tree_test.exe bin\hash_table_test.exe 
