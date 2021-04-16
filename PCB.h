#pragma once
#include <afxstr.h>
#include "CInstruction.h"
class PCB
{
	friend class CProcessSchedulingDlg;
	
public:
	//默认构造
	PCB() {
		m_nPID = 0;
		m_csProcessName = "";
		m_nRemainedTime = 0;
		m_pRuningInstruction = NULL;

		m_pInstructionList = NULL;
		m_pNextPCB = NULL;
		

	}
	//PCB(const PCB& r) {            //深拷贝(为新对象重新动态分配空间)
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

	//带进程号和进程名参数的构造函数
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

	//本进程添加一条指令
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
	PCB* m_pNextPCB;  //指向下一进程控制块
	int m_nPID;       //进程标识符ID
	CString m_csProcessName;   //进程名
	int m_nRemainedTime;       //当前运行指令运行还需要的时间
	CInstruction* m_pRuningInstruction;  //正在运行或将要运行的指令
	CInstruction* m_pInstructionList;   //指向本进程的指令序列（线性表）的第一


};


