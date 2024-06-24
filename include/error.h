#ifndef _ERROR_H_
#define _ERROR_H_

// OS error codes.

// Unspecified or unknown problem
#define E_UNSPECIFIED 1

// Environment doesn't exist or otherwise cannot be used in requested action
#define E_BAD_ENV 2

// Invalid parameter
#define E_INVAL 3

// Request failed due to memory shortage
#define E_NO_MEM 4

// Invalid syscall number
#define E_NO_SYS 5

// Attempt to create a new environment beyond the maximum allowed
#define E_NO_FREE_ENV 6

// Attempt to send to env that is not recving.
#define E_IPC_NOT_RECV 7

// File system error codes -- only seen in user-level

// No free space left on disk
#define E_NO_DISK 8

// Too many files are open
#define E_MAX_OPEN 9

// File or block not found
#define E_NOT_FOUND 10

// Bad path
#define E_BAD_PATH 11

// File already exists
#define E_FILE_EXISTS 12

// File not a valid executable
#define E_NOT_EXEC 13

// The target pool is not found
#define E_POOL_NOT_FOUND 14

// The pool has been bind
#define E_POOL_DOUBLE_BIND 15

// The pool is not bind
#define E_POOL_NOT_BIND 16

// Unlock a lock without being locked
#define E_NO_LOCK 17

// Unlock a lock which was locked by another env
#define E_LOCK_BY_OTHERS 18

/*
 * A quick wrapper around function calls to propagate errors.
 * Use this with caution, as it leaks resources we've acquired so far.
 */
#define try(expr)                                                                                  \
	do {                                                                                       \
		int _r = (expr);                                                                   \
		if (_r != 0)                                                                       \
			return _r;                                                                 \
	} while (0)

#endif // _ERROR_H_
