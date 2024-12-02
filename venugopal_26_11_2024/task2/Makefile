cc=gcc
cflags=-Iinclude
src1=source/read_data_from_serial_port.c
target=built/read_data_from_serial_port

$(target):$(src1)
	$(cc) $(cflags) $(src1)  -o $(target)
clean:
	rm -rf $(target) 

