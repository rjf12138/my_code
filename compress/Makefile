all:
	$(MAKE) -C src
	$(MAKE) -C test
	pwd
	-cp ./src/main ./test/test_prog ./

.PHONY: clean
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	-rm main test_prog

# install use for test
.PHONY: install
install:
	cp -f main /home/ruanjian/workspace/install/bin/