typedef byte ms8;

#define MSG_ERROR 0;
#define MSG_VALID 1;
#define MSG_FREE 2;

#define MSG_BUF_SIZE 100;

struct message
{
    ms8 msgState;
    umsg32 msg;
    pid32 processId;
    struct message *msgNext;
};

extern struct message msgtab[];