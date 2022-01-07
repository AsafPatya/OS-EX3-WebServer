#ifndef __REQUEST_H__
#include "WorkerThread.h"

void requestHandle(int fd, WorkerThread* workerThread, struct timeval arrival_time, struct timeval dispatch_interval);

#endif
