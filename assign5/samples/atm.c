/* 
 * File: atm.c
 * -----------
 * Simulated teller machine, mostly working code, but bugs lurk within...
*/

#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BANK_DATA_DIR "/afs/ir/class/cs107/samples/assign5/bank/"
#define DATABASE_FILE BANK_DATA_DIR "customers.db"
#define MAX_NAME_LEN 8
#define MIN_BALANCE 50
#define MAX_WITHDRAWAL 300

enum response { 
    NOT_AUTHORIZED = -1, 
    INSUFFICIENT_FUNDS = -2,
    EXCEEDS_LIMIT = -3,
};

typedef struct customer {
    int account_num;
    char name[MAX_NAME_LEN+1];
    int balance;
} customer;

typedef struct database {
    customer array[1000];
    int count;
} database;

database *read_database(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        error(1, errno, "%s", filename);
    }
    database *db = calloc(1, sizeof(*db));
    db->count = 0;
    for (int i = 0; i < sizeof(db->array)/sizeof(*db->array); i++) {
        if (fscanf(fp, "%8s $%d\n", db->array[i].name, &db->array[i].balance) != 2) {
            break;
        }
        db->array[i].account_num = i;
        db->count++;
    }
    fclose(fp);
    assert(db->count);
    return db;
}

bool is_authorized(const char *account_name) {
    /* Ignore the possibility of any issues in this code - getlogin()
     * reliably returns the correct username.
     */
    return strcmp(account_name, getlogin()) == 0;
}

int find_account(database *db, const char *username) {
    for (int i = 0; i < db->count; i++) {
        if (!strcmp(username, db->array[i].name)) {
            if (is_authorized(db->array[i].name)) {
                return db->array[i].account_num;
            } else {
                return NOT_AUTHORIZED;
            }
        }
    }
}

customer *lookup_by_name(database *db, const char *username) {
    int account_num = find_account(db, username);
    if (account_num >= 0) {
        return db->array + account_num;
    } else {
        return NULL;
    }
}

int read_secret_passcode(const char *filename) {
    int code = -1;
    char path[sizeof(BANK_DATA_DIR) + sizeof(filename) + 1];

    sprintf(path, "%s%s", BANK_DATA_DIR, filename);
    FILE *fp = fopen(path, "r"); 
    if (fp) {
        fscanf(fp, "%d", &code);
        fclose(fp);
    }
    return code;
}

// arg1 and arg2 are user-supplied credential for account number and passcode
customer *lookup_by_number(database *db, const char *arg1, const char *arg2) {
    int account_num = strtol(arg1, NULL, 0);
    if (account_num < 0 || account_num >= db->count) {
        return NULL;
    }

    int saved_code = read_secret_passcode(arg1);
    int entered_code = strtol(arg2, NULL, 0);
    if (saved_code < 0 || saved_code != entered_code) {
        return NULL;
    }

    return db->array + account_num;
}

bool withdraw(customer *cust, unsigned int amount) {
    if (cust->balance - amount >= MIN_BALANCE) { // PREVIOUSLY: cust->balance >= amount
        cust->balance -= amount;
        return true;
    }
    return false;
}

int process_request(customer *cust, unsigned int amount) {
    if (cust == NULL) {
        return NOT_AUTHORIZED;
    }
    if (amount > MAX_WITHDRAWAL) {
        return EXCEEDS_LIMIT;
    }
    if (!withdraw(cust, amount)) {
        return INSUFFICIENT_FUNDS;
    }
    return amount;
}

void dispense_money(int n) {
    printf("Please take your cash: ");
    for (int i = 0; i < n; i++) {
        printf("$");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        error(1, 0, "Required arguments are amount to withdraw and account credential, \nconsisting of either the username on the account or account number and passcode.");
    }

    database *db = read_database(DATABASE_FILE);
    customer *cust;
    int amount = atoi(argv[1]);

    if (argc == 4) {
        cust = lookup_by_number(db, argv[2], argv[3]);
    } else {
        cust = lookup_by_name(db, argv[2]);
    }

    int result = process_request(cust, amount);

    if (result >= 0) {
        dispense_money(result);
        // log successful transaction
        fprintf(stderr, "\t[Transaction Log: Presented credential as %s, account #%d, withdrew $%d, new balance = $%d]\n", 
            argv[2], cust->account_num, amount, cust->balance);
    } else {
        char *reason = "unknown";
        if (result == NOT_AUTHORIZED) {
            reason = "unauthorized.";
        } else if (result == INSUFFICIENT_FUNDS) {
            reason = "insufficient funds.";
        } else if (result == EXCEEDS_LIMIT) {
            reason = "exceeds transaction limit.";
        }
        
        printf("Request failed: %s\n", reason);
    }
    free(db);
    return 0;
}
