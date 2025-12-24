#include "TaskSystem.h"
#include <cmath>

TaskSystem::TaskSystem()
    : currentTaskIndex(0), completedTasks(0), interactionRadius(2.5f) {
}

void TaskSystem::initialize() {
    // Create tasks for the mansion level
    tasks.clear();
    currentTaskIndex = 0;
    completedTasks = 0;
    
    // Task 1: Find the study key
    Task task1;
    task1.id = 1;
    task1.description = "Find the study key in the entrance hall";
    task1.location = Vector3(10.0f, 1.0f, 10.0f);
    task1.radius = interactionRadius;
    task1.completed = false;
    tasks.push_back(task1);
    
    // Task 2: Unlock the study
    Task task2;
    task2.id = 2;
    task2.description = "Unlock the study door";
    task2.location = Vector3(25.0f, 1.0f, 15.0f);
    task2.radius = interactionRadius;
    task2.completed = false;
    tasks.push_back(task2);
    
    // Task 3: Read the research notes
    Task task3;
    task3.id = 3;
    task3.description = "Read the research notes on the desk";
    task3.location = Vector3(30.0f, 1.0f, 20.0f);
    task3.radius = interactionRadius;
    task3.completed = false;
    tasks.push_back(task3);
    
    // Task 4: Find the basement key
    Task task4;
    task4.id = 4;
    task4.description = "Find the basement key in the bedroom";
    task4.location = Vector3(40.0f, 1.0f, 35.0f);
    task4.radius = interactionRadius;
    task4.completed = false;
    tasks.push_back(task4);
    
    // Task 5: Enter the basement
    Task task5;
    task5.id = 5;
    task5.description = "Unlock and enter the basement";
    task5.location = Vector3(20.0f, 1.0f, 45.0f);
    task5.radius = interactionRadius;
    task5.completed = false;
    tasks.push_back(task5);
    
    // Task 6: Find the antidote formula
    Task task6;
    task6.id = 6;
    task6.description = "Find the antidote formula in the lab";
    task6.location = Vector3(15.0f, -5.0f, 50.0f);
    task6.radius = interactionRadius;
    task6.completed = false;
    tasks.push_back(task6);
    
    // Task 7: Collect chemical samples
    Task task7;
    task7.id = 7;
    task7.description = "Collect 3 chemical samples from the lab";
    task7.location = Vector3(10.0f, -5.0f, 55.0f);
    task7.radius = interactionRadius;
    task7.completed = false;
    tasks.push_back(task7);
    
    // Task 8: Escape through the front door
    Task task8;
    task8.id = 8;
    task8.description = "Escape through the front door";
    task8.location = Vector3(5.0f, 1.0f, 5.0f);
    task8.radius = interactionRadius;
    task8.completed = false;
    tasks.push_back(task8);
}

void TaskSystem::update(const Vector3& playerPos) {
    // Update task states if needed
}

bool TaskSystem::checkTaskCompletion(const Vector3& playerPos) {
    if (currentTaskIndex >= tasks.size()) {
        return false;
    }
    
    Task& currentTask = tasks[currentTaskIndex];
    if (currentTask.completed) {
        return false;
    }
    
    // Check if player is close enough to complete task
    Vector3 diff = currentTask.location - playerPos;
    float distance = diff.length();
    
    if (distance <= currentTask.radius) {
        currentTask.completed = true;
        completedTasks++;
        currentTaskIndex++;
        return true;
    }
    
    return false;
}

bool TaskSystem::allTasksCompleted() const {
    return completedTasks >= tasks.size();
}

int TaskSystem::getCompletedTaskCount() const {
    return completedTasks;
}

void TaskSystem::completeCurrentTask() {
    if (currentTaskIndex < tasks.size()) {
        tasks[currentTaskIndex].completed = true;
        completedTasks++;
        currentTaskIndex++;
    }
}

std::string TaskSystem::getTaskDescription() const {
    if (currentTaskIndex < tasks.size()) {
        return tasks[currentTaskIndex].description;
    }
    return "All tasks completed! Escape the mansion!";
}

float TaskSystem::getDistanceToCurrentTask(const Vector3& playerPos) const {
    if (currentTaskIndex < tasks.size()) {
        Vector3 diff = tasks[currentTaskIndex].location - playerPos;
        return diff.length();
    }
    return 0.0f;
}
