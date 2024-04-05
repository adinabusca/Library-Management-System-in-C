#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 2000
#define MAX_LOANS 100
#define MAX_PEOPLE 2000
#define MAX_NAME_LENGTH 100
#define MAX_TITLE_LENGTH 200
#define MAX_AUTHOR_LENGTH 100

// Book structure
struct Book {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int copies;
};

// Person structure
struct Person {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
};

// Loan structure
struct Loan {
    struct Person person;
    char bookTitle[MAX_TITLE_LENGTH];
    char bookAuthor[MAX_AUTHOR_LENGTH];
    int copiesBorrowed;
};

// Global variables
struct Book library[MAX_BOOKS];
struct Loan loans[MAX_LOANS];
struct Person people[MAX_PEOPLE];
int numBooks = 0;
int numLoans = 0;
int numPeople = 0;

// Function prototypes
void readFromFile_BOOKS();
void readFromFile_LOANS();
void readFromFile_PEOPLE();
void writeToFile_LOANS();
void writeToFile_BOOKS();
void writeToFile_PEOPLE();               
void login(struct Person *person);
void borrowBooks(struct Person *person);
void returnBooks(struct Person *person);
void donateBooks();
void viewLoans(struct Person *person);
void searchBooks();
void upper(char s[]);
int verify_login(char s[]);

