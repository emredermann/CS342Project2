all: scheduler
scheduler: schedule.c
		gcc -o scheduler schedule.c -lpthread -lm
clean:
		rm -fr *~ scheduler