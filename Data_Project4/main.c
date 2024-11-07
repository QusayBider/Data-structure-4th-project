// Qusay Bider                      ID: 1220649                     sec:1
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <limits.h>
#define  max 50
#define MAX_COLLEGES 100
///////////////////////////////////////////////////////////////////////////////////// part one
 struct Edge {
    char name[max];
    int distance;
} ;
struct College {
    char name[max];
    struct Edge edges[MAX_COLLEGES];
    int edgeCount;
} ;
struct MinHeap {
    int Index;
    int maxsize;
    char Heap[MAX_COLLEGES][max] ;
    int distances[MAX_COLLEGES];
};
//////////////////////////////////////////////////////////////////////////////////// part Two
struct Prerequisites{
    char vertex_string[max];
    struct NodeCourse*next;
};
struct ListCourse{
    char course[max];
    struct ListCourse*next;
    int visit;
    int indegree;
    struct Prerequisites* prerq;
};
struct queue{
    char course[max];
    struct queue *next;
};
///////////////////////////////////////////////////////////////////////////////////
void Menu();
void loadData();
void MinHeap_initialize(int maxsize);
void insert_to_heap(char element[], int distance);
void mainHeapify(int position);
int parent (int);
int Left_Child(int);
int RightChild(int);
int isLeafNode(int);
char *minimum_element_inheap();
struct College *findCollege(char *name);
struct College *addCollege(char *name);
void addEdge(char *from, char *to, int distance);
void dijkstra(char *start, char *end);
void  printTheGraph();

void addCourse(char course[]);
void addPrerequisites(char course[] ,char Prerequisites_course[]);
void enqueue(struct ListCourse*);
void dequeue();
void topologicalSort();
void printCourseGraph();
void printSortedCourse();
///////////////////////////////////////////////////////////////////////////////////
int option;
struct queue *front=NULL;
struct queue *rear=NULL;
struct MinHeap heap;
struct ListCourse *list_course;
struct ListCourse *list_sorted;
struct College colleges[MAX_COLLEGES];
int collegeCount = 0;
char path[MAX_COLLEGES][max];
int pathSize = 0;
int short_distbance=0;
int indexx;
int cycle = 0;
///////////////////////////////////////////////////////////////////////////////////
int main(void) {
    Menu();
    scanf("%d", &option);
    do {
        switch (option) {
            case 1:loadData();
                break;
            case 2: {
                char start[max];
                char end[max] ;
                printf("Enter the start distinction\n");
                scanf("%s",start);
                printf("Enter the end distinction\n");
                scanf("%s",end);
                dijkstra(start,end);
            }
                break;
            case 3:{
                char start[max];
                char end[max] ;
                printf("Enter the start distinction\n");
                scanf("%s",start);
                printf("Enter the end distinction\n");
                scanf("%s",end);
                dijkstra(start,end);
            }
                break;
            case 4:printTheGraph();
                break;
            case 5:topologicalSort();
                break;
            case 6:printSortedCourse();
                break;
            case 7:{
                printCourseGraph();
            }
                break;
            case 8:{
                free(heap.Heap);
                free(path);
                free(colleges);
                exit(0);
            }
            default:
                printf("The option is not valid.\n");
        }
        Menu();
        scanf("%d", &option);
    } while (option != 8);
    return 0;
}

