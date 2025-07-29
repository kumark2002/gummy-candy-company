#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORKERS 50
#define MAX_PROJECTS 100

// Forward declarations
typedef struct Worker Worker;
typedef struct FeatureNode FeatureNode;
typedef struct Project Project;

struct Worker {
    char *name;
    Project **projects;
    int projectCount;
};

struct FeatureNode {
    char *feature;
    struct FeatureNode *next;
};

struct Project {
    char *name;
    Worker **workers;
    int workerCount;
    FeatureNode *features;
};

void menu(void);
void clearInputBuffer(void);
char *getChars(void);
int isStringEmpty(const char *name);
int isWorkerNameExists(Worker *workers[], int workerCount, const char *name);
int isProjectNameExists(Project *projects[], int projectCount, const char *name);
int isFeatureExists(FeatureNode *head, const char *featureName);
void joinCompany(Worker *workers[], int *workerCount);
void openNewProject(Worker *workers[], int workerCount, Project *projects[], int *projectCount);
void workerJoinsProject(Worker *workers[], int workerCount, Project *projects[], int projectCount);
void addWorkerToProject(Worker *worker, Project *project);
void displayWorkers(Worker *workers[], int workerCount);
void displayProjects(Project *projects[], int projectCount);
void displayAllProjects(Project *projects[], int projectCount);
void displayAllWorkers(Worker *workers[], int workerCount, Project *projects[]);
void workOnProject(Worker *workers[], int workerCount,Project *projects[], int *projectCount);
void addFeatureToProject(Project *project);
void removeFeatureFromProject(Project *project);
void leaveCompany(Worker *workers[], int *workerCount, Project *projects[], int *projectCount);
void freeProject(Project *project);
void freeFeatures(FeatureNode *head);
void freeAllAllocations(Worker *workers[], int workerCount, Project *projects[], int projectCount);

