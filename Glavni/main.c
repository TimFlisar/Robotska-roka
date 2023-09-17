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
        fprintf(stderr, "Napaka pri odpiranju COM porta. Preverite, če je pravilno nastavljen.\n");
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
double toDAG(const int x, const int y)
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
double toHipt(const int x, const int y)
{
    return sqrt(x * x + y * y);
}

int main()
{
    while (1)
    {
        const double Zos0 = 220;
        double X, Y;

        // Vnos koordinat
        printf("Vnesi X in Y: ");
        scanf("%lf %lf", &X, &Y);

        // Izračun hipotenuze na pliskvi (Z0)
        double oddaljenostXYH = toHipt(X, Y);

        double os0DEG = toDAG(X, Y);
        double os1DEG = toDAG(Zos0, oddaljenostXYH);

        printf("\nos0: %lf\nos1: %lf", os0DEG, os1DEG);

        char message[256];
        sprintf(message, "0:%lf,1:%lf", os0DEG, os1DEG);
        sendData("COM5", message);
        printf("\n-----------------------------------------------\n");
    }
    // return 0;
}