#include <stdio.h>
#include <string.h>

int main()
{
    char word[100]; // Dovolj prostora za besedo
    int number;

    printf("Prosim, vnesite besedo (COM1, COM2, ..., COM99): ");
    scanf("%s", word);

    // Preveri dolžino besede
    if (strlen(word) >= 4 && strlen(word) <= 6 && strncmp(word, "COM", 3) == 0)
    {
        // Preberi številko
        if (sscanf(word + 3, "%d", &number) == 1 && number >= 1 && number <= 99)
        {
            printf("Vnesli ste veljavno besedo: %s\n", word);
        }
        else
        {
            printf("Niste vnesli veljavne številke.\n");
        }
    }
    else
    {
        printf("Niste vnesli veljavne besede.\n");
    }
    double x;
    scanf("%lf", x);
    return 0;
}
