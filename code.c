#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_CONTENT_LEN 200
#define STACK_SIZE 100
#define QUEUE_SIZE 100

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} User;

typedef struct Post {
    int id;
    char content[MAX_CONTENT_LEN];
    int likes;
    struct Post *next;
} Post;

typedef struct {
    int items[STACK_SIZE];
    int top;
} Stack;

typedef struct {
    Post *items[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;

User users[MAX_USERS];
int userCount = 0;
Post *postHead = NULL;
int nextPostId = 1;
Stack likeStack;
Queue feedQueue;

void initStack(Stack *s) {
    s->top = -1;
}

int isStackFull(Stack *s) {
    return s->top == STACK_SIZE - 1;
}

int isStackEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, int postId) {
    if (isStackFull(s)) {
        printf("Recent activity stack is full. Cannot record more likes.\n");
        return;
    }
    s->items[++(s->top)] = postId;
}

int pop(Stack *s) {
    if (isStackEmpty(s)) {
        printf("No recent like to undo.\n");
        return -1;
    }
    return s->items[(s->top)--];
}

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isQueueFull(Queue *q) {
    return q->count == QUEUE_SIZE;
}

int isQueueEmpty(Queue *q) {
    return q->count == 0;
}

void enqueue(Queue *q, Post *p) {
    if (isQueueFull(q)) {
        printf("Feed queue is full. Cannot enqueue more posts.\n");
        return;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->items[q->rear] = p;
    q->count++;
}

Post *dequeue(Queue *q) {
    Post *p;
    if (isQueueEmpty(q)) {
        return NULL;
    }
    p = q->items[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return p;
}

void displayFeedUsingQueue() {
    Post *current = postHead;
    initQueue(&feedQueue);

    while (current != NULL) {
        enqueue(&feedQueue, current);
        current = current->next;
    }

    if (isQueueEmpty(&feedQueue)) {
        printf("No posts in feed.\n");
        return;
    }

    printf("\n--- Feed (FIFO using Queue) ---\n");
    while (!isQueueEmpty(&feedQueue)) {
        Post *p = dequeue(&feedQueue);
        printf("ID: %d | Likes: %d | Content: %s\n", p->id, p->likes, p->content);
    }
    printf("--------------------------------\n");
}

void addUser() {
    char name[MAX_NAME_LEN];

    if (userCount == MAX_USERS) {
        printf("User limit reached. Cannot add more users.\n");
        return;
    }

    printf("Enter user name: ");
    scanf(" %[^\n]", name);

    users[userCount].id = userCount + 1;
    strncpy(users[userCount].name, name, MAX_NAME_LEN - 1);
    users[userCount].name[MAX_NAME_LEN - 1] = '\0';

    userCount++;
    printf("User added successfully!\n");
}

void viewUsers() {
    int i;

    if (userCount == 0) {
        printf("No users available.\n");
        return;
    }

    printf("\n--- Users ---\n");
    for (i = 0; i < userCount; i++) {
        printf("ID: %d | Name: %s\n", users[i].id, users[i].name);
    }
    printf("-------------\n");
}

Post *createPost(const char *content) {
    Post *newPost = (Post *)malloc(sizeof(Post));
    if (!newPost) {
        printf("Memory allocation failed for new post.\n");
        return NULL;
    }
    newPost->id = nextPostId++;
    strncpy(newPost->content, content, MAX_CONTENT_LEN - 1);
    newPost->content[MAX_CONTENT_LEN - 1] = '\0';
    newPost->likes = 0;
    newPost->next = NULL;
    return newPost;
}

void addPost() {
    char content[MAX_CONTENT_LEN];
    Post *newPost, *temp;

    printf("Enter post content: ");
    scanf(" %[^\n]", content);

    newPost = createPost(content);
    if (newPost == NULL) {
        return;
    }

    if (postHead == NULL) {
        postHead = newPost;
    } else {
        temp = postHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPost;
    }

    printf("Post added with ID: %d\n", newPost->id);
}

void viewPosts() {
    Post *temp = postHead;

    if (temp == NULL) {
        printf("No posts available.\n");
        return;
    }

    printf("\n--- Posts (Linked List) ---\n");
    while (temp != NULL) {
        printf("ID: %d | Likes: %d | Content: %s\n", temp->id, temp->likes, temp->content);
        temp = temp->next;
    }
    printf("---------------------------\n");

    displayFeedUsingQueue();
}

Post *findPostById(int id) {
    Post *temp = postHead;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void likePost() {
    int choice;
    int postId;
    Post *p;

    printf("\n1. Like a Post\n");
    printf("2. Undo Last Like\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Post ID to like: ");
        scanf("%d", &postId);

        p = findPostById(postId);
        if (p == NULL) {
            printf("Post with ID %d not found.\n", postId);
            return;
        }

        p->likes++;
        push(&likeStack, postId);
        printf("Post liked successfully!\n");
    } else if (choice == 2) {
        int lastLikedId = pop(&likeStack);
        if (lastLikedId == -1) {
            return;
        }
        p = findPostById(lastLikedId);
        if (p == NULL) {
            printf("The post for the last like no longer exists.\n");
            return;
        }
        if (p->likes > 0) {
            p->likes--;
            printf("Last like undone for Post ID %d.\n", lastLikedId);
        } else {
            printf("Post ID %d has no likes to undo.\n", lastLikedId);
        }
    } else {
        printf("Invalid choice in Like/Undo menu.\n");
    }
}

int countTotalPosts() {
    int count = 0;
    Post *temp = postHead;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

int countTotalLikes() {
    int total = 0;
    Post *temp = postHead;
    while (temp != NULL) {
        total += temp->likes;
        temp = temp->next;
    }
    return total;
}

void bubbleSortPostsByLikes(Post **arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j]->likes < arr[j + 1]->likes) {
                Post *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortPostsByLikes() {
    int n = countTotalPosts();
    Post *temp;
    int i = 0;

    if (n == 0) {
        printf("No posts to sort.\n");
        return;
    }

    Post **arr = (Post **)malloc(n * sizeof(Post *));
    if (!arr) {
        printf("Memory allocation failed for sorting.\n");
        return;
    }

    temp = postHead;
    while (temp != NULL) {
        arr[i++] = temp;
        temp = temp->next;
    }

    bubbleSortPostsByLikes(arr, n);

    printf("\n--- Posts Sorted by Likes (Descending) ---\n");
    for (i = 0; i < n; i++) {
        printf("ID: %d | Likes: %d | Content: %s\n", arr[i]->id, arr[i]->likes, arr[i]->content);
    }
    printf("------------------------------------------\n");

    free(arr);
}

void showMostLikedPost() {
    Post *temp = postHead;
    Post *mostLiked = NULL;

    if (temp == NULL) {
        printf("No posts available.\n");
        return;
    }

    mostLiked = temp;
    temp = temp->next;

    while (temp != NULL) {
        if (temp->likes > mostLiked->likes) {
            mostLiked = temp;
        }
        temp = temp->next;
    }

    printf("\nMost liked post:\n");
    printf("ID: %d | Likes: %d | Content: %s\n", mostLiked->id, mostLiked->likes, mostLiked->content);
    printf("Total posts: %d\n", countTotalPosts());
    printf("Total likes: %d\n", countTotalLikes());
}

void freePosts() {
    Post *temp;
    while (postHead != NULL) {
        temp = postHead;
        postHead = postHead->next;
        free(temp);
    }
}

void printMenu() {
    printf("\n====== Social Media System ======\n\n");
    printf("--- User ---\n");
    printf("1. Add User\n");
    printf("2. View Users\n\n");
    printf("--- Posts ---\n");
    printf("3. Add Post\n");
    printf("4. View Posts\n");
    printf("5. Like a Post\n\n");
    printf("--- Feed / Sorting ---\n");
    printf("6. Sort Posts by Likes\n");
    printf("7. Show Most Liked Post\n\n");
    printf("--- System ---\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    int exitProgram = 0;

    initStack(&likeStack);
    initQueue(&feedQueue);

    while (!exitProgram) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                addPost();
                break;
            case 4:
                viewPosts();
                break;
            case 5:
                likePost();
                break;
            case 6:
                sortPostsByLikes();
                break;
            case 7:
                showMostLikedPost();
                break;
            case 8:
                printf("Exiting...\n");
                exitProgram = 1;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    freePosts();
    return 0;
}