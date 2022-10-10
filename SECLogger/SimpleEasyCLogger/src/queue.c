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

LogQueue *InitQueueLogger(LoggerOperationState *loggerOperationState)
{

    LogQueue *tempQueue = ( LogQueue * )malloc(sizeof(LogQueue));
    if (tempQueue == NULL)
    {
        *loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
        return NULL;
    }

    tempQueue->head = NULL;
    tempQueue->tail = NULL;

    tempQueue->stop_wait = false;

    pthread_mutexattr_init(&(tempQueue->attr_queue));
    pthread_mutexattr_settype(&(tempQueue->attr_queue), PTHREAD_MUTEX_RECURSIVE);

    if (pthread_mutex_init(&(tempQueue->mutex_queue), &(tempQueue->attr_queue)) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED;
        free(tempQueue);
        return NULL;
    }
    if (pthread_cond_init(&(tempQueue->cond_queue), NULL) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_MUTEX_INITIALIZATION_FAILED;
        free(tempQueue);
        return NULL;
    }

    *loggerOperationState = LOGGER_OK_SUCCESS;
    return tempQueue;
}

void DestroyQueueLogger(LogQueue *queue, LoggerOperationState *loggerOperationState)
{
    if (queue == NULL)
    {
        *loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
        return;
    }

    if (pthread_mutex_lock(&(queue->mutex_queue)) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
        return;
    }

    lNode next    = NULL;
    lNode current = queue->head;

    if (!EmptyQueueLogger(queue, loggerOperationState))
    {
        do
        {
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

void DestroyQueueLoggerNode(lNode elemQueue)
{
    free(elemQueue->time_log);
    free(elemQueue->message);

    free(elemQueue);
}

void StopWaitLogger(LogQueue *queue, LoggerOperationState *loggerOperationState)
{
    if (pthread_mutex_lock(&(queue->mutex_queue)) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
        return;
    }
    queue->stop_wait = true;
    pthread_cond_signal(&(queue->cond_queue));

    *loggerOperationState = LOGGER_OK_SUCCESS;
    pthread_mutex_unlock(&(queue->mutex_queue));
}

bool EmptyQueueLogger(LogQueue *queue, LoggerOperationState *loggerOperationState)
{
    bool result = false;

    if (queue == NULL)
    {
        *loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
        return result;
    }

    if (pthread_mutex_lock(&(queue->mutex_queue)) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
        return result;
    }

    if (queue->head == NULL || queue->tail == NULL)
        result = true;

    *loggerOperationState = LOGGER_OK_SUCCESS;
    pthread_mutex_unlock(&(queue->mutex_queue));

    return result;
}

void PushAndSignalQueueLogger(LogQueue *queue, int level, const char *file, const char *function, int line, char *timeLog, char *message, LoggerOperationState *loggerOperationState)
{
    if (queue == NULL)
    {
        free(timeLog);
        free(message);
        *loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
        return;
    }

    if (pthread_mutex_lock(&(queue->mutex_queue)) != 0)
    {
        free(timeLog);
        free(message);
        *loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
        return;
    }

    lNode elemQueue = ( lNode )malloc(sizeof(LogNode));
    if (elemQueue == NULL)
    {
        free(timeLog);
        free(message);
        *loggerOperationState = LOGGER_ERROR_ALLOCATION_FAILED;
        pthread_mutex_unlock(&(queue->mutex_queue));
        return;
    }

    elemQueue->next = NULL;
    if (queue->tail)
        queue->tail->next = elemQueue;
    queue->tail = elemQueue;
    if (!queue->head)
        queue->head = elemQueue;

    elemQueue->level    = level;
    elemQueue->file     = file;
    elemQueue->function = function;
    elemQueue->line     = line;
    elemQueue->time_log = timeLog;
    elemQueue->message  = message;

    pthread_cond_signal(&(queue->cond_queue));

    *loggerOperationState = LOGGER_OK_SUCCESS;
    pthread_mutex_unlock(&(queue->mutex_queue));
}

lNode PullAndWaitQueueLogger(LogQueue *queue, LoggerOperationState *loggerOperationState)
{
    if (queue == NULL)
    {
        *loggerOperationState = LOGGER_ERROR_QUEUE_NULL_POINTER;
        return NULL;
    }

    if (pthread_mutex_lock(&(queue->mutex_queue)) != 0)
    {
        *loggerOperationState = LOGGER_ERROR_ACQUISITION_LOCK_FAILED;
        return NULL;
    }

    while (EmptyQueueLogger(queue, loggerOperationState) && !(queue->stop_wait))
    {
        pthread_cond_wait(&(queue->cond_queue), &(queue->mutex_queue));
    }

    if (queue->stop_wait)
    {
        pthread_mutex_unlock(&(queue->mutex_queue));
        return NULL;
    }

    lNode elemQueue = queue->head;
    if (queue->head)
        queue->head = queue->head->next;
    if (!queue->head)
        queue->tail = NULL;

    *loggerOperationState = LOGGER_OK_SUCCESS;

    pthread_mutex_unlock(&(queue->mutex_queue));
    return elemQueue;
}
