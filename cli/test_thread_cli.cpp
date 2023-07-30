#include <pthread.h>
#include <string.h>

#include <iostream>

void* threadEvent(void* data)
{
    int* sharedData = reinterpret_cast<int*>(data);

    std::cout << "Thread begin." << std::endl;

    *sharedData = 1;

    std::cout << "Thread end." << std::endl;

    return NULL;
}

int main(void)
{
    pthread_t tid;

    int sharedData = -1;

    int err = pthread_create(&tid, NULL, &threadEvent, &sharedData);

    if (err != 0)
    {
        std::cout << "Failed to create thread." << strerror(err);
        return 1;
    }

    pthread_join(tid, NULL);

    std::cout << "Shared memory = " << sharedData << std::endl;

    return 0;
}