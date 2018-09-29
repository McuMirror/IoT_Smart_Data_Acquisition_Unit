#include "linked_list.h"
#include "malloc.h"

#ifndef TURE
#define TURE 1
#endif


#ifndef FAULSE 
#define FAULSE 0
#endif




//��������ڵ�
Linked_List *CreateNode(void)
{ 
    Linked_List *p = (Linked_List *)mymalloc(sizeof(Linked_List));
    if (NULL == p)
    {
        return NULL;
    }
    memset(p, 0, sizeof(Linked_List));
    p->pNext = NULL;
    return p;
}

//��ȡ����ڵ��ַ
Linked_List *GetNode(Linked_List *pHeader, uint16_t num)
{
    uint8_t i;
    Linked_List *p = pHeader;
    if (NULL == p->pNext)
    {
        return FAULSE;
    }
    for (i = 0; i < num; i++)
    {
        if (p->pNext != NULL) //�ڵ��λ�ò��ô���������
				{
					p=p->pNext;
				}
				else{
					return FAULSE;
				}
		}
		return p;
}

//����ڵ�
//�β� pHeader:����ͷ ?num = 0:ͷ TAIL:β�� !0&&!TAIL : �����м�
//����ֵ TURE:�ɹ� FAULSE:ʧ��
uint8_t InsertNode(Linked_List * pHeader, uint16_t num)
{
		uint8_t i;
		Linked_List *p = pHeader;
		Linked_List *p1 = (Linked_List *)mymalloc(sizeof(Linked_List));
		if (NULL == p1)
		{
				return FAULSE;
		}
		if (0 == num) //��ͷ�����ӽڵ�
		{
			if(NULL == pHeader)
			{
				pHeader = p1;
			}
			else{
				pHeader->pNext = p1;
			}
		}
		else if (TAIL == num)//ĩβ���ӽڵ�
		{
			while(p->pNext != NULL)
			{
				p=p->pNext;
			}
			p->pNext = p1;
		}
		else if(num > 0 && num != TAIL)//�������м����ӽڵ�
		{
			for(i=0;i<num-1;i++)
			{
				if(NULL != p->pNext)//���ӽڵ��λ�ò��ô���������
				{
					p=p->pNext;
				}
				else
				{
					return FAULSE;
				}
			}
			p1->pNext = p->pNext;
			p->pNext = p1;
		}
		return true;
}

//ɾ������ڵ�
//�β� pHeader:����ͷ ?num = 0:ͷ TAIL:β�� !0&&!TAIL : �����м䣬ע��num����Ϊ0
//����ֵ TURE:�ɹ� FAULSE:ʧ��
uint8_t DeleNode(Linked_List * pHeader, uint16_t num)
{
		uint8_t i;
		Linked_List *p = pHeader, *p1, *p2;
		if (NULL == p) //������������
			{
				return FAULSE;
			}
		 if(0 == num)//��ͷ��ɾ���ڵ�
		 {
				p1 = pHeader;
				pHeader = pHeader->pNext;
				myfree(p1);
		 }
		 if (TAIL == num)//ĩβɾ���ڵ�
		 {
				while(p->pNext != NULL)
				{
					p1=p;//��ȡ�µ�β��
					p=p->pNext;//ָ����һ���ڵ�
				}
			p1->pNext = NULL;
			myfree(p);
		}
		else if(num > 0 && num != TAIL)//�������м�ɾ���ڵ�
		{
			for(i=0;i<num-1;i++)
			{
				if(NULL != p->pNext)//���ӽڵ��λ�ò��ô���������
				{
					p=p->pNext;
				}
				else{
					return FAULSE;
				}
			}
			p1 = p->pNext;
			p2 = p1->pNext;
			p->pNext = p2;
			myfree(p1);
		}
		return TURE;
}

//��ȡ������
uint16_t GetNodeNum(Linked_List * pHeader)
{
		uint16_t ret, i;
		Linked_List *p = pHeader;
		if (NULL == p) //������
		{
			return FAULSE;
		}
		for(i=1;p->pNext != NULL;i++)
		{
			p = p->pNext;
		}
		ret = i;
		return ret;
}
