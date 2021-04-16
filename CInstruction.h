#pragma once
 enum InstructionSet {INOUT,OUTPUT,WAIT,HALT,CALC};
class CInstruction
{
	friend class CProcessSchedulingDlg;
	friend class PCB; //��Ԫ�༴����PCB�п��Է��ʴ����private,protected����
public:
	CInstruction() {

	}

	~CInstruction() {
	
	}

	//CInstruction(const CInstruction& c) {                //���
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
	

	//���ι���
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
	CInstruction* m_pNextInstruction; //��������һ�����̵�����ָ���Ϊ����ָ�����У�
	int m_nRunTime; //��ָ����Ҫ���е�ʱ�䳤�ȣ���ʾΪ��ʱ��ʱ�����ĸ�����
	InstructionSet m_nInstructionID; //ָ�����ͱ�ʶ
	int remaind_Time;  //��ǰCPUָ��ʣ��ʱ��

};

