#include <iostream>

namespace TaskSpace
{
    class TaskBase
    {
    private:
        /* data */
    public:
        TaskBase() = default;
        ~TaskBase() = default;

        virtual void PreTask();
        virtual void DoTask();
        virtual void DoneTask();

        static TaskBase* CreateTask();
    };



    class Task
    {
    private:
        TaskBase* m_pTask;
    public:
        Task();
        ~Task();
    };
}


