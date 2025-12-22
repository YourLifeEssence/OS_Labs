#include "task2.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

void task2() {
    TrieNode* root = create_node();
    char buf[256];

    printf("=== Примитивная оболочка ===\n");

    while (1) {
        printf("\nМеню: [1] Register  [2] Login  [3] Exit\nВыберите: ");
        read_line(buf, sizeof(buf));
        if (strcmp(buf, "1") == 0 || strcasecmp(buf, "Register") == 0) {
            registration_flow(root);
        } else if (strcmp(buf, "2") == 0 || strcasecmp(buf, "Login") == 0) {
            char login[64]; int pin;
            if (login_flow(root, login, &pin)) {
                session_loop(root, login);
            } else {
                printf("Не удалось авторизоваться\n");
            }
        } else if (strcmp(buf, "3") == 0 || strcasecmp(buf, "Exit") == 0) {
            printf("Выход из программы\n");
            break;
        } else {
            printf("Неверный выбор\n");
        }
    }
    free_trie(root);
}

TrieNode* create_node() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (!node) {
        return NULL;
    }
    node->is_end = 0;
    node->pin = 0;
    node->is_limited = 0;
    node->commands_limit = 0;

    for (int i = 0; i < AlphSize; ++i) {
        node->children[i] = NULL;
    }
    return node;
}

int trie_insert(TrieNode* root, const char* key, int pin) {
    TrieNode* p = root;
    for (int i = 0; key[i]; ++i) {
        int index = char_to_index(key[i]);
        if (index < 0) return 0;
        if (!p ->children[index]) {
            p->children[index] = create_node();
            if (!p-> children[index]) return 0;
        }
        p = p->children[index];
    }
    if (p->is_end) return 0;
    p->is_end = 1;
    p->pin = pin;
    return 1;
}

int char_to_index(char c) {
    if (isalpha((unsigned char)c)) {
        c = tolower((unsigned char)c);
        return c - 'a';
    } else if (isdigit((unsigned char)c)) {
        return 26 + (c - '0');
    }
    return - 1;
}

int valid_login(const char* key) {
    int len = strlen(key);
    if (len == 0 || len > MAX_LOGIN_LEN) return 0;
    for (int i = 0; key[i]; ++i) {
        if (!isalnum((unsigned char)key[i])) return 0;
    }
    return 1;
}

TrieNode* search_node_tree(TrieNode* root, const char* key) {
    TrieNode* p = root;
    for (int i = 0; key[i]; ++i) {
        int index = char_to_index(key[i]);
        if (index < 0) return NULL;
        if (!p->children[index]) return NULL;
        p = p->children[index];
    }
    return p;
}

int trie_exists(TrieNode* root, const char* key) {
    TrieNode* node = search_node_tree(root, key);
    return node && node->is_end;
}

