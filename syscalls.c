#include "uart.h"

static uint8_t *heap_ptr = NULL;

int _write(int file, char* ptr, int len) {
    if (file == 1)
        uart2_write_buff((uint8_t*)ptr, (uint32_t)len);
    return -1;
}

int _fstat(int fd, void *st) {
    (void)fd, (void)st;
    return -1;
}

void* _sbrk(int incr) {
    extern uint8_t _end;
    extern uint8_t _estack; /* Symbol defined in the linker script */
    extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
    const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
    const uint8_t* max_heap = (uint8_t*)stack_limit;
    uint8_t *prev_heap_ptr;

    if (heap_ptr == NULL) {
        heap_ptr = &_end;
    }

    if(heap_ptr + incr > max_heap) {
        return (void*)-1;
    }

    prev_heap_ptr = heap_ptr;
    heap_ptr += incr;

    return (void*)prev_heap_ptr; 
}

int _close(int fd) {
    (void)fd;
    return -1;
}

int _isatty(int fd) {
    (void)fd;
    return 1;
}


int _read(int fd, char* ptr, int len) {
    (void)fd, (void)ptr, (void)len;
    return -1;
}

int _lseek(int fd, int ptr, int dir) {
    (void)fd, (void)ptr, (void)dir;
    return 0;
}