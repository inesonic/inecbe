/*-*-c++-*-*************************************************************************************************************
* Copyright 2016 - 2022 Inesonic, LLC.
* 
* This file is licensed under two licenses.
*
* Inesonic Commercial License, Version 1:
*   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
*   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
*   strictly prohibited.
*
* GNU Public License, Version 2:
*   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
*   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
*   version.
*   
*   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
*   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
*   details.
*   
*   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
*   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************************************************//**
* \file
*
* This header defines the \ref Cbe::JobQueue template class.
***********************************************************************************************************************/

/* .. sphinx-project inecbe */

#ifndef CBE_JOB_QUEUE_H
#define CBE_JOB_QUEUE_H

#include <QMutex>
#include <QSharedPointer>

#include <cstdint>

#include "cbe_common.h"

namespace Cbe {
    /**
     * Pure virtual template base class that can be used to construct compiler and linker job queues.  The calling
     * function is responsible for guarding against collisions due to concurrency.
     *
     * \param[in] T The class being used for job contexts.
     */
    template<typename T> class CBE_PUBLIC_API JobQueue {
        public:
            virtual ~JobQueue() {}

            /**
             * Method you can use to determine the number of pending jobs.
             *
             * \return Returns the current number of pending jobs.
             */
            virtual unsigned numberPendingJobs() const = 0;

            /**
             * Method you can use to enqueue a job.
             *
             * \param[in] newJob A shared pointer to the job to be enqueued.
             */
            virtual void enqueue(QSharedPointer<T> newJob) = 0;

            /**
             * Method you can use to enqueue a job.  The class will take ownership of the job.
             *
             * \param[in] newJob A shared pointer to the job to be enqueued.
             */
            inline void enqueue(T* newJob) {
                enqueue(QSharedPointer<T>(newJob));
            }

            /**
             * Method you can use to dequeue a job.
             *
             * \return Returns a shared pointer to the dequeued job.  The method will return a null pointer if the queue
             *         is empty.
             */
            virtual QSharedPointer<T> dequeue() = 0;
    };

    /**
     * Template class that instantiates a simple, 1 deep, job queue.  Adding a new job will cause it to replace the old
     * and pending job.
     *
     * \param[in] T The class being used for job contexts.
     */
    template<typename T> class CBE_PUBLIC_API SimpleJobQueue:public JobQueue<T> {
        public:
            /**
             * Method you can use to determine the number of pending jobs.
             *
             * \return Returns the current number of pending jobs.
             */
            unsigned numberPendingJobs() const final {
                return pendingJob.isNull() ? 0 : 1;
            }

            /**
             * Method you can use to enqueue a job.
             *
             * \param[in] newJob A shared pointer to the job to be enqueued.
             */
            void enqueue(QSharedPointer<T> newJob) final {
                pendingJob = newJob;
            }

            /**
             * Method you can use to dequeue a job.  The method will block until a job is available.
             *
             * \return Returns a shared pointer to the dequeued job.
             */
            QSharedPointer<T> dequeue() final {
                QSharedPointer<T> job;

                job = pendingJob;
                pendingJob.clear();

                return job;
            }

        private:
            /**
             * Shared pointer representing our 1-deep queue.
             */
            QSharedPointer<T> pendingJob;
    };
};

#endif