void print_time_now() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    printf("%02d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void print_date_now() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("%02d:%02d:%04d\n", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

int parse_datetime(const char* s, struct tm* out_tm) {
    int d, mo, y, hh, mm, ss;
    if (sscanf(s, "%d:%d:%d %d:%d:%d", &d, &mo, &y, &hh, &mm, &ss) != 6) {
        return 0;
    }
    if (d < 1 || d > 31 || mo < 1 || mo > 12 || y < 1900 || hh < 0 || hh > 23 || mm < 0 || mm > 59 || ss < 0 || ss > 59)
        return 0;
    memset(out_tm, 0, sizeof(struct tm));
    out_tm->tm_mday = d;
    out_tm->tm_mon = mo - 1;
    out_tm->tm_year = y - 1900;
    out_tm->tm_hour = hh;
    out_tm->tm_min = mm;
    out_tm->tm_sec = ss;
    return 1;
}

void handle_howmuch(const char* datetime_str, const char* flag) {
    struct tm t_parsed;
    if (!parse_datetime(datetime_str, &t_parsed)) {
        printf("Неверный формат datetime. Ожидается: дд:MM:гггг чч:мм:сс\n");
        return;
    }
    time_t t0 = mktime(&t_parsed);
    if (t0 == (time_t)-1) {
        printf("Ошибка преобразования времени\n");
        return;
    }
    time_t now = time(NULL);
    double diff = difftime(now, t0);

    if (strcmp(flag, "-s") == 0) {
        printf("%.0f секунд\n", diff);
    } else if (strcmp(flag, "-m") == 0) {
        printf("%.0f минут\n", diff / 60.0);
    } else if (strcmp(flag, "-h") == 0) {
        printf("%.0f часов\n", diff / 3600.0);
    } else if (strcmp(flag, "-y") == 0) {
        printf("%.6f лет\n", diff / (3600.0 * 24.0 * 365.25));
    } else {
        printf("Неверный флаг. Используйте -s, -m, -h или -y\n");
    }
}

void read_line(char* buf, size_t maxSize) {
    if (!fgets(buf, (int)maxSize, stdin)) {
        buf[0] = '\0';
        return;
    }
    // удалить завершающий \n
    size_t ln = strlen(buf);
    if (ln > 0 && buf[ln - 1] == '\n') buf[ln - 1] = '\0';
}

int tokenize(char* s, char* tokens[], int max_tokens) {
    int cnt = 0;
    char* p = strtok(s, " ");
    while (p && cnt < max_tokens) {
        tokens[cnt++] = p;
        p = strtok(NULL, " ");
    }
    return cnt;
}

void registration_flow(TrieNode* root) {
    char login[64], pinbuf[64];
    printf("=== Регистрация ===\n");
    while (1) {
        printf("Введите login (латиница и цифры, max %d): ", MAX_LOGIN_LEN);
        read_line(login, sizeof(login));
        if (!valid_login(login)) {
            printf("Неверный логин. Только латинские буквы и цифры, длина 1..%d\n", MAX_LOGIN_LEN);
            continue;
        }
        if (trie_exists(root, login)) {
            printf("Пользователь с таким логином уже существует\n");
            continue;
        }
        break;
    }
    int pin = -1;
    while (1) {
        printf("Введите PIN (целое %d..%d): ", MIN_PIN, MAX_PIN);
        read_line(pinbuf, sizeof(pinbuf));
        char* end;
        long v = strtol(pinbuf, &end, 10);
        if (end == pinbuf || *end != '\0') {
            printf("PIN должен быть целым числом\n");
            continue;
        }
        if (v < MIN_PIN || v > MAX_PIN) {
            printf("PIN вне допустимого диапазона\n");
            continue;
        }
        pin = (int)v;
        break;
    }
    if (trie_insert(root, login, pin)) {
        printf("Регистрация успешна. Можете войти под логином '%s'\n", login);
    } else {
        printf("Ошибка регистрации\n");
    }
}

int login_flow(TrieNode* root, char* out_login, int* out_pin) {
    char login[64], pinbuf[64];
    printf("=== Вход ===\n");
    printf("Login: ");
    read_line(login, sizeof(login));
    if (!valid_login(login)) {
        printf("Неверный логин (формат)\n");
        return 0;
    }
    TrieNode* node = search_node_tree(root, login);
    if (!node || !node->is_end) {
        printf("Пользователь не найден\n");
        return 0;
    }
    printf("PIN: ");
    read_line(pinbuf, sizeof(pinbuf));
    char* end;
    long v = strtol(pinbuf, &end, 10);
    if (end == pinbuf || *end != '\0') {
        printf("PIN должен быть целым\n");
        return 0;
    }
    if ((int)v != node->pin) {
        printf("Неверный PIN\n");
        return 0;
    }
    strcpy(out_login, login);
    *out_pin = node->pin;
    return 1;
}

void session_loop(TrieNode* root, const char* login) {
    TrieNode* node = search_node_tree(root, login);
    if (!node || !node->is_end) {
        printf("Ошибка сессии: пользователь не найден\n");
        return;
    }
    int session_cmd_count = 0;
    int sanction_limit = node->is_limited ? node->commands_limit : -1;

    printf("Успешный вход. Доступные команды: Time, Date, Howmuch <datetime> flag, Sanctions <username>, Logout\n");

    char line[512];
    while (1) {
        if (node->is_limited && sanction_limit >= 0 && session_cmd_count >= sanction_limit) {
            printf("Достигнут лимит команд в сеансе. Выполните Logout для выхода.\n");
            // Ждём только Logout
            printf("%s> ", login);
            read_line(line, sizeof(line));
            // разбор
            char tmp[512];
            strncpy(tmp, line, sizeof(tmp)-1); tmp[sizeof(tmp)-1] = '\0';
            char* tokens[MAX_TOKENS];
            int tcnt = tokenize(tmp, tokens, MAX_TOKENS);
            if (tcnt == 0) continue;
            if (strcasecmp(tokens[0], "Logout") == 0) {
                printf("Выход...\n");
                return;
            } else {
                printf("Запрещено выполнять команды. Введите Logout, чтобы выйти.\n");
                continue;
            }
        }

        printf("%s> ", login);
        read_line(line, sizeof(line));
        // пропустить пустые строки
        if (strlen(line) == 0) continue;

        char tmp[512];
        strncpy(tmp, line, sizeof(tmp)-1); tmp[sizeof(tmp)-1] = '\0';
        char* tokens[MAX_TOKENS];
        int tcnt = tokenize(tmp, tokens, MAX_TOKENS);
        if (tcnt == 0) continue;

        if (strcasecmp(tokens[0], "Logout") == 0) {
            printf("Выход из сессии...\n");
            return;
        }

        if (strcasecmp(tokens[0], "Time") == 0) {
            print_time_now();
            session_cmd_count++;
            continue;
        }

        if (strcasecmp(tokens[0], "Date") == 0) {
            print_date_now();
            session_cmd_count++;
            continue;
        }

        if (strcasecmp(tokens[0], "Howmuch") == 0) {
            if (tcnt < 4) {
                printf("Usage: Howmuch дд:MM:гггг чч:мм:сс <flag>\n");
            } else {
                char datetime_buf[128];
                snprintf(datetime_buf, sizeof(datetime_buf), "%s %s", tokens[1], tokens[2]);
                handle_howmuch(datetime_buf, tokens[3]);
                session_cmd_count++;
            }
            continue;
        }

        if (strcasecmp(tokens[0], "Sanctions") == 0) {
            if (tcnt < 2) {
                printf("Usage: Sanctions <username>\n");
                continue;
            }
            char* target = tokens[1];
            if (!valid_login(target)) {
                printf("Неверный логин-цель (формат)\n");
                continue;
            }
            TrieNode* tnode = search_node_tree(root, target);
            if (!tnode || !tnode->is_end) {
                printf("Пользователь '%s' не найден\n", target);
                continue;
            }
            printf("Вы действительно хотите ввести санкцию для '%s'? Для подтверждения введите 52: ", target);
            char confbuf[64];
            read_line(confbuf, sizeof(confbuf));
            if (strcmp(confbuf, "52") != 0) {
                printf("Подтверждение не принято. Отмена.\n");
                continue;
            }
            tnode->is_limited = 1;
            tnode->commands_limit = DEFAULT_SANCTION_LIMIT;
            printf("Санкция применена: пользователь '%s' теперь ограничен %d командами за сессию.\n", target, tnode->commands_limit);
            session_cmd_count++;
            continue;
        }

        printf("Неизвестная команда: %s\n", tokens[0]);
    }
}

void free_trie(TrieNode* node) {
    if (!node) return;

    for (int i = 0; i < AlphSize; i++) {
        if (node->children[i]) {
            free_trie(node->children[i]);
        }
    }
    free(node);
}