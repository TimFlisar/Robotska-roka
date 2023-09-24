#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

void sendData(const char *portname, const char *message)
{
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    hSerial = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Napaka pri odpiranju COM porta. Preverite, ce je pravilno nastavljen.\n");
        return;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        fprintf(stderr, "Napaka pri pridobivanju stanja COM porta.\n");
        CloseHandle(hSerial);
        return;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        fprintf(stderr, "Napaka pri nastavljanju stanja COM porta.\n");
        CloseHandle(hSerial);
        return;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        fprintf(stderr, "Napaka pri nastavljanju timeout-ov COM porta.\n");
        CloseHandle(hSerial);
        return;
    }

    DWORD bytes_written;
    if (!WriteFile(hSerial, message, strlen(message), &bytes_written, NULL))
    {
        fprintf(stderr, "Napaka pri pisanju v COM port.\n");
        CloseHandle(hSerial);
        return;
    }

    CloseHandle(hSerial);
}
double toDAG(const double x, const double y)
{
    // Izračun kota v radianih
    double kot_rad = atan2(y, x);

    // Pretvori v DEG
    double kot_DEG = kot_rad * 180 / M_PI;

    // Zagotovi, da je kot pozitiven
    if (kot_DEG < 0)
    {
        kot_DEG += 360;
    }
    return kot_DEG;
}
double toHipt(const double x, const double y)
{
    return sqrt(x * x + y * y);
}
double sssB(double a, double b, double c)
{
    double cosB = (pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * a * c);
    double kot_B_rad = acos(cosB);
    double kot_B_stopinje = kot_B_rad * (180 / M_PI);
    return kot_B_stopinje;
}
double sssA(double a, double b, double c)
{
    double cosA = (pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * b * c);
    double kot_A_rad = acos(cosA);
    double kot_A_stopinje = kot_A_rad * (180 / M_PI);
    return kot_A_stopinje;
}

int main()
{
    double stranica0 = 50, stranica1 = 200, stranica2 = 200;
    double os0DAG, os1DAG, os2DAG, os3DAG, os4DAG, os5DAG;
    double oddaljenostXYH, st12, ZminusST0;
    double os1DAGp1 = 90, os1DAGp2, os1DAGp3;
    double X, Y, Z;
    char port[6];
    int portnum;
    char YN;

    printf("Prosim, vnesite COM port (1, 2, 3, ..., 99): ");
    scanf("%d", port);
    sprintf(port, "COM%d", portnum);
    
    printf("Ali zelite spremeniti dolzine stranic, vpisi Y za ja,\ndolzine stranic so: stranica0 = 50, stranica1 = 200, stranica2 = 200\n:");
    scanf("%c", &YN);
    if (YN == 'Y' || YN == 'y')
    {
        printf("stranica0: ");
        scanf("%lf", &stranica0);
        printf("stranica1: ");
        scanf("%lf", &stranica1);
        printf("stranica2: ");
        scanf("%lf", &stranica2);
    }

    while (1)
    {
        printf("Vpisi X, Y in Z: ");
        scanf("%lf %lf %lf", &X, &Y, &Z);

        // izračun pomožne stranice oddaljenostXYH
        oddaljenostXYH = toHipt(X, Y);
        printf("\noddaljenostXYH: %lf\n", oddaljenostXYH);

        ZminusST0 = Z - stranica0;
        // izračun st12
        st12 = toHipt(oddaljenostXYH, ZminusST0);

        // izračun os1DAGp2
        os1DAGp2 = toDAG(oddaljenostXYH, ZminusST0);
        if (os1DAGp2 > 180)
        {
            os1DAGp2 = os1DAGp2 - 360;
        }
        printf("os1DAGp2: %lf\n", os1DAGp2);

        // izračun os1DAGp3
        os1DAGp3 = sssA(stranica2, stranica1, st12);
        printf("os1DAGp3: %lf\n", os1DAGp3);

        // izračun os1DAG
        os1DAG = os1DAGp1 + os1DAGp2 + os1DAGp3;
        printf("os1: %lf\n", os1DAG);

        // izračun osi 0
        os0DAG = toDAG(X, Y);
        printf("os0: %lf\n", os0DAG);
        // izračun osi 1
        // izračun osi 2
        // izračun osi 3
        // izračun osi 5

        char message[256];
        sprintf(message, "0:%lf 1:%lf", os0DAG, os1DAG);
        sendData(port, message);
        printf("Com port: %5c, num: %d\n", port, portnum);
        printf("\n-----------------------------------------------\n");
    }
}