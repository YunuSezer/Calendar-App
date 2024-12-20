#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REMINDERS 100
#define MAX_MESSAGE_LENGTH 100
#define FILENAME "reminders.txt"

// Hatýrlatýcý yapýsý
typedef struct {
    int day, month, year, hour, minute;
    char message[MAX_MESSAGE_LENGTH];
} Reminder;

Reminder reminders[MAX_REMINDERS];
int reminderCount = 0;

// Her ayýn gün sayýlarý 2025 yýlý için
const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char *monthNames[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

void saveRemindersToFile() {
    int i;
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("\nError: Could not save reminders to file.\n");
        return;
    }

    for (i = 0; i < reminderCount; i++) {
        fprintf(file, "%d %d %d %d %d %s\n", reminders[i].day, reminders[i].month, reminders[i].year,
                reminders[i].hour, reminders[i].minute, reminders[i].message);
    }

    fclose(file);
    printf("\nReminders saved successfully!\n");
}

void loadRemindersFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("\nNo saved reminders found. Starting fresh.\n");
        return;
    }

    reminderCount = 0;
    while (fscanf(file, "%d %d %d %d %d %[^\n]s", &reminders[reminderCount].day, &reminders[reminderCount].month,
                   &reminders[reminderCount].year, &reminders[reminderCount].hour, &reminders[reminderCount].minute,
                   reminders[reminderCount].message) != EOF) {
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
    getchar(); // Clear the buffer
    fgets(newReminder.message, MAX_MESSAGE_LENGTH, stdin);
    newReminder.message[strcspn(newReminder.message, "\n")] = 0; // Remove newline

    reminders[reminderCount++] = newReminder;

    printf("\nReminder added successfully!\n");
    saveRemindersToFile();
}

void listReminders() {
    int i;
    if (reminderCount == 0) {
        printf("\nNo reminders saved!\n");
        return;
    }

    printf("\nSaved reminders:\n");
    for (i = 0; i < reminderCount; i++) {
        printf("%d. %02d/%02d/%04d %02d:%02d: %s\n", i + 1, reminders[i].day, reminders[i].month,
               reminders[i].year, reminders[i].hour, reminders[i].minute, reminders[i].message);
    }
}

void deleteReminder() {
    if (reminderCount == 0) {
        printf("\nNo reminders to delete!\n");
        return;
    }

    listReminders();
    int i;
    int index;
    printf("\nEnter the number of the reminder to delete: ");
    scanf("%d", &index);

    if (index < 1 || index > reminderCount) {
        printf("\nInvalid selection!\n");
        return;
    }

    // Þu anki hatýrlatýcýyý silmek için, hatýrlatýcýlarý kaydýrýyoruz
    for (i = index - 1; i < reminderCount - 1; i++) {
        reminders[i] = reminders[i + 1];
    }
    reminderCount--;

    printf("\nReminder deleted successfully!\n");
    saveRemindersToFile();
}

void displayCalendar() {
    int month, day;
    int i;
    int startDay = 3; // 1 Jan 2025 is a Wednesday (0 = Sunday, 1 = Monday, ... 6 = Saturday)

    printf("\n--- 2025 Calendar ---\n");
    for (month = 0; month < 12; month++) {
        printf("\n  %s\n", monthNames[month]);
        printf("Su Mo Tu We Th Fr Sa\n");

        // Ýlk hafta için boþluklarý yazdýrma
        for (i = 0; i < startDay; i++) {
            printf("   ");
        }

        for (day = 1; day <= daysInMonth[month]; day++) {
            printf("%2d ", day);

            // Haftanýn sonuna geldiðinde yeni bir satýr baþlat
            if ((startDay + day) % 7 == 0) {
                printf("\n");
            }
        }

        printf("\n");
        startDay = (startDay + daysInMonth[month]) % 7; // Sonraki ayýn ilk günü
    }
}

int main() {
    int choice;

    loadRemindersFromFile();

    while (1) {
        printf("\n--- Calendar Reminder ---\n");
        printf("1. Add Reminder\n");
        printf("2. List Reminders\n");
        printf("3. Delete Reminder\n");
        printf("4. View Calendar\n");
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
                displayCalendar();
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
