gbuild:
	mpicc main.c mpiUtil.c sortUtil.c fileUtil.c -o main -lm

clean:
	rm -f *.o ./exe1
run:
	mpiexec -np 16 ./main
