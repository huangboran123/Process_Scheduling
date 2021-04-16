
// Process_SchedulingDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Process_Scheduling.h"
#include "Process_SchedulingDlg.h"
#include "afxdialogex.h"
#include <thread>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PCB* m_pReadyPCBs; //就绪队列
PCB* m_pBackupReadyPCBs; //后备就绪队列
PCB* m_pInputWaittingPCBs;//输入等待队列  
PCB* m_pOutputWaittingPCBs;//输出等待队列
PCB* m_pPureWaittingPCBs;//其他等待队列







// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProcessSchedulingDlg 对话框



CProcessSchedulingDlg::CProcessSchedulingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_SCHEDULING_DIALOG, pParent)
	, m_nTimeSlice(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessSchedulingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TimeSlice, m_nTimeSlice);
}

BEGIN_MESSAGE_MAP(CProcessSchedulingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OpenFile, &CProcessSchedulingDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_StartScheduling, &CProcessSchedulingDlg::OnBnClickedBtnStartscheduling)
	ON_BN_CLICKED(IDC_BTN_StopScheduling, &CProcessSchedulingDlg::OnBnClickedBtnStopscheduling)

END_MESSAGE_MAP()


// CProcessSchedulingDlg 消息处理程序

BOOL CProcessSchedulingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProcessSchedulingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessSchedulingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProcessSchedulingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开虚拟程序文件pc.txt
void CProcessSchedulingDlg::OnBnClickedBtnOpenfile()
{
	LoadPCBs();  //将文件进程信息载入内存并在就绪队列显示



	// TODO: 在此添加控件通知处理程序代码
}


//开始调度
void CProcessSchedulingDlg::OnBnClickedBtnStartscheduling()
{
	DoSchedule();    //开始调度
	// TODO: 在此添加控件通知处理程序代码
}

//暂停调度
void CProcessSchedulingDlg::OnBnClickedBtnStopscheduling()
{

	displayFlag = 1;
	// TODO: 在此添加控件通知处理程序代码
}

