#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct score
{
	int num;
	int English;
	int math;
	struct list_head list;          //����������
};

int list_init()
{
    struct list_head score_head;        //���������������ͷ

    //���������ڵ� Ȼ����뵽������
    struct score stu1, stu2, stu3;
    struct list_head *pos;              //����һ�����ָ��
    struct score *tmp;                  //����һ��score�ṹ�����

	INIT_LIST_HEAD(&score_head);    //��ʼ������ͷ ���һ��˫��ѭ������Ĵ���

	stu1.num = 1;
	stu1.English = 59;
	stu1.math = 99;

	//Ȼ�������ڵ���뵽������
	list_add_tail(&(stu1.list), &score_head);//ʹ��β�巨

	stu2.num = 2;
	stu2.English = 69;
	stu2.math = 98;
	list_add_tail(&(stu2.list), &score_head);

	stu3.num = 3;
	stu3.English = 89;
	stu3.math = 97;
	list_add_tail(&(stu3.list), &score_head);

	//������������ÿ�α��������ݴ�ӡ����
	list_for_each(pos, &score_head) //�����pos���Զ�������ֵ
	{
		tmp = list_entry(pos, struct score, list);
		printf("num: %d, English: %d, math: %d\n", tmp->num, tmp->English, tmp->math);
	}

	return 0;
}

void list_exit()
{
	//�˳�ʱɾ�����
	list_del(&(stu1.list));
	list_del(&(stu2.list));
	list_del(&(stu3.list));
	printf("mylist exit!\n");
}

int main()
{
    list_init();
    list_exit();

    return 0;
}
