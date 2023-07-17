#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int ret = -1;
  int msg_flags, smsg_id,
      rmsg_id; /*创建消息队列函数所用的标志位，以及收消息与发消息的队列id*/
  key_t key1, key2; /*队列的键值*/

  struct msgmbuf { /*消息的缓冲区结构*/
    long mtype;    // 注意long与int的字长问题
    char mtext[10];
  };
  struct msgmbuf msg_mbuf; /*创建消息缓冲区*/

  int msg_sflags, msg_rflags;        /*收发消息函数所用的标志位*/
  const char* msgpath1 = "/usr/bin"; /*消息key1产生所用的路径*/
  const char* msgpath2 = "/usr/bin"; /*消息key2产生所用的路径*/
  key1 = ftok(msgpath1, 'b');        /*产生key1*/
  key2 = ftok(msgpath2, 'a');        /*产生key2*/
  if (key1 != -1 || key2 != -1)      /*产生key成功*/
  {
    printf("成功建立KEY\n");
  } else /*产生key失败*/
  {
    printf("建立KEY失败\n");
  }
  msg_flags = IPC_CREAT;  //|IPC_EXCL;		/*设置创建消息的标志位*/
  smsg_id = msgget(key1, msg_flags | 0666); /*建立收消息的消息队列*/
  rmsg_id = msgget(key2, msg_flags | 0666); /*建立发消息的消息队列*/
  if (-1 == smsg_id || -1 == rmsg_id) {
    printf("消息建立失败\n");
    return 0;
  }

  pid_t pid;
  pid = fork(); /*通过fork()创建子进程，主进程进行发消息，子进程进行收消息*/

  while (1) {
    if (pid != 0) {            /*主进程*/
      msg_sflags = IPC_NOWAIT; /*当消息队列满了的时候不等待*/
      msg_mbuf.mtype = 10;     /*设置发送的消息类型*/
      sleep(1);
      char* content;
      content = (char*)malloc(10 * sizeof(char));
      printf("input:\n");
      scanf("%s", content);                /*用户输入内容*/
      if (strncmp(content, "end", 3) == 0) /*如果前三个字符为end，则跳出循环*/
        break;

      memcpy(msg_mbuf.mtext, content, 10);              /*复制字符串*/
      ret = msgsnd(smsg_id, &msg_mbuf, 10, msg_sflags); /*发送消息*/
      if (-1 == ret) {
        printf("发送消息失败\n");
      }
    } else { /*子进程*/
      sleep(1);
      msg_mbuf.mtype = 10;      /*设置收消息的类型*/
      msg_rflags = IPC_NOWAIT;  //|MSG_NOERROR;
      ret = msgrcv(rmsg_id, &msg_mbuf, 10, 10, msg_rflags); /*接收消息*/
      if (-1 == ret) {
        // printf("接收消息错误: %d, %s\n", errno, strerror(errno));
        /*可添加出错处理等*/
      } else {
        printf("接收消息成功，长度：%d\n", ret);
        printf("content:%s\n", msg_mbuf.mtext);
      }
    }
  }

  ret = msgctl(rmsg_id, IPC_RMID, NULL); /*删除收消息的队列*/
  if (-1 == ret) {
    printf("删除消息失败\n");
    return 0;
  }
  return 0;
}
