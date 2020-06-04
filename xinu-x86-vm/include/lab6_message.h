// Define a message state flag value.
typedef byte ms8;

// The different message states that a message can be in.
#define MSG_ERROR 0
#define MSG_VALID 1
#define MSG_FREE 2

// the size of the message table.
#define MSG_BUF_SIZE 500

// This structure holds information about the message.
// This structure is used to build the message table.
struct message
{
    ms8 msgState;            // the message state.
    umsg32 msg;              // the message.
    pid32 processId;         // the process id that this message is for.
    struct message *msgNext; // a pointer to the next message.
};

// just let everyone know that there is a message table and what it is called.
extern struct message msgtab[];

// function definitions.
syscall receiver(void);