#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Введите файл\n");
        return -1;
    }

    FILE* file_input = fopen(argv[1], "r");
    if (file_input == NULL)
    {
        printf("Не удалось открыть файл\n");
        return -1;
    }

    char meta[1024]; // вынимаем первые три строчки
    fgets(meta, sizeof(meta), file_input);
    fgets(meta, sizeof(meta), file_input);
    fgets(meta, sizeof(meta), file_input);
    int hours, minutes, seconds;
    float azimuth, ignored_value;

    char buf[1024];
    int prev_hours = -1, prev_minutes = -1, prev_seconds = -1;
    float prev_azimuth = 0;

    while (fgets(buf, sizeof(buf), file_input))
    {
        // читаем данные из строки, можно было сразу с файла
        if (sscanf(buf, "%d.%d.%d %f %f", &hours, &minutes, &seconds, &azimuth, &ignored_value) == 5)
        {
            // если мы не на первом шагу
            if (prev_hours != -1)
            {
                int current_time = hours * 3600 + minutes * 60 + seconds;
                int previous_time = prev_hours * 3600 + prev_minutes * 60 + prev_seconds;

                // обработка перехода времени через нулевое
                if (current_time < previous_time)
                {
                    current_time += 24 * 3600; // добавляем сутки
                }

                int delta_time = current_time - previous_time; // изменение времени на основе предыдущих данных

                if (delta_time > 0)
                {
                    float delta_azimuth = azimuth - prev_azimuth; // изменение азимута на основе предыдущих данных
                    if (delta_azimuth > 180.0)
                    {
                        delta_azimuth -= 360; // учёт перехода с 359 на 1
                    }
                    else if (delta_azimuth < -180.0)
                    {
                        delta_azimuth += 360; // учёт перехода с 1 на 359
                    }

                    float angular_speed = delta_azimuth / delta_time; // вычисляем скорость

                    printf("Время: %02d:%02d:%02d, Азимут: %.2f, Скорость: %.2f град/с\n",
                           hours, minutes, seconds, azimuth, angular_speed);
                }
            }

            // обновляем предыдущие данные
            prev_hours = hours;
            prev_minutes = minutes;
            prev_seconds = seconds;
            prev_azimuth = azimuth;
        }
    }

    fclose(file_input);
    return 0;
}
