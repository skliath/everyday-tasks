#include <cassert>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Task {
private:
    string title;
    bool completed;

public:
    Task(const string& title = "", bool completed = false)
            : title(title), completed(completed) {}

    const string& getTitle() const { return title; }
    bool isCompleted() const { return completed; }

    void setTitle(const string& newTitle) { title = newTitle; }
    void setCompleted(bool value) { completed = value; }
};

class TaskManager {
private:
    vector<Task> tasks;

public:
    void addTask(const string& title) {
        tasks.emplace_back(title, false);
    }

    bool removeTask(size_t index) {
        if (index >= tasks.size()) return false;
        tasks.erase(tasks.begin() + static_cast<long>(index));
        return true;
    }

    bool editTask(size_t index, const string& newTitle) {
        if (index >= tasks.size()) return false;
        tasks[index].setTitle(newTitle);
        return true;
    }

    bool toggleCompleted(size_t index) {
        if (index >= tasks.size()) return false;
        tasks[index].setCompleted(!tasks[index].isCompleted());
        return true;
    }

    const vector<Task>& getTasks() const { return tasks; }
    size_t size() const { return tasks.size(); }
};

// ------------------ ТЕСТИ ------------------

void testAddTask() {
    TaskManager manager;
    manager.addTask("Test task");

    assert(manager.size() == 1);
    assert(manager.getTasks()[0].getTitle() == "Test task");
    assert(manager.getTasks()[0].isCompleted() == false);
}

void testEditTask() {
    TaskManager manager;
    manager.addTask("Old title");

    bool ok = manager.editTask(0, "New title");
    assert(ok == true);
    assert(manager.getTasks()[0].getTitle() == "New title");
}

void testToggleCompleted() {
    TaskManager manager;
    manager.addTask("Task");

    bool ok1 = manager.toggleCompleted(0);
    assert(ok1 == true);
    assert(manager.getTasks()[0].isCompleted() == true);

    bool ok2 = manager.toggleCompleted(0);
    assert(ok2 == true);
    assert(manager.getTasks()[0].isCompleted() == false);
}

int main() {
    testAddTask();
    testEditTask();
    testToggleCompleted();

    cout << "All tests passed!\n";
    return 0;
}
