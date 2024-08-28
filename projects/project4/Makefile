PROGS = public01 public02 public03 public04 public05 public06 student_tests

all: $(PROGS)

public01: calendar.o my_memory_checker_216.o public01.o
	gcc -o public01 calendar.o my_memory_checker_216.o public01.o
public02: calendar.o my_memory_checker_216.o public02.o
	gcc -o public02 calendar.o my_memory_checker_216.o public02.o
public03: calendar.o my_memory_checker_216.o public03.o
	gcc -o public03 calendar.o my_memory_checker_216.o public03.o
public04: calendar.o my_memory_checker_216.o public04.o
	gcc -o public04 calendar.o my_memory_checker_216.o public04.o
public05: calendar.o my_memory_checker_216.o public05.o
	gcc -o public05 calendar.o my_memory_checker_216.o public05.o
public06: calendar.o my_memory_checker_216.o public06.o
	gcc -o public06 calendar.o my_memory_checker_216.o public06.o

student_tests: calendar.o my_memory_checker_216.o student_tests.o
	gcc -o student_tests calendar.o my_memory_checker_216.o student_tests.o
calendar.o: calendar.c calendar.h event.h
	gcc -c calendar.c
my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h 
	gcc -c my_memory_checker_216.c
student_tests.o: student_tests.c calendar.h my_memory_checker_216.h event.h 
	gcc -c student_tests.c

public01.o: public01.c calendar.h my_memory_checker_216.h event.h
	gcc -c public01.c
public02.o: public02.c calendar.h my_memory_checker_216.h event.h
	gcc -c public02.c
public03.o: public03.c calendar.h my_memory_checker_216.h event.h
	gcc -c public03.c
public04.o: public04.c calendar.h my_memory_checker_216.h event.h
	gcc -c public04.c
publi05.o: public05.c calendar.h event.h 
	gcc -c public05.c 
publi06.o: public06.c calendar.h event.h
	gcc -c public06.c 

clean:
	rm -f *.o $(PROGS) a.out


