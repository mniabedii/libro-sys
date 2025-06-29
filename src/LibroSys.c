//Welcome!
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Color Codes:
#define red     "\e[0;31m"
#define green   "\e[0;32m"
#define blue    "\e[0;34m"
#define bwhite  "\e[1;37m"
#define blueb   "\e[44m"
#define hblue   "\e[0;94m"
#define reset   "\e[0m"

struct Shelf {
    char Genre[50];
    int Capacity;
};

struct Library {
    char library_Name[100];
    char Address[200];
    struct Shelf Shelves[28];
};

struct Book {
    char Book_Name[100];
    char Author_Name[100];
    int ISBN;
    char Genre[50];
    int Price;
    char Publisher[100];
    int is_in_library; // 0 or 1
    int which_library; // keeps the number of library the book is located in.
    int which_shelf; // keeps the number of shelf of the library the book is located in.
};

struct SoldOut {
    char BookName[100];
    char Author_Name[100];
    int Price;
    int LibName;
    char CustomerName[100];
};

struct Customer {
    char CustomerName[100];
    int id;
    int balance;
    int tpp; //Total Past Purchases
};

void cls();
void ClearInputBuffer();
void ClearScreen();
void PrintBookInfo(struct Book *Book1);
void PrintSoldBookInfo(struct SoldOut *Book1);
void DeleteBook(struct Book *Book1);
void AddBook(int i, struct Book BooksDataBase[i]);
int ShowAndSelectLibrary(struct Library All_Libraries[4]);
void Search_and_ShowBook(struct Book BooksDataBase[336], struct Library All_Libraries[4]);
int Search_by_ISBN(struct Book BooksDataBase[336]);
void swap(int *p, int *q);
void assign_genre_to_shelf(struct Library All_Libraries[4]);

