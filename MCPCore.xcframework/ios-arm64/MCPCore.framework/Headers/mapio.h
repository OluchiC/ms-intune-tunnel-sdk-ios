/**
 * @file mapio.h
 *
 * Contains a secure, API-compatable implementation of many libc and BSD socket
 * functions related to file-based and network I/O. See the original functions'
 * documentation for more information on general API usage.
 *
 * @note the functions located here are used on multiple platforms. For
 *   functions only used on specific platforms, consult the appropriate
 *   platform-specific mapio_*.h header file
 */

#ifndef __MAPIO_H_
#define __MAPIO_H_

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE 1
#endif
#define ATLAS_SOCKETS_LAYER

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdbool.h>
#include <wchar.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <dlfcn.h>

#if defined(__APPLE__) || defined(TARGET_OS_MAC)
#include <xlocale.h>
#include <sys/syscall.h>
#include <aio.h>
#include <sys/event.h>
#include <copyfile.h>
#include <sys/mount.h>
#else
#include <sys/statfs.h>
#endif

#ifdef __cplusplus
#include "IMCPInterception.h"
#endif

#if defined(__APPLE__)
typedef off_t off64_t;
#endif

#if defined(__ANDROID__)
typedef void * android_net_context;
typedef off64_t fpos64_t;
#include <sys/select.h>

#include <android/api-level.h>
#if __ANDROID_API__ >= 21
#define POST_ANDROID_L
#else
#define PRE_ANDROID_L
#endif /* __ANDROID_API__ */

#endif

#pragma GCC visibility push(default)

