ROOT=`pwd`
SUB_DIR=$(ROOT)/server $(ROOT)/client $(ROOT)/test
LIB_DIR=$(ROOT)/server $(ROOT)/client


.PHONY : all
all:
	@for i in $(SUB_DIR); do \
		(cd $$i && make); \
	done

.PHONY : clean
clean:
	@for i in $(SUB_DIR); do \
		(cd $$i && make clean);\
	done

install:
	@for i in $(LIB_DIR); do \
		(cd $$i && make install);\
	done