int main() {

    cls();
    
    puts(bwhite"\t\t···Welcome to LibroSys···");
    puts("\t   ···Chain Library Management System···\n"reset);

    struct Book BooksDataBase[336]; // The warehouse for all of my books
    struct SoldOut SoldBooks[336];
    struct Customer Customers[336];
    struct Library All_Libraries[4];
    //introducing our libraries:
        //Library Number 1:
        strcpy(All_Libraries[0].library_Name, "Downtown");
        strcpy(All_Libraries[0].Address, "Block 21, E24 Street, Downtown, NYC, NY");
        
        //Library Number 2:
        strcpy(All_Libraries[1].library_Name, "Cambridge");
        strcpy(All_Libraries[1].Address, "Block 13, St.Junior Street, West T6 Highway, NYC, NY");
        
        //Library Number 3:
        strcpy(All_Libraries[2].library_Name, "Cityhall");
        strcpy(All_Libraries[2].Address, "Block 11, IV Alley, HI street, East Town, NYC, NY");
        
        //Library Number 4:
        strcpy(All_Libraries[3].library_Name, "J.R.Oppenheimer");
        strcpy(All_Libraries[3].Address, "Room 32, J.R.Oppenheimer Conferece Building, 15th Street, Downtown, NYC, NY");

    assign_genre_to_shelf(All_Libraries);
    // 4 Libraries, each 28 shelves, each shelf contains 3 books = 336 books overall

    for (int i = 0; i < 336; i++) {// This part of the code should run for the first time only
            DeleteBook(BooksDataBase + i);
            SoldBooks[i].LibName = -1000;
            strcpy(SoldBooks[i].CustomerName, "##########");
            strcpy(Customers[i].CustomerName, "##########");
    }

    FILE *file1;
    file1 = fopen("Warehouse.csv","r");
    int i = 0;
    while ((i < 336 && (fscanf(file1," %[^\n] %[^\n]%d %[^\n]%d %[^\n]%d%d%d", BooksDataBase[i].Book_Name, BooksDataBase[i].Author_Name, &(BooksDataBase[i].ISBN), BooksDataBase[i].Genre, &(BooksDataBase[i].Price), BooksDataBase[i].Publisher, &(BooksDataBase[i].is_in_library), &(BooksDataBase[i].which_library), &(BooksDataBase[i].which_shelf))) == 9)) {
        i++;
    }
    fclose(file1);
    
    file1 = fopen("Soldout.csv", "r");
    i = 0;
    while ((i < 336 && (fscanf(file1, " %[^\n] %[^\n] %d%d %[^\n]", SoldBooks[i].BookName, SoldBooks[i].Author_Name, &(SoldBooks[i].Price), &(SoldBooks[i].LibName), SoldBooks[i].CustomerName)))) {
        i++;
    }
    fclose(file1);

    file1 = fopen("Capacity.csv", "r");
    i = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 28; j++) {
            fscanf(file1,"%d", &(All_Libraries[i].Shelves[j].Capacity));
        }
    }

    file1 = fopen("Customers.csv", "r");
    i = 0;
    while ((i < 336 && (fscanf(file1, " %[^\n]%d%d%d", Customers[i].CustomerName, &(Customers[i].id), &(Customers[i].balance), &(Customers[i].tpp))))) {
        i++;
    }
    fclose(file1);
    
        
    while (1) {
        puts(blueb"Main Menu"reset);
        puts("1. " blue"Add Book"reset);
        puts("2. " blue"Search Book"reset);
        puts("3. " blue"Delete Book"reset);
        puts("4. " blue"Display Stocked Books"reset);
        puts("5. " blue"Assign Book to Library"reset);
        puts("6. " blue"Sell Book"reset);
        puts("7. " blue"Sales History"reset);
        puts("8. " blue"Save Current Data"reset);
        puts("9. " red"Exit"reset);

        int choice;
        puts("\nEnter your choice" blue" (1-9)"reset":");
        scanf("%d", &choice);

        while (!(choice >= 1 && choice <= 9)) {
            puts(red"Your Selection must be between 1 - 9. Please try again"reset);
            scanf("%d", &choice);
        }

        cls();

        //Exit:
        if (choice == 9) 
            break;

        switch(choice) {
            case 1: {
                //Add Book:
                puts(blueb"Add Book"reset);
                for (int i = 0; i < 336; i++) {
                    if ((strcmp(BooksDataBase[i].Book_Name,"##########")) == 0) {
                        AddBook(i, BooksDataBase);
                        break;
                    }
                }
                break;
            }
            case 2: {
                //Search Book:
                puts(blueb"Search Book"reset);
                Search_and_ShowBook(BooksDataBase, All_Libraries);
                break;
            }
            case 3: {
                //Delete Book:
                int index;
                int yesorno;
                puts(blueb"Delete Book"reset);
                index = Search_by_ISBN(BooksDataBase);
                if (index >= 0) {
                    putchar('\n');
                    PrintBookInfo(BooksDataBase + index);
                    if (BooksDataBase[index].is_in_library == 1) {
                        printf(blue"\nAbove book is in %s library\n\n"reset, All_Libraries[BooksDataBase[index].which_library - 1].library_Name);
                    }
                    else 
                        puts(blue"Above book is currently in no library!\n"reset);
                    do {
                        puts("Are you sure you want to delete this book?");
                        puts("1. " blue"Yes\n"reset"2. " blue"No\n\n"reset blue"(1 or 2)"reset ":");
                        scanf("%d", &yesorno);
                    } while (yesorno != 1 && yesorno != 2);

                    if (yesorno == 2) {
                        puts("Returning to main menu...\n");
                        sleep(1);
                        cls();
                        break;
                    }
                    if (yesorno == 1) {
                            All_Libraries[BooksDataBase[index].which_library - 1].Shelves[BooksDataBase[index].which_library].Capacity++;
                            DeleteBook(BooksDataBase + index);
                            puts(green"\nThe book was deleted successfully!\n"reset);
                        }
                    }
                ClearScreen();
                break;
            }
            case 4: {
                //Display stocked books:
                int counter = 1;
                puts(blue"Display Stocked Books"reset);

                for (int i = 0; i < 336; i++) {
                    if ((BooksDataBase[i].is_in_library == 0) && (strcmp(BooksDataBase[i].Book_Name, "##########")) != 0) {
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                    }
                }
                ClearScreen();
                break;
            }
            case 5: {
                //Assign book to library:
                int index;
                puts(blueb"Assign Book to Library"reset);
                printf("Search for the book you are willing to assign to a library\n\n");
                index = Search_by_ISBN(BooksDataBase);
                if (index >= 0) {
                    printf("\nWhich library are you going to assign this book to?\n");
                    
                    int select = ShowAndSelectLibrary(All_Libraries);

                    int indexforcase;
                    int available_genre = 0;
                    switch (select) {    
                        case 1: {
                            indexforcase = 0;
                            break;
                        }
                        case 2: {
                            indexforcase = 1;
                            break;
                        }
                        case 3: {
                            indexforcase = 2;
                            break;
                        }
                        case 4: {
                            indexforcase = 3;
                            break;
                        }
                    }
                    for (int i = 0; i < 28; i += 2) {// even indexes
                        if ((strcmp(All_Libraries[indexforcase].Shelves[i].Genre,BooksDataBase[index].Genre) == 0)) {
                            available_genre = 1;
                            //there will be two shelf for every genre
                             if (All_Libraries[indexforcase].Shelves[i].Capacity == 0 && All_Libraries[indexforcase].Shelves[i + 1].Capacity == 0) {
                                puts(red"Unfortunately the library is at full capacity! Please use another library\n"reset);
                            }
                            else if (All_Libraries[indexforcase].Shelves[i].Capacity >= All_Libraries[indexforcase].Shelves[i + 1].Capacity) {
                                if (BooksDataBase[index].is_in_library == 1) {
                                    if (BooksDataBase[index].which_library == select) {
                                        puts(red"The book is already in your selected library!\n"reset);
                                    }
                                    else {
                                        printf(green"The book is no longer in %s library and it was transferred to your selected library.\n"reset, All_Libraries[BooksDataBase[index].which_library - 1].library_Name);
                                        (All_Libraries[BooksDataBase[index].which_library - 1].Shelves[BooksDataBase[index].which_shelf].Capacity)++;
                                    }
                                }
                                else if (BooksDataBase[index].is_in_library == 0) {
                                    puts(green"The book was transferred to your selected library\n"reset);
                                }
                                BooksDataBase[index].is_in_library = 1;
                                BooksDataBase[index].which_library = indexforcase + 1;
                                BooksDataBase[index].which_shelf = i;
                                (All_Libraries[indexforcase].Shelves[i].Capacity)--;
                            }
                            else if (All_Libraries[indexforcase].Shelves[i].Capacity < All_Libraries[indexforcase].Shelves[i + 1].Capacity) {
                                if (BooksDataBase[index].is_in_library == 1) {
                                    if (BooksDataBase[index].which_library == select) {    
                                        puts(red"The book is already in your selected library!\n"reset);
                                    }
                                    else {
                                        printf(green"The book is no longer in library number %d and it was transferred to your selected library\n"reset, BooksDataBase[index].which_library);
                                        (All_Libraries[BooksDataBase[index].which_library - 1].Shelves[BooksDataBase[index].which_shelf].Capacity)++;
                                    }
                                }
                                else if (BooksDataBase[index].is_in_library == 0) {
                                    puts(green"The book was transferred to your selected library\n"reset);
                                }
                                BooksDataBase[index].is_in_library = 1;
                                BooksDataBase[index].which_library = indexforcase + 1;
                                BooksDataBase[index].which_shelf = i + 1;
                                (All_Libraries[indexforcase].Shelves[i + 1].Capacity)--;
                            }
                        }
                    }
                    if (available_genre == 0) 
                        puts(red"Your selected library does not have a shelf with such a genre!\n"reset);
                }

                ClearScreen();
                break;
            }
            case 6: {
                //Sell Book:
                char Name[100];
                int id;
                int has_signedup = 0;
                int index;
                puts(blueb"Sell Book"reset);
                printf("Moving to Customer's panel...");
                
                sleep(3);
                cls();

                puts(hblue"Customer's Panel\n"reset);
                puts("Enter your Name and ID to login or sign up:");
                printf("Name:\t");
                scanf(" %[^\n]", Name);
                printf("ID (your ID must be a 6-digit number):\t");
                scanf("%d", &(id));
                while ((id/100000) > 9 || (id/100000) < 1) {
                    puts(red"Please enter a 6 digit number for ID!"reset);
                    scanf("%d", &id);
                }

                for (index = 0; index < 336; index++) {
                    if ((strcmp(Name, Customers[index].CustomerName)) == 0 && id == Customers[index].id) {
                        has_signedup = 1;
                        break;
                        //if user has already signed up his info will be on "index" index of array
                    }
                }

                if (has_signedup == 0) {
                    printf(green"\nSigned up successfully!"reset);
                }
                else {
                    printf(green"\nLogged in successfully!"reset);
                }

                sleep(2);
                cls();

                if (has_signedup == 0) {
                    for (int i = 0; i < 336; i++) {
                        if ((strcmp(Customers[i].CustomerName, "##########")) == 0) {
                            index = i;
                            break;
                        }
                    }
                    strcpy(Customers[index].CustomerName, Name);
                    Customers[index].id = id;
                    printf(green"Hello and Welcome to LibroSys %s! Glad to Have you here:)\n"reset, Customers[index].CustomerName);
                    printf("Your balance is: "green"%d$\n\n"reset, Customers[index].balance);
                }
                else if (has_signedup == 1) {
                    printf(green"Welcome Back %s!\n"reset, Customers[index].CustomerName);
                    printf("Your balance is: "green"%d$\n"reset, Customers[index].balance);
                    printf("Your total past purchases are: "green"%d$\n\n"reset, Customers[index].tpp);
                }

                int selected_lib;
                int numbering = 1;
                struct Book picked_lib_books[336];
                int picked_book;
                int soldout_book;
                int paid_price;
                int select;
                int credit;
                int is_any = 0;
                puts("Choose the library of the book you want to buy:");

                selected_lib = ShowAndSelectLibrary(All_Libraries);
                putchar('\n');
                for (int i = 0; i < 336; i++) {
                    if (BooksDataBase[i].which_library == selected_lib && (strcmp(BooksDataBase[i].Book_Name, "##########")) != 0) {
                        is_any = 1;
                        printf("%d. ", numbering);
                        printf(blue"%s\n"reset, BooksDataBase[i].Book_Name);
                        // Copying structure: (I turned this into a function but there was compiler error for no reason!)
                        strcpy(picked_lib_books[numbering].Book_Name,BooksDataBase[i].Book_Name);
                        strcpy(picked_lib_books[numbering].Author_Name,BooksDataBase[i].Author_Name);
                        picked_lib_books[numbering].ISBN = BooksDataBase[i].ISBN;
                        strcpy(picked_lib_books[numbering].Genre,BooksDataBase[i].Genre);
                        picked_lib_books[numbering].Price = BooksDataBase[i].Price;
                        strcpy(picked_lib_books[numbering].Publisher,BooksDataBase[i].Publisher);
                        //
                        numbering++;
                    }
                }
                if (is_any == 0) {
                    puts(red"No book is assigned to the library selected!\n"reset);
                    ClearScreen();
                    break;
                }
                puts("\nWhich book are you going to buy?");
                do {
                printf("(1 - %d):\n", numbering - 1);
                scanf("%d", &(picked_book));
                } while (picked_book < 1 || picked_book >= numbering);
                putchar('\n');
                PrintBookInfo(picked_lib_books + picked_book);
                puts(green"Nice Choice!\n"reset);

                for (soldout_book = 0; soldout_book < 336; soldout_book++) {
                    if (BooksDataBase[soldout_book].ISBN == picked_lib_books[picked_book].ISBN) {
                        break;
                    }
                }

                puts("Press Enter to continue the shopping process...");
                getchar(); // Consume the newline character from the previous Enter key press
                ClearInputBuffer();

                printf("The price of the book is " blue"%d$\n"reset, picked_lib_books[picked_book].Price);
                if (Customers[index].balance >= 0 && Customers[index].balance < picked_lib_books[picked_book].Price) {
                    puts(red"\nYour balance is insufficient to buy the book!"reset);
                    puts("Do you want to...");
                    puts("1. " blue"Add to your credit"reset);
                    puts("2. " red"Exit to main menu"reset);
                    do {
                    puts("Choose " blue"1 "reset"or "blue"2"reset":");
                    scanf("%d", &select);
                    } while (select > 2 || select < 1);
                    if (select == 2) {
                        puts("\nReturning to main menu...");
                        sleep(1);
                        cls();
                        break;
                    }
                    else if (select == 1) {
                        puts("\nHow much are you going to add to your credit?");
                        scanf("%d", &credit);
                        printf(green"\nAmount of %d$ was credited to your account successfully!\n"reset, credit);
                        Customers[index].balance += credit;
                        printf("Current account balance: " green"%d$\n\n"reset, credit);
                        if (Customers[index].balance < picked_lib_books[picked_book].Price) {
                            printf(red"Your account balance is still insufficient to purchase the book!\n"reset);
                            puts("Returning to main menu...");
                            sleep(1);
                            cls();
                            break;
                        }

                        puts("Press Enter to continue the shopping process...");
                        getchar(); // Consume the newline character from the previous Enter key press
                        ClearInputBuffer();
                        
                        for (int i = 0; i < 336; i++) {
                            if (strcmp(SoldBooks[i].CustomerName,"##########") == 0) {
                                strcpy(SoldBooks[i].BookName,picked_lib_books[picked_book].Book_Name);
                                strcpy(SoldBooks[i].Author_Name,picked_lib_books[picked_book].Author_Name);
                                SoldBooks[i].Price = picked_lib_books[picked_book].Price;
                                strcpy(SoldBooks[i].CustomerName, Name);
                                SoldBooks[i].LibName = selected_lib;
                                break;
                        }
                    }
                    (All_Libraries[BooksDataBase[soldout_book].which_library - 1].Shelves[BooksDataBase[soldout_book].which_shelf].Capacity)++;
                    DeleteBook(BooksDataBase + soldout_book);
                    puts(green"\nSuccessful purchase!"reset);
                    printf("Current account balance: " green"%d$\n"reset, Customers[index].balance - picked_lib_books[picked_book].Price);
                    Customers[index].balance -= picked_lib_books[picked_book].Price;
                    Customers[index].tpp += picked_lib_books[picked_book].Price;
                    puts(green"Thank you for your purchase!\n\n"reset);
                    }
                }
                else if (Customers[index].balance >= picked_lib_books[picked_book].Price) {
                    
                    puts("Are you sure to proceed with the purchase?");
                    puts("1. "blue"Yes\n"reset"2. "blue"No\n"reset);
                    do {
                        puts("choose "blue"(1 - 2)"reset":");
                        scanf("%d", &select);
                    } while (select > 2 || select < 1);
                    if (select == 2) {
                        puts("Returning to main menu...");
                        sleep(1);
                        cls();
                        break;
                    }
                    else if (select == 1) {
                    
                    for (int i = 0; i < 336; i++) {
                        if (strcmp(SoldBooks[i].CustomerName,"##########") == 0) {
                            strcpy(SoldBooks[i].BookName,picked_lib_books[picked_book].Book_Name);
                            strcpy(SoldBooks[i].Author_Name,picked_lib_books[picked_book].Author_Name);
                            SoldBooks[i].Price = picked_lib_books[picked_book].Price;
                            strcpy(SoldBooks[i].CustomerName, Name);
                            SoldBooks[i].LibName = selected_lib;
                            break;
                        }
                    }    
                    (All_Libraries[BooksDataBase[soldout_book].which_library - 1].Shelves[BooksDataBase[soldout_book].which_shelf].Capacity)++;
                    DeleteBook(BooksDataBase + soldout_book);
                    puts(green"Successful purchase!"reset);
                    printf("Current account balance: " green"%d$\n"reset, Customers[index].balance - picked_lib_books[picked_book].Price);
                    Customers[index].balance -= picked_lib_books[picked_book].Price;
                    Customers[index].tpp += picked_lib_books[picked_book].Price;
                    puts(green"Thank you for your purchase!\n\n"reset);
                    }
                }
                ClearScreen();
                break;
            }   
            case 7: {
                //View Sold Book History:
                puts(blueb"Sales History"reset);
                int select;
                char Customer[100];
                int is_any = 0;
                int numbering = 1;
                puts("View Sold Book History by:\n1. "blue"Customer's Name\n"reset"2. "blue"Library's Name"reset);
                do {
                puts("Select "blue"(1 - 2)"reset":");
                scanf("%d", &select);
                } while (select > 2 || select < 1);
                
                cls();
                
                switch (select) {
                    case 1: {
                        puts(bwhite"View by customer's name:\n"reset);
                        puts("Enter Customer's Name:");
                        scanf(" %[^\n]", Customer);
                        putchar('\n');
                        for (int i = 0; i < 336; i++) {
                            if ((strcmp(SoldBooks[i].CustomerName, Customer)) == 0) {
                                printf("%d.\n", numbering);
                                numbering++;
                                PrintSoldBookInfo(SoldBooks + i);
                                printf("Bought from "blue"%s "reset"library\n", All_Libraries[SoldBooks[i].LibName - 1].library_Name);
                                printf("By "blue"%s\n\n"reset, SoldBooks[i].CustomerName);
                                is_any = 1;
                            }
                        }
                        if (is_any == 0) 
                            puts(red"No book has been bought by this customer!\n"reset);

                        break;
                    }
                    case 2: {
                        puts(bwhite"View by Library's name:\n"reset);
                        for (int i = 0; i < 4; i++) {
                            printf("%d. ", i + 1);
                            printf(blue"%s\n"reset, All_Libraries[i].library_Name);
                            printf("Located in: "blue"%s\n"reset, All_Libraries[i].Address);
                        }
                        int select;
                        puts("\nSelect "blue"(1 - 4)"reset":");
                        scanf("%d", &select);
                        while (select > 4 || select < 1) {
                        puts(red"wrong input, try again:\n"reset);
                        scanf("%d", &select);
                        }
                        putchar('\n');
                        for (int i = 0; i < 336; i++) {
                            if (select == SoldBooks[i].LibName) {
                                printf("%d.\n", numbering);
                                numbering++;
                                PrintSoldBookInfo(SoldBooks + i);
                                printf("Bought By "blue"%s\n"reset, SoldBooks[i].CustomerName);
                                printf("From "blue"%s "reset"library\n\n", All_Libraries[SoldBooks[i].LibName - 1].library_Name);
                                is_any = 1;
                            }
                        }
                            if (is_any == 0) 
                            puts(red"No book has been bought from this library!\n"reset);
                    }   
                        break;
                }

                ClearScreen();
                break;
            }
            case 8: {
                //Save Current Data:
                puts(blueb"Save Current Data"reset);
                puts("Saving...");
                sleep(1);

                file1 = fopen("Warehouse.csv", "w");
                for (int i = 0; i < 336; i++) {
                    fprintf(file1, "%s\n%s\n%d\n%s\n%d\n%s\n%d\n%d\n%d\n\n", BooksDataBase[i].Book_Name, BooksDataBase[i].Author_Name, BooksDataBase[i].ISBN, BooksDataBase[i].Genre, BooksDataBase[i].Price, BooksDataBase[i].Publisher, BooksDataBase[i].is_in_library, BooksDataBase[i].which_library, BooksDataBase[i].which_shelf);
                }
                fclose(file1);

                file1 = fopen("Soldout.csv", "w");
                for (int i = 0; i < 336; i++) {
                    fprintf(file1, "%s\n%s\n%d\n%d\n%s\n\n", SoldBooks[i].BookName, SoldBooks[i].Author_Name, SoldBooks[i].Price, SoldBooks[i].LibName, SoldBooks[i].CustomerName);
                }
                fclose(file1);

                file1 = fopen("Capacity.csv", "w");
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 28; j++) {
                        fprintf(file1, "%d\n", All_Libraries[i].Shelves[j].Capacity);
                    }
                }
                fclose(file1);

                file1 = fopen("Customers.csv", "w");
                for (int i = 0; i < 336; i++) {
                    fprintf(file1, "%s\n%d\n%d\n%d\n\n", Customers[i].CustomerName, Customers[i].id, Customers[i].balance, Customers[i].tpp);
                }
                fclose(file1);

                puts(green"\nCurrent data saved"reset);

                ClearScreen();
                break;
            }
        }
    }
    return 0;
}