#ifdef __cplusplus
extern "C" {

void MAPIO_setInterceptionCB(struct MCP_INTERCEPTION_CALLBACKS const *pCBs);
void MAPIO_setPluginIOCB(const unsigned int pioTypes, PLUGIN_FILEIO_CALLBACKS *pCallbacks);

// doesn't belong here.
FD_CONTEXT_HANDLE MAPIO_acquire_fd_context_public(int fd);
void MAPIO_release_fd_context_public(FD_CONTEXT_HANDLE pContextHdl);
PLUGIN_FILEIO_CALLBACKS *MAPIO_get_fd_callbacks_public(int fd);
PLUGIN_FILEIO_CALLBACKS *MAPIO_get_passthrough_public();
int MAPIO_assign_fd_data_public(int fd, fd_data_t *pFdData);
#endif

void MAPIO_init(const char* tmp_dir_path, size_t len);
void MAPIO_enablePAVE();
void MAPIO_disablePAVE();
void MAPIO_setExemptedPaths(const char* paths[], const size_t numPaths);
void MAPIO_enablePluginIO(const unsigned int pioTypes);
void MAPIO_disablePluginIO(const unsigned int pioTypes);
#if defined(__ANDROID__)
void MAPIO_addJNIClassReplacement(const char *originalFQClassName, const char *replacementFQClassName);
void MAPIO_addJNIMethodReplacement(const char *originalFQClassName,
                                   const char *originalMethodName,
                                   const char *originalMethodSig,
                                   const char *replacementFQClassName,
                                   const char *replacementMethodName,
                                   const char *replacementMethodSig);
#endif
bool MAPIO_isExempted(const char *path);

// Used for Android only
pthread_key_t MAPIO_get_fork_and_exec_bypass_key();
void MAPIO_acquireForkExecBypass(int *outWasPreviouslyAcquired);
void MAPIO_resetForkExecBypass(int state);

const char *MAPIO_get_tmp_dir_public();
void MAPIO_lock_fd_public(int fd);
void MAPIO_unlock_fd_public(int fd);

void MAPIO_abort();

int MAPIO_close(int fd);
#ifdef __ANDROID__
int MAPIO_android_fdsan_close_with_tag(int fd, uint64_t expected_tag);
#endif

int MAPIO_dup(int fd);
int MAPIO_dup2(int fd, int fd2);

int MAPIO_fcntl(int fd, int cmd, ...);
int MAPIO_ioctl(int fd, unsigned long req, ...);

ssize_t MAPIO_read(int fd, void *buf, size_t count);
ssize_t MAPIO_readv(int fd, const struct iovec *iov, int iov_count);
ssize_t MAPIO_write(int fd, const void *buf, size_t count);
ssize_t MAPIO_writev(int fd, const struct iovec *iov, int iov_count);

int MAPIO_fcntl64(int fd, int cmd, void* arg);

#if defined(__ANDROID__)
  /* FORTIFY */
ssize_t MAPIO___read_chk(int fd, void *buf, size_t count, size_t buf_size);
ssize_t MAPIO___recvfrom_chk(int s, void *mem, size_t len, size_t buflen, int flags,
                             struct sockaddr *from, socklen_t *fromlen);
ssize_t MAPIO___write_chk(int fd, const void* buf, size_t count, size_t buf_size);
ssize_t MAPIO___sendto_chk(int socket, const void* buf, size_t len, size_t buflen,
                           int flags, const struct sockaddr* dest_addr,
                           socklen_t addrlen);

int MAPIO_android_getaddrinfofornetcontext(const char * p1, const char *p2, const struct addrinfo *p3,
                                           const android_net_context p4, struct addrinfo **p5);
struct hostent *MAPIO_android_gethostbyaddrfornet(const void *p1, socklen_t p2, int p3, unsigned p4, unsigned p5);
struct hostent *MAPIO_android_gethostbynamefornet(const char *p1, int p2, unsigned p3, unsigned p4);
int MAPIO_sigaction(int p1, const struct sigaction* p2, struct sigaction* p3);
int MAPIO_dladdr(void* p1, Dl_info *p2);
void *MAPIO_dlvsym(void *p1, char *p2, char *p3);
#endif

#ifdef ATLAS_SOCKETS_LAYER
/*
 * Socket API prototypes
 */
int MAPIO_socket(int domain, int type, int protocol);

int MAPIO_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int MAPIO_listen(int s, int backlog);
int MAPIO_bind(int s, const struct sockaddr *name, socklen_t namelen);
int MAPIO_connect(int s, const struct sockaddr *name, socklen_t namelen);

int MAPIO_shutdown(int s, int how);

int MAPIO_getpeername(int s, struct sockaddr *name, socklen_t *namelen);
int MAPIO_getsockname(int s, struct sockaddr *name, socklen_t *namelen);

int MAPIO_poll(struct pollfd *fds, unsigned int nfds, int timo);
#ifdef _GNU_SOURCE
int MAPIO_ppoll(struct pollfd *fds, nfds_t nfds,
                const struct timespec *timeout_ts, const sigset_t *sigmask);
#endif
int MAPIO_select(int maxfdp1, fd_set *rfds, fd_set *wfds, fd_set *efds,
                 struct timeval *timeout);
int MAPIO_pselect(int maxfdp1, fd_set *rfds, fd_set *wfds, fd_set *efds,
                  struct timespec *timeout, const sigset_t *sigmask);

ssize_t MAPIO_send(int s, const void *data, size_t size, int flags);
void MAPIO_log(char *msg);
ssize_t MAPIO_sendmsg(int s, const struct msghdr *msg, int flags);
ssize_t MAPIO_sendto(int s, const void *data, size_t size, int flags,
                     const struct sockaddr *to, socklen_t tolen);

ssize_t MAPIO_recv(int s, void *mem, size_t len, int flags);
ssize_t MAPIO_recvmsg(int s, struct msghdr *msg, int flags);
ssize_t MAPIO_recvfrom(int s, void *mem, size_t len, int flags,
                       struct sockaddr *from, socklen_t *fromlen);

int MAPIO_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
int MAPIO_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);

void MAPIO_freeaddrinfo(struct addrinfo *ai);
int MAPIO_getaddrinfo(const char *nodename, const char *servname,
                      const struct addrinfo *hints, struct addrinfo **res);
struct hostent *MAPIO_gethostbyname(const char *name);
struct hostent *MAPIO_gethostbyname_cached(const char *name);
void MAPIO_flush_host_cached_entry(const char *name);
int MAPIO_set_external_cache_query_fp(struct hostent* (*gethostname_cached_fp)(const char* name));

int MAPIO_android_getaddrinfo(const char *nodename, const char *servname,
                              const struct addrinfo *hints, unsigned int netid,
                              unsigned int mark, struct addrinfo **res);

#if defined(__ANDROID__)
struct hostent	*MAPIO_gethostbyaddr(const char *, int, int);
int MAPIO_gethostbyname_r(const char *, struct hostent *, char *, size_t, struct hostent **, int *);
struct hostent	*MAPIO_gethostbyname2(const char *, int);
#endif
int MAPIO_getnameinfo(const struct sockaddr *, socklen_t, char *, size_t, char *, size_t, int);


#endif

#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop

#endif /* defined(__MAPIO_H_) */
