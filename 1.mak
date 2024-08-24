OUT=3
IN=test0.cpp LPng.cpp crc.cpp
$(OUT):$(IN)
	g++ -o $(OUT) -O2 $(IN)