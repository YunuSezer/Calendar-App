#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // MessageBox ve Sleep için gerekli

#define MAX_REMINDERS 100
#define MAX_MESSAGE_LENGTH 100

typedef struct {
    int day, month, year, hour, minute;
    char message[MAX_MESSAGE_LENGTH];
} Reminder;

Reminder reminders[MAX_REMINDERS];
int reminderCount = 0;

void addReminder();
void listReminders();
void checkReminders();
void deleteReminder();
void saveRemindersToFile();
void loadRemindersFromFile();

void saveRemindersToFile() {
    FILE *file = fopen("reminders.txt", "w");
    if (!file) {
        printf("\nError: Could not save reminders to file.\n");
        return;
    }

    for (int i = 0; i < reminderCount; i++) {
        fprintf(file, "%d %d %d %d %d %s\n",
                reminders[i].day, reminders[i].month, reminders[i].year,
                reminders[i].hour, reminders[i].minute, reminders[i].message);
    }

    fclose(file);
    printf("\nReminders saved successfully!\n");
}

void loadRemindersFromFile() {
    FILE *file = fopen("reminders.txt", "r");
    if (!file) {
        printf("\nNo saved reminders found. Starting fresh.\n");
        return;
    }

    while (fscanf(file, "%d %d %d %d %d %[^\n]s",
                  &reminders[reminderCount].day, &reminders[reminderCount].month,
                  &reminders[reminderCount].year, &reminders[reminderCount].hour,
                  &reminders[reminderCount].minute, reminders[reminderCount].message) != EOF) {
        reminderCount++;
        if (reminderCount >= MAX_REMINDERS) break;
    }

    fclose(file);
    printf("\nReminders loaded successfully!\n");
}

void addReminder() {
    if (reminderCount >= MAX_REMINDERS) {
        printf("\nReminder list is full!\n");
        return;
    }

    Reminder newReminder;

    printf("\nEnter the date for the reminder (DD MM YYYY): ");
    scanf("%d %d %d", &newReminder.day, &newReminder.month, &newReminder.year);

    printf("Enter the time for the reminder (HH MM): ");
    scanf("%d %d", &newReminder.hour, &newReminder.minute);

    printf("Enter the message: ");
    getchar(); // Input buffer temizle
    fgets(newReminder.message, MAX_MESSAGE_LENGTH, stdin);
    newReminder.message[strcspn(newReminder.message, "\n")] = 0; // Yeni satýrý kaldýr

    reminders[reminderCount++] = newReminder;

    printf("\nReminder added successfully!\n");
    saveRemindersToFile();
}

void listReminders() {
    if (reminderCount == 0) {
        printf("\nNo reminders saved!\n");
        return;
    }

    printf("\nSaved reminders:\n");
    for (int i = 0; i < reminderCount; i++) {
        printf("%d. %02d/%02d/%04d %02d:%02d: %s\n", i + 1,
               reminders[i].day, reminders[i].month, reminders[i].year,
               reminders[i].hour, reminders[i].minute, reminders[i].message);
    }
}

void deleteReminder() {
    if (reminderCount == 0) {
        printf("\nNo reminders to delete!\n");
        return;
    }

    listReminders();

    int index;
    printf("\nEnter the number of the reminder to delete: ");
    scanf("%d", &index);

    if (index < 1 || index > reminderCount) {
        printf("\nInvalid selection!\n");
        return;
    }

    for (int i = index - 1; i < reminderCount - 1; i++) {
        reminders[i] = reminders[i + 1];
    }
    reminderCount--;

    printf("\nReminder deleted successfully!\n");
    saveRemindersToFile();
}

void checkReminders() {
    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);

    for (int i = 0; i < reminderCount; i++) {
        if (reminders[i].year == (currentTime->tm_year + 1900) &&
            reminders[i].month == (currentTime->tm_mon + 1) &&
            reminders[i].day == currentTime->tm_mday &&
            reminders[i].hour == currentTime->tm_hour &&
            reminders[i].minute == currentTime->tm_min) {

            // Terminal bildirimi
            printf("\nReminder: %s\n", reminders[i].message);

            // Masaüstü bildirimi
            MessageBox(NULL, reminders[i].message, "Reminder Alert", MB_OK | MB_ICONINFORMATION);

            // Ayný hatýrlatýcý tekrar gösterilmesin diye 60 saniye bekle
            Sleep(60000);
        }
    }
}

int main() {
    int choice;

    loadRemindersFromFile();

    while (1) {
        printf("\n--- Reminder Application ---\n");
        printf("1. Add Reminder\n");
        printf("2. List Reminders\n");
        printf("3. Delete Reminder\n");
        printf("4. Start Reminder Check\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addReminder();
                break;
            case 2:
                listReminders();
                break;
            case 3:
                deleteReminder();
                break;
            case 4:
                printf("\nReminder checking started...\n");
                while (1) {
                    checkReminders();
                    Sleep(10000); // 10 saniyede bir kontrol
                }
                break;
            case 5:
                printf("\nExiting...\n");
                saveRemindersToFile();
                exit(0);
            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}
