#pragma once
#include <afxstr.h>
#include "CInstruction.h"
class PCB
{
	friend class CProcessSchedulingDlg;
	
public:
	//Ĭ�Ϲ���
	PCB() {
		m_nPID = 0;
		m_csProcessName = "";
		m_nRemainedTime = 0;
		m_pRuningInstruction = NULL;

		m_pInstructionList = NULL;
		m_pNextPCB = NULL;
		

	}
	//PCB(const PCB& r) {            //���(Ϊ�¶������¶�̬����ռ�)
	//	m_pNextPCB = new PCB();
	//	if (r.m_pRuningInstruction) {
	//		CInstruction RI(*r.m_pRuningInstruction);
	//		m_pRuningInstruction = &RI;
	//	}
	//	else {
	//		m_pRuningInstruction = NULL;
	//	}
	//	if(r.m_pInstructionList){
	//	CInstruction IL(*r.m_pInstructionList);
	//	m_pInstructionList = &IL;
	//	}
	//	else {
	//		m_pInstructionList = NULL;
	//	}

	//	m_pNextPCB = r.m_pNextPCB;
	//	m_nPID = r.m_nPID;
	//	m_csProcessName = r.m_csProcessName;
	//	m_nRemainedTime = r.m_nRemainedTime;


	//
	//}

	//�����̺źͽ����������Ĺ��캯��
	PCB(int pid, CString pname) {

		m_nPID = pid;
		m_csProcessName = pname;
		m_nRemainedTime = 0;//
		m_pRuningInstruction = NULL;

		m_pInstructionList = NULL;
		m_pNextPCB = NULL;

	}

	~PCB() {
		CInstruction* pTemp;
		while (m_pInstructionList)
		{
			pTemp = m_pInstructionList;

			m_pInstructionList = pTemp->m_pNextInstruction;
			
			delete pTemp;
			

		}

	}

	//���������һ��ָ��
	void AppendInstruction(CInstruction* pInstruction)
	{
		CInstruction* pTempInstruction;
		if (m_pInstructionList == NULL)
		{
			//empty
			m_pInstructionList = pInstruction;
		}
		else
		{
			//more than one node
			pTempInstruction = m_pInstructionList;
			while (pTempInstruction->m_pNextInstruction != NULL)
				pTempInstruction = pTempInstruction->m_pNextInstruction;
			pTempInstruction->m_pNextInstruction = pInstruction;
		}

	}

	void setNextInstruction() {
		m_pRuningInstruction = m_pRuningInstruction->m_pNextInstruction;
	}
	InstructionSet getCurrentInstype() {

		return m_pRuningInstruction->gettype();

	}
	int getpid() {
		return m_nPID;
	}
	CInstruction* getinstruct() {
		return  m_pRuningInstruction;
	}
	PCB* getNextPCB() {
		return m_pNextPCB;

	}

	void setNextPCB(PCB* pPCB) {
		m_pNextPCB = pPCB;
	
	}
	


private:
	PCB* m_pNextPCB;  //ָ����һ���̿��ƿ�
	int m_nPID;       //���̱�ʶ��ID
	CString m_csProcessName;   //������
	int m_nRemainedTime;       //��ǰ����ָ�����л���Ҫ��ʱ��
	CInstruction* m_pRuningInstruction;  //�������л�Ҫ���е�ָ��
	CInstruction* m_pInstructionList;   //ָ�򱾽��̵�ָ�����У����Ա��ĵ�һ


};


