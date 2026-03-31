# 📱 Social Media Management System (DSA Project)

A robust command-line application built in **C** that simulates core social media functionalities. This project demonstrates the practical application of multiple **Data Structures and Algorithms** to manage users, posts, and interactions efficiently.

## 🚀 Key Features & Data Structures Implemented

This project goes beyond simple coding by integrating several fundamental DSA concepts:

* **Linked Lists**: Used as the primary structure for dynamic **Post Management**. This allows for infinite post creation without pre-defining memory limits.
* **Stacks (LIFO)**: Implemented a `likeStack` to handle the **Undo Like** functionality. It records post IDs so users can revert their most recent activity.
* **Queues (FIFO)**: Utilized a `feedQueue` to generate and display the **News Feed**. This ensures posts are processed and displayed in the order they were created.
* **Sorting Algorithms**: Features a **Bubble Sort** implementation to rank posts by popularity (Likes), displaying a "Trending" feed in descending order.
* **Dynamic Memory Management**: Uses `malloc` for node creation and includes a `freePosts` function to prevent memory leaks, ensuring the system is efficient.

## 🛠️ System Components

| Component | Responsibility |
| :--- | :--- |
| **User Management** | Create and store user profiles in a structured array. |
| **Post Engine** | Create posts with unique IDs and content using dynamic allocation. |
| **Interaction System** | Like posts and undo likes using a stack-based history. |
| **Analytics** | Calculate total likes, total posts, and identify the "Most Liked Post". |

## 🎓 Academic Profile
## 👥 Contributors
This project was a collaborative effort by:
Group 28:
* **W.M. Kavindu Kalhara** (Index: 244221B) - Post Management System & DSA Implementation.
* **Course:** Data Structures & Algorithms (DSA)

## 💻 How to Run
1.  Ensure you have a C compiler (like `gcc`) installed.
2.  Compile the source code:
    ```bash
    gcc social_media.c -o social_media_app
    ```
3.  Run the executable:
    ```bash
    ./social_media_app
    ```
