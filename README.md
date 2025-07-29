Below is a **sample run** of the program, followed by a deep dive into its **key function**, `addWorkerToProject`, and a flowchart illustrating its logic.

---

## 🖥️ Sample Program Output

```
Welcome!
Please select the desired option:
1. Join the company
2. Open a new project
3. Join an existing project
4. See what projects are in work
5. See which people are in what projects
6. Work on existing project
7. Leave the company
8. Exit
Enter your choice: 1
Enter the name of the new worker: Alice

Welcome!
Please select the desired option:
...
Enter your choice: 1
Enter the name of the new worker: Bob

Welcome!
Please select the desired option:
2
Who are you? Choose a number:
1. Alice
2. Bob
Enter your choice: 1
Enter the name of the new project: Apollo

Welcome!
Please select the desired option:
3
Select a worker to join a project:
1. Alice
2. Bob
Enter your choice: 2
Choose a project to join:
1. Apollo (Workers: 1)
Enter your choice: 1

Welcome!
Please select the desired option:
4
Project: Apollo
- Alice
Features of the project:
  No features

Welcome!
Please select the desired option:
8
Exiting.
```

---

## 🔑 Highlighted Function: `addWorkerToProject`

> **Signature:**
> `void addWorkerToProject(Worker *worker, Project *project);`
>
> **Purpose:**
> Safely adds a `Worker` pointer to a `Project`’s dynamic array of workers, avoiding duplicates and managing memory.

### **Step-by-Step Description**

1. **Duplicate Check**
   Loop through `project->workers[]`; if `worker` is already present, **return immediately**.
2. **Allocate or Resize**

   * If `project->workerCount == 0`, allocate a new array with `malloc(sizeof(Worker *))`.
   * Otherwise, use `realloc` to grow `project->workers` by one pointer.
3. **Add Worker**
   Store the `worker` pointer at `project->workers[project->workerCount]`.
4. **Increment**
   Increase `project->workerCount` by 1.
5. **Return**
   Exit the function, with the project now including the new team member.

---

## 📈 Flowchart of `addWorkerToProject`

This flowchart visualizes exactly how the function checks for existing members, allocates memory, appends the worker, updates the count, and exits—ensuring robust project membership management.
![Uploading image.png…]()
