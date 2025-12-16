#ifndef LABS_TASK2_H
#define LABS_TASK2_H

#include "task2.h"
#include <stdlib.h>
#include <time.h>

#define DEFAULT_SANCTION_LIMIT 5
#define MAX_LOGIN_LEN 6
#define MAX_PIN 100000
#define AlphSize 36
#define MAX_TOKENS 10
#define MIN_PIN 0

typedef struct TrieNode {
    struct TrieNode* children[AlphSize];
    int is_end;
    int pin;
    int is_limited;
    int commands_limit;
} TrieNode;

TrieNode* create_node();

int trie_insert(TrieNode* root, const char* key,int pin);

int char_to_index(char c);

int valid_login(const char* key);

TrieNode* search_node_tree(TrieNode* root, const char* key);

int trie_exists(TrieNode* root, const char* key);

void print_time_now();

void print_date_now();

void task2();

int parse_datetime(const char* s, struct tm* out_tm);

void handle_howmuch(const char* datetime_str, const char* flag);

void read_line(char* buf, size_t sz);

int tokenize(char* s, char* tokens[], int max_tokens);

void registration_flow(TrieNode* root);

int login_flow(TrieNode* root, char* out_login, int* out_pin);

void session_loop(TrieNode* root, const char* login);

void free_trie(TrieNode* node);

#endif //LABS_TASK2_H