# kernel modules
obj-m += globalmem.o

KVERS = $(shell uname -r)
PWD = $(shell pwd)

# specify flags for the module compilation
#EXTRA_CFALGS=-g -O0

build: kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
