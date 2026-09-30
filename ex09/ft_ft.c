#include <stdio.h>
#include <limits.h>

void    ft_ft(int *nbr)
{
    *nbr = 42;
}

int main(void)
{
    int cases[] = {0, 1, -1, INT_MIN, INT_MAX};

    for (int i = 0; i < 5; i++)
    {
  //      int n = cases[i];

  //      ft_ft(&n);
        ft_ft(&cases[i]);
        printf("%d\n", cases[i]);
    }
    return 0;
}