#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Task
{
private:
    int taskId;
    string title, description, dueDate;
    bool completed;

protected:
    int priority;

public:
    Task(int id, string t, string desc, string date, int prio = 1) : taskId(id), title(t), description(desc), dueDate(date), completed(false), priority(prio) {}

    virtual ~Task() {}

    virtual string getTaskType() const = 0;

    virtual void display() const
    {
        cout << "\n--- " << getTaskType() << " ---\n"
             << "ID: " << taskId << "\n"
             << "Title: " << title << "\n"
             << "Description: " << description << "\n"
             << "Due Date: " << dueDate << "\n"
             << "Priority: " << priority << "\n"
             << "Status: " << (completed ? "Completed" : "Pending") << "\n";
    }

    virtual int calcUrgency() const { return priority * (completed ? 0 : 1); }

    int getTaskId() const { return taskId; }

    string getTitle() const { return title; }

    string getDescription() const { return description; }

    string getDueDate() const { return dueDate; }

    bool isCompleted() const { return completed; }

    int getPriority() const { return priority; }

    void setCompleted(bool status) { completed = status; }

    void setPriority(int prio)
    {
        if (prio >= 1 && prio <= 5)
            priority = prio;
    }

    void setTitle(string t) { title = t; }

    void setDescription(string desc) { description = desc; }

    void setDueDate(string date) { dueDate = date; }

    virtual void save(ofstream &file) const
    {
        file << getTaskType() << "\n"
             << taskId << "\n"
             << title << "\n"
             << description << "\n"
             << dueDate << "\n"
             << priority << "\n"
             << completed << "\n";
    }

    friend bool compareTasks(const Task *t1, const Task *t2);

    bool operator>(const Task &other) const { return this->priority > other.priority; }
    bool operator<(const Task &other) const { return this->priority < other.priority; }
};

class PersonalTask : public Task
{
private:
    string category;

public:
    PersonalTask(int id, string t, string desc, string date, string cat, int prio = 2) : Task(id, t, desc, date, prio), category(cat) {}

    string getTaskType() const override { return "PERSONAL TASK"; }

    void display() const override
    {
        Task::display();
        cout << "Category: " << category << "\n";
    }

    int calcUrgency() const override { return Task::calcUrgency(); }

    void save(ofstream &file) const override
    {
        Task::save(file);
        file << category << "\n";
    }

    string getCategory() const { return category; }
};

class WorkTask : public Task
{
private:
    string projectName, assignedBy;

public:
    WorkTask(int id, string t, string desc, string date, string proj, string assigned, int prio = 3) : Task(id, t, desc, date, prio), projectName(proj), assignedBy(assigned) {}

    string getTaskType() const override { return "WORK TASK"; }

    void display() const override
    {
        Task::display();
        cout << "Project: " << projectName << "\n"
             << "Assigned By: " << assignedBy << "\n";
    }

    int calcUrgency() const override { return Task::calcUrgency() * 2; }

    void save(ofstream &file) const override
    {
        Task::save(file);
        file << projectName << "\n"
             << assignedBy << "\n";
    }

    string getProjectName() const { return projectName; }
};

class UrgentTask : public Task
{
private:
    string escalationContact;
    int hoursRemaining;

public:
    UrgentTask(int id, string t, string desc, string date, string contact, int hours, int prio = 5) : Task(id, t, desc, date, prio), escalationContact(contact), hoursRemaining(hours) {}

    string getTaskType() const override { return "URGENT TASK"; }

    void display() const override
    {
        Task::display();
        cout << "Escalation Contact: " << escalationContact << "\n"
             << "Hours Remaining: " << hoursRemaining << "\n";

        if (hoursRemaining < 24)
        {
            cout << "*** CRITICAL - LESS THAN 24 HOURS ***\n";
        }
    }

    int calcUrgency() const override
    {
        int baseScore = Task::calcUrgency() * 3;
        (hoursRemaining < 24) ? baseScore += 20 : (hoursRemaining < 48) ? baseScore += 10
                                                                        : baseScore += 0;
        return baseScore;
    }

    void save(ofstream &file) const override
    {
        Task::save(file);
        file << escalationContact << "\n"
             << hoursRemaining << "\n";
    }

    void updateHours(int hours) { hoursRemaining = hours; }
};

using TaskPtr = Task *;
using TaskVec = vector<Task *>;

bool compareTasks(const Task *t1, const Task *t2)
{
    if (t1->priority != t2->priority)
    {
        return t1->priority > t2->priority;
    }
    return !t1->completed && t2->completed;
}

class TaskManager
{
private:
    TaskVec tasks;
    int nextTaskId;
    string pendingTasksFile, completedTasksFile;

public:
    TaskManager(string pendingFile = "pending_tasks.txt", string completedFile = "completed_tasks.txt") : nextTaskId(1), pendingTasksFile(pendingFile), completedTasksFile(completedFile)
    {
        loadTasks();
    }

    ~TaskManager()
    {
        for_each(tasks.begin(), tasks.end(), [](Task *task)
                 { delete task; });
        tasks.clear();
    }

