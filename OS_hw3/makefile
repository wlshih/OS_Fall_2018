FIRSTFN = 0516076_hw3-1.c
SECONDFN = 0516076_hw3-2.c
THIRDFN = 0516076_hw3-3.c
FORTHFN = 0516076_hw3-4.c
TESTF1 = Q1.txt
TESTF2 = Q2.txt
TESTF3 = Q3.txt
TESTF4 = Q4.txt
ANS1FN = ans1.txt
ANS2FN = ans2.txt
ANS3FN = ans3.txt
ANS4FN = ans4.txt

.PHONY: all clean

all:
	gcc -o sjf $(FIRSTFN)
	gcc -o srtf $(SECONDFN)
	gcc -o rr $(THIRDFN)
	gcc -o multi $(FORTHFN)

testall:
	./sjf $(TESTF1)
	./srtf $(TESTF2)
	./rr $(TESTF3)
	./multi $(TESTF4)

test1:
	./sjf $(TESTF1)

test2:
	./srtf $(TESTF2)

test3:
	./rr $(TESTF3)

test4:
	./multi $(TESTF4)

clean:
	rm -f $(ANS1FN) $(ANS2FN) $(ANS3FN) $(ANS4FN)
	rm -f sjf srtf rr multi