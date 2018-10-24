# Index
- [Go](https://bwuah.github.io/compendium#go-compendium)
- [C](https://bwuah.github.io/compendium#c-compendium)
- [x86](https://bwuah.github.io/compendium#assembly-x86-intel)

# Go Compendium

### Misc

- `defer FUNCTION()` waits with execution until surrounding function is returned, the paras will be copied already though
- `_ = function()` throws away the result. Useful for e.g. `num, _ := strconv.Atoi("5")`
- `fmt.Printf("%v", ANY_TYPED_VAR)` %v prints the argument using it's types matching format
- use `...` to use optional paras

```Go
func test(a... int) {
	print(len(a))
}
```

### Variables 

- `var num int`
- `num := 5`

### Strings
- reading from stdin:

```Go
reader := bufio.NewReader(os.Stdin)
text, _ := reader.ReadString('\n')
```
- reading from file:

```Go
f, err := os.Open("file.txt")
defer f.Close()
check(err)
buf := make([]byte, 64)
f.Read(buf)
```
- writing to file:

```Go
f, err := os.Create("file.txt")
defer f.Close()
check(err)
f.Write([]byte(str))
```
- `str1 := str2` copies, and does'nt only copy reference
- `str1 == str2` actually compares values, not only reference
- `str1 + str2` results in appended string
- `strconv.Itoa(5))` return "5"
- `num, err := strconv.Atoi("5")` sets num to 5
- `strings.Count(string, substring)` counts substrings in string
- `strings.Split(string, seperator)` returns a slice of string, with the separated substrings
- `text=strings.Replace(text,old, new, n)`replaces all `old` with `new` up to `n` times

### Slices

- `make([]TYPE, SIZE)` returns a slice of TYPE of size SIZE
- `string(byte_slice)` returns a string from a given slice of byte
- array with slices

 ```Go
matrix := make([][]int, ROWS)
for i:=0; i<ROWS; i++{
	matrix[i] = make([]int, COLS)
}
```
- iterate over slices

```Go
list := make([]int, 5)
...
for indx, elem := range list{
    fmt.Printf("%d: %d\n", indx, elem)
}
```

### Structs

```Go 
type Person struct {
    name string
}
```

### Network

##### TCP Server

```Go
ln,_ := net.Listen("tcp", ":4711")
var wg sync.WaitGroup
for {
    conn,_ := ln.Accept()
    wg.Add(1)
    go func(c net.Conn){
        defer wg.Done()
        res,_ := bufio.NewReader(conn).ReadBytes(0)
        fmt.Fprintf(conn, "%s", str)
    }(conn)
}
wg.Wait()
```
##### TCP Client

```Go
Go conn,_ := net.Dial("tcp", "192.168.0.38:4711")
fmt.Fprintf(conn, "%s", str)
res,_ := bufio.NewReader(conn).ReadBytes(0)
```

### System
- `os.Args` is the list of the cmd args, with `os.Args[0]` being the executable name
- `out, err := exec.Command(para[0], para[1,:]...).CombinedOutput()` runs para[0] with arguments, the output goes to out

### Goroutines

```Go
var wg = sync.WaitGroup{}
wg.Add(1)
go func() {
    defer wg.Done()
    fmt.Print("Hello from Thread\n")
}()
wg.Wait()
```

- using Mutexes:

```Go
var mutex = sync.Mutex{}
mutex.Lock()
...
mutex.Unlock()
```

- channels: unbuffered

```Go
c := make(chan int)
go func() {
    c <- 2 //waits until other Goroutine receives
}()
println(<-c)
```

- channels: buffered: `c := make(chan int, 1)` <br>Goroutine will not be waiting for receiver, only waits when buffered channel is full.

- Semaphore with channels:

```Go
var sem = make(chan int, RSRC)

func foo(){
	sem<-0 //grab one rsrc
	...
	<-sem //release rsrc
}
```

### CGO

```Go
//#include <stdio.h>
//void print_it() {printf("LuL");}
import "C"
func main() {
    C.print_it()
}
```

and run with `go run main.go` in Linux.

**or** run `go build` while

```Go
//#include "file.h"
import "C"
...
```
# C Compendium

### IO

- stdout is a file

 ```C
FILE* fout = stdout; 
fprintf(fout, "hello\n");
//stdout is := fdopen(1, "w");
``` 

- limit scans to n characters *(leave one extra space for '\0' tho)*

```C
read_items = fscanf(fin, "%20s %20s", vn, nn);
```

- flush buffer

```C
fprintf(out, "hey");
fflush(out);
```

- superior version

```C
void flush(FILE* in) //superior to fflush, somehow
{
    char buf;
    for(;(buf = fgetc(in)) != '\n' && buf != EOF;);
}
```

- clear console

```C
fprintf(out, "\033[H\033[2J");
```

### Files
- stream to fd with **int fileno(FILE\* stream)**

```C
FILE* out = fopen("test.txt", "w");
int fd = fileno(out); //!
close(1); //closing stdout
dup(fd); //stdout is now "test.txt"
```

- fd to stream with **FILE\* fdopen(int fd, char\* mode)**

```C
FILE* out = fdopen(1, "w");
fprintf(out, "hello\n");
```

### Memory
- always alloc one character more than strlen, for the '\0' terminator

```C
new->vn = malloc(strlen(vn)+1);
new->nn = malloc(strlen(nn)+1);
strcpy(new->vn, vn);
strcpy(new->nn, nn);
```

- format string overflow-safe

```C
char* str = malloc(sizeof(char)*16);
snprintf(str, 16, "%u.%u.%u.%u", x0, x1, x2, x3);
```

### Multi- Proc/Thread
- **int fork()**

```C
int pid = fork();
    if(!pid) //child
    {
        ...
	exit(EXIT_SUCCESS);
    }
    else //parent
    {
        wait(NULL);
        ...
```

- **int pipe(int[2])**

```C
int fd[2];
pipe(fd);
int pid = fork();
if(pid) { write(fd[1], "parent->child"); }
else { write(fd[0], "child->parent"); }
```

- Threading

```C
void* thread_callee(void* data)
{ ... }

void thread_caller()
{   
    struct data info;
    pthread_t num;
    pthread_create(&num, NULL, thread_callee, (void*) &info);
    pthread_join(num, NULL);
}
```

- detaching thread

```C
pthread_t num;
pthread_create(&num, NULL, callee, (void *) data);
pthread_detach(num);
```

- Mutexes

```C
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
//critical
pthread_mutex_unlock(&mutex);
```

### System
- execute

```C
execvp(list[0], list);
//when list is a tokenized char** of the arguments
```

### Networking
##### UDP

```C
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
```
- server

```C
int fd = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in addr ;
addr.sin_family = AF_INET;
addr.sin_port = htons(4711);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));

char buf[64];
int len, flen;
struct sockaddr_in from;
flen = sizeof(struct sockaddr_in);
len = recvfrom(fd, buf, 64, 0, (struct sockaddr*) &from, &flen);
close(fd);
```

- client

```C
fd = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in dest;
dest.sin_family = AF_INET;
dest.sin_port = htons(4711);
dest.sin_addr.s_addr = inet_addr("127.0.0.1");

char buf[64];

err = sendto(fd, buf, 64, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

close(fd);
```
##### TCP 

```C
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
```
- client

```C
fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in dest;
dest.sin_family = AF_INET;
dest.sin_port = htons(4711);
dest.sin_addr.s_addr = inet_addr("127.0.0.1");

connect(fd, (struct sockaddr*) &dest, sizeof(sockaddr_in));

char buf[64];

write(fd, buf, 64);

close(fd);
```
- server

```C
int fd, new_sock;
struct sockaddr_in client_addr, server_addr;
socklen_t sockaddr_in_len = sizeof(struct sockaddr_in);

fd = socket(AF_INET, SOCK_STREAM, 0);

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(4711);
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(fd, (struct sockaddr *) &server_addr, sockaddr_in_len);

listen(fd, 5);

new_sock = accept(fd, (struct sockaddr *) &client_addr, &sockaddr_in_len);

printf("Received connection from %s\n", inet_ntoa(client_addr.sin_addr));

char buf[2048];

read(new_sock, buf, 2048);
```
# Assembly x86 (intel)

### General and Compilation

```assembly
.intel_syntax noprefix
.data
  ...
.global _start
_start:
  ...
   mov eax, 1
   mov ebx, 1
   int 0x08
```

can be compiled and run with:

```
as -32 --gstabs -o TARGET.o FILENAME.ASM
ld -m elf_i386 TARGET.o
./a.out
```

### .data Section

##### creating variables 
`num: .int 3` <br>

|0|1|2|3|
|---|---|---|---|
|00000011|0..0|0..0|0..0|

num is a label, at that position in .data we store an int (int should be 4 Bytes) <br>
`str: .string "A String"` <br>
str is a label, at that position in .data we store a string  

|0|1|2|3|4|5|6|7|8|
|---|---|---|---|---|---|---|---|---|
|'a'|32|'S'|'t'|'r'|'i'|'n'|'g'|0|

##### calculating length
- `str_len = . - str - 1` <br>the -1 is for the implicit null <br>
- `num_len = . - num` <br> these are not variables in the compiled prog, just macros for the dev. Thus you can set them anywhere you want, granted you did'nt put more data segments after the one they are refering to.
### .text Section

##### Instructions
- [Instructions](http://www.jegerlehner.ch/intel/IntelCodeTable.pdf)
- `mov eax, ebx` is eax:=ebx
- but single bytes can be addressed (lil' endian) <br> `movb eax, num+3` <br> copies num[3] to eax, so 0.

##### Stack

```assembly
sub esp, 4
mov [esp], eax

# equals

push eax
```

and analogue:

```assembly
pop eax
xor eax, eax

# equals

add esp, 4
xor eax, eax
```

### Syscalls
- [Syscalls](http://syscalls.kernelgrok.com/)
- 1: sys_exit
- 4: sys_write
### Macros
*todo*
### Calls and cdecl
- caller:

```assembly
push eax # para n
... 
push eax # para 1

call FUNC

add esp, <4*n>
```

- callee: 

```assembly
push ebp
mov ebp, esp
# these two can be 'enter 0,0' too

mov eax, [ebp+8] # para 1
...
mov eax, [ebp+4+n*4] # para n

... # here is the body

mov esp, ebp
pop ebp
# these two can be 'leave'

ret
```

- local vars

```assembly
push eax
...
add esp, 4
```

##### Calling x86 from C
- write a routine, that follows cdecl

```assembly
.global FUNCNAME
.type FUNCNAME @function
FUNCNAME:
  ...  
```
- declare the function, with parameter list <br>
`void print_int(int num, int base)`
- compile \*.asm: `as -32 -o NAME.o NAME.asm`
- compile \*.c: `gcc -m32 NAME.o OTHERNAME.c`
- run: `./a.out`
- Disassemble with <br> `objdump -S -C -d -M intel NAME.o` <br> `objdump --source --demangle --disassemble -M intel NAME.o`

##### inline assembly in C
- write instructions

```asm
int big_endian()
{
	asm
	(
		"xor eax, eax;"
		"inc eax;"
		"push eax;"
		"movb al, [esp+3];"
		"mov esp, ebp;"
	);
}
```
- use variables

```asm
int in, out1, out2;
	in = 5;
	asm
	(
		"mov eax, %2;" 
		"mov %0, eax;" 
		"mov %1, eax;"		
		: "=r"(out1), "=r"(out2)
		: "r"(in)
	);
```
- compile with
```gcc -m32 -masm=intel FILENAME.c``` 