Worker *workers[MAX_WORKERS];
Project *projects[MAX_PROJECTS];
int workerCount = 0;
int projectCount = 0;

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;

    do {
        printf("\nWelcome!\n");
        printf("Please select the desired option:\n");
        printf("1. Join the company\n");
        printf("2. Open a new project\n");
        printf("3. Join an existing project\n");
        printf("4. See what projects are in work\n");
        printf("5. See which people are in what projects\n");
        printf("6. Work on existing project\n");
        printf("7. Leave the company\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer(); // Clearing input buffer to avoid unexpected behavior

        switch (choice) {
            case 1:
                joinCompany(workers, &workerCount);
                break;
            case 2:
                openNewProject(workers, workerCount, projects, &projectCount);
                break;
            case 3:
                workerJoinsProject(workers, workerCount, projects, projectCount);
                break;
            case 4:
                displayAllProjects(projects, projectCount);
                break;
            case 5:
                displayAllWorkers(workers, workerCount, projects);
                break;
            case 6:
                workOnProject(workers, workerCount,projects, &projectCount);
                break;
            case 7:
                leaveCompany(workers, &workerCount, projects, &projectCount);
                break;
            case 8:
                printf("Exiting...\n");
                freeAllAllocations(workers, workerCount, projects, projectCount);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char *getChars() {
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin);
    // Remove newline character if it exists
    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }
    // Allocate memory for the string and copy the contents from the buffer
    char *input = (char *)malloc(strlen(buffer) + 1);
    strcpy(input, buffer);
    return input;
}

int isStringEmpty(const char *name) {
    return strlen(name) == 0;
}

int isWorkerNameExists(Worker *workers[], int workerCount, const char *name) {
    for (int i = 0; i < workerCount; ++i) {
        if (strcmp(workers[i]->name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int isProjectNameExists(Project *projects[], int projectCount, const char *name) {
    for (int i = 0; i < projectCount; ++i) {
        if (strcmp(projects[i]->name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

int isFeatureExists(FeatureNode *head, const char *featureName) {
    while (head != NULL) {
        if (strcmp(head->feature, featureName) == 0) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

void joinCompany(Worker *workers[], int *workerCount) {
    if (*workerCount == MAX_WORKERS) {
        printf("Maximum number of workers reached.\n");
        return;
    }

    printf("Enter the name of the new worker: ");
    char *name = getChars();

    if (isStringEmpty(name)) {
        printf("Name cannot be empty.\n");
        free(name);
        return;
    }

    if (isWorkerNameExists(workers, *workerCount, name)) {
        printf("Worker with the same name already exists.\n");
        free(name);
        return;
    }

    Worker *newWorker = (Worker *)malloc(sizeof(Worker));
    newWorker->name = name;
    newWorker->projectCount = 0;
    newWorker->projects = NULL; // Initialize projects array to NULL

    workers[*workerCount] = newWorker;
    (*workerCount)++;
}


void openNewProject(Worker *workers[], int workerCount, Project *projects[], int *projectCount) {
    if (workerCount == 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }

    if (*projectCount == MAX_PROJECTS) {
        printf("Maximum number of projects reached.\n");
        return;
    }

    printf("Who are you? Choose a number:\n");
    for (int i = 0; i < workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
    printf("Enter your choice: ");
    int workerChoice;
    scanf("%d", &workerChoice);
    clearInputBuffer();

    if (workerChoice < 1 || workerChoice > workerCount) {
        printf("Invalid selection.\n");
        return;
    }

    printf("Enter the name of the new project: ");
    char *projectName = getChars();

    if (isStringEmpty(projectName)) {
        printf("Project name cannot be empty.\n");
        free(projectName);
        return;
    }

    if (isProjectNameExists(projects, *projectCount, projectName)) {
        printf("Project with the same name already exists.\n");
        free(projectName);
        return;
    }

    Project *newProject = (Project *)malloc(sizeof(Project));
    newProject->name = projectName;
    newProject->workerCount = 0;
    newProject->features = NULL;
    newProject->workers = NULL; // Initialize workers array to NULL
    addWorkerToProject(workers[workerChoice - 1], newProject);

    projects[*projectCount] = newProject;
    (*projectCount)++;
}
void workerJoinsProject(Worker *workers[], int workerCount, Project *projects[], int projectCount) {
    if (workerCount == 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }

    if (projectCount == 0) {
        printf("There are no projects open yet, please open a project first.\n");
        return;
    }

    printf("Select a worker to join a project:\n");
    for (int i = 0; i < workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
    printf("Enter your choice: ");
    int workerChoice;
    scanf("%d", &workerChoice);
    clearInputBuffer();

    if (workerChoice < 1 || workerChoice > workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    Worker *selectedWorker = workers[workerChoice - 1];

    printf("Choose a project to join:\n");
    for (int i = 0; i < projectCount; ++i) {
        printf("%d. %s (Workers: %d)\n", i + 1, projects[i]->name, projects[i]->workerCount);
    }
    printf("Enter your choice: ");
    int projectChoice;
    scanf("%d", &projectChoice);
    clearInputBuffer();

    if (projectChoice < 1 || projectChoice > projectCount) {
        printf("Invalid project selection.\n");
        return;
    }

    Project *selectedProject = projects[projectChoice - 1];

    if (selectedProject->workerCount >= 4) {
        printf("Project is full.\n");
        return;
    }

    // Check if the worker is trying to join his own project
    for (int i = 0; i < selectedProject->workerCount; ++i) {
        if (selectedProject->workers[i] == selectedWorker) {
            printf("This worker is already part of the project '%s'\n", selectedProject->name);
            return;
        }
    }

    // Check if the worker is trying to join a project they've created
    for (int i = 0; i < selectedWorker->projectCount; ++i) {
        if (selectedWorker->projects[i] == selectedProject) {
            printf("You cannot join a project you've created.\n");
            return;
        }
    }

    addWorkerToProject(selectedWorker, selectedProject);
    
}


void addWorkerToProject(Worker *worker, Project *project) {
    // Check if worker is already assigned to the project
    for (int i = 0; i < project->workerCount; ++i) {
        if (project->workers[i] == worker) {
            return;
        }
    }

    // Add worker to project
    if (project->workerCount == 0) {
        project->workers = (Worker **)malloc(sizeof(Worker *));
    } else {
        project->workers = (Worker **)realloc(project->workers, (project->workerCount + 1) * sizeof(Worker *));
    }

    project->workers[project->workerCount] = worker;
    project->workerCount++;
}

void displayWorkers(Worker *workers[], int workerCount) {
    if (workerCount == 0) {
        printf("There are no workers in the company yet.\n");
        return;
    }

    printf("Workers in the company:\n");
    for (int i = 0; i < workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
}

void displayProjects(Project *projects[], int projectCount) {
    if (projectCount == 0) {
        printf("There are no projects open yet.\n");
        return;
    }

    printf("Open projects:\n");
    for (int i = 0; i < projectCount; ++i) {
        printf("%d. %s\n", i + 1, projects[i]->name);
    }
}

void displayAllProjects(Project *projects[], int projectCount) {
    if (projectCount == 0) {
        printf("There are no projects open yet.\n");
        return;
    }

    for (int i = 0; i < projectCount; ++i) {
        printf("Project: %s\n", projects[i]->name);
       // printf("Workers in this project:\n");
        for (int j = 0; j < projects[i]->workerCount; ++j) {
            printf("- %s\n", projects[i]->workers[j]->name);
        }
        printf("Features of the project:\n");
        if (projects[i]->features == NULL) {
            printf("  No features\n");
        } else {
            FeatureNode *currentFeature = projects[i]->features;
            while (currentFeature != NULL) {
                printf("  - %s\n", currentFeature->feature);
                currentFeature = currentFeature->next;
            }
        }
        printf("\n");
    }
}


void displayAllWorkers(Worker *workers[], int workerCount, Project *projects[]) {
    if (workerCount == 0) {
        printf("There are no workers in the company yet.\n");
        return;
    }

   // printf("Workers in projects:\n");
    for (int i = 0; i < workerCount; ++i) {
        printf("Worker: %s\n", workers[i]->name);
         printf("  Projects this worker is involved in:\n");
        int found = 0;
        for (int j = 0; j < projectCount; ++j) {
            
            for (int k = 0; k < projects[j]->workerCount; ++k) {
                if (strcmp(projects[j]->workers[k]->name, workers[i]->name) == 0) {
                    printf("    - %s\n", projects[j]->name);
                    found = 1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        if (!found) {
            printf("    Not involved in any projects.\n");
        }
        printf("\n");
    }
}

void workOnProject(Worker *workers[], int workerCount, Project *projects[], int *projectCount) {
    if (workerCount == 0) {
        printf("There are no workers in the company yet, please join the company first.\n");
        return;
    }

    if (*projectCount == 0) {
        printf("There are no projects open yet, please open a project first.\n");
        return;
    }

    printf("Select a worker:\n");
    for (int i = 0; i < workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
    printf("Enter the worker's number: ");
    int workerChoice;
    scanf("%d", &workerChoice);
    clearInputBuffer();

    if (workerChoice < 1 || workerChoice > workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    Worker *selectedWorker = workers[workerChoice - 1];

    if (selectedWorker->projectCount > 0) {
        printf("The selected worker is already involved in a project.\n");
        return;
    }

    int found = 0;
    int projectNumber = 1; // Counter for project number

     printf("Select a project for %s:\n", selectedWorker->name);
// Check if worker is already in the project
int isWorkerInProject = 0;
for (int i = 0; i < *projectCount; ++i) {
    isWorkerInProject = 0; // Reset the flag for each project
    for (int j = 0; j < projects[i]->workerCount; ++j) {
        if (strcmp(projects[i]->workers[j]->name, selectedWorker->name) == 0) {
            isWorkerInProject = 1;
            break;
        }
    }
    if (isWorkerInProject) {
        continue; // Skip this project if worker is already in it
    }
    printf("%d. %s\n", projectNumber++, projects[i]->name);
}

   



    printf("Enter the project number: ");
    int projectChoice;
    scanf("%d", &projectChoice);
    clearInputBuffer();

    if (projectChoice < 1 || projectChoice > *projectCount) {
        printf("Invalid project selection.\n");
        return;
    }

    Project *selectedProject = projects[projectChoice - 1];

    // //Check if worker is already in the project
    // for (int i = 0; i < selectedProject->workerCount; ++i) {
    //     if (selectedProject->workers[i] == selectedWorker) {
    //         printf("This worker is already part of the project '%s'\n", selectedProject->name);
    //         return;
    //     }
    // }

    // Add the selected project to the worker
    selectedWorker->projects = (Project **)realloc(selectedWorker->projects, sizeof(Project *));
    selectedWorker->projects[selectedWorker->projectCount++] = selectedProject;

    // Add the worker to the project
    selectedProject->workers = (Worker **)realloc(selectedProject->workers, sizeof(Worker *));
    selectedProject->workers[selectedProject->workerCount++] = selectedWorker;

    // Add or remove features
    
        printf("Do you want to add or remove a feature? (add/remove): ");
        char action[10];
        scanf("%s", action);
        clearInputBuffer();

        if (strcmp(action, "add") == 0) {
            addFeatureToProject(selectedProject);
        } else if (strcmp(action, "remove") == 0) {
            removeFeatureFromProject(selectedProject);
        } else {
            printf("Invalid action. Please choose 'add', 'remove'.\n");
        }
    

   // selectedWorker->projectCount = 0; // Reset project count so worker can add or remove features again

    // Display the main menu
    printf("Welcome!\n");
    menu();
}




void addFeatureToProject(Project *project) {
    printf("Enter the name of the feature: ");
    char *featureName = getChars();

    if (isStringEmpty(featureName)) {
        printf("Feature name cannot be empty.\n");
        free(featureName);
        return;
    }

    if (isFeatureExists(project->features, featureName)) {
        printf("Feature already exists in the project.\n");
        free(featureName);
        return;
    }

    // Create a new feature node
    FeatureNode *newFeature = (FeatureNode *)malloc(sizeof(FeatureNode));
    newFeature->feature = featureName;
    newFeature->next = NULL;

    // Add the new feature to the end of the list
    if (project->features == NULL) {
        project->features = newFeature;
    } else {
        FeatureNode *current = project->features;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newFeature;
    }

}

void removeFeatureFromProject(Project *project) {
    if (project->features == NULL) {
        printf("There are no features in this project to remove.\n");
        return;
    }

    printf("Select a feature to remove:\n");
    FeatureNode *current = project->features;
    int k = 1; // Start counting from 1
    while (current != NULL) {
        printf("%d. %s\n", k, current->feature);
        current = current->next;
        k++;
    }

    printf("Enter the number of the feature to remove: ");
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice < 1 || choice >= k) {
        printf("Invalid feature selection.\n");
        return;
    }

    // Traverse the list again to find the selected feature
    FeatureNode *prev = NULL;
    current = project->features;
    k = 1; // Reset counter
    while (current != NULL) {
        if (k == choice) {
            if (prev == NULL) {
                // Deleting the first node
                project->features = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->feature);
            free(current);
            
            return;
        }
        prev = current;
        current = current->next;
        k++;
    }

    printf("Feature not found.\n");
}


void leaveCompany(Worker *workers[], int *workerCount, Project *projects[], int *projectCount) {
    if (*workerCount == 0) {
        printf("There are no workers in the company to leave.\n");
        return;
    }

    printf("Select a worker to leave:\n");
    for (int i = 0; i < *workerCount; ++i) {
        printf("%d. %s\n", i + 1, workers[i]->name);
    }
    printf("Enter the worker's number: ");
    int workerChoice;
    scanf("%d", &workerChoice);
    clearInputBuffer();

    if (workerChoice < 1 || workerChoice > *workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }

    Worker *selectedWorker = workers[workerChoice - 1];

    // Remove the worker from all projects
    for (int i = 0; i < selectedWorker->projectCount; ++i) {
        Project *project = selectedWorker->projects[i];
        int index = -1;
        for (int j = 0; j < project->workerCount; ++j) {
            if (project->workers[j] == selectedWorker) {
                index = j;
                break;
            }
        }
        if (index != -1) {
            // Remove worker from project
            for (int j = index; j < project->workerCount - 1; ++j) {
                project->workers[j] = project->workers[j + 1];
            }
            project->workerCount--;
        }
    }

    // Free worker's memory and remove from the workers array
    free(selectedWorker->name);
    free(selectedWorker->projects);
    free(selectedWorker);
    for (int i = workerChoice - 1; i < *workerCount - 1; ++i) {
        workers[i] = workers[i + 1];
    }
    (*workerCount)--;

   // printf("Worker left the company successfully.\n");
}

void freeProject(Project *project) {
    free(project->name);
    free(project->workers);
    freeFeatures(project->features);
    free(project);
}

void freeFeatures(FeatureNode *head) {
    while (head != NULL) {
        FeatureNode *temp = head;
        head = head->next;
        free(temp->feature);
        free(temp);
    }
}

void freeAllAllocations(Worker *workers[], int workerCount, Project *projects[], int projectCount) {
    // Free workers
    for (int i = 0; i < workerCount; ++i) {
        free(workers[i]->name);
        for (int j = 0; j < workers[i]->projectCount; ++j) {
            free(workers[i]->projects[j]); // Free each project pointer in the worker
        }
        free(workers[i]->projects); // Free projects array of the worker
        free(workers[i]);
    }
    // Free projects
    for (int i = 0; i < projectCount; ++i) {
        freeProject(projects[i]);
    }
}
