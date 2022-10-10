/*
 ============================================================================
 Name        : queue.h
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#include "lib.h"

typedef struct LogQueueNode *lNode;
typedef struct LogQueueNode {

  int level;
  const char *file;
  const char *function;
  int line;
  char *message;
  char *time_log;
  lNode next;

}LogNode;

typedef struct{

	lNode head;
	lNode tail;

	bool stop_wait;

} LogQueue;


pthread_mutex_t mutex_queue;
pthread_mutexattr_t attr_queue;
pthread_cond_t cond_queue;

LogQueue* InitQueueLogger(LoggerOperationState*);
void DestroyQueueLogger(LogQueue*, LoggerOperationState*);
void DestroyQueueLoggerNode(lNode);
bool EmptyQueueLogger(LogQueue*, LoggerOperationState*);
void StopWaitLogger(LogQueue*, LoggerOperationState*);
void PushAndSignalQueueLogger(LogQueue*, int, const char *, const char *, int, char *, char *, LoggerOperationState *);
lNode PullAndWaitQueueLogger(LogQueue*, LoggerOperationState *);

#endif /* QUEUE_H_ */