void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void ClearScreen() {
    puts("Press Enter to continue...");
    getchar(); // Consume the newline character from the previous Enter key press
    ClearInputBuffer(); // Clear any remaining characters from the input buffer
    printf("\033[2J\033[H");
}

void cls() {
    printf("\033[2J\033[H");
}

void PrintBookInfo(struct Book *Book1) {
    printf("Name: " blue"%s\n"reset, Book1 -> Book_Name);
    printf("Author: " blue"%s\n"reset, Book1 -> Author_Name);
    printf("ISBN: " blue"%d\n"reset, Book1 -> ISBN);
    printf("Genre: " blue"%s\n"reset, Book1 -> Genre);
    printf("Price: " blue"%d$\n"reset, Book1 -> Price);
    printf("Publisher: " blue"%s\n"reset, Book1 -> Publisher);
}
void PrintSoldBookInfo(struct SoldOut *Book1) {
    printf("Name: " blue"%s\n"reset, Book1 -> BookName);
    printf("Author: " blue"%s\n"reset, Book1 -> Author_Name);
    printf("Price: " blue"%d$\n"reset, Book1 -> Price);
}

void DeleteBook(struct Book *Book1) {
    strcpy(Book1 -> Book_Name, "##########");
    strcpy(Book1 -> Author_Name, "##########");
    Book1 -> ISBN = -1000;
    strcpy(Book1 -> Genre, "##########");
    Book1 -> Price = -1000;
    strcpy(Book1 -> Publisher, "##########");
    Book1 -> is_in_library = 0;
}

