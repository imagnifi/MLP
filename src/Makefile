CXX=g++ -std=c++17
LIBS = -lgtest -lstdc++ -lpthread -lrt -lm
SOURCE = model/network/basic_network.cc \
		model/network/matrix/matrix_network.cc \
		model/matrix/s21_matrix.cc \
		model/network/graph/graph_network.cc \
		model/network/graph/neuron.cc \
		model/reader/reader.cc
TRACE =# */matrix/matrix.h \
		*/matrix/s21_matrix_oop.h \
		*/model/network/network.h \
		*/matrix/s21_support_func.cpp \
		*/matrix/s21_matrix.cpp
PR_NAME = mlp
SRC = controller  model tests \
	  utils view CMakeLists.txt main.cc

OS=$(shell uname)
ifeq ($(OS), Linux)
  LIBS=-lgtest -lstdc++  -lpthread -lrt -lm
  OPEN=xdg-open
else
  LIBS=-lgtest -lstdc++ -pthread
  OPEN=open
endif


build: clean
	@rm -rf ../build
	mkdir -p ../build
	@cmake -B../build
	@make -C ../build
	@cd ../build &&  rm -rf ./*[^mlp\.app] CMakeCache.txt mlp_autogen

install: build

uninstall:
	@rm -rf ../build

dist:
	@-rm -rf DIST
	@-mkdir DIST
	@cp -R ../build/* DIST
	@tar -czvf mlp.tar DIST
	@-rm -rf DIST

dvi:
	@-mkdir -p dvi
	@doxygen Doxyfile

test:
	@$(CXX) tests/tests.cc $(SOURCE) $(LIBS) -o test.out
	@./test.out

gcov_report: clean
	@$(CXX) --coverage tests/tests.cc  $(SOURCE) $(LIBS) -o gcov_report
	@./gcov_report
	@lcov  -t "gcov_report" -o gcov_report.info -c -d . --no-external
	@lcov -r gcov_report.info $(TRACE) -o gcov_report.info
	@genhtml -o report gcov_report.info
	$(OPEN) ./report/index.html

lint:
	@clang-format -i --style=Google ./*/*h ./*/*cc ./*/*/*h ./*/*/*cc
	@clang-format -n --style=Google ./*/*h ./*/*cc ./*/*/*h ./*/*/*cc

clean:
	@rm -rf *.o *.a *.gcno *.gcda *.info *.dSYM gcov_report *.out
