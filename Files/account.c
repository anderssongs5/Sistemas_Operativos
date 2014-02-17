#include "account.h"
#include <stdio.h>

void withdraw(struct account *a, double amount) {
    a->money = a->money - amount;
}

void deposit(struct account *a, double amount) {
    a->money = a->money + amount;
}

int transfer(struct account *out, struct account *in, double amount){
	//printf("Money: %.3f\n", amount);
	if(out->money < amount){
		
		return (0);
	}
	
	//printf("ID: %ld  - Money: %f\n", out->id, out->money);
	//printf("ID: %ld  - Money: %f\n", in->id, in->money);
	
	withdraw(out, amount);
	deposit(in, amount);
	
	//printf("ID: %ld  - Money: %f\n", out->id, out->money);
	//printf("ID: %ld  - Money: %f\n\n", in->id, in->money);		
	
    return (1);
}
