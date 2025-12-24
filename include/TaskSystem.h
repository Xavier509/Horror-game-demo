#ifndef TASK_SYSTEM_H
#define TASK_SYSTEM_H

#include "Game.h"
#include <vector>
#include <string>

class TaskSystem {
public:
    TaskSystem();
    
    void initialize();
    void update(const Vector3& playerPos);
    
    bool checkTaskCompletion(const Vector3& playerPos);
    bool allTasksCompleted() const;
    
    int getCompletedTaskCount() const;
    int getTotalTaskCount() const { return tasks.size(); }
    
    std::vector<Task> getTasks() const { return tasks; }
    Task* getCurrentTask() { return currentTaskIndex < tasks.size() ? &tasks[currentTaskIndex] : nullptr; }
    
    void completeCurrentTask();
    
    std::string getTaskDescription() const;
    float getDistanceToCurrentTask(const Vector3& playerPos) const;
    
private:
    std::vector<Task> tasks;
    int currentTaskIndex;
    int completedTasks;
    
    float interactionRadius;
};

#endif // TASK_SYSTEM_H
