#ifndef uCUTILS_RET_H
#define uCUTILS_RET_H

typedef enum { UCRET_OK = 0, UCRET_ENOMEM = -1, UCRET_EPARAM = -2 } ucret_t;

static inline const char *ucstrerror(ucret_t r) {
  switch (r) {
  case UCRET_ENOMEM:
    return "Failed to allocate memory";
  case UCRET_EPARAM:
    return "Invalid parameter";
  case UCRET_OK:
    return "No error";
  default:
    return "Unknown error";
  }
}

#endif // uCUTILS_RET_H