void AddBook(int i, struct Book BooksDataBase[i]) {
    int already_added = 0;
    printf("Enter the book name:\n");
    scanf(" %[^\n]", BooksDataBase[i].Book_Name);
    while ((strcmp(BooksDataBase[i].Book_Name, "##########")) == 0) {
        printf(red"Error, Try again\n"reset);
        scanf(" %[^\n]", BooksDataBase[i].Book_Name);
    }
    printf("Enter author's name:\n");
    scanf(" %[^\n]", BooksDataBase[i].Author_Name);
    printf("Enter ISBN:\n");
    scanf("%d", &(BooksDataBase[i].ISBN));
    printf("Enter the genre of the book:\n");
    scanf(" %[^\n]", BooksDataBase[i].Genre);
    printf("Enter the price:\n");
    scanf("%d", &(BooksDataBase[i].Price));
    printf("Enter the publisher:\n");
    scanf(" %[^\n]", BooksDataBase[i].Publisher);
    for (int j = 0; j < i; j++) {
        if ((strcmp(BooksDataBase[i].Book_Name, BooksDataBase[j].Book_Name)) == 0 && (strcmp(BooksDataBase[i].Author_Name, BooksDataBase[j].Author_Name)) == 0 && (strcmp(BooksDataBase[i].Genre, BooksDataBase[j].Genre)) == 0 && (strcmp(BooksDataBase[i].Publisher, BooksDataBase[j].Publisher)) == 0 && BooksDataBase[i].ISBN == BooksDataBase[j].ISBN && BooksDataBase[i].Price == BooksDataBase[j].Price) {
            puts(red"Unsuccessful! The book has already been added to the warehouse!\n"reset);
            DeleteBook(BooksDataBase + i);
            already_added = 1;
            break;
        }
    }
    if (already_added == 0) {
        for (int j = i + 1; j < 336; j++) {
            if ((strcmp(BooksDataBase[i].Book_Name, BooksDataBase[j].Book_Name)) == 0 && (strcmp(BooksDataBase[i].Author_Name, BooksDataBase[j].Author_Name)) == 0 && (strcmp(BooksDataBase[i].Publisher, BooksDataBase[j].Publisher)) == 0 && BooksDataBase[i].ISBN == BooksDataBase[j].ISBN && BooksDataBase[i].Price == BooksDataBase[j].Price) {
                puts(red"Unsuccessful! The book has already been added to the warehouse!\n"reset);
                DeleteBook(BooksDataBase + i);
                already_added = 1;
                break;
            }
        }
    }
    if (already_added == 0)
        printf(green"The book was added to the warehouse successfully!\n\n"reset);

    ClearScreen();
}


