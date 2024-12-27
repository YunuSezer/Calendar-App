#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> // MessageBox ve Sleep için gerekli

#define MAX_REMINDERS 100
#define MAX_MESSAGE_LENGTH 100

// Renk kodlarý
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

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
void showCalendarOption();
void displayCalendar(int month, int year);

// Hatýrlatýcýlarý dosyaya kaydet
void saveRemindersToFile() {
    FILE *file = fopen("reminders.txt", "w");
    if (!file) {
        printf(RED "\nError: Could not save reminders to file.\n" RESET);
        return;
    }

    for (int i = 0; i < reminderCount; i++) {
        fprintf(file, "%d %d %d %d %d %s\n",
                reminders[i].day, reminders[i].month, reminders[i].year,
                reminders[i].hour, reminders[i].minute, reminders[i].message);
    }

    fclose(file);
    printf(GREEN "\nReminders saved successfully!\n" RESET);
}

// Hatýrlatýcýlarý dosyadan yükle
void loadRemindersFromFile() {
    FILE *file = fopen("reminders.txt", "r");
    if (!file) {
        printf(BLUE "\nNo saved reminders found. Starting fresh.\n" RESET);
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
    printf(GREEN "\nReminders loaded successfully!\n" RESET);
}

// Hatýrlatýcý ekleme
void addReminder() {
    if (reminderCount >= MAX_REMINDERS) {
        printf(RED "\nReminder list is full!\n" RESET);
        return;
    }

    Reminder newReminder;

    printf(YELLOW "\nEnter the date for the reminder (DD MM YYYY): " RESET);
    scanf("%d %d %d", &newReminder.day, &newReminder.month, &newReminder.year);

    printf(YELLOW "Enter the time for the reminder (HH MM): " RESET);
    scanf("%d %d", &newReminder.hour, &newReminder.minute);

    printf(YELLOW "Enter the message: " RESET);
    getchar(); // Input buffer temizle
    fgets(newReminder.message, MAX_MESSAGE_LENGTH, stdin);
    newReminder.message[strcspn(newReminder.message, "\n")] = 0; // Yeni satýrý kaldýr

    reminders[reminderCount++] = newReminder;

    printf(GREEN "\nReminder added successfully!\n" RESET);
    saveRemindersToFile();
}

// Hatýrlatýcýlarý listeleme
void listReminders() {
    if (reminderCount == 0) {
        printf(RED "\nNo reminders saved!\n" RESET);
        return;
    }

    printf(BLUE "\nSaved reminders:\n" RESET);
    for (int i = 0; i < reminderCount; i++) {
        printf(GREEN "%d. %02d/%02d/%04d %02d:%02d: %s\n" RESET, i + 1,
               reminders[i].day, reminders[i].month, reminders[i].year,
               reminders[i].hour, reminders[i].minute, reminders[i].message);
    }
}

// Hatýrlatýcý silme
void deleteReminder() {
    if (reminderCount == 0) {
        printf(RED "\nNo reminders to delete!\n" RESET);
        return;
    }

    listReminders();

    int index;
    printf(YELLOW "\nEnter the number of the reminder to delete: " RESET);
    scanf("%d", &index);

    if (index < 1 || index > reminderCount) {
        printf(RED "\nInvalid selection!\n" RESET);
        return;
    }

    for (int i = index - 1; i < reminderCount - 1; i++) {
        reminders[i] = reminders[i + 1];
    }
    reminderCount--;

    printf(GREEN "\nReminder deleted successfully!\n" RESET);
    saveRemindersToFile();
}

// Hatýrlatýcý kontrol etme
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
            printf(GREEN "\nReminder: %s\n" RESET, reminders[i].message);

            // Masaüstü bildirimi
            MessageBox(NULL, reminders[i].message, "Reminder Alert", MB_OK | MB_ICONINFORMATION);

            // Ayný hatýrlatýcý tekrar gösterilmesin diye 60 saniye bekle
            Sleep(60000);
        }
    }
}

// Takvim görüntüleme
void displayCalendar(int month, int year) {
    const char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Artýk yýl kontrolü
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }

    struct tm firstDay = {0};
    firstDay.tm_mday = 1;
    firstDay.tm_mon = month - 1; // tm_mon 0 tabanlýdýr
    firstDay.tm_year = year - 1900; // tm_year 1900'den baþlar
    mktime(&firstDay);

    int startDay = firstDay.tm_wday; // Haftanýn ilk günü (0 = Pazar)

    printf(BLUE "\n    %s %d\n" RESET, months[month - 1], year);
    printf(BLUE "Su  Mo  Tu  We  Th  Fr  Sa\n" RESET);

    for (int i = 0; i < startDay; i++) {
        printf("    ");
    }

    for (int day = 1; day <= daysInMonth[month - 1]; day++) {
        printf("%2d  ", day);

        if ((day + startDay) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void showCalendarOption() {
    int month, year;
    printf(YELLOW "\nEnter month and year (MM YYYY): " RESET);
    scanf("%d %d", &month, &year);

    if (month < 1 || month > 12 || year < 1) {
        printf(RED "\nInvalid date!\n" RESET);
        return;
    }

    displayCalendar(month, year);
}

int main() {
    int choice;

    loadRemindersFromFile();

    while (1) {
        printf(RED "\n--- Reminder Application ---\n" RESET);
        printf(RED "1. Add Reminder\n" RESET);
        printf(RED "2. List Reminders\n" RESET);
        printf(RED "3. Delete Reminder\n" RESET);
        printf(RED "4. Start Reminder Check\n" RESET);
        printf(RED "5. Show Calendar\n" RESET);
        printf(RED "6. Exit\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
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
                printf(GREEN "\nReminder checking started...\n" RESET);
                while (1) {
                    checkReminders();
                    Sleep(10000); // 10 saniyede bir kontrol
                }
                break;
            case 5:
                showCalendarOption();
                break;
            case 6:
                printf(GREEN "\nExiting...\n" RESET);
                saveRemindersToFile();
                exit(0);
            default:
                printf(RED "\nInvalid choice!\n" RESET);
        }
    }

    return 0;
}
