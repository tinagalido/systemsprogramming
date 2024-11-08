/* Compile with -lrt */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>     /* For 0_ flags */

typedef struct 
{
    char name[20];
    int count;
} Person;

int main()
{
    mqd_t messageQueueDescriptor;
    Person person;
    struct mq_attr attributes;
    int index;
    int status;
    long numMessages,numRead;
    unsigned priority;

    /* Step 1 : Open the message queue*/
    messageQueueDescriptor = mq_open(
        "/MyFirstMessageQueue",
        O_RDONLY
    );

    if (messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    /* Step 2: Read the message queue attributes*/
    status = mq_getattr(
        messageQueueDescriptor, 
        &attributes
    );
    if (status != 0)
    {
        printf("Failed to get attributes of message queue.\n");
        exit(EXIT_FAILURE);
    }

    printf("Message size: %ld\n", attributes.mq_msgsize);
    printf("Maximum number of messages: %ld\n", attributes.mq_maxmsg);
    printf("Current number of messages: %ld\n", attributes.mq_curmsgs);

    numMessages = attributes.mq_curmsgs;
    /* Step 3: Read the messages one at a time and print out*/
    for (index = 0; index < numMessages; index++)
    {
        numRead = mq_receive(
            messageQueueDescriptor,
            (char*)&person,
            sizeof(Person),
            &priority
        );
        if (numRead == -1)
        {
            printf("REading from message queue failed.\n");
            exit(EXIT_FAILURE);
        }

        printf("Name: %s; count:%d; priority: %d\n",person.name, person.count, priority);
    
    }
    
    /* Step 4: Close the message queue */
    status = mq_close(messageQueueDescriptor);
    if (status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}
