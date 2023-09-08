#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846

int main()
{
    while (1)
    {
        double X, Y;

        // Vnos koordinat
        printf("Vnesi X in Y: ");
        scanf("%lf %lf", &X, &Y);

        // Preveri, če je točka v izhodišču
        if (X == 0 && Y == 0)
        {
            printf("Točka je v izhodišču.\n");
        }

        // Izračun kota v radianih
        double kot_rad = atan2(Y, X);

        // Pretvori v stopinje
        double kot_stopinje = kot_rad * 180 / M_PI;

        // Zagotovi, da je kot pozitiven
        if (kot_stopinje < 0)
        {
            kot_stopinje += 360;
        }

        printf("\n%.2lf\n-----------------------------------------------\n", kot_stopinje);
    }
    return 0;
}