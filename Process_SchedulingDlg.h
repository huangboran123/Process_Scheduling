
// Process_SchedulingDlg.h: 头文件
//

#pragma once
#include "PCB.h"


// CProcessSchedulingDlg 对话框之定义的对话框类
class CProcessSchedulingDlg : public CDialogEx
{
// 构造
public:
	CProcessSchedulingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_SCHEDULING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持动态数据交换，负责控件与变量之间的关联


// 实现
protected:
	HICON m_hIcon; //应用程序句柄

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();   //重载初始化对话框
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam); //定义消息WM_SYSCOMMAND处理函数
	afx_msg void OnPaint();  //定义消息WM_PAINT处理函数
	afx_msg HCURSOR OnQueryDragIcon();  //定义消息ON_WM_QUERYDRAGICON处理函数
	DECLARE_MESSAGE_MAP()  //消息映射
public:
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnStartscheduling();
	afx_msg void OnBnClickedBtnStopscheduling();


	int m_nTimeSlice;//时间片大小（定时器时间间隔的倍数）
	int displayFlag;


	void LoadPCBs();//从文件中加载要试验的进程信息

	void RemoveProcess(PCB* pPCB);//删除进程
	void DoSchedule();


	void AppendReadyQueue(PCB* pPCB);//pPCB所指节点添加到就绪队尾

	void AppendBackupReadyQueue(PCB* pPCB);//pPCB所指节点添加到后备就绪队尾
	void AppendWaitQueue(PCB* pPCB);//pPCB所指节点添加到等待队尾
	void AppendInputQueue( PCB *pPCB);//pPCB所指节点添加到Input队尾
	void AppendOutputQueue(PCB* pPCB);//pPCB所指节点添加到Output队尾

};



UINT InputWaitThread(LPVOID pParam);  //输入等待队列线程函数

UINT OutputWaitThread(LPVOID pParam); //输出等待队列线程函数

UINT PureWaitThread(LPVOID pParam);  //其它等待队列线程函数

void delay(int n);  //延时函数