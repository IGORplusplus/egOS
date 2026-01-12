#include <stdio.h>
void terminal_write(const char *str, int len) {
    for (int i = 0; i < len; i++) {
        *(char*)(0x10000000) = str[i];
    }
}

/* Uncomment line11 - line39
 * when implementing formatted output
 */

#include <stdlib.h>  // for itoa() and utoa()
#include <string.h>  // for strlen() and strcat()
#include <stdarg.h>  // for va_start(), va_end(), va_arg() and va_copy()

char* ulltoa(unsigned long long value, char* str, int base) {
    static const char digits[] = "0123456789";
    char buf[65];
    int i = 0;

    //if there is no value then it returns the string 0
    if (value == 0) {
	str[0] = '0';
	str[1] = '\0';
	return str;
    }

    while (value > 0) {
	buf[i++] = digits[value % base];
	value /= base;
    }

    for(int j=0; j < i; j++)
	str[j] = buf[i - j - 1];
    str[i] = '\0';

    return str;
}

void format_to_str(char* out, const char* fmt, va_list args) {
    for(out[0] = 0; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            strncat(out, fmt, 1);
        } else {
            fmt++;
            if (*fmt == 's') {
                strcat(out, va_arg(args, char*));
            } else if (*fmt == 'c') {
		char ch = (char) va_arg(args, int); // va_arg promotes char to int
		size_t len = strlen(out);
		out[len] = ch;    // append the character
		out[len + 1] = '\0'; // null-terminate
	    } else if (*fmt == 'd') {
                itoa(va_arg(args, int), out + strlen(out), 10);
            } else if (*fmt == 'u') {
		utoa(va_arg(args, unsigned int), out + strlen(out), 10);
	    } else if (*fmt == 'x') {
		utoa(va_arg(args, unsigned int), out + strlen(out), 16);
	    } else if (*fmt == 'p') {
		strcat(out, "0x");
		ulltoa((unsigned long)va_arg(args, void*), out + strlen(out), 16);
	    } else if (*fmt == 'l' && *(fmt+1) == 'l' && *(fmt+2) == 'u') {
		fmt += 2;
		ulltoa(va_arg(args, unsigned long long), out + strlen(out), 10);
	    }
        }
    }
}

unsigned int format_to_str_len(const char* fmt, va_list args_copy) {
    int len = 0;
    for(int i = 0; i < strlen(fmt); i++) {
	if(fmt[i] != '%') {
	    len++;
	    continue;
	} 
	i++;

	//handle multi-lettered formats here
	if(fmt[i] == 'l' && fmt[i+1] == 'l') {
	    i += 2;
	    switch(fmt[i]) {
		case 'u': {
		    unsigned long long v = va_arg(args_copy, unsigned long long);
		    unsigned long long tmp = v;
		    do {
			len++;
			tmp /= 10;
		    } while(tmp != 0);
		    break;
		}
		case 'd':
		case 'i': {
		    long long v = va_arg(args_copy, long long);
		    if(v < 0) {
			//for the negative sign
			len++;
			v *= -1;

		    }
		    unsigned long long tmp = (unsigned long long)v;
		    do {
			len++;
			tmp /= 10;
		    } while(tmp != 0);
		    break;
		}
	    }
	    continue;
	}

	switch(fmt[i]) {
	    case 'd':
	    case 'i': {
		int v = va_arg(args_copy, int);
		if (v < 0) {
		    len++;
		    v = -v;
		}
		unsigned tmp = (unsigned)v;
		do {
		    len++;
		    tmp /= 10;
		} while (tmp != 0);
		break;
	    }

	    //string
	    case 's': {
		const char* s = va_arg(args_copy, const char*);
		while(*s++)
		    len++;
		break;
	    }
	    //
	    case 'x': {
		unsigned v = va_arg(args_copy, unsigned);
		unsigned tmp = v;
		break;
	    }
	    //
	    case 'p': {
		break;
	    }
	    case 'u': {
		unsigned v = va_arg(args_copy, unsigned);
		break;
	    }
	    case 'c': {
		len++;
		break;
	    }
	    default: {
		break;
	    }
	}
    }
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    va_list args_copy;
    va_copy(args_copy, args);

    unsigned int len = format_to_str_len(format, args_copy);
    char *buf = malloc(len + 1);

    format_to_str(buf, format, args);
    va_end(args);
    terminal_write(buf, strlen(buf));

    strcpy(buf, "");
    strcat(buf, "\n\r");
    terminal_write(buf, strlen(buf));

    va_end(args_copy);
    free(buf);

    return 0;
}


/* Uncomment line46 - line57
 * when implementing dynamic memory allocation
 */

extern char __heap_start, __heap_end;
static char* brk = &__heap_start;
char* _sbrk(int size) {
    if (brk + size > (char*)&__heap_end) {
        terminal_write("_sbrk: heap grows too large\r\n", 29);
        return NULL;
    }

    char* old_brk = brk;
    brk += size;
    return old_brk;
}


int main() {
    char* msg = "Hello, World!\n\r";
    terminal_write(msg, 15);

    /* Uncomment this line of code
     * when implementing formatted output
     */
    printf("%s-%u is awesome!\n\r", "egos", 2000);

    printf("%c is character $", '$');
    printf("%c is character 0", (char)48);
    printf("%x is integer 1234 in hexadecimal", 1234);
    printf("%u is the maximum of unsigned int", (unsigned int)0xFFFFFFFF);
    printf("%p is the hexadecimal address of the hello-world string", msg);
    printf("%llu is the maximum of unsigned long long", 0xFFFFFFFFFFFFFFFFULL);


    return 0;
}
