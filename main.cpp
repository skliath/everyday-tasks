#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using std::cin;
using std::cout;
using std::string;
using std::vector;

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

    void setTasks(const vector<Task>& newTasks) {
        tasks = newTasks;
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

    const vector<Task>& getTasks() const {
        return tasks;
    }

    bool empty() const { return tasks.empty(); }
    size_t size() const { return tasks.size(); }
};

class FileManager {
public:
    bool save(const string& filename, const vector<Task>& tasks) {
        std::ofstream out(filename);
        if (!out.is_open()) return false;

        for (const auto& t : tasks) {
            out << (t.isCompleted() ? 1 : 0) << '\t' << t.getTitle() << '\n';
        }
        return true;
    }

    vector<Task> load(const string& filename) {
        vector<Task> tasks;
        std::ifstream in(filename);
        if (!in.is_open()) return tasks;

        string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;

            bool completed = false;
            string title;

            size_t tabPos = line.find('\t');
            if (tabPos != string::npos) {
                string flag = line.substr(0, tabPos);
                title = line.substr(tabPos + 1);
                completed = (flag == "1");
            } else {
                title = line;
                completed = false;
            }

            tasks.emplace_back(title, completed);
        }

        return tasks;
    }
};

class Application {
private:
    TaskManager manager;
    FileManager file;
    const string filename = "tasks.txt";

    void waitForEnter() {
        cout << "\nНатисніть Enter, щоб продовжити...";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // один Enter
    }

    void showTasks() {
        const auto& tasks = manager.getTasks();
        if (tasks.empty()) {
            cout << "Список завдань порожній.\n";
            return;
        }

        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << (i + 1) << ". "
                 << (tasks[i].isCompleted() ? "[x] " : "[ ] ")
                 << tasks[i].getTitle() << '\n';
        }
    }

    int readInt() {
        int value;
        while (true) {
            cin >> value;
            if (!cin.fail()) {
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Введіть число: ";
        }
    }

    bool readTaskIndex(size_t& outIndex) {
        if (manager.empty()) {
            cout << "Список порожній.\n";
            return false;
        }

        cout << "Введіть номер завдання (1-" << manager.size() << "): ";
        int n = readInt();

        if (n < 1 || static_cast<size_t>(n) > manager.size()) {
            cout << "Невірний номер.\n";
            return false;
        }

        outIndex = static_cast<size_t>(n - 1);
        return true;

    }

public:
    void showMenu() {
        cout << "\n~~~ Everyday Tasks ~~~\n";
        cout << "1. Додати завдання\n";
        cout << "2. Переглянути список\n";
        cout << "3. Позначити виконане/невиконане\n";
        cout << "4. Редагувати назву завдання\n";
        cout << "5. Видалити завдання\n";
        cout << "6. Зберегти у файл\n";
        cout << "0. Вийти\n";
        cout << "Ваш вибір: ";
    }

    void handleChoice(int choice) {
        if (choice == 1) {
            cout << "Введіть назву завдання: ";
            string title;
            std::getline(cin, title);

            if (title.empty()) {
                cout << "Назва не може бути порожньою.\n";
                return;
            }

            manager.addTask(title);
            cout << "Завдання додано.\n";
        }
        else if (choice == 2) {
            showTasks();
            waitForEnter();
        }
        else if (choice == 3) {
            size_t idx;
            showTasks();
            if (!readTaskIndex(idx)) return;

            manager.toggleCompleted(idx);
            cout << "Статус змінено.\n";
        }
        else if (choice == 4) {
            size_t idx;
            showTasks();
            if (!readTaskIndex(idx)) return;

            cout << "Введіть нову назву: ";
            string newTitle;
            std::getline(cin, newTitle);

            if (newTitle.empty()) {
                cout << "Назва не може бути порожньою.\n";
                return;
            }

            manager.editTask(idx, newTitle);
            cout << "Завдання відредаговано.\n";
        }
        else if (choice == 5) {
            size_t idx;
            showTasks();
            if (!readTaskIndex(idx)) return;

            manager.removeTask(idx);
            cout << "Завдання видалено.\n";
        }
        else if (choice == 6) {
            bool ok = file.save(filename, manager.getTasks());
            cout << (ok ? "Збережено у файл.\n" : "Помилка збереження.\n");
        }
        else if (choice == 0) {
        }
        else {
            cout << "Невірний вибір.\n";
        }
    }

    void run() {
        vector<Task> loaded = file.load(filename);
        manager.setTasks(loaded);

        if (!loaded.empty()) {
            cout << "Завантажено завдань з файлу: " << loaded.size() << "\n";
        }

        while (true) {
            showMenu();
            int choice = readInt();

            if (choice == 0) {
                file.save(filename, manager.getTasks());
                cout << "До побачення! (Список збережено)\n";
                break;
            }

            handleChoice(choice);
        }
    }
};

int main() {
    Application app;
    app.run();
    return 0;
}
