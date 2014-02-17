#ifndef _ACCOUNT_H
#define _ACCOUNT_H

struct account {
   double money;    
   long id;
};

void withdraw(struct account *a, double amount);

void deposit(struct account *a, double amount);

int transfer(struct account *out, struct account *in, double amount);

#endif
