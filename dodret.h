#ifndef DOD_RET_H
#define DOD_RET_H

typedef enum {
    DODRET_OK = 0, DODRET_ENOMEM = -1, DODRET_EPARAM = -2
} dodret_t;

static inline const char *dodstrerror(dodret_t r) {
  switch (r) {
    case DODRET_ENOMEM:
      return "Failed to allocate memory";
    case DODRET_EPARAM:
      return "Invalid parameter";
    case DODRET_OK:
      return "No error";
    default:
      return "Unknown error";
  }
}

#endif // DOD_RET_H
