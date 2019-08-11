#ifndef uCUTILS_UCTEST_H
#define uCUTILS_UCTEST_H

#ifndef UCTEST_SPACE1
#define UCTEST_SPACE1 "21"
#endif

#ifndef UCTEST_SPACE2
#define UCTEST_SPACE2 "6"
#endif

#define UCTEST_PRINT_THEAD() fprintf(stderr, "| %"UCTEST_SPACE1"s | %"UCTEST_SPACE2"s |\n", "Test", "Result")
#define UCTEST_RUN(msg, test)                       \
  do {                                              \
    fprintf(stderr, "| %"UCTEST_SPACE1"s | ", msg); \
    test;                                           \
    fprintf(stderr, "%"UCTEST_SPACE2"s |\n", "OK"); \
  } while (0)

#endif //uCUTILS_UCTEST_H
