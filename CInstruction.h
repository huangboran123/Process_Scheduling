#pragma once
 enum InstructionSet {INOUT,OUTPUT,WAIT,HALT,CALC};
class CInstruction
{
	friend class CProcessSchedulingDlg;
	friend class PCB; //友元类即在类PCB中可以访问此类的private,protected属性
public:
	CInstruction() {

	}

	~CInstruction() {
	
	}

	//CInstruction(const CInstruction& c) {                //深拷贝
	//	
	//		m_pNextInstruction = new CInstruction();
	//		m_pNextInstruction = c.m_pNextInstruction;
	//		m_nRunTime = c.m_nRunTime;
	//	    m_nInstructionID = c.m_nInstructionID;
	//	
	//		
	//	
	//	
	//}
	

	//带参构造
	CInstruction(InstructionSet iid, int rt) {
		m_nInstructionID = iid;
		m_nRunTime = rt;
		if (iid == CALC) {
			remaind_Time = rt;
		}
		else {

			remaind_Time = 0;
		}
	}
	InstructionSet gettype() {
		return m_nInstructionID;
	}
	int gettime() {
		return m_nRunTime;
	}


private:
	CInstruction* m_pNextInstruction; //用于链接一个进程的所有指令成为链表（指令序列）
	int m_nRunTime; //本指令需要运行的时间长度（表示为定时器时间间隔的个数）
	InstructionSet m_nInstructionID; //指令类型标识
	int remaind_Time;  //当前CPU指令剩余时间

};

