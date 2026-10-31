#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

volatile sig_atomic_t alarm_triggered = 0;
volatile sig_atomic_t alarm_cancelled = 0;

void alarm_signal_handler(int signal)
{
    alarm_triggered = 1;
}

void cancel_signal_handler(int signal)
{
    alarm_cancelled = 1;
}

void alarm_process(time_t alarm_time)
{
    struct sigaction alarm_action;
    struct sigaction cancel_action;

    memset(&alarm_action, 0, sizeof(alarm_action));
    alarm_action.sa_handler = alarm_signal_handler;
    sigemptyset(&alarm_action.sa_mask);

    sigaction(SIGUSR1, &alarm_action, NULL);

    memset(&cancel_action, 0, sizeof(cancel_action));
    cancel_action.sa_handler = cancel_signal_handler;
    sigemptyset(&cancel_action.sa_mask);

    sigaction(SIGTERM, &cancel_action, NULL);

    timer_t timer_id;

    struct sigevent timer_event;
    memset(&timer_event, 0, sizeof(timer_event));

    timer_event.sigev_notify = SIGEV_SIGNAL;
    timer_event.sigev_signo = SIGUSR1;

    if (timer_create(CLOCK_REALTIME, &timer_event, &timer_id) == -1)
    {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    struct itimerspec timer_spec;
    memset(&timer_spec, 0, sizeof(timer_spec));

    timer_spec.it_value.tv_sec = alarm_time;
    timer_spec.it_value.tv_nsec = 0;

    if (timer_settime(timer_id, TIMER_ABSTIME, &timer_spec, NULL) == -1)
    {
        perror("timer_settime");
        timer_delete(timer_id);
        exit(EXIT_FAILURE);
    }

    while (!alarm_triggered && !alarm_cancelled)
    {
        pause();
    }

    if (alarm_triggered)
    {
        printf("\n");
        printf("========================================\n");
        printf("           ALARM RINGING!\n");
        printf("========================================\n");

        for (int i = 0; i < 10; i++)
        {
            printf("\a");
            fflush(stdout);
            usleep(300000);
        }

        printf("\nAlarm completed.\n");
    }
    else if (alarm_cancelled)
    {
        printf("\nAlarm process cancelled.\n");
    }

    timer_delete(timer_id);

    exit(EXIT_SUCCESS);
}

int create_alarm_time(char *input, time_t *result)
{
    int hour;
    int minute;

    if (sscanf(input, "%d:%d", &hour, &minute) != 2)
    {
        return 0;
    }

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        return 0;
    }

    time_t current_time = time(NULL);

    struct tm alarm_tm = *localtime(&current_time);

    alarm_tm.tm_hour = hour;
    alarm_tm.tm_min = minute;
    alarm_tm.tm_sec = 0;

    time_t alarm_time = mktime(&alarm_tm);

    if (alarm_time <= current_time)
    {
        alarm_tm.tm_mday++;
        alarm_time = mktime(&alarm_tm);
    }

    *result = alarm_time;

    return 1;
}

void display_current_time()
{
    time_t current_time = time(NULL);

    struct tm *current_tm = localtime(&current_time);

    char time_string[100];

    strftime(
        time_string,
        sizeof(time_string),
        "%Y-%m-%d %H:%M:%S",
        current_tm
    );

    printf("\nCurrent System Time: %s\n", time_string);
}

void display_alarm(time_t alarm_time, int alarm_active)
{
    if (!alarm_active)
    {
        printf("\nNo alarm is currently set.\n");
        return;
    }

    struct tm *alarm_tm = localtime(&alarm_time);

    char alarm_string[100];

    strftime(
        alarm_string,
        sizeof(alarm_string),
        "%Y-%m-%d %H:%M:%S",
        alarm_tm
    );

    printf("\nScheduled Alarm: %s\n", alarm_string);
    printf("Status: ACTIVE\n");
}

int main()
{
    int choice;

    pid_t alarm_pid = -1;

    time_t scheduled_alarm = 0;

    int alarm_active = 0;

    char input[100];

    printf("\n");
    printf("========================================\n");
    printf("             ALARM CLOCK\n");
    printf("========================================\n");

    while (1)
    {
        if (alarm_active)
        {
            int status;

            pid_t result = waitpid(
                alarm_pid,
                &status,
                WNOHANG
            );

            if (result == alarm_pid)
            {
                alarm_active = 0;
                alarm_pid = -1;
            }
        }

        printf("\n");
        printf("------------- MENU --------------------\n");
        printf("1. Set Alarm\n");
        printf("2. View Alarm\n");
        printf("3. Cancel Alarm\n");
        printf("4. View Current Time\n");
        printf("5. Exit\n");
        printf("----------------------------------------\n");

        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        choice = atoi(input);

        if (choice == 1)
        {
            if (alarm_active)
            {
                printf("\nAn alarm is already active.\n");
                printf("Cancel it before setting a new alarm.\n");
                continue;
            }

            printf("\nEnter alarm time (HH:MM): ");

            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                continue;
            }

            input[strcspn(input, "\n")] = '\0';

            time_t new_alarm;

            if (!create_alarm_time(input, &new_alarm))
            {
                printf("\nInvalid time format.\n");
                printf("Please use HH:MM, for example 08:30.\n");
                continue;
            }

            alarm_pid = fork();

            if (alarm_pid < 0)
            {
                perror("fork");
                alarm_pid = -1;
                continue;
            }

            if (alarm_pid == 0)
            {
                alarm_process(new_alarm);
            }
            else
            {
                scheduled_alarm = new_alarm;
                alarm_active = 1;

                printf("\nAlarm successfully set.\n");
                printf("Alarm Process ID: %d\n", alarm_pid);

                struct tm *alarm_tm = localtime(&scheduled_alarm);

                char alarm_string[100];

                strftime(
                    alarm_string,
                    sizeof(alarm_string),
                    "%Y-%m-%d %H:%M:%S",
                    alarm_tm
                );

                printf("Alarm Time: %s\n", alarm_string);
            }
        }

        else if (choice == 2)
        {
            display_alarm(
                scheduled_alarm,
                alarm_active
            );
        }

        else if (choice == 3)
        {
            if (!alarm_active)
            {
                printf("\nNo active alarm to cancel.\n");
            }
            else
            {
                if (kill(alarm_pid, SIGTERM) == -1)
                {
                    perror("kill");
                }
                else
                {
                    waitpid(
                        alarm_pid,
                        NULL,
                        0
                    );

                    alarm_active = 0;
                    alarm_pid = -1;

                    printf("\nAlarm cancelled successfully.\n");
                }
            }
        }

        else if (choice == 4)
        {
            display_current_time();
        }

        else if (choice == 5)
        {
            if (alarm_active)
            {
                kill(alarm_pid, SIGTERM);

                waitpid(
                    alarm_pid,
                    NULL,
                    0
                );
            }

            printf("\nAlarm Clock closed.\n");

            break;
        }

        else
        {
            printf("\nInvalid choice.\n");
            printf("Please enter a number from 1 to 5.\n");
        }
    }

    return 0;
}
