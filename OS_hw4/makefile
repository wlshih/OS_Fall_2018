EXSN = example_hw4.cpp
HWSN = 0516076_hw4.cpp
BONUSSN = 0516076_hw4_bonus.cpp

OUTPUT1FN = output1.bmp
OUTPUT2FN = output2.bmp
OUTPUT3FN = output3.bmp
OUTPUT4FN = output4.bmp
OUTPUT5FN = output5.bmp
ANSDIR = ANS/
ANS1FN = output1.bmp
ANS2FN = output2.bmp
ANS3FN = output3.bmp
ANS4FN = output4.bmp
ANS5FN = output5.bmp

all:

ex:
	g++ -std=c++11 -pthread $(EXSN)
	./a.out
hw:
	g++ -std=c++11 -pthread $(HWSN)
	./a.out
bonus:
	g++ -std=c++11 -pthread $(BONUSSN)
	./a.out

sh: 
	sh Speed.sh

mae:
	./MAE.out $(OUTPUT1FN) $(ANSDIR)$(ANS1FN)
	./MAE.out $(OUTPUT2FN) $(ANSDIR)$(ANS2FN)
	./MAE.out $(OUTPUT3FN) $(ANSDIR)$(ANS3FN)
	./MAE.out $(OUTPUT4FN) $(ANSDIR)$(ANS4FN)
	./MAE.out $(OUTPUT5FN) $(ANSDIR)$(ANS5FN)

clean:
	rm -f a.out $(OUTPUT1FN) $(OUTPUT2FN) $(OUTPUT3FN) $(OUTPUT4FN) $(OUTPUT5FN)