void loadData() {
    FILE *in,*input;
    char Temp_InputData[1000000];
    in = fopen("input_buildings.txt", "r");
    input=fopen("input_courses.txt", "r");
    if (in == NULL) {
        printf("Error! can't open the file\n");
        return;
    }
    while (fgets(Temp_InputData, sizeof(Temp_InputData), in) != NULL) {
        Temp_InputData[strcspn(Temp_InputData, "\n")] = '\0';
        char *s = strtok(Temp_InputData, "#");
        char *s2 = strtok(NULL, "#");
        int distance = atoi(strtok(NULL, "#"));
        addEdge(s, s2, distance);
    }
    if (input == NULL) {
        printf("Error! can't open the file\n");
        return;
    }
    while (fgets(Temp_InputData, sizeof(Temp_InputData), input) != NULL) {
        char *s = strtok(Temp_InputData, "#");
        if(s[0]!='\n')
            addCourse(s);

        char *s2 = strtok(NULL, "#");
        if(s2==NULL||s2[0]=='\n')
            continue;
        do {
            s2[strcspn(s2, "\n")] = '\0';
            addPrerequisites(s, s2);
            s2 = strtok(NULL, "#");
        } while (s2 != NULL);

    }
    printf("The data has been loaded\n");
}
struct College *addCollege(char *name) {                                                // Function to add a college.
    strcpy(colleges[collegeCount].name, name);
    colleges[collegeCount].edgeCount = 0;
    return &colleges[collegeCount++];
}
struct College *findCollege(char *name) {                                           // Function to find a college by name.
    for (int i = 0; i < collegeCount; i++) {
        if (strcmp(colleges[i].name, name) == 0) {
            indexx=i;
            return &colleges[i];
        }
    }
    return NULL;
}
void addEdge(char *from, char *to, int distance) {                                  // Function to add an edge between two colleges.
    struct College *fromCollege = findCollege(from);                         // in this line we are cheek if the college exists.
    if (fromCollege == NULL) {
        fromCollege = addCollege(from);
    }
    struct Edge *edge = &fromCollege->edges[fromCollege->edgeCount++];             // in this line, we are adding a new edge to college.
    strcpy(edge->name, to);
    edge->distance = distance;

    struct College *toCollege = findCollege(to);
    if (toCollege == NULL) {
        addCollege(to);
    }
}
void MinHeap_initialize(int maxsize) {                                                   // this function use to initialize a heap array.
    heap.maxsize = maxsize;
    heap.Index = 1;
    for (int i = 0; i < maxsize; i++) {                                                  // this for loop use for pass to all nodes in the array.
        strcpy(heap.Heap[i], "");
    }
}

int parent(int position) {                                                             // this function use to return the parent of a node.
    return position / 2;
}

int Left_Child(int position) {                                                        // this function use to return the left child of a node.
    return (2 * position) ;
}

int RightChild(int position) {                                                        // this function use to return the right child of a node.
    return (2 * position) + 1;
}

int isLeafNode(int position) {                                                        // this function use to check if a node is a leaf.
    return (position >= (heap.Index / 2) && position <= heap.Index);
}

void swap(int fposition, int sposition) {                                             // this function use to swap two nodes "fposition" first node in the heap,"sposition" second node in the heap.
    char tempString[max];
    int tempDistance;

    strcpy(tempString, heap.Heap[fposition]);
    tempDistance = heap.distances[fposition];

    strcpy(heap.Heap[fposition], heap.Heap[sposition]);
    heap.distances[fposition] = heap.distances[sposition];

    strcpy(heap.Heap[sposition], tempString);
    heap.distances[sposition] = tempDistance;
}
void insert_to_heap(char element[], int distance) {                                           // this function use to insert_to_heap an element into the heap.
    if (heap.Index >= heap.maxsize) {
        return;
    }
    strcpy(heap.Heap[heap.Index], element);
    heap.distances[heap.Index] = distance;
    int current = heap.Index++;
    while (current > 0 && heap.distances[current] < heap.distances[parent(current)]) {       // this line cheek and swap if the child distances less than parent dictance node.
        swap(current, parent(current));
        current = parent(current);
    }
}
void mainHeapify(int position) {                                                                // Function to heapify the node at position.
    if (!isLeafNode(position)) {
        int left = Left_Child(position);
        int right = RightChild(position);
        int smallest = position;

        if (left < heap.Index && heap.distances[left] < heap.distances[smallest]) {
            smallest = left;
        }
        if (right < heap.Index && heap.distances[right] < heap.distances[smallest]) {
            smallest = right;
        }

        if (smallest != position) {                                                 // cheek if satisfies the heap property.
            swap(position, smallest);
            mainHeapify(smallest);
        }
    }
}

