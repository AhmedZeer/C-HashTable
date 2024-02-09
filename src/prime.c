#include <math.h>
#include "prime.h"

//Bruteforce way to know if it is  a prime number.

int is_prime(const int x){
  if ( x < 2 ){
    return -1;
  }

  if ( x < 4 ){
    return 1;
  }

  if( ( x % 2 ) == 0 ){
    return 0;
  }

  int i = 3;

  for( i ; i <= floor(sqrt((double)x)) ; i += 2){
    if( ( x % i ) == 0){
      return 0;
    }
  }

  return 1;
}

int next_prime(int x){
  while( is_prime(x) != 1 ){
    x++;
  }
  return x;
} 
