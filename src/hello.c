#include <stdio.h>

#define HOT 1
#define WET 2
#define RED 4
#define OLD 8

union bar {
    char p;
    struct {
	unsigned char hot:1;
	unsigned char wet:1;
	unsigned char red:1;
	unsigned char old:1;
    } bits;
};


int main(void) {
    union bar f = {0};

    f.p = OLD;
    printf("%lu\n", sizeof(union bar));

    if(f.bits.old) {
	printf("OLD\n");
    }
    printf("%d\n", f.bits.hot);
    printf("%d\n", f.bits.wet);
    printf("%d\n", f.bits.red);
    printf("%d\n", f.bits.old);
    return 0;
 }
