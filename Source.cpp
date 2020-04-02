//â.12
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
clock_t c(short array1[8], short array2[8])
{
	clock_t cur;
	short res[8];
	int counter;
	cur = clock();
	for (int n = 0; n < 10000000; n++) {
		for (counter = 0; counter < 8; counter++) {
			res[counter] = (array1[counter] + array2[counter]) / 2;
		}
	}
	return clock() - cur;
}

clock_t asm_without(short array1[8], short array2[8])
{
	clock_t cur;
	short res[8];
	int counter;
	int cnt;
	cur = clock();
	for (int n = 0; n < 10000000; n++) {
		cnt = 8;
		_asm {
			pusha
			xor esi, esi
			xor ecx, ecx
		loop2:
			mov eax, array1[esi]
			mov ebx, array2[esi]
			add ax, bx
			shr ax, 1
			mov res[esi], ax
			add esi, 2
			dec cnt
			jnz loop2
			popa
		}
	}
	return clock() - cur;
}

clock_t asm_with(short array1[8], short array2[8])
{
	clock_t cur;
	short res[8];
	int counter;
	int cnt;
	cur = clock();
	for (int n = 0; n < 10000000; n++) {
		cnt = 8;
		_asm {
			pusha
			xor esi, esi
		loop1 :
			movq mm0, array1[esi]
			movq mm1, array2[esi]
			pavgw mm0, mm1
			movd res[esi], mm0;
			add esi, 4
			sub cnt, 2
			jnz loop1
			emms
			popa
		}
	}
	return clock() - cur;
}

int main()
{
	int counter, exit;
	int size = 8;
	short array1[8];
	short array2[8];
	short res[8];
	do {

		for (counter = 0; counter < size; counter++) {
			srand(clock() + counter);
			array1[counter] = (short)rand();
		}
		for (counter = 0; counter < size; counter++) {
			srand(clock() + counter);
			array2[counter] = (short)rand();
		}

		printf("C: %d ms\n", c(array1, array2));
		printf("Asm without MMX: %d ms\n", asm_without(array1, array2));
		printf("Asm with MMX: %d ms\n", asm_with(array1, array2));

		printf("\n");
		printf("Do you want to continue? 1 = yes, 2 = no\n");
		while (!scanf_s("%d", &exit)) {
			rewind(stdin);
		}
	} while (exit != 2);
	return 0;
}