#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 50

#define MATHS 1
#define PHYSICS 2
#define CHEMISTRY 3

typedef struct {
    int id;
    char name[50];
    char author[50];
    int department;
    int available;
} Book;

typedef struct {
    int id;
    char name[50];
    char password[20];
    int borrowedBook;
} Student;

static Book books[MAX_BOOKS];
static Student students[MAX_STUDENTS];
static int bookCount = 0;
static int studentCount = 0;

const char *departmentName(int dept)
{
    switch (dept) {
        case MATHS: return "Mathematics";
        case PHYSICS: return "Physics";
        case CHEMISTRY: return "Chemistry";
        default: return "Unknown";
    }
}

void saveData(void)
{
    FILE *fp = fopen("books.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", bookCount);
        for (int i = 0; i < bookCount; i++) {
            fprintf(fp, "%d %s %s %d %d\n",
                    books[i].id, books[i].name, books[i].author,
                    books[i].department, books[i].available);
        }
        fclose(fp);
    }

    fp = fopen("students.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", studentCount);
        for (int i = 0; i < studentCount; i++) {
            fprintf(fp, "%d %s %s %d\n",
                    students[i].id, students[i].name,
                    students[i].password, students[i].borrowedBook);
        }
        fclose(fp);
    }
}

void loadData(void)
{
    FILE *fp = fopen("books.txt", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%d", &bookCount) != 1 ||
            bookCount < 0 || bookCount > MAX_BOOKS) {
            bookCount = 0;
        } else {
            for (int i = 0; i < bookCount; i++) {
                if (fscanf(fp, "%d %49s %49s %d %d",
                           &books[i].id, books[i].name, books[i].author,
                           &books[i].department, &books[i].available) != 5) {
                    bookCount = i;
                    break;
                }
            }
        }
        fclose(fp);
    }

    fp = fopen("students.txt", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%d", &studentCount) != 1 ||
            studentCount < 0 || studentCount > MAX_STUDENTS) {
            studentCount = 0;
        } else {
            for (int i = 0; i < studentCount; i++) {
                if (fscanf(fp, "%d %49s %19s %d",
                           &students[i].id, students[i].name,
                           students[i].password,
                           &students[i].borrowedBook) != 4) {
                    studentCount = i;
                    break;
                }
            }
        }
        fclose(fp);
    }
}

void addDefaultData(void)
{
    if (studentCount == 0) {
        students[0] = (Student){101, "Student1", "1234", 0};
        students[1] = (Student){102, "Student2", "1234", 0};
        studentCount = 2;
    }

    if (bookCount == 0) {
        books[0] = (Book){1, "Algebra", "John", MATHS, 1};
        books[1] = (Book){2, "Calculus", "David", MATHS, 1};
        books[2] = (Book){3, "Mechanics", "Newton", PHYSICS, 1};
        books[3] = (Book){4, "Optics", "Young", PHYSICS, 1};
        books[4] = (Book){5, "Organic", "Morrison", CHEMISTRY, 1};
        books[5] = (Book){6, "Inorganic", "Jain", CHEMISTRY, 1};
        bookCount = 6;
    }
    saveData();
}

void viewBooks(int department)
{
    int found = 0;
    printf("\n--- %s Books ---\n", departmentName(department));
    for (int i = 0; i < bookCount; i++) {
        if (books[i].department == department) {
            printf("ID: %d | Name: %s | Author: %s | Status: %s\n",
                   books[i].id, books[i].name, books[i].author,
                   books[i].available ? "Available" : "Borrowed");
            found = 1;
        }
    }
    if (!found) printf("No books in this department.\n");
}

void viewAllBooks(void)
{
    printf("\n========== ALL BOOKS ==========\n");
    for (int i = 0; i < bookCount; i++) {
        printf("ID: %d | Name: %s | Author: %s | Department: %s | Status: %s\n",
               books[i].id, books[i].name, books[i].author,
               departmentName(books[i].department),
               books[i].available ? "Available" : "Borrowed");
    }
    if (bookCount == 0) printf("No books found.\n");
}

void borrowBook(int index)
{
    int id;
    if (students[index].borrowedBook != 0) {
        printf("\nYou have already borrowed a book. Return it first.\n");
        return;
    }

    viewAllBooks();
    printf("Enter Book ID to borrow: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            if (!books[i].available) {
                printf("Book is already borrowed.\n");
                return;
            }
            books[i].available = 0;
            students[index].borrowedBook = id;
            saveData();
            printf("Book borrowed successfully.\n");
            return;
        }
    }
    printf("Book ID not found.\n");
}

void returnBook(int index)
{
    int id = students[index].borrowedBook;
    if (id == 0) {
        printf("You have not borrowed any book.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            books[i].available = 1;
            students[index].borrowedBook = 0;
            saveData();
            printf("Book returned successfully.\n");
            return;
        }
    }
    printf("Borrowed book record was not found. Contact the admin.\n");
}

