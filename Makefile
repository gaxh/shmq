OPTIMIZE := -O3

HEADERS := shm_helper.h shm_queue.h
CPPS := shm_helper.cpp shm_queue.cpp

all : shm_test

shm_test : shm_test.cpp $(HEADERS) $(CPPS)
	g++ -o $@ shm_test.cpp ${CPPS} ${OPTIMIZE} -g -Wall

all : shm_queue_test_read

shm_queue_test_read : shm_queue_test_read.cpp $(HEADERS) $(CPPS)
	g++ -o $@ shm_queue_test_read.cpp ${CPPS} ${OPTIMIZE} -g -Wall

all : shm_queue_test_write

shm_queue_test_write : shm_queue_test_write.cpp $(HEADERS) $(CPPS)
	g++ -o $@ shm_queue_test_write.cpp ${CPPS} ${OPTIMIZE} -g -Wall

clean:
	rm -f shm_test
	rm -f shm_queue_test_read
	rm -f shm_queue_test_write