void Search_and_ShowBook(struct Book BooksDataBase[336], struct Library All_Libraries[4]) {
    struct Book looking_for_book;
    int was_available = 0; // The book may not be available in the warehouse
    int filter;
    printf("How are you willing to search for the book?\n");
    puts("1. " blue"By Name"reset);
    puts("2. " blue"By Author's Name"reset);
    puts("3. " blue"By ISBN"reset);
    puts("4. " blue"By Genre"reset);
    puts("5. " blue"By Price Range"reset);
    puts("6. " blue"By Publisher\n"reset);
    printf("Enter Search Filter " blue"(1 - 6)"reset":\n");
    scanf("%d", &filter);
    while (!(filter >= 1 && filter <= 6)) {
        puts(red"Your Selection must be between 1 - 6, try again"reset);
        scanf("%d", &filter);
    }

    cls();

    switch (filter) {
        case 1: {
            //By Name:
            puts(bwhite"Search By Name"reset);
            int counter = 1;
            printf("Enter the book name or a part of it:\n");
            scanf(" %[^\n]", looking_for_book.Book_Name);
            for (int i = 0; i < 336; i++) {
                if (strstr(BooksDataBase[i].Book_Name, looking_for_book.Book_Name) != NULL) { // strcmp returns 0 if two strings are equal
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                    if (BooksDataBase[i].is_in_library == 1) {
                         if (BooksDataBase[i].is_in_library == 1) {
                            printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                            printf(blue"at shelf %d with %s genre\n\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                        }
                    }
                }
            }
            if (was_available == 0) 
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                
            printf(green"Search is complete!\n\n"reset);

            ClearScreen();
            break;
        }
        case 2: {
            //By Author's Name:
            puts(bwhite"Search By Author's Name"reset);
            int counter = 1;
            printf("Enter the author's name or a part of it:\n");
            scanf(" %[^\n]", looking_for_book.Author_Name);
            for (int i = 0; i < 336; i++) {
                if (strstr(BooksDataBase[i].Author_Name, looking_for_book.Author_Name) != NULL) { // strcmp returns 0 if two strings are equal
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                    if (BooksDataBase[i].is_in_library == 1) {
                        printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                            printf(blue"at shelf %d with %s genre\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                    }
                }
            }
            if (was_available == 0) 
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                
            printf(green"Search is complete!\n\n"reset);

            ClearScreen();
            break;
        }
        case 3: {
            //By ISBN:
            int counter = 1;
            puts(bwhite"Search By ISBN"reset);
            printf("Enter the book ISBN:\n");
            scanf("%d", &(looking_for_book.ISBN));
            for (int i = 0; i < 336; i++) {
                if (BooksDataBase[i].ISBN == looking_for_book.ISBN) {
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                     if (BooksDataBase[i].is_in_library == 1) {
                        printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                            printf(blue"at shelf %d with %s genre\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                    }
                }
            }
            if (was_available == 0) 
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                
            printf(green"Search is complete!\n\n"reset);
            
            ClearScreen();
            break;
        }
        case 4: {
            //By Genre:
            int counter = 1;
            puts(bwhite"Search By Genre"reset);
            printf("Enter the book genre:\n");
            scanf(" %[^\n]", looking_for_book.Genre);
            for (int i = 0; i < 336; i++) {
                if ((strcmp(BooksDataBase[i].Genre,looking_for_book.Genre)) == 0) { // strcmp returns 0 if two strings are equal
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                    if (BooksDataBase[i].is_in_library == 1) {
                         if (BooksDataBase[i].is_in_library == 1) {
                            printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                            printf(blue"at shelf %d with %s genre\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                        }
                    }
                }
            }
            if (was_available == 0) 
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                
            printf(green"Search is complete!\n\n"reset);
            
            ClearScreen();
            break;
        }
        case 5: {
            //By Price Range:
            puts(bwhite"Search By Price Range\n"reset);
            int counter = 1;
            int min,max;
            printf("Enter the minimum price:\n");
            scanf("%d", &min);
            printf("Enter the maximum price:\n");
            scanf("%d", &max);
            if (max < min) {
                printf(blue"\nMinimum and Maximum price corrected! (Maximum price was bigger than minimum!)\n"reset);
                swap(&max, &min);
            }
            for (int i = 0; i < 336; i++) {
                if (BooksDataBase[i].Price >= min && BooksDataBase[i].Price <= max) {
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                     if (BooksDataBase[i].is_in_library == 1) {
                        printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                        printf(blue"at shelf %d with %s genre\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                    }
                }
            }
            if (was_available == 0)
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);

            printf(green"Search is complete!\n\n"reset);

            ClearScreen();
            break;
        }
        case 6: {
            //By Publisher:
            puts(bwhite"Search By Publisher"reset);
            int counter = 1;
            printf("Enter the book publisher or a part of it:\n");
            scanf(" %[^\n]", looking_for_book.Publisher);
            for (int i = 0; i < 336; i++) {
                if (strstr(BooksDataBase[i].Publisher, looking_for_book.Publisher) != NULL) { // strcmp returns 0 if two strings are equal
                    was_available = 1;
                    printf("%d.\n", counter);
                    counter++;
                    PrintBookInfo(BooksDataBase + i);
                    putchar('\n');
                    if (BooksDataBase[i].is_in_library == 1) {
                         if (BooksDataBase[i].is_in_library == 1) {
                            printf(blue"Your searched book is located at library number %d (%s library)\n"reset, BooksDataBase[i].which_library,All_Libraries[BooksDataBase[i].which_library - 1]);
                            printf(blue"at shelf %d with %s genre\n"reset, BooksDataBase[i].which_shelf, BooksDataBase[i].Genre);
                        }
                    }
                }
            }
            if (was_available == 0) 
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                
            printf(green"Search is complete!\n\n"reset);
            
            ClearScreen();
            break;
        }
    }
}
int Search_by_ISBN(struct Book BooksDataBase[336]) {
    struct Book looking_for_book;
    int was_available = 0; // The book may not be available in the warehouse
    printf("Enter the Full Name and the ISBN of the book:\n");
    printf("Name:\t");
    scanf(" %[^\n]", looking_for_book.Book_Name);
    printf("ISBN:\t");
    scanf("%d", &(looking_for_book.ISBN));
            for (int i = 0; i < 336; i++) {
                if ((strcmp(BooksDataBase[i].Book_Name, looking_for_book.Book_Name) == 0) && BooksDataBase[i].ISBN == looking_for_book.ISBN) { // strcmp returns 0 if two strings are equal
                    was_available = 1;
                    return i;
                }
            }
            if (was_available == 0) {
                printf(red"The book you searched for isn't available in the warehouse!\n\n"reset);
                return -1;
            }
}
void swap(int *p, int *q) {
    int temp;
    temp = *p;
    *p = *q;
    *q = temp;
}
void assign_genre_to_shelf(struct Library All_Libraries[4]) {
    for (int i = 0; i < 4; i++) {
        strcpy(All_Libraries[i].Shelves[0].Genre, "Historical");
        strcpy(All_Libraries[i].Shelves[1].Genre, "Historical");
        strcpy(All_Libraries[i].Shelves[2].Genre, "Biography");
        strcpy(All_Libraries[i].Shelves[3].Genre, "Biography");
        strcpy(All_Libraries[i].Shelves[4].Genre, "Adventure");
        strcpy(All_Libraries[i].Shelves[5].Genre, "Adventure");
        strcpy(All_Libraries[i].Shelves[6].Genre, "Romance");
        strcpy(All_Libraries[i].Shelves[7].Genre, "Romance");
        strcpy(All_Libraries[i].Shelves[8].Genre, "Mystery");
        strcpy(All_Libraries[i].Shelves[9].Genre, "Mystery");
        strcpy(All_Libraries[i].Shelves[10].Genre, "Horror");
        strcpy(All_Libraries[i].Shelves[11].Genre, "Horror");
        strcpy(All_Libraries[i].Shelves[12].Genre, "Crime");
        strcpy(All_Libraries[i].Shelves[13].Genre, "Crime");
        strcpy(All_Libraries[i].Shelves[14].Genre, "Motivational");
        strcpy(All_Libraries[i].Shelves[15].Genre, "Motivational");
        strcpy(All_Libraries[i].Shelves[16].Genre, "Scientific");
        strcpy(All_Libraries[i].Shelves[17].Genre, "Scientific");
        strcpy(All_Libraries[i].Shelves[18].Genre, "Health");
        strcpy(All_Libraries[i].Shelves[19].Genre, "Health");
        strcpy(All_Libraries[i].Shelves[20].Genre, "Travel");
        strcpy(All_Libraries[i].Shelves[21].Genre, "Travel");
        strcpy(All_Libraries[i].Shelves[22].Genre, "Children's");
        strcpy(All_Libraries[i].Shelves[23].Genre, "Children's");
        strcpy(All_Libraries[i].Shelves[24].Genre, "Action");
        strcpy(All_Libraries[i].Shelves[25].Genre, "Action");
        strcpy(All_Libraries[i].Shelves[26].Genre, "Sport");
        strcpy(All_Libraries[i].Shelves[27].Genre, "Sport");
    }
}
int ShowAndSelectLibrary(struct Library All_Libraries[4]) {
    int selected_lib;
    for (int i = 0; i < 4; i++) {
        printf("%d. ", i + 1);
        printf(blue"%s\n"reset, All_Libraries[i].library_Name);
        printf("Located at: " blue"%s\n\n"reset, All_Libraries[i].Address);
    }
    puts(blue"(1 - 4):"reset);
    scanf("%d", &selected_lib);
    while (selected_lib > 4 || selected_lib < 1) {
    puts(red"wrong input, try again:"reset);
    scanf("%d", &selected_lib);
    }
    return selected_lib;
}