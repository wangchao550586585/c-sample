ALL:write_fifo read_fifo

read_fifo:read_fifo.c
	gcc  $^ -o  $@ -Wall -g -levent

write_fifo:write_fifo.c
	gcc  $^ -o  $@ -Wall -g -levent

#%.o : %.c
#	gcc  $^ -o  $@ -levent -Wall -g