#ifndef DOD_DODTEST_H
#define DOD_DODTEST_H

#ifndef DODTEST_SPACE1
#define DODTEST_SPACE1 "21"
#endif

#ifndef DODTEST_SPACE2
#define DODTEST_SPACE2 "6"
#endif

#define DODTEST_PRINT_THEAD()                                                  \
  fprintf(stderr, "| %" DODTEST_SPACE1 "s | %" DODTEST_SPACE2 "s |\n", "Test", \
          "Result")
#define DODTEST_RUN(msg, test)                                                 \
  do {                                                                         \
    fprintf(stderr, "| %" DODTEST_SPACE1 "s | ", msg);                         \
    test;                                                                      \
    fprintf(stderr, "%" DODTEST_SPACE2 "s |\n", "OK");                         \
  } while (0)

#endif // DOD_DODTEST_H