    void addPersonal()
    {
        string title, desc, date, category;
        int priority;

        cin.ignore();
        cout << "Enter task title: ";
        if (!getline(cin, title))
            return;
        cout << "Enter description: ";
        if (!getline(cin, desc))
            return;
        cout << "Enter due date (YYYY-MM-DD): ";
        if (!getline(cin, date))
            return;
        cout << "Enter category (Health/Learning/Hobby/Other): ";
        if (!getline(cin, category))
            return;
        cout << "Enter priority (1-5, 5 is highest): ";
        if (!(cin >> priority) || priority < 1 || priority > 5)
            return;

        Task *newTask = new PersonalTask(nextTaskId++, title, desc, date, category, priority);
        tasks.push_back(newTask);
        cout << "\nPersonal task added successfully!\n";
    }

    void addWork()
    {
        string title, desc, date, project, assignedBy;
        int priority;

        cin.ignore();
        cout << "Enter task title: ";
        if (!getline(cin, title))
            return;
        cout << "Enter description: ";
        if (!getline(cin, desc))
            return;
        cout << "Enter due date (YYYY-MM-DD): ";
        if (!getline(cin, date))
            return;
        cout << "Enter project name: ";
        if (!getline(cin, project))
            return;
        cout << "Enter assigned by: ";
        if (!getline(cin, assignedBy))
            return;
        cout << "Enter priority (1-5, 5 is highest): ";
        if (!(cin >> priority) || priority < 1 || priority > 5)
            return;

        Task *newTask = new WorkTask(nextTaskId++, title, desc, date, project, assignedBy, priority);
        tasks.push_back(newTask);
        cout << "\nWork task added successfully!\n";
    }

    void addUrgent()
    {
        string title, desc, date, contact;
        int priority, hours;

        cin.ignore();
        cout << "Enter task title: ";
        if (!getline(cin, title))
            return;
        cout << "Enter description: ";
        if (!getline(cin, desc))
            return;
        cout << "Enter due date (YYYY-MM-DD): ";
        if (!getline(cin, date))
            return;
        cout << "Enter escalation contact: ";
        if (!getline(cin, contact))
            return;
        cout << "Enter hours remaining: ";
        if (!(cin >> hours) || hours < 0)
            return;
        cout << "Enter priority (1-5, 5 is highest): ";
        if (!(cin >> priority) || priority < 1 || priority > 5)
            return;

        Task *newTask = new UrgentTask(nextTaskId++, title, desc, date, contact, hours, priority);
        tasks.push_back(newTask);
        cout << "\nUrgent task added successfully!\n";
    }

    void showAll() const
    {
        if (tasks.empty())
        {
            cout << "\nNo tasks available.\n";
            return;
        }

        cout << "\n========== ALL TASKS ==========\n";
        for (const Task *task : tasks)
        {
            task->display();
            cout << "Urgency Score: " << task->calcUrgency() << "\n";
        }
    }

    void showPending() const
    {
        bool found = false;
        cout << "\n========== PENDING TASKS ==========\n";
        for (const Task *task : tasks)
        {
            if (!task->isCompleted())
            {
                task->display();
                cout << "Urgency Score: " << task->calcUrgency() << "\n";
                found = true;
            }
        }
        if (!found)
            cout << "No pending tasks!\n";
    }

    void showCompleted() const
    {
        bool found = false;
        cout << "\n========== COMPLETED TASKS ==========\n";
        for (const Task *task : tasks)
        {
            if (task->isCompleted())
            {
                task->display();
                found = true;
            }
        }
        if (!found)
            cout << "No completed tasks!\n";
    }

    void markDone()
    {
        int id;
        cout << "Enter task ID to mark as completed: ";
        if (!(cin >> id))
            return;

        for (Task *task : tasks)
        {
            if (task->getTaskId() == id)
            {
                if (task->isCompleted())
                {
                    cout << "Task is already completed!\n";
                }
                else
                {
                    task->setCompleted(true);
                    cout << "Task marked as completed!\n";
                    saveCompleted(task);
                }
                return;
            }
        }
        cout << "Task not found!\n";
    }