//main function
int main(int argc, char *argv[]) {
    
    if (verify_login(argv[1])==1){
        upper(argv[2]);
        upper(argv[3]);
        readFromFile_PEOPLE();
        struct Person currentUser;
        strcpy(currentUser.name, argv[2]);
        strcpy(currentUser.surname, argv[3]);

        login(&currentUser);


        readFromFile_BOOKS();
        readFromFile_LOANS();
        int choice;
        do {
            printf("\nLibrary Management System\n");
            printf("1. Borrow Books\n");
            printf("2. Return / Donate Books\n");
            printf("3. View Loans\n");
            printf("4. Search for Books\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    borrowBooks(&currentUser);
                    break;
                case 2:
                    printf("Do you want to donate or return books?:\n");
                    printf("1.Donate Books\n");
                    printf("2.Return Books\n");
                    printf("Enter you choice:");
                    int choose;
                    scanf("%d",&choose);
                    if (choose == 1){
                        donateBooks();
                    }else{
                        returnBooks(&currentUser);

                    }
                    break;
                case 3:
                    viewLoans(&currentUser);
                    break;
                case 4:
                    searchBooks();
                    break;
                case 5:
                    writeToFile_LOANS();
                    writeToFile_BOOKS();
                    writeToFile_PEOPLE();
                    printf("Exiting the program.\n");
                    break;
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } while (choice != 5);

    }else{
        printf("You made a mistake while trying to login. You should have introduced the following: login [NAME] [SURNAME].");
    }
    return 0;
}

//input verification functions
int verify_login(char s[]){
    if (strcmp(s,"login")==0){
        return 1;
    }
    return 0;
}


void upper(char s[]){
    for (int i = 0; i < strlen(s); i++){
        if (s[i] >= 'a' && s[i] <='z'){
            s[i] = s[i] - 32;
        }
    }
}

// Functions to read  data from a CSV file
void readFromFile_BOOKS() {
    FILE *file = fopen("books.csv", "r");
    if (file == NULL) {
        printf("Could not open books file for reading.\n");
        return;
    }

    while (fscanf(file, " %[^,],%[^,],%d\n", library[numBooks].title, library[numBooks].author, &library[numBooks].copies) == 3) {
        numBooks++;
    }

    fclose(file);
}

void readFromFile_PEOPLE() {
    FILE *file = fopen("people.csv", "r");
    if (file == NULL) {
        printf("Could not open people file for reading.\n");
        return;
    }

    while (fscanf(file, " %[^,],%[^,\n]", people[numPeople].name,people[numPeople].surname ) == 2) {
        numPeople++;
    }

    fclose(file);
}

void readFromFile_LOANS() {
    FILE *file = fopen("loans.csv", "r");
    if (file == NULL) {
        printf("Could not open loans file for reading.\n");
        return;
    }

    while (fscanf(file, " %[^,],%[^,],%[^,],%[^,],%d\n",  loans[numLoans].person.name, loans[numLoans].person.surname, loans[numLoans].bookTitle, loans[numLoans].bookAuthor, &loans[numLoans].copiesBorrowed) == 5) {
        numLoans++;
    }

    fclose(file);
}


// Functions to write  data to a CSV file
void writeToFile_LOANS() {
    FILE *file = fopen("loans.csv", "w");
    if (file == NULL) {
        printf("Could not open loans file for writing.\n");
        return;
    }

    for (int i = 0; i < numLoans; i++) {
        fprintf(file, "%s,%s,%s,%s,%d\n", loans[i].person.name, loans[i].person.surname, loans[i].bookTitle, loans[i].bookAuthor, loans[i].copiesBorrowed);
    }

    fclose(file);
}

void writeToFile_BOOKS() {
    FILE *file = fopen("books.csv", "w");
    if (file == NULL) {
        printf("Could not open books file for writing.\n");
        return;
    }

    for (int i = 0; i < numBooks; i++) {
        fprintf(file, "%s,%s,%d\n", library[i].title, library[i].author, library[i].copies);
    }

    fclose(file);
}

void writeToFile_PEOPLE() {
    FILE *file = fopen("people.csv", "w");
    if (file == NULL) {
        printf("Could not open people file for writing.\n");
        return;
    }

    for (int i = 0; i < numPeople; i++) {
        fprintf(file, "%s,%s\n", people[i].name, people[i].surname);
    }

    fclose(file);
}

// Function to simulate login
void login(struct Person *person) {
    printf("Logged in as %s %s.\n", person->name, person->surname);
     
}


// Function to borrow books
void borrowBooks(struct Person *person) {
    char title[MAX_TITLE_LENGTH];
    printf("Enter the title of the book you want to borrow: ");
    scanf(" %[^\n]", title);
    upper(title);
    int copies;
    printf("Enter the number of copies you want to borrow: ");
    scanf("%d", &copies);

    int bookIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(title, library[i].title) == 0) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found in the library.\n");
        return;
    }

    if (library[bookIndex].copies < copies) {
        printf("Requested number of copies not available.\n");
        return;
    }
    
    int indexLoan = -1;
    for (int i = 0; i < numLoans; i++){
        if (strcmp(loans[i].person.name, person->name) == 0 && strcmp(loans[i].person.surname, person->surname) == 0){
            if (strcmp(loans[i].bookTitle, library[bookIndex].title) == 0 && loans[i].copiesBorrowed > 0){
                indexLoan = i;
                break;
            }
        }
    }
    if (indexLoan != -1){
        loans[indexLoan].copiesBorrowed = loans[indexLoan].copiesBorrowed + copies;

    }else{
        strcpy(loans[numLoans].person.name, person->name);
        strcpy(loans[numLoans].person.surname, person->surname);
        strcpy(loans[numLoans].bookTitle, library[bookIndex].title);
        strcpy(loans[numLoans].bookAuthor, library[bookIndex].author);
        loans[numLoans].copiesBorrowed = copies;

        library[bookIndex].copies -= copies;

        numLoans++;

    }
     
    printf("Books borrowed successfully.\n");
}


// Function to return books
void returnBooks(struct Person *person) {
   char title[MAX_TITLE_LENGTH];
    printf("Enter the title of the book you want to return: ");
    scanf(" %[^\n]", title);
    upper(title);
    int copies;
    printf("Enter the number of copies you want to return: ");
    scanf("%d", &copies);

    int loanIndex = -1;
    for (int i = 0; i < numLoans; i++) {
        if (strcmp(title, loans[i].bookTitle) == 0  && strcmp(person->name, loans[i].person.name) == 0 && strcmp(person->surname, loans[i].person.surname) == 0) {
            loanIndex = i;
            break;
        }
    }

    if (loanIndex == -1) {
        printf("You have not borrowed this book.\n");
        return;
    }

    int bookIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(title, library[i].title) == 0) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found in the library.\n");
        return;
    }

    library[bookIndex].copies += copies;

    printf("Books returned successfully.\n");

    // Remove the loan from the loans array
    if (loans[loanIndex].copiesBorrowed-copies == 0){
        for (int i = loanIndex; i < numLoans - 1; i++) {
             loans[i] = loans[i + 1];
        }
        numLoans--;
    }else{
        loans[loanIndex].copiesBorrowed = loans[loanIndex].copiesBorrowed -copies;
    }
     
}


