// Qusay Bider                      ID: 1220649                     sec:1
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define  max 50
#define MAX_COLLEGES 100
///////////////////////////////////////////////////////////////////////////////////// part one
typedef struct {
    char name[max];
    int distance;
} Edge;
typedef struct {
    char name[max];
    Edge edges[MAX_COLLEGES];
    int edgeCount;
} College;
struct MinHeap {
    int size;
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
College *findCollege(char *name);
College *addCollege(char *name);
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
College colleges[MAX_COLLEGES];
int collegeCount = 0;
char path[MAX_COLLEGES][max];
int pathSize = 0;
int short_distbance=0;
int indexx;
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
                printf("Shortest distance: %d\n",short_distbance);
            }
                break;
            case 3:{
                if(pathSize!=0) {
                    printf("Shortest path: ");
                    for (int i = pathSize - 1; i >= 0; i--) {
                        printf("%s", path[i]);
                        if (i > 0) {
                            printf(" -> ");
                        }
                    }
                    printf("\nTotal distance: %d\n", short_distbance);
                }
                else
                    printf("please inter to building to find the shortest path and distance\n");
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
            case 8: exit(0);
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

// Function to add a college
College *addCollege(char *name) {
    strcpy(colleges[collegeCount].name, name);
    colleges[collegeCount].edgeCount = 0;
    return &colleges[collegeCount++];
}

// Function to find a college by name
College *findCollege(char *name) {
    for (int i = 0; i < collegeCount; i++) {
        if (strcmp(colleges[i].name, name) == 0) {
            indexx=i;
            return &colleges[i];
        }
    }
    return NULL;
}
// Function to add an edge between two colleges
void addEdge(char *from, char *to, int distance) {
    College *fromCollege = findCollege(from);
    if (fromCollege == NULL) {
        fromCollege = addCollege(from);
    }
    Edge *edge = &fromCollege->edges[fromCollege->edgeCount++];
    strcpy(edge->name, to);
    edge->distance = distance;

    College *toCollege = findCollege(to);
    if (toCollege == NULL) {
        addCollege(to);
    }
}
void MinHeap_initialize(int maxsize) {
    heap.maxsize = maxsize;
    heap.size = 0;
    for (int i = 0; i < maxsize; i++) {
        strcpy(heap.Heap[i], "");
    }
}

// Helper function to return the parent of a node
int parent(int position) {
    return position-1 / 2;
}

// Helper function to return the left child of a node
int Left_Child(int position) {
    return (2 * position)+1;
}

// Helper function to return the right child of a node
int RightChild(int position) {
    return (2 * position) + 2;
}

// Helper function to check if a node is a leaf
int isLeafNode(int position) {
    return (position >= (heap.size / 2) && position <= heap.size);
}

// Helper function to swap two nodes
void swap(int fposition, int sposition) {
    char tempStr[max];
    int tempDist;

    strcpy(tempStr, heap.Heap[fposition]);
    tempDist = heap.distances[fposition];

    strcpy(heap.Heap[fposition], heap.Heap[sposition]);
    heap.distances[fposition] = heap.distances[sposition];

    strcpy(heap.Heap[sposition], tempStr);
    heap.distances[sposition] = tempDist;
}

// Function to insert an element into the heap
void insert_to_heap(char element[], int distance) {
    if (heap.size >= heap.maxsize) {
        return;
    }
    strcpy(heap.Heap[heap.size], element);
    heap.distances[heap.size] = distance;
    int current = heap.size++;
    while (current > 0 && heap.distances[current] < heap.distances[parent(current)]) {
        swap(current, parent(current));
        current = parent(current);
    }
}
// Function to heapify the node at position
void mainHeapify(int position) {
    if (!isLeafNode(position)) {
        int left = Left_Child(position);
        int right = RightChild(position);
        int smallest = position;

        if (left < heap.size && heap.distances[left] < heap.distances[smallest]) {
            smallest = left;
        }
        if (right < heap.size && heap.distances[right] < heap.distances[smallest]) {
            smallest = right;
        }

        if (smallest != position) {
            swap(position, smallest);
            mainHeapify(smallest);
        }
    }
}

// Function to extract the minimum element from the heap
char *minimum_element_inheap() {
    if (heap.size == 0) {
        return NULL;
    }
    static char minElement[max];
    strcpy(minElement, heap.Heap[0]);
    strcpy(heap.Heap[0], heap.Heap[--heap.size]);
    heap.distances[0] = heap.distances[heap.size];
    mainHeapify(0);
    return minElement;
}
// Function to implement Dijkstra's algorithm to find the shortest path
void dijkstra(char *start, char *end) {
    MinHeap_init(MAX_COLLEGES);
    int distances[MAX_COLLEGES];
    int visited[MAX_COLLEGES] = {0};
    int prev[MAX_COLLEGES];

    for (int i = 0; i < MAX_COLLEGES; i++) {
        distances[i] = INT_MAX;
        prev[i] = -1;
    }
    College *startCollege = findCollege(start);
    if (startCollege == NULL) {
        printf("Start college not found.\n");
        return;
    }

    distances[indexx] = 0;
    insert(start, 0);

    while (heap.size > 0) {
        char *currentName = extractMin();
        College *currentCollege = findCollege(currentName);

        int copyindex=indexx;

        if (visited[indexx]) {
            continue;
        }
        visited[indexx] = 1;

        if (strcmp(currentName, end) == 0) {
            break;
        }

        for (int i = 0; i < currentCollege->edgeCount; i++) {
            Edge *edge = &currentCollege->edges[i];
            College *neighborCollege = findCollege(edge->name);

            if (!visited[indexx]) {
                int newDist = distances[ copyindex] + edge->distance;
                if (newDist < distances[indexx]) {
                    distances[indexx] = newDist;
                    prev[indexx] =  copyindex;
                    insert(neighborCollege->name, newDist);
                }
            }
        }
    }
    College *endCollege = findCollege(end);
    if (endCollege == NULL || distances[indexx] == INT_MAX) {
        printf("No path found.\n");
        return;
    }
    pathSize = 0;
    // Print the shortest path

    for (int at = indexx; at != -1; at = prev[at]) {
        strcpy(path[pathSize++], colleges[at].name);
    }
    short_distbance = distances[indexx];

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
    if (list_course == NULL) {            // If the list is empty, set the new course as the head of the list
        list_course = newcourse;
    } else {                             // Otherwise, find the end of the list and add the new course there
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
    // Find the course in the list
    struct ListCourse *currentCourse = list_course;
    while (currentCourse != NULL && strcmp(currentCourse->course, course) != 0) {
        currentCourse = currentCourse->next;
    }
    if (currentCourse->prerq == NULL) {                                                                   // Add the new prerequisite to the course
        currentCourse->prerq = newPrerequisites;
    } else {
        struct Prerequisites *currentPrereq = currentCourse->prerq;
        while (currentPrereq->next != NULL) {
            currentPrereq = (struct Prerequisites *) currentPrereq->next;
        }
        currentPrereq->next = (struct NodeCourse *) newPrerequisites;
    }
    currentCourse->indegree++;
}
void printCourseGraph(){
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
void printSortedCourse(){
    while (list_sorted!=NULL) {
        printf("%s->",list_sorted->course);
        list_sorted=list_sorted->next;
    }
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
        // Create a new node for the sorted list
        struct ListCourse *NewNode = (struct ListCourse*) malloc(sizeof(struct ListCourse));
        strcpy(NewNode->course, front->course);
        NewNode->next = NULL;
        NewNode->prerq = NULL;
        // Add the new node to the end of list_sorted
        if (list_sorted == NULL) {
            list_sorted = NewNode;
        } else {
            struct ListCourse *current = list_sorted;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = NewNode;
        }
        // Move front to the next node
        struct QueueNode *temp = (struct QueueNode *) front;
        front = front->next;
        free(temp);
    }
}
void topologicalSort(){
    struct ListCourse *current=list_course;
    while(current!=NULL){
        if(current->indegree==0){
            if(current->visit==0){
                enqueue(current);
                current->visit=1;
            }
        }
        current=current->next;
    }
    while (front!=NULL){
        dequeue();
        struct ListCourse *Temp=list_course;
        while (Temp!=NULL){
            if(Temp->indegree==0){
                if(Temp->visit==0){
                    enqueue(Temp);
                    Temp->visit=1;
                }
            }
            Temp=Temp->next;
        }
    }
    printf("The Topological Sort Done\n");
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