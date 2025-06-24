#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_LEN 256
#define FILENAME "tasks.txt"

typedef struct
{
    char description[MAX_LEN];
    int completed;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

// Load tasks from file
void load_tasks()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
        return;

    while (fgets(tasks[task_count].description, MAX_LEN, file) != NULL)
    {
        size_t len = strlen(tasks[task_count].description);
        if (len > 0 && tasks[task_count].description[len - 1] == '\n')
        {
            tasks[task_count].description[len - 1] = '\0';
        }

        // Check for [x] at start to mark as done
        if (strncmp(tasks[task_count].description, "[x] ", 4) == 0)
        {
            tasks[task_count].completed = 1;
            memmove(tasks[task_count].description, tasks[task_count].description + 4, strlen(tasks[task_count].description) - 3);
        }
        else
        {
            tasks[task_count].completed = 0;
        }

        task_count++;
    }

    fclose(file);
}

// Save tasks to file
void save_tasks()
{
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Error saving tasks!\n");
        return;
    }

    for (int i = 0; i < task_count; i++)
    {
        if (tasks[i].completed)
            fprintf(file, "[x] %s\n", tasks[i].description);
        else
            fprintf(file, "%s\n", tasks[i].description);
    }

    fclose(file);
}

// Display task list
void list_tasks()
{
    if (task_count == 0)
    {
        printf("\nNo tasks found.\n");
        return;
    }

    printf("\n--- TO-DO LIST ---\n");
    for (int i = 0; i < task_count; i++)
    {
        printf("%d. [%c] %s\n", i + 1, tasks[i].completed ? 'x' : ' ', tasks[i].description);
    }
}

// Add a task
void add_task()
{
    if (task_count >= MAX_TASKS)
    {
        printf("Task list full!\n");
        return;
    }

    printf("Enter new task: ");
    getchar(); // Consume leftover newline
    fgets(tasks[task_count].description, MAX_LEN, stdin);

    // Remove trailing newline
    size_t len = strlen(tasks[task_count].description);
    if (len > 0 && tasks[task_count].description[len - 1] == '\n')
    {
        tasks[task_count].description[len - 1] = '\0';
    }

    tasks[task_count].completed = 0;
    task_count++;

    printf("Task added!\n");
}

// Mark a task as done
void mark_done()
{
    int index;
    list_tasks();
    printf("\nEnter task number to mark as done: ");
    scanf("%d", &index);

    if (index < 1 || index > task_count)
    {
        printf("Invalid task number!\n");
        return;
    }

    tasks[index - 1].completed = 1;
    printf("Task marked as completed!\n");
}

// Remove a task
void remove_task()
{
    int index;
    list_tasks();
    printf("\nEnter task number to remove: ");
    scanf("%d", &index);

    if (index < 1 || index > task_count)
    {
        printf("Invalid task number!\n");
        return;
    }

    for (int i = index - 1; i < task_count - 1; i++)
    {
        tasks[i] = tasks[i + 1];
    }

    task_count--;
    printf("Task removed.\n");
}

// Show menu
void show_menu()
{
    printf("\n--- TO-DO LIST MENU ---\n");
    printf("1. View Tasks\n");
    printf("2. Add Task\n");
    printf("3. Mark Task as Done\n");
    printf("4. Remove Task\n");
    printf("5. Save & Exit\n");
    printf("------------------------\n");
    printf("Enter your choice: ");
}

int main()
{
    int choice;

    load_tasks();

    while (1)
    {
        show_menu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            list_tasks();
            break;
        case 2:
            add_task();
            break;
        case 3:
            mark_done();
            break;
        case 4:
            remove_task();
            break;
        case 5:
            save_tasks();
            printf("Tasks saved. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
