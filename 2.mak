OUT=5
IN=test1.cpp LPng.cpp crc.cpp
$(OUT):$(IN)
	g++ -o $(OUT) -O2 $(IN)