// Function to donate books
void donateBooks() {
    char title_book[MAX_TITLE_LENGTH], author_book[MAX_AUTHOR_LENGTH];
    if (numBooks >= MAX_BOOKS) {
        printf("Library is full, cannot add more books.\n");
        return;
    }

    printf("Enter the title of the book you want to donate: ");
    scanf(" %[^\n]", title_book);
    upper(title_book);

    int found = 0, indexBook;
    for (int i = 0; i < numBooks; i++){
         if (strcmp(title_book,library[i].title) == 0){
            found = 1;
            indexBook = i;
            break;
         }
    }

    if (found == 1){
        int donated_copies;
        printf("Enter the number of copies you want to donate: ");
        scanf("%d",&donated_copies);
        library[indexBook].copies += donated_copies;


    }else{
        strcpy(library[numBooks].title,title_book);

        printf("Enter the author of the book: ");
        scanf(" %[^\n]", author_book);
        upper(author_book);
        strcpy(library[numBooks].author,author_book);

        printf("Enter the number of copies you want to donate: ");
        scanf("%d", &library[numBooks].copies);

        numBooks++;


    }
    
    printf("Book donated successfully.\n");
}


// Function to view loans made by the person
void viewLoans(struct Person *person) {
    printf("Loans made by %s %s:\n", person->name, person->surname);
    for (int i = 0; i < numLoans; i++) {
        if (strcmp(person->name, loans[i].person.name) == 0 && strcmp(person->surname, loans[i].person.surname) == 0) {
            printf("Book: %s by %s, Copies Borrowed: %d\n", loans[i].bookTitle, loans[i].bookAuthor, loans[i].copiesBorrowed);
        }
    }
}


// Function to search for books
void searchBooks() {
    printf("Choose the method to search the books:\n");
    printf("1. Title or Author\n");
    printf("2. Title and Author\n");
    printf("Enter your choice:");
    int choose;
    scanf("%d",&choose);

    if (choose == 1){
        char search[MAX_TITLE_LENGTH];
        printf("Enter the title or author of the book you want to search: ");
        scanf(" %[^\n]", search);
        upper(search);
        printf("Search Results:\n");

        int found = 0;
        for (int i = 0; i < numBooks; i++) {
            if (strstr(library[i].title, search) != NULL || strstr(library[i].author, search) != NULL) {
                printf("Title: %s\n", library[i].title);
                printf("Author: %s\n", library[i].author);
                printf("Copies available: %d\n", library[i].copies);
                printf("\n");
                found = 1;
            }
        }

        if (!found) {
            printf("No books found matching the search query.\n");
        }

    }else{
        char title_book[MAX_TITLE_LENGTH], autho_book[MAX_AUTHOR_LENGTH];
        printf("Enter the title of the book you want to search: ");
        scanf(" %[^\n]", title_book);
        upper(title_book);

        printf("Enter the author of the book you want to search: ");
        scanf(" %[^\n]", autho_book);
        upper(autho_book);

        printf("Search Results:\n");

        int found = 0;
        for (int i = 0; i < numBooks; i++) {
            if (strcmp(library[i].title, title_book) == 0 && strcmp(library[i].author, autho_book) == 0) {
                printf("Title: %s\n", library[i].title);
                printf("Author: %s\n", library[i].author);
                printf("Copies available: %d\n", library[i].copies);
                printf("\n");
                found = 1;
            }
        }

        if (!found) {
            printf("No books found matching the search criteria.\n");
        }
    }
     
}