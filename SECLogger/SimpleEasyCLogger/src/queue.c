/*
 ============================================================================
 Name        : queue.c
 Author      : Emanuele Cennamo
 Version     :
 Copyright   : apache 2.0 license
 Description : Logger library
 ============================================================================
 */


#include "queue.h"

LogQueue* InitQueueLogger(LoggerOperationState *loggerOperationState) {

	LogQueue* tempQueue = (LogQueue*)malloc(sizeof(LogQueue));
	if(tempQueue == NULL){
		*loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
		return NULL;
	}

	tempQueue->head=NULL;
	tempQueue->tail=NULL;

	tempQueue->stop_wait = false;

	pthread_mutexattr_init(&(tempQueue->attr_queue));
	pthread_mutexattr_settype(&(tempQueue->attr_queue), PTHREAD_MUTEX_RECURSIVE);

	if(pthread_mutex_init(&(tempQueue->mutex_queue), &(tempQueue->attr_queue)) != 0){
		*loggerOperationState = LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED;
		free(tempQueue);
		return NULL;
	}
	if(pthread_cond_init(&(tempQueue->cond_queue), NULL) != 0){
		*loggerOperationState = LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED;
		free(tempQueue);
		return NULL;
	}

	*loggerOperationState = LOGGER_OK_SUCCESS;
	return tempQueue;
}

void DestroyQueueLogger(LogQueue* queue, LoggerOperationState *loggerOperationState) {
	if (queue == NULL) {
		*loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
		return;
	}

	if(pthread_mutex_lock(&(queue->mutex_queue)) != 0) {
		*loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
		return;
	}

	lNode next;
	lNode current = queue->head;

	if(!EmptyQueueLogger(queue, loggerOperationState)) {
		do {
			next = current->next;
			DestroyQueueLoggerNode(current);
			current = next;
		} while (current != NULL);
	}

	*loggerOperationState = LOGGER_OK_SUCCESS;
	pthread_mutex_unlock(&(queue->mutex_queue));
	pthread_mutex_destroy(&(queue->mutex_queue));
	free(queue);

}

void DestroyQueueLoggerNode(lNode ElemQueue) {
	free(ElemQueue->time_log);
	free(ElemQueue->message);

	free(ElemQueue);
}

void StopWaitLogger(LogQueue* queue, LoggerOperationState *loggerOperationState) {
	if (pthread_mutex_lock(&(queue->mutex_queue)) != 0) {
		*loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
		return;
	}
	queue->stop_wait = true;
	pthread_cond_signal(&(queue->cond_queue));

	*loggerOperationState = LOGGER_OK_SUCCESS;
	pthread_mutex_unlock(&(queue->mutex_queue));
}

bool EmptyQueueLogger(LogQueue* queue, LoggerOperationState *loggerOperationState) {
	bool result = false;

	if (queue == NULL) {
		*loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
		return result;
	}

	if (pthread_mutex_lock(&(queue->mutex_queue)) != 0) {
		*loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
		return result;
	}

	if(queue->head == NULL || queue->tail == NULL)
		result = true;

	*loggerOperationState = LOGGER_OK_SUCCESS;
	pthread_mutex_unlock(&(queue->mutex_queue));

 	return result;
}

void PushAndSignalQueueLogger(LogQueue* queue, int level, const char *file, const char *function, int line, char* time_log, char *message, LoggerOperationState *loggerOperationState) {
	if(queue == NULL){
		free(time_log);
		free(message);
		*loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
		return;
	}

	if (pthread_mutex_lock(&(queue->mutex_queue)) != 0) {
		free(time_log);
		free(message);
		*loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
		return ;
	}

	lNode ElemQueue = (lNode)malloc(sizeof(LogNode));
	if(ElemQueue == NULL){
		free(time_log);
		free(message);
		*loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
		pthread_mutex_unlock(&(queue->mutex_queue));
		return;
	}

	ElemQueue->next=NULL;
	if (queue->tail)
		queue->tail->next=ElemQueue;
	queue->tail=ElemQueue;
	if(!queue->head)
		queue->head=ElemQueue;

	ElemQueue->level = level;
	ElemQueue->file = file;
	ElemQueue->function = function;
	ElemQueue->line = line;
	ElemQueue->time_log = time_log;
	ElemQueue->message = message;

	pthread_cond_signal(&(queue->cond_queue));

	*loggerOperationState = LOGGER_OK_SUCCESS;
	pthread_mutex_unlock(&(queue->mutex_queue));
}

lNode PullAndWaitQueueLogger(LogQueue* queue, LoggerOperationState *loggerOperationState) {
	if (queue == NULL) {
		*loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
		return NULL;
	}

	if (pthread_mutex_lock(&(queue->mutex_queue)) != 0) {
		*loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
		return NULL;
	}

	while(EmptyQueueLogger(queue, loggerOperationState) && !(queue->stop_wait)) {
		pthread_cond_wait(&(queue->cond_queue),&(queue->mutex_queue));
	}

	if(queue->stop_wait) {
		pthread_mutex_unlock(&(queue->mutex_queue));
		return NULL;
	}

	lNode ElemQueue = queue->head;
	if (queue->head)
		queue->head = queue->head->next;
	if (!queue->head)
		queue->tail = NULL;
	*loggerOperationState = LOGGER_OK_SUCCESS;

	pthread_mutex_unlock(&(queue->mutex_queue));
	return ElemQueue;
}

