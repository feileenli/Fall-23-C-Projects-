#include <stdio.h>
#include <stdlib.h>

void test(const char *str) {
   int x = atoi(str);

   printf("For \"%s\" we get %d\n", str, x);
}

int main(void) {
   test("10");
   test("020");
   test("0");
   test("");
   test("-1");
   test("-78");
   test("-5.76");
   test("house");
   test("   ");

   return 0;
}
