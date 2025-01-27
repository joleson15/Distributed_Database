RepCRec: main.cpp
	g++ main.cpp site.cpp transaction.cpp operation.cpp TransactionManager.cpp -o RepCRec

clean:
	rm -f RepCRec *~
