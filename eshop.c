#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define CATALOG_SIZE 20
#define NUM_CLIENTS 5
#define NUM_ORDERS 10

// Δομή για τα προϊόντα
struct product {
    char description[100];
    float price;
    int item_count;  // Διαθέσιμα τεμάχια
};

struct product catalog[CATALOG_SIZE] = {
    {"Product 1", 10.50, 2},
    {"Product 2", 15.00, 2},
    {"Product 3", 23.00, 2},
    {"Product 4", 46.50, 2},
    {"Product 5", 12.00, 2},
    {"Product 6", 20.00, 2},
    {"Product 7", 30.00, 2},
    {"Product 8", 17.50, 2},
    {"Product 9", 22.00, 2},
    {"Product 10", 8.00, 2},
    {"Product 11", 14.50, 2},
    {"Product 12", 16.00, 2},
    {"Product 13", 21.50, 2},
    {"Product 14", 27.00, 2},
    {"Product 15", 11.00, 2},
    {"Product 16", 13.00, 2},
    {"Product 17", 27.00, 2},
    {"Product 18", 50.00, 2},
    {"Product 19", 8.50, 2},
    {"Product 20", 33.00, 2},
};

// Αρχεία επικοινωνίας με πελάτες
int pipes_parent_to_child[NUM_CLIENTS][2];
int pipes_child_to_parent[NUM_CLIENTS][2];

void handle_order(int client_id) {
    int product_id;
    while (read(pipes_parent_to_child[client_id][0], &product_id, sizeof(product_id)) > 0) {
        // Εξυπηρετούμε την παραγγελία του πελάτη
        struct product *p = &catalog[product_id];
        char message[200];
        
        if (p->item_count > 0) {
            p->item_count--;
            snprintf(message, sizeof(message), "Purchase complete, your total is: %.2f euro", p->price);
        } else {
            snprintf(message, sizeof(message), "Sorry, the product is out of stock");
        }

        // Αποστολή της απάντησης στον πελάτη
        write(pipes_child_to_parent[client_id][1], message, sizeof(message));

        sleep(1); // 1 δευτερόλεπτο αναμονής για την εξυπηρέτηση
    }
}

int main() {
    srand(time(NULL));

    pid_t pid;
    int num_clients = NUM_CLIENTS;

    // Δημιουργία των pipes και των πελατών
    for (int i = 0; i < num_clients; i++) {
        if (pipe(pipes_parent_to_child[i]) == -1 || pipe(pipes_child_to_parent[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }

        pid = fork();
        if (pid == 0) {
            // Κώδικας για τον πελάτη
            close(pipes_parent_to_child[i][0]);  // Κλείσιμο του read pipe
            close(pipes_child_to_parent[i][1]);  // Κλείσιμο του write pipe
            
            for (int j = 0; j < NUM_ORDERS; j++) {
                int product_id = rand() % CATALOG_SIZE; // Τυχαία επιλογή προϊόντος
                write(pipes_parent_to_child[i][1], &product_id, sizeof(product_id));
                
                // Περιμένει την απάντηση από τον εξυπηρετητή
                char message[200];
                read(pipes_child_to_parent[i][0], message, sizeof(message));
                printf("Client %d: %s\n", i + 1, message);
                
                sleep(1);  // 1 δευτερόλεπτο αναμονής για την επόμενη παραγγελία
            }
            exit(0);
        }
    }

    // Κώδικας για τον εξυπηρετητή
    for (int i = 0; i < num_clients; i++) {
        close(pipes_parent_to_child[i][1]);  // Κλείσιμο του write pipe
        close(pipes_child_to_parent[i][0]);  // Κλείσιμο του read pipe
        handle_order(i);
    }

    // Περιμένει να ολοκληρωθούν όλοι οι πελάτες
    for (int i = 0; i < num_clients; i++) {
        wait(NULL);
    }

    return 0;
}

