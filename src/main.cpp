#include <iostream>
#include <vector>
#include <fstream>

#define MAX_TASKS 15
#define TASKS_VECTOR_FILE_PATH "./bin/vector.dat"

//Function declaration
void readTasksFile();
void writeTasksFile();
void printTasks();
short menu();
void processOption(short option);
void createTask();
void completeTask();
void deleteTask();
void deleteCompleteTasks();

class Task{
private:
    std::string taskText;
    bool complete;
public:
    Task(std::string text){
        taskText = text;
        complete = false;
    }
    void setText(std::string newText){
        taskText = newText;
    }
    void completeTask(){
        if(complete){
            std::cout << "Task \'" << taskText << "\' is already complete.\n\n";
            return;
        }
        complete = true;
    }
    std::string text(){
        return taskText;
    }
    bool isComplete(){
        return complete;
    }
};

std::vector<Task> tasks;
std::ifstream readFile;
std::ofstream writeFile;

int main(void){
    short option;

    readTasksFile();

    do{
        printTasks();
        option = menu();
        if(option == 6){
            std::cout << "Exiting...\n\n";
            break;
        }
        processOption(option);
    }while(true);

    writeTasksFile();

    return 0;
}

void readTasksFile(){
    readFile.open(TASKS_VECTOR_FILE_PATH, std::ios::binary);
    if(!readFile){
        writeFile.open(TASKS_VECTOR_FILE_PATH, std::ios::binary);
        writeFile.close();
        readFile.open(TASKS_VECTOR_FILE_PATH, std::ios::binary);
    }

    Task readTask("");
    while(readFile.read(reinterpret_cast<char*>(&readTask), sizeof(Task))){
        tasks.push_back(readTask);
    }
    readFile.close();
}

void writeTasksFile(){
    writeFile.open(TASKS_VECTOR_FILE_PATH, std::ios::binary);
    for(const Task t : tasks)
        writeFile.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

void printTasks(){
    if(!tasks.empty()){
        int id = 0;

        std::cout << "---CURRENT TASKS---\n\n";
        for(auto t = tasks.begin() ; t < tasks.end() ; t++){
            std::cout << ++id << " - " << t->text() << "\n";
            if(t->isComplete()){
                std::cout << "Task complete!\n\n";
                continue;
            }
            std::cout << "Task not complete...\n\n";
        }
    }
    else
        std::cout << "No tasks assigned yet...\n\n";
    
}

short menu(){
    std::cout << "1. Create Task\n";
    std::cout << "2. Complete Task\n";
    std::cout << "3. Delete Task\n";
    std::cout << "4. Delete All Complete Tasks\n";
    std::cout << "5. Delete All Tasks\n\n";
    std::cout << "6. Exit\n\n";

    short opt;

    do{
        std::cout << "Enter an option: ";
        std::cin >> opt;
        if(opt >= 2 && opt <= 5 && tasks.empty()){
            std::cout << "No tasks yet - Create a task first!\n";
            opt = -1;
        }
    }while(opt < 1 || opt > 6);
    std::cout << std::endl;

    return opt;
}

void processOption(short option){
    switch(option){
        case 1:
            createTask();
            break;
        case 2:
            completeTask();
            break;
        case 3:
            deleteTask();
            break;
        case 4:
            deleteCompleteTasks();
            break;
        default:        // -> 5
            tasks.clear();
    }
}

void createTask(){
    if(tasks.size() == MAX_TASKS){
        std::cout << "The maximum number of tasks has already been reached...\n\n";
        return;
    }

    std::string text;
    std::cout << "What is your task?\n";
    std::getline(std::cin >> std::ws, text);

    Task newTask(text);
    tasks.insert(tasks.end(), newTask);

    std::cout << "Task \"" << text << "\" has been added!\n\n";
}

void completeTask(){
    int id;

    std::cout << "Enter the ID of the task to be completed: ";
    std::cin >> id;
    if(id < 1 || id > tasks.size()){
        std::cout << "That task doesn't exist...\n\n";
        return;
    }

    int i = 0;
    auto t = tasks.begin();

    while(++i < id)
        t++;
    
    t->completeTask();
    std::cout << "Task \'" << t->text() << "\' is now complete!\n\n";
}

void deleteTask(){
    int id;

    std::cout << "Enter the ID of the task to be deleted: ";
    std::cin >> id;
    if(id < 1 || id > tasks.size()){
        std::cout << "That task doesn't exist...\n\n";
        return;
    }

    int i = 0;
    auto t = tasks.begin();

    while(++i < id)
        t++;

    tasks.erase(t);
    std::cout << "Task \'" << t->text() << "\' has been removed!\n\n";
}

void deleteCompleteTasks(){
    for(auto t = tasks.begin() ; t < tasks.end() ; t++)
        if(t->isComplete()){
            tasks.erase(t);
            std::cout << "Task \'" << t->text() << "\' has been removed!\n";
        }
    std::cout << std::endl;
}