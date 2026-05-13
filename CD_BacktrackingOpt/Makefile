COMPILE_OPTIONS = -Wall -O3 -std=c++11

TestCDOpt.exe: TestCDOpt.o BacktrackingCDOpt.o CD_Data.o
	g++ -o TestCDOpt.exe TestCDOpt.o BacktrackingCDOpt.o CD_Data.o

TestCDOpt.o: BacktrackingCDOpt.hh BacktrackingOpt.hh TestCDOpt.cc CD_Data.hh
	g++ -c $(COMPILE_OPTIONS) TestCDOpt.cc

BacktrackingCDOpt.o: BacktrackingCDOpt.hh BacktrackingCDOpt.cc CD_Data.hh
	g++ -c $(COMPILE_OPTIONS) BacktrackingCDOpt.cc

CD_Data.o: CD_Data.cc CD_Data.hh
	g++ -c $(COMPILE_OPTIONS) CD_Data.cc

clean:
	rm -f TestCDOpt.exe TestCDOpt.o BacktrackingCDOpt.o CD_Data.o
