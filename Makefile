OUT=report
IN=test.cpp LPng.cpp crc.cpp
$(OUT):$(IN)
	g++ -o $(OUT) -O2 $(IN)