void studentMenu(int index)
{
    int choice, dept;
    do {
        printf("\n========== STUDENT MENU ==========\n");
        printf("1. View Departments\n2. View Books\n3. Borrow Book\n");
        printf("4. Return Book\n5. My Borrowed Book\n6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) return;

        switch (choice) {
            case 1:
                printf("1. Mathematics\n2. Physics\n3. Chemistry\n");
                break;
            case 2:
                printf("Select department (1-Maths, 2-Physics, 3-Chemistry): ");
                if (scanf("%d", &dept) != 1) return;
                if (dept >= MATHS && dept <= CHEMISTRY) viewBooks(dept);
                else printf("Invalid department.\n");
                break;
            case 3:
                borrowBook(index);
                break;
            case 4:
                returnBook(index);
                break;
            case 5:
                if (students[index].borrowedBook == 0) {
                    printf("No book currently borrowed.\n");
                } else {
                    for (int i = 0; i < bookCount; i++) {
                        if (books[i].id == students[index].borrowedBook) {
                            printf("Borrowed book: %s (ID: %d)\n",
                                   books[i].name, books[i].id);
                            break;
                        }
                    }
                }
                break;
            case 6:
                printf("Student logged out.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 6);
}

void addBook(void)
{
    Book b;
    if (bookCount >= MAX_BOOKS) {
        printf("Book storage is full.\n");
        return;
    }

    printf("Enter Book ID: ");
    if (scanf("%d", &b.id) != 1) return;

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == b.id) {
            printf("Book ID already exists.\n");
            return;
        }
    }

    printf("Enter Book Name (single word): ");
    if (scanf("%49s", b.name) != 1) return;
    printf("Enter Author Name (single word): ");
    if (scanf("%49s", b.author) != 1) return;
    printf("Department (1-Maths, 2-Physics, 3-Chemistry): ");
    if (scanf("%d", &b.department) != 1) return;

    if (b.department < MATHS || b.department > CHEMISTRY) {
        printf("Invalid department.\n");
        return;
    }

    b.available = 1;
    books[bookCount++] = b;
    saveData();
    printf("Book added successfully.\n");
}

void deleteBook(void)
{
    int id;
    printf("Enter Book ID to delete: ");
    if (scanf("%d", &id) != 1) return;

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            if (!books[i].available) {
                printf("Cannot delete a borrowed book.\n");
                return;
            }
            for (int j = i; j < bookCount - 1; j++)
                books[j] = books[j + 1];
            bookCount--;
            saveData();
            printf("Book deleted successfully.\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void updateBook(void)
{
    int id, dept;
    printf("Enter Book ID to update: ");
    if (scanf("%d", &id) != 1) return;

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            printf("Enter new Book Name (single word): ");
            if (scanf("%49s", books[i].name) != 1) return;
            printf("Enter new Author Name (single word): ");
            if (scanf("%49s", books[i].author) != 1) return;
            printf("Department (1-Maths, 2-Physics, 3-Chemistry): ");
            if (scanf("%d", &dept) != 1) return;

            if (dept < MATHS || dept > CHEMISTRY) {
                printf("Invalid department. Update cancelled.\n");
                return;
            }
            books[i].department = dept;
            saveData();
            printf("Book updated successfully.\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void viewStudentRecords(void)
{
    printf("\n========== STUDENT RECORDS ==========\n");
    for (int i = 0; i < studentCount; i++) {
        printf("Student ID: %d | Name: %s | Borrowed Book ID: ",
               students[i].id, students[i].name);
        if (students[i].borrowedBook == 0) printf("None\n");
        else printf("%d\n", students[i].borrowedBook);
    }
}

void adminMenu(void)
{
    int choice;
    do {
        printf("\n========== ADMIN MENU ==========\n");
        printf("1. View All Books\n2. Add Book\n3. Delete Book\n");
        printf("4. Update Book\n5. View Student Records\n6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) return;

        switch (choice) {
            case 1: viewAllBooks(); break;
            case 2: addBook(); break;
            case 3: deleteBook(); break;
            case 4: updateBook(); break;
            case 5: viewStudentRecords(); break;
            case 6: printf("Admin logged out.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 6);
}

void studentLogin(void)
{
    int id;
    char password[20];

    printf("Enter Student ID: ");
    if (scanf("%d", &id) != 1) return;
    printf("Enter Password: ");
    if (scanf("%19s", password) != 1) return;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id &&
            strcmp(students[i].password, password) == 0) {
            printf("Login successful. Welcome %s!\n", students[i].name);
            studentMenu(i);
            return;
        }
    }
    printf("Invalid Student ID or Password.\n");
}

void adminLogin(void)
{
    char username[30], password[20];

    printf("Enter Admin Username: ");
    if (scanf("%29s", username) != 1) return;
    printf("Enter Admin Password: ");
    if (scanf("%19s", password) != 1) return;

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0) {
        printf("Admin login successful.\n");
        adminMenu();
    } else {
        printf("Invalid Admin Credentials.\n");
    }
}

int main(void)
{
    int choice;
    loadData();
    addDefaultData();

    do {
        printf("\n================================\n");
        printf("     LIBRARY MANAGEMENT SYSTEM\n");
        printf("================================\n");
        printf("1. Student Login\n2. Admin Login\n3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1: studentLogin(); break;
            case 2: adminLogin(); break;
            case 3:
                saveData();
                printf("Exiting Library System...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 3);

    return 0;
}