    void deleteTask()
    {
        int id;
        cout << "Enter task ID to delete: ";
        if (!(cin >> id))
            return;

        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            if ((*it)->getTaskId() == id)
            {
                delete *it;
                tasks.erase(it);
                cout << "Task deleted successfully!\n";
                return;
            }
        }
        cout << "Task not found!\n";
    }

    void sortByPriority()
    {
        sort(tasks.begin(), tasks.end(), [](const TaskPtr &a, const TaskPtr &b)
             { return compareTasks(b, a); });

        cout << "\nTasks sorted by priority!\n";
    }

    void sortByUrgency()
    {
        sort(tasks.begin(), tasks.end(), [](const TaskPtr &a, const TaskPtr &b)
             { return a->calcUrgency() > b->calcUrgency(); });

        cout << "\nTasks sorted by urgency score!\n";
    }

    void saveTasks()
    {
        ofstream file(pendingTasksFile, ios::trunc);
        if (!file.is_open())
        {
            cerr << "Error: Cannot open file for writing!\n";
            return;
        }

        for (const Task *task : tasks)
            if (!task->isCompleted())
                task->save(file);

        file.close();
        cout << "Pending tasks saved to file successfully!\n";
    }

    void saveCompleted(const Task *task)
    {
        ofstream file(completedTasksFile, ios::app);
        if (!file.is_open())
        {
            cerr << "Error: Cannot open completed tasks file!\n";
            return;
        }

        task->save(file);
        file.close();
    }

    void loadTasks()
    {
        ifstream file(pendingTasksFile);
        if (!file.is_open())
        {
            cout << "No previous tasks file found. Starting fresh.\n";
            return;
        }

        string taskType;
        while (getline(file, taskType))
        {
            if (taskType.empty())
                continue;

            int id, priority;
            bool completed;
            string title, desc, date, line;

            if (!getline(file, line) || line.empty())
                break;

            try
            {
                id = stoi(line);
            }
            catch (...)
            {
                cerr << "Corrupt ID entry in file. Stopping load.\n";
                break;
            }

            if (!getline(file, title) || title.empty())
                break;
            if (!getline(file, desc) || desc.empty())
                break;
            if (!getline(file, date) || date.empty())
                break;
            if (!getline(file, line) || line.empty())
                break;

            try
            {
                priority = stoi(line);
            }
            catch (...)
            {
                cerr << "Corrupt priority entry in file. Stopping load.\n";
                break;
            }

            if (!getline(file, line))
                break;

            completed = (line == "1");

            Task *task = nullptr;

            if (taskType == "PERSONAL TASK")
            {
                string category;
                if (!getline(file, category))
                    break;
                task = new PersonalTask(id, title, desc, date, category, priority);
            }
            else if (taskType == "WORK TASK")
            {
                string project, assignedBy;
                if (!getline(file, project) || !getline(file, assignedBy))
                    break;
                task = new WorkTask(id, title, desc, date, project, assignedBy, priority);
            }
            else if (taskType == "URGENT TASK")
            {
                string contact;
                int hours;
                if (!getline(file, contact) || !getline(file, line))
                    break;
                try
                {
                    hours = stoi(line);
                }
                catch (...)
                {
                    cerr << "Corrupt hours entry in file. Stopping load.\n";
                    break;
                }
                task = new UrgentTask(id, title, desc, date, contact, hours, priority);
            }

            if (task != nullptr)
            {
                task->setCompleted(completed);
                tasks.push_back(task);
                if (id >= nextTaskId)
                    nextTaskId = (id + 1);
            }
        }

        file.close();
        cout << "Loaded " << tasks.size() << " tasks from file.\n";
    }

    void showStats() const
    {
        int total = tasks.size();
        int completed = 0, pending = 0, personal = 0, work = 0, urgent = 0;

        for (const Task *task : tasks)
        {
            (task->isCompleted()) ? completed++ : pending++;
            (task->getTaskType() == "PERSONAL TASK") ? personal++ : (task->getTaskType() == "WORK TASK") ? work++
                                                                : (task->getTaskType() == "URGENT TASK") ? urgent++
                                                                                                         : 0;
        }

        cout << "\n========== TASK STATISTICS ==========\n"
             << "Total Tasks: " << total << "\n"
             << "Completed: " << completed << "\n"
             << "Pending: " << pending << "\n"
             << "\nTask Types:" << "\n"
             << "  Personal Tasks: " << personal << "\n"
             << "  Work Tasks: " << work << "\n"
             << "  Urgent Tasks: " << urgent << "\n";

        if (total > 0)
            cout << "\nCompletion Rate: " << fixed << setprecision(1) << (completed * 100.0 / total) << "%\n";
    }
};

int main()
{
    TaskManager manager;
    int choice;

    do
    {
        cout << "\n============ TASK_MAN_ORION ============\n"
             << "1.  Add Personal Task\n"
             << "2.  Add Work Task\n"
             << "3.  Add Urgent Task\n"
             << "4.  Display All Tasks\n"
             << "5.  Display Pending Tasks\n"
             << "6.  Display Completed Tasks\n"
             << "7.  Mark Task as Completed\n"
             << "8.  Delete Task\n"
             << "9.  Sort Tasks by Priority\n"
             << "10. Sort Tasks by Urgency Score\n"
             << "11. Display Statistics\n"
             << "12. Save Tasks to File\n"
             << "13. Exit\n"
             << "===========================================\n"
             << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            manager.addPersonal();
            break;
        case 2:
            manager.addWork();
            break;
        case 3:
            manager.addUrgent();
            break;
        case 4:
            manager.showAll();
            break;
        case 5:
            manager.showPending();
            break;
        case 6:
            manager.showCompleted();
            break;
        case 7:
            manager.markDone();
            break;
        case 8:
            manager.deleteTask();
            break;
        case 9:
            manager.sortByPriority();
            break;
        case 10:
            manager.sortByUrgency();
            break;
        case 11:
            manager.showStats();
            break;
        case 12:
            manager.saveTasks();
            break;
        case 13:
            manager.saveTasks();
            cout << "\nThank you for using Task Scheduling System!\n"
                 << "Tasks have been saved. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }

    } while (choice != 13);

    return 0;
}
