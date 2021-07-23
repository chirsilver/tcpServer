main:main.o mutex.o task.o threadpoll.o cond.o
	@g++ -o main -g $^ -lpthread 
	@echo "LINK $^ \033[01;32m OK \033[0m"
	@rm -rf *.o
	@echo "RM OBJS \033[01;32m OK \033[0m"
%.o:%.cc
	@g++ -o $@ -c -g $< -lpthread
	@echo "BUILD $@ \033[01;32m OK \033[0m"

.PHONY = clean
clean:
	rm -rf *.o