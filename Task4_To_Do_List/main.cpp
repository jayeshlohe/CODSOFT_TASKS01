#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

struct Task {
    string description;
    bool completed;
};

vector<Task> tasks;

void addTask() {
    string taskName;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter task description: ";
    getline(cin, taskName);

    if (taskName.empty()) {
        cout << "Task cannot be empty!\n";
        return;
    }

    tasks.push_back({taskName, false});
    cout << "Task added successfully!\n";
}

void viewTasks() {
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return;
    }

    cout << "\n===== YOUR TO-DO LIST =====\n";

    for (int i = 0; i < (int)tasks.size(); i++) {
        cout << i + 1 << ". " << tasks[i].description;
        cout << " [" << (tasks[i].completed
                         ? "Completed" : "Pending") << "]\n";
    }
}

void markCompleted() {
    if (tasks.empty()) {
        cout << "No tasks to mark as completed.\n";
        return;
    }

    viewTasks();

    int taskNumber;
    cout << "Enter task number to mark as completed: ";

    if (!(cin >> taskNumber)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (taskNumber >= 1 && taskNumber <= (int)tasks.size()) {
        tasks[taskNumber - 1].completed = true;
        cout << "Task marked as completed!\n";
    } else {
        cout << "Invalid task number!\n";
    }
}

void removeTask() {
    if (tasks.empty()) {
        cout << "No tasks to remove.\n";
        return;
    }

    viewTasks();

    int taskNumber;
    cout << "Enter task number to remove: ";

    if (!(cin >> taskNumber)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (taskNumber >= 1 && taskNumber <= (int)tasks.size()) {
        tasks.erase(tasks.begin() + taskNumber - 1);
        cout << "Task removed successfully!\n";
    } else {
        cout << "Invalid task number!\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n===== TO-DO LIST MANAGER =====\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Remove Task\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addTask();
                break;

            case 2:
                viewTasks();
                break;

            case 3:
                markCompleted();
                break;

            case 4:
                removeTask();
                break;

            case 5:
                cout << "Exiting To-Do List Manager. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