char *minimum_element_inheap() {                                                     // this function use to extract the minimum element from the heap.
    if (heap.Index == 0) {
        return NULL;
    }
    static char minElement[max];
    strcpy(minElement, heap.Heap[1]);
    strcpy(heap.Heap[1], heap.Heap[--heap.Index]);
    heap.distances[1] = heap.distances[heap.Index];
    mainHeapify(1);
    return minElement;
}
void dijkstra(char *start, char *end) {                                           // Function to implement Dijkstra's algorithm to find the shortest path.
    MinHeap_initialize(MAX_COLLEGES);
    int distances[MAX_COLLEGES];
    int visited[MAX_COLLEGES] = {0};
    int previous_node[MAX_COLLEGES];                                             // "previous_node" An array to store the previous node in the shortest path.

    for (int i = 0; i < MAX_COLLEGES; i++) {
       distances[i] = INT_MAX;
        previous_node[i] = -1;
    }
    struct College *startCollege = findCollege(start);                    // this line use to cheek if the first distinction exists.
    if (startCollege == NULL) {
        printf("Start college not found.\n");
        return;
    }

    distances[indexx] = 0;
    insert_to_heap(start, 0);

    while (heap.Index > 0) {
        char *currentName = minimum_element_inheap();
        struct College *currentCollege = findCollege(currentName);

        int copyindex = indexx;

        if (visited[indexx]) {
            continue;
        }
        visited[indexx] = 1;

        if (strcmp(currentName, end) == 0) {
            break;
        }

        for (int i = 0; i < currentCollege->edgeCount; i++) {
            struct Edge *edge = &currentCollege->edges[i];
            struct College *neighborCollege = findCollege(edge->name);

            if (!visited[indexx]) {
                int newDist = distances[copyindex] + edge->distance;
                if (newDist < distances[indexx]) {
                    distances[indexx] = newDist;
                    previous_node[indexx] = copyindex;
                    insert_to_heap(neighborCollege->name, newDist);
                }
            }
        }
    }
    struct College *endCollege = findCollege(end);
    if (endCollege == NULL || distances[indexx] == INT_MAX) {
        printf("No path found.\n");
        return;
    }
    pathSize = 0;
    for (int at = indexx; at != -1; at = previous_node[at]) {                      // store the shortest path
        strcpy(path[pathSize++], colleges[at].name);
    }
    short_distbance = distances[indexx];
    if(option==3) {
        printf("Shortest path: ");
        for (int i = pathSize - 1; i >= 0; i--) {
            printf("%s", path[i]);
            if (i > 0) {
                printf(" -> ");
            }
        }
    }
        printf("\nTotal distance: %d\n",short_distbance);

}
void  printTheGraph(){
    for (int i = 0; i < collegeCount; i++) {
        printf("building : %s ->\n",colleges[i].name);
        for (int j = 0; j < colleges[i].edgeCount; j++) {
            printf("building : %s distance :%d\n", colleges[i].edges[j].name, colleges[i].edges[j].distance);
        }
        printf("\n");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  part Two
void addCourse(char course[]) {
    struct ListCourse *newcourse = (struct ListCourse *) malloc(sizeof(struct ListCourse));
    strcpy(newcourse->course, course);
    newcourse->prerq = NULL;
    newcourse->next = NULL;
    newcourse->indegree = 0;
    newcourse->visit = 0;
    if (list_course == NULL) {                                        // If the list is empty, set the new course as the head of the list
        list_course = newcourse;
    } else {                                                         // Otherwise, find the end of the list and add the new course there
        struct ListCourse *current = list_course;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newcourse;
    }
}
void addPrerequisites(char course[], char Prerequisites_course[]) {
    struct Prerequisites *newPrerequisites = (struct Prerequisites *) malloc(sizeof(struct Prerequisites));
    strcpy(newPrerequisites->vertex_string, Prerequisites_course);
    newPrerequisites->next = NULL;
    struct ListCourse *currentCourse = list_course;                                                              // Find the course in the list
    while (currentCourse != NULL && strcmp(currentCourse->course, course) != 0) {
        currentCourse = currentCourse->next;
    }
    if (currentCourse->prerq == NULL) {                                                                       // Add the new prerequisite to the course
        currentCourse->prerq = newPrerequisites;
    } else {                                                                                                  // Otherwise, find the end of the current prerequisites list and add the new prerequisite

        struct Prerequisites *currentPrereq = currentCourse->prerq;
        while (currentPrereq->next != NULL) {
            currentPrereq = (struct Prerequisites *) currentPrereq->next;
        }
        currentPrereq->next = (struct NodeCourse *) newPrerequisites;
    }
    currentCourse->indegree++;
}

void enqueue(struct ListCourse* course ){
    struct queue* new_queue = (struct queue*)malloc(sizeof(struct queue));
    strcpy(new_queue->course,course->course);
    new_queue->next = NULL;
    if (front == NULL && rear == NULL) {
        front = rear = new_queue;
        return;
    }
    rear->next=new_queue;
    rear = rear->next;
}
void dequeue() {
    if (front == NULL) {
        printf("Queue is empty\n");
    } else {
        struct QueueNode *temp = (struct QueueNode *) front;              // Move front to the next node
        front = front->next;
        free(temp);
    }
}
void topologicalSort() {
    struct ListCourse *current = list_course;
    int count = 0;                                                         // To count the number of nodes processed

    while (current != NULL) {
        if (current->indegree == 0) {
            if (current->visit == 0) {
                enqueue(current);
                current->visit = 1;
            }
        }
        current = current->next;
    }
    while (front != NULL) {
        struct ListCourse *Temp2Course = list_course;
        while (Temp2Course != NULL) {
            struct Prerequisites *currentPrereq = Temp2Course->prerq;
            while (currentPrereq != NULL) {
                if (strcasecmp(currentPrereq->vertex_string, front->course) == 0) {
                    Temp2Course->indegree--;
                }
                currentPrereq = (struct Prerequisites *) currentPrereq->next;
            }
            Temp2Course = Temp2Course->next;
        }
        struct ListCourse *NewNode = (struct ListCourse*) malloc(sizeof(struct ListCourse));                     // Create a new node for the sorted list
        strcpy(NewNode->course, front->course);
        NewNode->next = NULL;
        NewNode->prerq = NULL;
        if (list_sorted == NULL) {                                                                                    // Add the new node to the end of list_sorted
            list_sorted = NewNode;
        } else {
            struct ListCourse *current = list_sorted;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = NewNode;
        }
        dequeue();
        count++;                                                      // Increment the count when a node is processed
        struct ListCourse *Temp = list_course;
        while (Temp != NULL) {
            if (Temp->indegree == 0) {
                if (Temp->visit == 0) {
                    enqueue(Temp);
                    Temp->visit = 1;
                }
            }
            Temp = Temp->next;
        }
    }
    current = list_course;                                            // Check if there are any nodes left with non-zero indegree
    /*while (current != NULL) {
        if (current->indegree > 0) {
            cycle = 1;
            break;
        }
        current = current->next;
    }*/
    if (cycle) {
        printf("There is a cycle in the graph\n");
    } else {
        printf("The Topological Sort Done\n");
    }
}

void printSortedCourse(){    // this function use to print the sorted course
    if(cycle==0) {
        while (list_sorted!=NULL) {
            printf("%s->",list_sorted->course);
            list_sorted=list_sorted->next;
        }
    }
    else
        printf("There is a cycle in the graph\n");

}
void printCourseGraph(){                                           // this function use to print the Graph of courses
    while (list_course!=NULL){
        printf("%s :",list_course->course);
        while (list_course->prerq!=NULL){
            printf("-> %s",list_course->prerq->vertex_string);
            list_course->prerq= (struct Prerequisites *) list_course->prerq->next;
        }
        printf("\n");
        list_course=list_course->next;
    }
}
void Menu() {
    printf("\n1. Load the two files."
           "\n2. Calculate the shortest distance between two buildings."
           "\n3. Print the shortest route between two buildings and the total distance."
           "\n4. print the Graph of building"
           "\n5. Sort the courses using the topological sort."
           "\n6. Print the sorted courses."
           "\n7. Print the Graph courses."
           "\n8. Exit the application.\n Please enter your option :\n");
}