//从文件中加载要试验的进程信息
void CProcessSchedulingDlg::LoadPCBs()
{
	int index;     //字符索引号
	CString strText;  //一行字符串
	CString first;  //行首字符
	CString right;  //行首全右字符串
	CString strPathName;
	CListBox* pReadyListBox = (CListBox*)GetDlgItem(IDC_LIST_ReadyQueue);  //关联要显示文本的组件
	pReadyListBox->ResetContent();
	//创建文件对话框
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, _T("文本文件(*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)  //按下确定键后
	{
		CStdioFile file;
		//获得文件路径
		strPathName = dlg.GetPathName();
		//打开文件
		if (!file.Open(strPathName, CFile::modeRead))  //文件绝对地址和打开方式
		{
			AfxMessageBox(_T("文件打开失败。"));
			return;
		}
		//读文件
		strText = _T("");
		while (file.ReadString(strText)) {          //读取一行文本到缓冲区
			first = CString(strText.GetAt(0));       //获取行第一个字符
			if (!first.Compare(_T("P"))) {            //若是进程
				index = strText.Find('P') + 1;
				right = strText.Right(strText.GetLength() - index);  //获取进程号
				int pid= _ttoi(right);  //CString到int类型转换
				//pListBox->AddString(right);  //测试用
				PCB *pcb = new PCB(pid, strText);   //创建进程控制快(指令序列同理),使用new可以释放原来的地址空间，避免了实参和形参指向同意地址空间的问题
				(*pcb).m_pNextPCB = NULL;
				
				int instrucflag = 1;   //判断一个进程的指令是否读完
				while ((instrucflag == 1)&&file.ReadString(strText)) {     //读取指令
					CString Array=_T("ICOWH");         //解决分支语句条件不能使用CString指令分别表示ICOH
					first = CString(strText.GetAt(0));
					index = strText.Find(first) + 1;
					right = strText.Right(strText.GetLength() - index);  //获取时间
					int time = _ttoi(right);  //CString到int类型转换
					int instruction = Array.Find(first);
					
					switch (instruction) {                       //控制传输跳过实例化在case下加上{}
					case 0: {
						
						CInstruction *input =new CInstruction(INOUT, time);
						(*input).m_pNextInstruction = NULL;
						(*pcb).AppendInstruction(input);   ///给PCB添加输入指令

						break;
					}
					case 1: {
						
						CInstruction *calc = new CInstruction(CALC, time);
						(*calc).m_pNextInstruction = NULL;
					
						(*pcb).AppendInstruction(calc);   ///给PCB添加计算指令

						break; 
					}
					case 2: {
						CInstruction *output =new CInstruction(OUTPUT, time);
						(*output).m_pNextInstruction = NULL;
						(*pcb).AppendInstruction(output);   ///给PCB输出计算指令
						break;
					}
					case 3: {
						CInstruction *wait = new CInstruction(WAIT, time);
						(*wait).m_pNextInstruction = NULL;
						(*pcb).AppendInstruction(wait);   ///给PCB添加等待指令
						break;
					}
						
					case 4: {
						CInstruction *halt =new CInstruction(HALT, time);
						(*halt).m_pNextInstruction = NULL;
						(*pcb).AppendInstruction(halt);   ///给PCB添加结束指令
						instrucflag = 0;
						AppendReadyQueue(pcb);      //将一个进程写入就绪PCB队列
					
						break;
						
					}
					
					
					
					}

				
				}
				

			}


			//pListBox->AddString(strText);  //插入一行数据


		}

		//关闭文件
		file.Close();
	}
	PCB* TEMP = m_pReadyPCBs;
	while (TEMP) {                                   //显示就绪列表
		pReadyListBox->AddString(TEMP->m_csProcessName);
		TEMP = TEMP->m_pNextPCB;
	
	}


}


//从某队列中删除进程
void CProcessSchedulingDlg::RemoveProcess(PCB* pPCB)
{
	/*delete pPCB;*/
}


//开始调度
void CProcessSchedulingDlg::DoSchedule()
{

	CListBox* pCurrentListBox = (CListBox*)GetDlgItem(IDC_LIST_CurrentProcess);  //显示当前运行
	CListBox* pBackupListBox = (CListBox*)GetDlgItem(IDC_LIST_BackupReadyQueue);  //显示就绪进程
	CListBox* pInputListBox = (CListBox*)GetDlgItem(IDC_LIST_InputWaitQueue);  //显示输入等待进程
	CListBox* pOutputListBox = (CListBox*)GetDlgItem(IDC_LIST_OutputWaitQueue);  //显示输出等待进程
	CListBox* pPureWaitListBox = (CListBox*)GetDlgItem(IDC_LIST_OtherWaitQueue);  //显示其它等待进程
	UpdateData();         //更新编辑框内容

	CInstruction* pRunningInstruction;
	InstructionSet TypeOfInstrction;    //定义指令类型   
	PCB *pCurrentPCB;
	displayFlag = 0;

	m_pBackupReadyPCBs =m_pReadyPCBs;
	   
	while (m_pBackupReadyPCBs || m_pInputWaittingPCBs || m_pOutputWaittingPCBs || m_pPureWaittingPCBs) {
		if (m_pBackupReadyPCBs) {   //就绪队列存在就可取出首进程
		
			pCurrentPCB = new PCB(*m_pBackupReadyPCBs);  //注意变量的生存周期相当于每次都创建一个新的地址空间
		
		   
			(*pCurrentPCB).m_pNextPCB = NULL;  //取单个进程
			int timeslice = m_nTimeSlice;    //局部时间片

			m_pBackupReadyPCBs = m_pBackupReadyPCBs->m_pNextPCB;  //从就绪列表中删除当前进程

			pBackupListBox->ResetContent();                   //后备就绪显示
			PCB* temp = m_pBackupReadyPCBs;
			while (temp) {
				pBackupListBox->AddString(temp->m_csProcessName);
				temp = temp->m_pNextPCB;
			}
			                                                  //当前运行显示
			pCurrentListBox->ResetContent();
			pCurrentListBox->AddString((pCurrentPCB)->m_csProcessName);

			                                                   //输入等待显示
			pInputListBox->ResetContent();
			PCB* temp2 = m_pInputWaittingPCBs;
			while (temp2) {
				pInputListBox->AddString(temp2->m_csProcessName);
				temp2 = temp2->m_pNextPCB;
			}

			                                                   //输出等待显示
			pOutputListBox->ResetContent();                
			PCB* temp3 = m_pOutputWaittingPCBs;
			while (temp) {
				pOutputListBox->AddString(temp->m_csProcessName);
				temp = temp->m_pNextPCB;
			}


			pPureWaitListBox->ResetContent();                //其它等待显示
			PCB* temp4 = m_pPureWaittingPCBs;
			while (temp) {
				pPureWaitListBox->AddString(temp->m_csProcessName);
				temp = temp->m_pNextPCB;
			}

		
			delay(3000);
			while (displayFlag) {};

			if (!(*pCurrentPCB).m_pRuningInstruction) {        //判断是否存在运行指令,否在写入第一条指令
				(*pCurrentPCB).m_pRuningInstruction = (*pCurrentPCB).m_pInstructionList;
			}

			int InstructFlag = 1;   //指令为C以外其他类型的都加入相应队列

			while ((InstructFlag == 1) && (*pCurrentPCB).m_pRuningInstruction) {   //判断此进程是否存在运行指令和是否已经加入等待队列
				pRunningInstruction = (*pCurrentPCB).m_pRuningInstruction;
				TypeOfInstrction = pRunningInstruction->m_nInstructionID;
				switch (TypeOfInstrction) {
				case HALT:

					RemoveProcess(pCurrentPCB);
					InstructFlag = 0;  //退出本PCB的指令循环
					break;
				case INOUT: {

					//加入输入等待队列
					AppendInputQueue(pCurrentPCB);
					//thread inout(&CProcessSchedulingDlg:: TreadWaittingQueue,&pCurrentPCB);  //将类的成员函数作为线程函数
					//inout.detach();

					AfxBeginThread(InputWaitThread,pCurrentPCB,0,0,0,NULL);           //输入线程处理
					InstructFlag = 0;  //退出本PCB的指令循环
					break;
				}
				case OUTPUT:
					AppendOutputQueue(pCurrentPCB);
					AfxBeginThread(OutputWaitThread, pCurrentPCB, 0, 0, 0, NULL);       //输出线程处理
					InstructFlag = 0;
					break;

				case CALC:
					if (pRunningInstruction->m_nRunTime == pRunningInstruction->remaind_Time) {    //第一次运行c指令
						if (timeslice >= pRunningInstruction->m_nRunTime) {
							Sleep(pRunningInstruction->m_nRunTime);     //模拟CPU执行一段时间
							timeslice = timeslice - pRunningInstruction->m_nRunTime;  //剩余时间片
							pRunningInstruction->remaind_Time = 0;
						}
						else {
							Sleep(timeslice);
							pRunningInstruction->remaind_Time = pRunningInstruction->remaind_Time - timeslice;
							timeslice = 0;
						}
					}
					else {                                   //非首次运行c
					
						if (timeslice >= pRunningInstruction->remaind_Time) {
							Sleep(pRunningInstruction->remaind_Time);     //模拟CPU执行一段时间
							timeslice = timeslice - pRunningInstruction->remaind_Time;  //剩余时间片
							pRunningInstruction->remaind_Time = 0;
						}
						else {
							Sleep(timeslice);
							pRunningInstruction->remaind_Time = pRunningInstruction->remaind_Time - timeslice;
							timeslice = 0;
						}
					
					}
					if (timeslice == 0) {         //时间片耗尽进入等待队列
						AppendWaitQueue(pCurrentPCB);
						AfxBeginThread(PureWaitThread, pCurrentPCB, 0, 0, 0, NULL);        //其他等待线程处理
						InstructFlag = 0;
						break;
					
					}
					else {
						pCurrentPCB->m_pRuningInstruction = pRunningInstruction->m_pNextInstruction;   //下一条指令
						break;
					}

				case WAIT:
					AppendWaitQueue(pCurrentPCB);
					AfxBeginThread(PureWaitThread, pCurrentPCB, 0, 0, 0, NULL);        //其他等待线程处理
					InstructFlag = 0;
					break;

			    }

		    }
		
		}

		
	}
	
}









//添加进程到PCB列表尾部
void CProcessSchedulingDlg::AppendReadyQueue(PCB* pPCB)
{
	PCB* pTempPCB;
	if (m_pReadyPCBs == NULL)
	{
		//empty
		m_pReadyPCBs = pPCB;
	}
	else
	{
		//more than one node
		pTempPCB = m_pReadyPCBs;
		while (pTempPCB->m_pNextPCB != NULL)   //插入尾
			pTempPCB = pTempPCB->m_pNextPCB;
		pTempPCB->m_pNextPCB = pPCB;
	}

}

void CProcessSchedulingDlg::AppendBackupReadyQueue(PCB* pPCB)
{
}

//执行到wait指令的进程加入队尾（链表尾)
void CProcessSchedulingDlg::AppendWaitQueue(PCB* pPCB)
{
	PCB* pTempPCB;
	if (!m_pPureWaittingPCBs) {
		m_pPureWaittingPCBs = pPCB;
	}
	else {
		pTempPCB = m_pPureWaittingPCBs;
		while (pTempPCB->m_pNextPCB) {
			pTempPCB = pTempPCB->m_pNextPCB;
		}
		pTempPCB->m_pNextPCB = pPCB;

	}

}

//执行到inout指令的进程加入队尾（链表尾）
void CProcessSchedulingDlg::AppendInputQueue( PCB *pPCB)
{
	PCB* pTempPCB;
	if (!m_pInputWaittingPCBs) {
		m_pInputWaittingPCBs = pPCB;
	}
	else{
		pTempPCB = m_pInputWaittingPCBs;
		while (pTempPCB->m_pNextPCB) {
			pTempPCB = pTempPCB->m_pNextPCB;
		}
		pTempPCB->m_pNextPCB = pPCB;
	
	}


}

//执行到output指令的进程加入队尾（链表尾）
void CProcessSchedulingDlg::AppendOutputQueue(PCB* pPCB)
{
	PCB* pTempPCB;
	if (!m_pOutputWaittingPCBs) {
		m_pOutputWaittingPCBs = pPCB;
	}
	else {
		pTempPCB = m_pOutputWaittingPCBs;
		while (pTempPCB->m_pNextPCB) {
			pTempPCB = pTempPCB->m_pNextPCB;
		}
		pTempPCB->m_pNextPCB = pPCB;

	}
}

//等待线程处理函数使用接口对队列进行处理
UINT InputWaitThread(LPVOID pParam)
{
	
		PCB *ThreatEvent =new PCB(*(PCB*)pParam);   //获取参数
		
		
			PCB* TEMP = m_pInputWaittingPCBs;

			if (m_pInputWaittingPCBs->getpid() == (*ThreatEvent).getpid()) {     //删除头节点
				Sleep((*ThreatEvent).getinstruct()->gettime());      //等待一段时间
				(*ThreatEvent).setNextInstruction(); //设置下一条指令
				PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
				if (m_pBackupReadyPCBs == NULL)
				{
					//empty
					m_pBackupReadyPCBs = ThreatEvent;  //释放变量
				}
				else
				{
					//more than one node
					pTempPCB = m_pBackupReadyPCBs;
					while (pTempPCB->getNextPCB() != NULL)   //插入尾
						pTempPCB = pTempPCB->getNextPCB();
					pTempPCB->setNextPCB(ThreatEvent);
				}

				m_pInputWaittingPCBs = m_pInputWaittingPCBs->getNextPCB();
			}
			else                                                               //删除其他节点    
			{
				while (TEMP->getNextPCB()&& (TEMP->getNextPCB()->getpid()!= (*ThreatEvent).getpid())) {
					TEMP = TEMP->getNextPCB();                                         //找到相等的前一地址
			    }
				if (TEMP->getNextPCB() && TEMP->getNextPCB()->getpid() == (*ThreatEvent).getpid()) {  //删除操作
					Sleep((*ThreatEvent).getinstruct()->gettime());    //等待一段时间
				    (*ThreatEvent).setNextInstruction(); //设置下一条指令


					PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
					if (m_pBackupReadyPCBs == NULL)
					{
						//empty
						m_pBackupReadyPCBs = ThreatEvent;  //释放变量
					}
					else
					{
						//more than one node
						pTempPCB = m_pBackupReadyPCBs;
						while (pTempPCB->getNextPCB() != NULL)   //插入尾
							pTempPCB = pTempPCB->getNextPCB();
						pTempPCB->setNextPCB(ThreatEvent);
					}

					if (TEMP->getNextPCB() == NULL) {  //多线程修改同一动态全局变量，原来的TEMP指向可能改变了
						TEMP = NULL;
					}
					else {
						TEMP->setNextPCB(TEMP->getNextPCB()->getNextPCB());
					} 
					      
					
				}
				

			}
	return 1;
}

UINT OutputWaitThread(LPVOID pParam)
{
	PCB* ThreatEvent = new PCB(*(PCB*)pParam);   //获取参数


	PCB* TEMP = m_pOutputWaittingPCBs;

	if (m_pOutputWaittingPCBs->getpid() == (*ThreatEvent).getpid()) {     //删除头节点
		Sleep((*ThreatEvent).getinstruct()->gettime());      //等待一段时间
		(*ThreatEvent).setNextInstruction(); //设置下一条指令
		PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
		if (m_pBackupReadyPCBs == NULL)
		{
			//empty
			m_pBackupReadyPCBs = ThreatEvent;  //释放变量
		}
		else
		{
			//more than one node
			pTempPCB = m_pBackupReadyPCBs;
			while (pTempPCB->getNextPCB() != NULL)   //插入尾
				pTempPCB = pTempPCB->getNextPCB();
			pTempPCB->setNextPCB(ThreatEvent);
		}

		m_pOutputWaittingPCBs = m_pOutputWaittingPCBs->getNextPCB();
	}
	else                                                               //删除其他节点    
	{
		while (TEMP->getNextPCB() && (TEMP->getNextPCB()->getpid() != (*ThreatEvent).getpid())) {
			TEMP = TEMP->getNextPCB();                                         //找到相等的前一地址
		}
		if (TEMP->getNextPCB() && TEMP->getpid() == (*ThreatEvent).getpid()) {  //删除操作
			Sleep((*ThreatEvent).getinstruct()->gettime());    //等待一段时间
			(*ThreatEvent).setNextInstruction(); //设置下一条指令


			PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
			if (m_pBackupReadyPCBs == NULL)
			{
				//empty
				m_pBackupReadyPCBs = ThreatEvent;  //释放变量
			}
			else
			{
				//more than one node
				pTempPCB = m_pBackupReadyPCBs;
				while (pTempPCB->getNextPCB() != NULL)   //插入尾
					pTempPCB = pTempPCB->getNextPCB();
				pTempPCB->setNextPCB(ThreatEvent);
			}


			if (TEMP->getNextPCB() == NULL) {  //多线程修改同一动态变量，原来的temp指向可能改变了
				TEMP = NULL;
			}
			else {
				TEMP->setNextPCB(TEMP->getNextPCB()->getNextPCB());
			}

		}
		

	}

	return 0;
}

UINT PureWaitThread(LPVOID pParam)
{
	PCB* ThreatEvent = new PCB(*(PCB*)pParam);   //获取参数


	PCB* TEMP = m_pPureWaittingPCBs;

	if (m_pPureWaittingPCBs->getpid() == (*ThreatEvent).getpid()) {     //删除头节点
		Sleep((*ThreatEvent).getinstruct()->gettime());      //等待一段时间
		(*ThreatEvent).setNextInstruction(); //设置下一条指令
		PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
		if (m_pBackupReadyPCBs == NULL)
		{
			//empty
			m_pBackupReadyPCBs = ThreatEvent;  //释放变量
		}
		else
		{
			//more than one node
			pTempPCB = m_pBackupReadyPCBs;
			while (pTempPCB->getNextPCB() != NULL)   //插入尾
				pTempPCB = pTempPCB->getNextPCB();
			pTempPCB->setNextPCB(ThreatEvent);
		}

		m_pPureWaittingPCBs = m_pPureWaittingPCBs->getNextPCB();
	}
	else                                                               //删除其他节点    
	{
		while (TEMP->getNextPCB() && (TEMP->getNextPCB()->getpid() != (*ThreatEvent).getpid())) {
			TEMP = TEMP->getNextPCB();                                         //找到相等的前一地址
		}
		if (TEMP->getNextPCB() && TEMP->getNextPCB()->getpid() == (*ThreatEvent).getpid()) {  //删除操作
			Sleep((*ThreatEvent).getinstruct()->gettime());    //等待一段时间
			(*ThreatEvent).setNextInstruction(); //设置下一条指令


			PCB* pTempPCB;                        //将匹配的已完成的输入等待队列加入到后备就绪队列            
			if (m_pBackupReadyPCBs == NULL)
			{
				//empty
				m_pBackupReadyPCBs = ThreatEvent;  //释放变量
			}
			else
			{
				//more than one node
				pTempPCB = m_pBackupReadyPCBs;
				while (pTempPCB->getNextPCB() != NULL)   //插入尾
					pTempPCB = pTempPCB->getNextPCB();
				pTempPCB->setNextPCB(ThreatEvent);
			}


			if (TEMP->getNextPCB() == NULL) {  //多线程修改同一动态变量，原来的temp指向可能改变了
				TEMP = NULL;
			}
			else {
				TEMP->setNextPCB(TEMP->getNextPCB()->getNextPCB());
			}

		}
		

	}

	return 0;
}

void delay(int n)
{
	MSG msg;
	while (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(n);
}


