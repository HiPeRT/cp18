PROGS = ex_create ex_rr ex_cancellation ex_sem ex_mutex ex_cond ex_sem2

OBJS = $(addsuffix .o, $(PROGS))

all: $(PROGS)

clean:
	rm -rf $(PROGS)

%: %.c
	gcc -Wall $< -o $@ -lpthread
